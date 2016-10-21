#include <iostream>
#include <cstddef>
#include <memory>
#include <cstring>

#include "SQException.h"
#include "SQCommunicator.h"

namespace SQ{
namespace SQCommunicator{
    SQCommunicator::SQCommunicator(SOCKET const& s) : _sock(s),_time(0),_size(0),_datagram("") {
        wsaInit();
    }

    SQCommunicator::~SQCommunicator(){
        wsaClean(); 
    }

    void SQCommunicator::build(SQ::SQPacket::SQPacket const& packet){ 
        //checking reserved option 
        for(auto opt : packet.getOptionsList()){
            if(std::get<0>(opt)[0] == '#'){
                throw SQ::SQException("[SQCommunicator::build] Option starting with '#' are reserved to the system.");
            }
        }             

        std::string strOption = ""; 
        //adding header '#size:datagrame_size' 
        SQ::SQPacket::OptionsList optlist = packet.getOptionsList();
        optlist.push_back(std::make_pair("#size",SQ::PATCH::to_string(packet.data().size())));

        //convert options to a string
        if(optlist.size() > 0){
            for (auto option : optlist){
                strOption += std::get<0>(option)+":"+std::get<1>(option)+"\n"; 
            }
            strOption += "\n";
        }  

        // we add 1 for last caracter '\n'
        int sz = 1 + sizeof(packet.dest()) + sizeof(packet.src()) + sizeof(SQ::SQPacket::uchar) + sizeof(SQ::SQPacket::uchar) + sizeof(SQ::SQPacket::uchar) + strOption.size() + packet.data().size();
        char * msg = new char[sz];
        //std::fill (msg,msg+sz,0);
        memset (msg,0,sz);

        int pos(0);
        *((int*)msg) = packet.dest();
        pos += sizeof(packet.dest());

        *((int*)(msg+pos)) = packet.src();
        pos += sizeof(packet.src());

        *((char*)(msg+pos)) = (int)packet.method();  
        pos += sizeof(SQ::SQPacket::uchar);

        *((char*)(msg+pos)) = (int)packet.parameter();  
        pos += sizeof(SQ::SQPacket::uchar);

        *((char*)(msg+pos)) = optlist.size();
        pos += sizeof(SQ::SQPacket::uchar); 
        pos += 1;

        memcpy(msg+pos,strOption.c_str(),strOption.size());
        pos += strOption.size();

        memcpy(msg+pos,packet.data().c_str(),packet.data().size());

        _datagram = std::string(msg,sz);

        delete[] msg;
    }

    void SQCommunicator::extract(SQ::SQPacket::SQPacket & packet){
        if(_datagram.size()==0)
            throw SQ::SQException("[SQCommunicator::Extract] The packet to extract is empty.");

        if(_datagram.size() < SQ::SQPacket::HEADERLENGTH)
            throw SQ::SQException("[SQCommunicator::Extract] The packet is corrupted.");

        //extracting data
        char* msg = new char[_datagram.size()]; 
        memset (msg,0,_datagram.size());
        memcpy(msg,_datagram.c_str(),_datagram.size());

        int pos(0);
        packet.clear(); 

        packet.dest(*((int*)msg));
        pos += sizeof(packet.dest());            

        packet.src(*((int*)(msg+pos)));
        pos += sizeof(packet.dest());            

        int tmp = *((char*)(msg+pos));
        packet.method(tmp);
        pos += sizeof(packet.method());

        tmp = *((char*)(msg+pos));
        packet.parameter(tmp);
        pos += sizeof(packet.parameter());

        int nbOptions = *((char*)(msg+pos));
        pos += sizeof(SQ::SQPacket::uchar);
        pos += 1;

        SQ::SQPacket::OptionsList list;
        std::vector<std::string> part;

        //let read option lines
        std::stringstream  ss;
        ss << std::string((msg+pos),_datagram.size()-pos);        
        //ss << std::string((msg+pos),_buffersize-pos);        
        while(nbOptions < SQ::SQPacket::UCHAR_MAX && nbOptions-- > 0){  
            std::string optionStr = "";
            std::getline(ss,optionStr);     
            if(optionStr.size()<= 3) // this is the minimum size for a valid option. For instantce :      "a:5" where a is the key and 5 the value 
                throw SQ::SQException(std::string("[SQCommunicator::Extract] An error occurs while trying to extract options from the packet. The option n° "+SQ::PATCH::to_string((list.size()+1))+" is bad : \n => "+optionStr).c_str()); 
            pos += optionStr.size() +1; 
            SQ::PATCH::split(part,optionStr,':'); 
            if(part.size() > 1)
                list.push_back(std::make_pair(part[0],part[1]));  
        }  
        if(nbOptions > 0){
            pos += 1; // +1 for the '\n' which ends options
        }

        //retrieving datagram size
        int dgramSize = -1;
        for(auto opt : list){
            if(std::get<0>(opt) == "#size"){
                dgramSize = std::stoi(std::get<1>(opt)); 
                break;
            }
        }
        if(dgramSize == -1) 
            throw SQ::SQException("[SQCommunicator::extract] The datagram size is not set.");

        packet.setOptionsList(std::move(list));

        //resizing to the minimal string
        dgramSize = ( dgramSize <= (_datagram.size()-pos) )? dgramSize : _datagram.size()-pos; 
        packet.data(std::string((msg+pos),dgramSize+1));

        delete[] msg; 

    }

    void SQCommunicator::read(SQ::SQPacket::SQPacket & packet, int const& buffersize){
        _buffersize = buffersize <= BUFFER_MINSIZE
                ? BUFFER_MINSIZE
                : (buffersize > BUFFER_MAXSIZE ? BUFFER_MAXSIZE : buffersize );
 
        char* buffer = new char[_buffersize]; 
        _datagram = ""; 

        int sock_err = recv(sock(),buffer,_buffersize,0); 
        if(sock_err==-1)
                throw SQ::SQException("[SQCommunicator::read] An error occurs on the network.");
        if(sock_err==0)
                throw SQ::SQException("[SQCommunicator::read] Dead socket.");
 
        _datagram = std::string(buffer,_buffersize);               
           
	extract(buffer,packet);
        
        delete [] buffer;
    }

    void SQCommunicator::write(SQ::SQPacket::SQPacket const& packet, int const& buffersize){
        _buffersize = buffersize <= BUFFER_MINSIZE
                ? BUFFER_MINSIZE
                : (buffersize > BUFFER_MAXSIZE ? BUFFER_MAXSIZE : buffersize );
				//char buffer[_buffersize];
        _datagram.resize(static_cast<size_t>(_buffersize));
							
        build(packet);
        //memcpy(buffer,_datagram.c_str(),_buffersize);
        //std::cout << "BUFFER SEND ::::: " << buffer << std::endl;
        //int sock_err = send(sock(),buffer,_buffersize,0);
        int sock_err = send(sock(),_datagram.c_str(),_buffersize,0);
	std::cout << sock_err << " caracters sent" << std::endl;

        if(sock_err==1)
            throw SQ::SQException("[SQCommunicator::write] Sending packet failed.");
    }

    void SQCommunicator::wsaInit(){
    #if defined(_WIN32) || defined(WIN32)
        WSADATA wsa;
        if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
        {
            throw SQ::SQException("WSAStartup failed !");
        }
    #endif
    }

    void SQCommunicator::wsaClean(){
    #if defined(_WIN32) || defined(WIN32)
        WSACleanup();
    #endif
    }



}
}