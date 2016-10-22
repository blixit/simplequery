#include <iostream>
#include <functional>

#include "simplequery.h" 


 
class Server : public SQ::SQNetEntity::SQServer{
    public: 
        Server() : SQ::SQNetEntity::SQServer(){};

        void on_connect(SQ::SQNetEntity::SQFinalClient const& c){
            std::cout << " The client "+ SQ::PATCH::to_string(c.id) +" is connected on socket "+ SQ::PATCH::to_string(c.sock) +" ... " << std::endl;
            std::cout << " # clients " << this->getClients().size() << std::endl;
        }

        void on_leave(SOCKET const& s){
            std::vector<SQ::SQNetEntity::SQFinalClient> clients = this->getClients();
            std::cout << " # clients " << this->getClients().size() << std::endl; 
            std::cout << " The client on socket " << s << " is disconnected ... " << std::endl;
        }

        virtual void on_read(SQ::SQPacket::SQPacket const& packet){
            std::cout << " A packet read ... " << std::endl;
            std::cout << "src : " << packet.src() << std::endl; 
            std::cout << "dest : " << packet.dest() << std::endl; 
            std::cout << "method : " << (int)packet.method() << std::endl; 
            std::cout << "param : " << (int)packet.parameter() << std::endl; 
            std::cout << "options : " << packet.nbOptions() << std::endl; 
            std::cout << "data : " << packet.data() << std::endl; 
            for(auto opt : packet.getOptionsList()){
                std::cout << " - " << std::get<0>(opt)+":"+std::get<1>(opt) << std::endl;
            }
        }
        
        void* on_test(void* args){
            std::cout << " test executed " << " on " << ((Server*)args)->port() << std::endl;
            return args;
        }
};

void* cb(void* args){
    std::cout << " cb executed " << std::endl;
    return args;
}

int main(/*int argc, char** argv*/) { 
  
    SQ::SQNetEntity::SQNetEntity::initializer();

    Server server;
    server.DEBUGSTATE = true;
    server.port(SQ::SQNetEntity::DEFAULT_PORT); //1607
    server.maxClients(2);

    std::cout << server.is_server() << std::endl;
    std::cout << server.isRunning() << std::endl;
      
    /*typedef std::function<void*(void*)> FUNC;
    FUNC f = std::bind(&Server::on_test,&server,std::placeholders::_1); 
    
    SQ::SQEvents::SQEvents ev('d',"message",f,(void*)(&server)); 
    ev.toString();
    ev.run();*/
    
    server.getEvents().push_back(
        SQ::SQEvents::SQEvents (
            'd',"message",std::bind(&Server::on_test,&server,std::placeholders::_1),(void*)(&server)
    ));
 
    std::cout << " ** " << std::endl; 

    SQ::SQException::LOG(SQ::SQException::LOGSTR("Hello this is an error !!",__FUNCTION__, __LINE__, __FILE__)); 

    try{
        server.startService(); 
        server.queryListener();

        server.stopService();

    }catch(SQ::SQException const& e){
        Log(std::string(e.what()),server.DEBUGSTATE) 
    }

    SQ::SQNetEntity::SQNetEntity::destroyer();

  	return 0;
}