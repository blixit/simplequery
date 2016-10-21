#include <iostream>

#include "SQPacket.h"

namespace SQ{
namespace SQPacket{
    SQPacket::SQPacket(){
            _dest = 0;
            _src = 0;
            _method = 0;
            _parameter = 0; 
            _data = "";
    }

    SQPacket::SQPacket(int const& dest, int const& src, uchar const& method, uchar const& parameter, OptionsList list, std::string data )
            : _dest(dest), _src(src), _method(method), _parameter(parameter), _optionslist(list), _data(data)
        {}
    SQPacket::~SQPacket(){	}

    SQPacket& SQPacket::clear(){
        _dest = 0;
        _src = 0;
        _method = 0;
        _parameter = 0; 
        _data = "";
        _optionslist.clear();
        _data.clear();
    }
	 	
}
}