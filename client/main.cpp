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
        SQ::SQPacket::SQDataHeaders list;
        list.add("content","sq");
        list.add("size","45");
        list.add("port","56");
        list.add("name","Le livre de la jungle");
        /*SQ::SQPacket::SQPacket p(0,0,7,1,list,"GET www.google.com");
        SQ::SQCommunicator::SQCommunicator com(-1);
        com.build(p);
      p.data("dedede");
        com.extract(p);*/
        
        client.sconnect("127.0.0.1",SQ::SQNetEntity::DEFAULT_PORT);  

        std::shared_ptr<SQ::SQCommunicator::SQCommunicator> pcom = std::move(client.com());
        SQ::SQCommunicator::SQCommunicator* pcom2 = client.ptrCom();

        if(pcom ){
            SQ::SQPacket::SQPacket packet;
            std::cout << "client 1 " << std::endl << "pcom : " << pcom.get() << std::endl;
            SQ::SQPacket::SQPacket p2(-1,18,1,2,list,"GET www.google.com");
            pcom->write(p2); 
            p2.method(2);
            pcom->write(p2); 
            std::cout << "Data : " << p2.data() << std::endl;
            std::cout << "nbOptions : " << p2.nbOptions() << std::endl;
        
            /*pcom->read(packet);//, SQ::SQCommunicator::BUFFER_MAXSIZE);
          
            std::cout << "src : " << packet.src() << std::endl; 
            std::cout << "dest : " << packet.dest() << std::endl; 
            std::cout << "method : " << (int)packet.method() << std::endl; 
            std::cout << "param : " << (int)packet.parameter() << std::endl; 
            std::cout << "options : " << packet.nbOptions() << std::endl; 
            std::cout << "data : " << packet.data() << std::endl; */
        }

        if(pcom2){
            std::cout <<  " pcom2 : " << pcom2 << std::endl; 
        } 

        client.sdisconnect();

    }catch(SQ::SQException const& e){
        //std::cout << "[Error] " << e.what() << std::endl;
        Log(std::string(e.what()),client.DEBUGSTATE);
    }
   
    SQ::SQNetEntity::SQNetEntity::destroyer(); 
    return 0;
}