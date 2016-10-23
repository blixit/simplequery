#include <thread>
#include <algorithm>  
#include <exception> 
#include <ctime> 

#include "SQEvents.h"
#include "SQServer.h"


namespace SQ{
namespace SQNetEntity{
    SQServer::SQServer( int const& portnum)  : _maxClients(DEFAULT_CLIENT_NUMBER), _port(portnum) {
        isRunning(false);
				nbClients(0);
				srand(time(NULL));
    }

    SQServer::~SQServer()    {
        //dtor
    }

    SQServer::SQServer(const SQServer& other)    {
        //copy ctor
    }

    SQServer& SQServer::operator=(const SQServer& rhs)    {
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        return *this;
    } 
	
    bool SQServer::is_server() { return static_cast<bool>(SQ::SQNetEntity::isServer); }

    SOCKET SQServer::waitClient(){  

        SOCKET csock;
        SOCKADDR_IN csin;
        socklen_t crecsize = sizeof(csin);
        csock = accept(sock(), (SOCKADDR*)&csin, &crecsize);
        if((int)csock == SOCKET_ERROR)
            throw SQException("An error occurs while trying to accept a new client.");
        /*/Timeout receive operations
        struct timeval tv;
        tv.tv_sec = 60; //timeout
        tv.tv_usec = 0;
        //setsockopt(csock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));*/

	Log("A new connection on socket "+PATCH::to_string(csock)+ " ...",DEBUGSTATE) 
					
        return csock;  
    }

    int SQServer::startService() {

        Log("The server has been initialized ..." ,DEBUGSTATE)

        SOCKADDR_IN sin;
        socklen_t recsize = sizeof(sin);
        sock(socket(AF_INET , SOCK_STREAM , 0 ));
        if(sock() == INVALID_SOCKET)
                        throw SQException("The server initialization failed.");
        int tmp = 1;
        setsockopt(sock(),SOL_SOCKET,SO_REUSEADDR,&tmp,sizeof(int));

        sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
        sin.sin_port = htons(port()); /* Listage du port */
        sin.sin_family = AF_INET; /* Protocole familial (IP) */

        if(bind(sock(), (SOCKADDR*)&sin, recsize)==-1)
                        throw SQException("Binding step failed.");
        if(listen(sock(), (int)maxClients())==-1)
                        throw SQException("The server failed to start listening.");

        isRunning(true);
        Log("The server is listening on the port "+ SQ::PATCH::to_string(port()) +" ..." ,DEBUGSTATE) 

        std::thread ticketManagement(&SQ::SQNetEntity::SQServer::getTicket,this);			
        Log("The ticket service is launched ... " ,DEBUGSTATE) 

        while(isRunning()){
                Log("Waiting for a new client ... " ,DEBUGSTATE) 
                SOCKET csock = waitClient();

                _mutex.lock(); 
                //start waiting for a ticket by enterring the queue
                _ticketQueue.push(csock); 
                bool added = addClientToQueue();
                _mutex.unlock();

                if(!added){
                        sdisconnect(csock);
                        continue;
                }
                Log(SQ::PATCH::to_string(csock) +" is getting a ticket ...",DEBUGSTATE)   

        } 

        ticketManagement.join();

        stopService();

        return 0;
    }

    void SQServer::stopService(){
        if(_com.get()){
            shutdown(_com.get()->sock(),2);
            close(_com.get()->sock());
            _com.get()->sock(-1);
	}
        this->isRunning(false);
    }
		
    void SQServer::getTicket(){
        while(true){ 
            try{ 
                _mutex.lock();
                if(_ticketQueue.empty()){   
                        _mutex.unlock();
                        continue;
                }
                _mutex.unlock();

                //check if there're enough resources 
                if(nbClients() <= maxClients()){ 
                    _mutex.lock();  
                    //get a ticket
                    SOCKET csock = _ticketQueue.front();
                    _ticketQueue.pop(); 
                    Log(SQ::PATCH::to_string(nbClients()) +" clients ...",DEBUGSTATE) 
                    Log(SQ::PATCH::to_string(csock) +" got a ticket ...",DEBUGSTATE) 

                    _mutex.unlock();

                    //generate an unique id for the current client 
                    int id;
                    do{
                            id = rand()% 8999999 + 1000000;  
                    }while(std::find (_clients.begin(), _clients.end(),SQFinalClient(id,csock)) != _clients.end() );


                    Log("New given id "+ SQ::PATCH::to_string(id) +" ...",DEBUGSTATE) 
                    //call the routine callback 
                    SQFinalClient client(id,csock);
                    this->_clients.push_back(client);
                    
                    authentify(client);                    		

                    //wait for packet
                    std::thread(&SQ::SQNetEntity::SQServer::queryListener,this,SQFinalClient(id,csock)).detach();

                }else{
                    //_mutex.lock();
                    //Log(SQ::PATCH::to_string(_ticketQueue.size()) +" still waiting for a ticket ...",DEBUGSTATE)  
                    //_mutex.unlock();
                } 
            }catch(std::exception const& e){
                    Log("The ticket office noticed a perturbation : "+std::string(e.what()),DEBUGSTATE) 
            }
        }
    }

    /**
     * Close the given socket and remove the related client.
     * @param s
     */
    void SQServer::sdisconnect(SOCKET const& s){
        int i(0);
        for(SQ::SQNetEntity::SQFinalClient client : _clients){ 
            if(client.getSock() == s){
                _mutex.lock();
                _clients.erase(_clients.begin()+i);
                delClientToQueue();
                _mutex.unlock();
                break;
            }
            i++;
        }
        close(s); 
        this->on_leave(s);    
    }

    void SQServer::authentify(SQFinalClient const& client){
        try{
            std::unique_ptr<SQ::SQCommunicator::SQCommunicator> pcom = std::unique_ptr<SQCommunicator::SQCommunicator>(new SQCommunicator::SQCommunicator(client.getSock()));
            if(pcom.get() == nullptr){
                    throw SQException("[queryListener] The communicattor is not set.");
            }
            SQ::SQPacket::OptionsList list;  
            
            pcom.get()->write(SQ::SQPacket::SQPacket(client.getId(),SQ::SERVER_ID,SQ::post,SQ::PARAM_ID,list," "));
            this->on_connect(client);
        }catch(SQException & e){
            sdisconnect(client.getSock());  
        }
    }
    void SQServer::queryListener(SQFinalClient const& c){
        while(isRunning()){
            try{
                // read packet
                SQ::SQPacket::SQPacket packet; 
                std::unique_ptr<SQ::SQCommunicator::SQCommunicator> pcom = std::unique_ptr<SQCommunicator::SQCommunicator>(new SQCommunicator::SQCommunicator(c.sock));
                if(pcom.get() == nullptr){
                        throw SQException("[queryListener] The communicattor is not set.");
                }
                pcom.get()->read(packet);//, SQ::SQCommunicator::BUFFER_MAXSIZE);
                bool bind = this->on_read(packet);
                if(bind){  
                    SQ::SQNetEntity::SQNetEntity::bindExecute(packet, this->getEvents());
                }
            }catch(SQException & e){
               
                break;
            } 
        }
        sdisconnect(c.sock); 
    }
	 
    /*void SQServer::on_read(SQ::SQPacket::SQPacket const& packet){
        // catch event 
    }*/
	
}
}