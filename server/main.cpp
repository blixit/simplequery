#include <iostream>
#include <exception>
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

        bool on_read(SQ::SQPacket::SQStringPacket const& packet){ 
            /*
             * If a registered SQEvent handles this packet and you want to execute the related callback function,
             * just return true. Else return false;
             */
            return true;
        }
        
        void* on_test(void* packet, void* args){
            std::cout << " test executed on " << ((Server*)args)->port() << std::endl;
            SQ::SQPacket::SQStringPacket p = *(SQ::SQPacket::SQStringPacket*)packet;
            std::cout << p.toString() << std::endl;
            return args;
        }
        
        void* on_data(void* packet, void* args){
            std::cout << " data received on " << ((Server*)args)->port() << std::endl;
            SQ::SQPacket::SQStringPacket p = *(SQ::SQPacket::SQStringPacket*)packet;
            std::cout << p.toString() << std::endl;
            return args;
        }
};
 

int main(/*int argc, char** argv*/) { 
  
    SQ::SQNetEntity::SQNetEntity::initializer();

    Server server;
    server.DEBUGSTATE = true;
    server.port(SQ::SQNetEntity::DEFAULT_PORT); //1607
    server.maxClients(2); 
      
    //Events
    SQ::SQEvents::SQEventsList events;
    events.push_back(SQ::SQEvents::SQEvents (
        SQ::SQEvents::SQEVTYPE_M, SQ::PATCH::to_string(1) ,std::bind(&Server::on_test,&server,std::placeholders::_1,std::placeholders::_2),(void*)(&server)
    ));
    events.push_back(SQ::SQEvents::SQEvents (
        SQ::SQEvents::SQEVTYPE_M|SQ::SQEvents::SQEVTYPE_D, "2-GET www.google.com",std::bind(&Server::on_data,&server,std::placeholders::_1,std::placeholders::_2),(void*)(&server)
    ));
    server.setEvents(events); 
 
    //launching service
    try{
        server.startService();  
        
    }catch(SQ::SQException const& e){
        Log(std::string(e.what()),server.DEBUGSTATE) 
    }catch(std::exception const& e){
        Log(std::string(e.what()),server.DEBUGSTATE) 
    }
    //stoping the service
    server.stopService();
    

    SQ::SQNetEntity::SQNetEntity::destroyer();

    return 0;
}