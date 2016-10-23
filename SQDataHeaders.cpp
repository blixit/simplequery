/* 
 * File:   SQDataHeaders.cpp
 * Author: Blixit 
 * 
 * Created on 23 octobre 2016, 13:39
 */
#include <iostream>
#include "SQDataHeaders.h"


namespace SQ{
namespace SQPacket{
    
    /**
     * Finds an header which matches the key
     * @param beginpos
     * @param endpos
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findbyKey(std::string const& key){
        return find(key);
    }
    
    /**
     * Finds an header which which matches the key between the 2 given positions
     * @param beginpos
     * @param endpos
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findbyKey(SQDataHeadersIterator const& beginpos, SQDataHeadersIterator const& endpos, std::string const& key){
        SQDataHeadersIterator it = beginpos;
        while(it != endpos){
            if(it->first == key)
                break;
            it++;
        }
        return it;
    }
    
    /**
     * Finds the first header using a reserved key word
     * @param beginpos
     * @param endpos
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findAnyReservedKey(){  
        return findAnyReservedKey(begin(),end());
    }
    
    /**
     * Finds the first header using a reserved key word between 2 given positions
     * @param beginpos
     * @param endpos
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findAnyReservedKey(SQDataHeadersIterator const& beginpos, SQDataHeadersIterator const& endpos){
        SQDataHeadersIterator it = beginpos; 
        while(it != endpos){  
            if(it->first[0] == '#')
                break;
            it++;
        }
        return it;
    }
    
    /**
     * Finds an element by value
     * @param value
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findbyValue(std::string const& value){
        return findbyValue(begin(),end(),value);
    }
    
    /**
     * Finds an element by value between beginpos and endpos
     * @param beginpos
     * @param endpos
     * @param value
     * @return 
     */
    SQDataHeadersIterator SQDataHeaders::findbyValue(SQDataHeadersIterator const& beginpos, SQDataHeadersIterator const& endpos, std::string const& value){
        SQDataHeadersIterator it = beginpos;
        while(it != endpos){
            if(it->second == value)
                break;
            it++;
        }
        return it;
    }
    
    /**
     * Returns the list as a string for the building of the packet
     * @see {@link SQCommunicator}
     * @return 
     */
    std::string SQDataHeaders::preparedString(){
        std::string strOption = "";
        if(size() > 0){
            SQDataHeadersIterator it = begin();
            while(it != end()){                
                strOption += it->first + ":" + it->second + "\n"; 
                it++;
            } 
            strOption += "\n";
        }  
        return strOption;
    }
    
    /**
     * Returns the value which matches the given key
     * @param key
     * @return the value
     */
    std::string SQDataHeaders::getValue(std::string const& key){
        SQDataHeadersIterator it = find(key);
        if(it != end())
            return it->second;
        return "";
    }
    
    /**
     * Returns the key which matches the given value.
     * @param value the value to find
     * @return the key
     */
    std::string SQDataHeaders::getKey(std::string const& value){
        SQDataHeadersIterator it = findbyValue(value);
        if(it != end())
            return it->first;
        return "";
    }
 
}
}

