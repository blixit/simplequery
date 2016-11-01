#include <string> //to_string

#include "SQClient.h"


namespace SQ{
namespace SQNetEntity{

    SQClient::SQClient()   {
        isRunning(false);
    }

    SQClient::~SQClient()    {
    }

    SQClient::SQClient(const SQClient& other)  {
        //copy ctor
    }

    SQClient& SQClient::operator=(const SQClient& rhs)    {
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        return *this;
    }

    bool SQClient::is_server() { return static_cast<bool>(SQ::SQNetEntity::isClient); }

    /**
        \brief Connection to a server
        \warning this function use sock_stream-af_inet socket
    */
    void SQClient::sconnect(std::string const& ip, int const& port) {

        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        SOCKADDR_IN sin;

        if(s == INVALID_SOCKET)
            throw SQException("The client initialization failed.");

        _com = std::shared_ptr<SQCommunicator::SQCommunicator>(new SQCommunicator::SQCommunicator(s));

        sin.sin_addr.s_addr = inet_addr(ip.c_str()); // * l'adresse se trouve dans le champ h_addr de la structure hostinfo * /
        sin.sin_port = htons(port); // * on utilise htons pour le port * /
        sin.sin_family = AF_INET;

        if(connect(s,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR){
            std::string msg = "The connection to [" + ip + ":" + SQ::PATCH::to_string(port) + "] failed.";
            throw SQException(msg.c_str());
        }
        
        SQFinalClient clientInfo = getAuthId(s);  
        
        isRunning(true);
        
        queryListener(clientInfo);

    }
    
    SQFinalClient SQClient::getAuthId(SOCKET const& s){
        
        SQ::SQPacket::SQStringPacket packet; 
        std::unique_ptr<SQ::SQCommunicator::SQCommunicator> pcom = std::unique_ptr<SQCommunicator::SQCommunicator>(new SQCommunicator::SQCommunicator(s));
        if(pcom.get() == nullptr){
            throw SQException("[getAuthId] The communicattor is not set.");
        }
        pcom.get()->read(packet);//, SQ::SQCommunicator::BUFFER_MAXSIZE);
        this->on_read(packet);  
        SQFinalClient clientInfo(packet.dest(),s); 
        this->on_connect(clientInfo);
        
        return clientInfo;
    } 

    void SQClient::sdisconnect(){
        if(_com.get()){
            this->on_leave(_com.get()->sock());
            close(_com.get()->sock());
            _com.get()->sock(-1);
        }
        
    }
    
    void SQClient::queryListener(SQFinalClient const& c){
        while(isRunning()){
            try{
                // read packet
                SQ::SQPacket::SQStringPacket packet; 
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
                Log(std::string(e.what()),DEBUGSTATE) 
                break;
            } 
        }
        sdisconnect(); 
    }
}
}