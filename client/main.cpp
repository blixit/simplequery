#include <iostream>
#include <sstream>

#include "simplequery.h" 

//using namespace std;

int main(int argc, char** argv) { 
    SQ::SQNetEntity::SQNetEntity::initializer();
   
    SQ::SQNetEntity::SQClient client;   
    client.DEBUGSTATE = true;
    try{
        SQ::SQPacket::OptionsList list;
      list.push_back(std::make_pair("content","sq"));
      list.push_back(std::make_pair("size","45"));
      list.push_back(std::make_pair("port","56"));
      list.push_back(std::make_pair("name","Le livre de la jungle"));
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
            std::cout << "Data : " << p2.data() << std::endl;
            std::cout << "nbOptions : " << p2.nbOptions() << std::endl;
        
            pcom->read(packet);//, SQ::SQCommunicator::BUFFER_MAXSIZE);
          
            std::cout << "src : " << packet.src() << std::endl; 
            std::cout << "dest : " << packet.dest() << std::endl; 
            std::cout << "method : " << (int)packet.method() << std::endl; 
            std::cout << "param : " << (int)packet.parameter() << std::endl; 
            std::cout << "options : " << packet.nbOptions() << std::endl; 
            std::cout << "data : " << packet.data() << std::endl; 
        }

        if(pcom2){
            std::cout <<  " pcom2 : " << pcom2 << std::endl; 
        } 

        client.sdisconnect();

    }catch(SQ::SQException const& e){
        //std::cout << "[Error] " << e.what() << std::endl;
        Log(std::string(e.what()),client.DEBUGSTATE);
    }
   
    SQ::SQNetEntity::SQNetEntity::initializer(); 
    return 0;
}