/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SQDataHeaders.h
 * Author: Blixit 
 *
 * Created on 23 octobre 2016, 13:39
 */

#ifndef SQDATAHEADERS_H
#define SQDATAHEADERS_H

#include <algorithm>
#include <map>

namespace SQ{
namespace SQPacket{
    
    typedef std::map<std::string,std::string>::iterator SQDataHeadersIterator;
    
    class SQDataHeaders : public std::map<std::string,std::string>{
    public:        
        
        //use the constructor from vector
        using std::map<std::string,std::string>::map;
        
        SQDataHeadersIterator findbyKey(std::string const& key);
        SQDataHeadersIterator findbyKey(SQDataHeadersIterator const& begin, SQDataHeadersIterator const& end, std::string const& key);
        SQDataHeadersIterator findAnyReservedKey();
        SQDataHeadersIterator findAnyReservedKey(SQDataHeadersIterator const& begin, SQDataHeadersIterator const& end);
        SQDataHeadersIterator findbyValue(std::string const& value);
        SQDataHeadersIterator findbyValue(SQDataHeadersIterator const& begin, SQDataHeadersIterator const& end, std::string const& value);
        
        std::string preparedString();
        std::string getValue(std::string const& key);
        std::string getKey(std::string const& value);
        
        void add(std::string const& key, std::string const& value);
        
        
    private:

    };
}
}

#endif /* SQDATAHEADERS_H */

