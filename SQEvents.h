#ifndef SQEVENTS_H
#define SQEVENTS_H

#include <vector>
#include <functional>
#include <iostream>

#include "SQPacket.h"


namespace SQ{
namespace SQEvents{
    class SQPacket;
    
    typedef unsigned char uchar;
    /**
     * Un pointeur de fonctions
     */ 
    typedef std::function<bool(void*, void*)> SQCallbackFunction; 
     
    //using SQCallbackFunction = std::function<bool(void*, void*)>;

    //les types de messages à tracker
    static const uchar SQEVTYPE_ALL = 7; // m + p + d
    static const uchar SQEVTYPE_M = 4; // m 
    static const uchar SQEVTYPE_P = 2; // p
    static const uchar SQEVTYPE_D = 1; // d  
    
    /*
     * Gestion des évènements
     */
    
    class SQEvents{
        public :
            SQEvents() : 
            type('\0'), message("") {
                callback = NULL;
                parameters = NULL; 
            }
            /*SQEvents( T const& cb2, char const& c, std::string const& msg,void* params) : 
                type('\0'), message("") {
                callback = NULL;
                callback2 = cb2;
                parameters = NULL;                 
            }*/              
            SQEvents(uchar const& c, std::string const& msg, SQCallbackFunction cb, void* params) : 
            type(c), message(msg), callback(cb), parameters(params) {}
            SQEvents(SQEvents const& e)  : 
            type(e.getType()), message(e.getMessage()), callback(e.getCallback()), parameters(e.getParameters()) {}

            inline uchar getType() const { return type;} 
            inline void setType(uchar const& value) { type = value;} 
            inline std::string getMessage() const { return message;} 
            inline void setMessage(std::string const& value) { message = value;} 
            inline SQCallbackFunction getCallback() const { return callback;} 
            inline void setCallback(SQCallbackFunction const& value) { callback = value;} 
            inline void* getParameters() const { return parameters;} 
            inline void setParameters(void* value) { parameters = value;} 
             
            inline bool run(void* p){
                return callback(p,parameters); 
            }
            inline void toString(){
                std::cout << getType() << " " << getMessage() << " " << std::endl;
            }                   
            
        private :
            /*
             * type of message to track : 1.methode, 2.parameter or 3.data
             */
            uchar type;
            /*
             * message to track
             */
            std::string message;
            /*
             * Callback function to trigger if a message is catched
             */
            SQCallbackFunction callback;
            //T callback2;
            /*
             * Parameters to give to the callback function
             */
            void* parameters;
    };
    
    /**
     * Type which represents a list of events.
     */
    typedef std::vector<SQEvents> SQEventsList;
    /*template<typename T>
    using SQEventsList = std::vector<SQEvents<T> ;*/
  
}
}

#endif // SQEVENTS_H