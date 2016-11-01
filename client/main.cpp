#include <iostream>
#include <sstream>

#include "simplequery.h" 
 
class Client : public SQ::SQNetEntity::SQClient{
    public: 
        SQ::SQNetEntity::SQFinalClient infos;
        
        Client() : SQ::SQNetEntity::SQClient(), infos(0,0){ 
        };

        void on_connect(SQ::SQNetEntity::SQFinalClient const& c){
            infos = c;
            std::cout << " Connected with ID " << c.getId() << "... " << std::endl;
            int x;
            std::cin >> x;
        }

        void on_leave(SOCKET const& s){ 
            std::cout << " Disconnexion... " << std::endl;
        }
    
    
        
};

int main(int argc, char** argv) { 
    SQ::SQNetEntity::SQNetEntity::initializer();
   
    Client client;   
    client.DEBUGSTATE = true;
    try{          
        client.sconnect("127.0.0.1",SQ::SQNetEntity::DEFAULT_PORT);           

        client.sdisconnect();

    }catch(SQ::SQException const& e){
        //std::cout << "[Error] " << e.what() << std::endl;
        Log(std::string(e.what()),client.DEBUGSTATE);
    }
   
    SQ::SQNetEntity::SQNetEntity::destroyer(); 
    return 0;
}