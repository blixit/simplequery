#include <iostream>

#include "simplequery.h" 
 
class Server : public SQ::SQNetEntity::SQServer{
    public: 
        Server() : SQ::SQNetEntity::SQServer(){};

        void on_connect(SQ::SQNetEntity::SQFinalClient const& c){
            std::cout << " The client "+ SQ::PATCH::to_string(c.id) +" is connected on socket "+ SQ::PATCH::to_string(c.sock) +" ... " << std::endl;
        }

        void on_leave(){
            std::cout << " The client is disconnected ... " << std::endl;
        }

        void on_read(SQ::SQPacket::SQPacket const& packet){
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
};

int main(int argc, char** argv) { 
  
  SQ::SQNetEntity::SQNetEntity::initializer();
  
  Server server;
  server.DEBUGSTATE = true;
  server.port(SQ::SQNetEntity::DEFAULT_PORT); //1607
  server.maxClients(1);

  std::cout << server.is_server() << std::endl;
  std::cout << server.isRunning() << std::endl;
  
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