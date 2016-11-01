#include <iostream>

#include "SQStringPacket.h"

namespace SQ{
namespace SQPacket{
    SQStringPacket::SQStringPacket(){
            _dest = 0;
            _src = 0;
            _method = 0;
            _parameter = 0; 
            _data = "";
            _optionslist.clear();
    }
    
    SQStringPacket::SQStringPacket(SQStringPacket const& p){
            _dest = p.dest();
            _src = p.src();
            _method = p.method();
            _parameter = p.parameter(); 
            _data = p.data();  
            _optionslist = p.getOptionsList();
    }
    
    SQStringPacket::SQStringPacket(int const& dest, int const& src, uchar const& method, uchar const& parameter, SQDataHeaders list, std::string data )
            : _dest(dest), _src(src), _method(method), _parameter(parameter), _optionslist(list), _data(data)
        {}
    
    SQStringPacket::~SQStringPacket(){	}

    SQStringPacket& SQStringPacket::clear(){
        _dest = 0;
        _src = 0;
        _method = 0;
        _parameter = 0; 
        _data = "";
        _optionslist.clear();
        _data.clear();
        return *this;
    }
	 	
}
}