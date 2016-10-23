#include "SQNetEntity.h"

namespace SQ{
namespace SQNetEntity{ 
    int SQNetEntity::_initializer = 0;
    
    SQNetEntity::SQNetEntity()  {
        if(SQNetEntity::_initializer != 1)
            throw SQException("[SQNetEntity] The initializer has not been called. ");
    }

    SQNetEntity::~SQNetEntity()  {
        //dtor
    }
    
    /**
     * This function must be call before all calls of this library
     */
    void SQNetEntity::initializer(){
    #if defined(_WIN32) || defined(WIN32)
        WSADATA wsa;
        if(WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
        {
            throw SQ::SQException("WSAStartup failed !");
        }
    #endif
        SQNetEntity::_initializer = 1;
    }
    
    /**
     * This function must be call after calls of this library
     */
    void SQNetEntity::destroyer(){
    #if defined(_WIN32) || defined(WIN32)
        WSACleanup();
    #endif
        SQNetEntity::_initializer = 0;
    }

    /**
     * Returns the entity kind. Each derived class overrides this function.
     * @return 
     */
    bool SQNetEntity::is_server()  {

        throw SQException("[is_server] This method can't be called from an SQNetEntity object.");

        return false; //this line void compilation failure
    }
    /**
     * Make a treatment on the incoming packet. This function is called before any callback function of
     * the events list.
     * This function is overriden by derivated classes.
     * @param packet
     * @return True to execute the default operations in the server chain right after on_read(). False
     * to interrupt the chain. 
     */
    bool SQNetEntity::on_read(SQ::SQPacket::SQPacket const& packet){ return true;  }
    void SQNetEntity::queryListener(SQFinalClient const& c){}
     
    bool SQNetEntity::bindExecute(SQ::SQPacket::SQPacket const& p, SQ::SQEvents::SQEventsList liste){  
           
        for(SQ::SQEvents::SQEvents ev : liste){  
            if(ev.getType() == SQ::SQEvents::SQEVTYPE_ALL){  //0111
                if(ev.getMessage() == SQ::PATCH::to_string((int)p.method())+'-'+SQ::PATCH::to_string((int)p.parameter())+'-'+p.data()){ 
                    return ev.run((void*)(&p));
                }
            }else if(ev.getType() == SQ::SQEvents::SQEVTYPE_M){  // 0100 
                 if(ev.getMessage() == SQ::PATCH::to_string((int)p.method())){ 
                    return ev.run((void*)(&p));
                }
            }else if(ev.getType() == SQ::SQEvents::SQEVTYPE_P){  //0010
                if(ev.getMessage() == SQ::PATCH::to_string((int)p.parameter())){ 
                    return ev.run((void*)(&p));
                }
            }else if(ev.getType() == SQ::SQEvents::SQEVTYPE_D){  //0001                 
                if(ev.getMessage() == p.data() ){ 
                    return ev.run((void*)(&p));
                }
            }else if(ev.getType() == (SQ::SQEvents::SQEVTYPE_M|SQ::SQEvents::SQEVTYPE_P)){ ///0110 not 0001 
                if(ev.getMessage() == SQ::PATCH::to_string((int)p.method())+'-'+SQ::PATCH::to_string((int)p.parameter())){  
                    return ev.run((void*)(&p));
                }
            }else if(ev.getType() == (SQ::SQEvents::SQEVTYPE_M|SQ::SQEvents::SQEVTYPE_D)){ ///0101 not 0010
                if(ev.getMessage() == SQ::PATCH::to_string((int)p.method())+'-'+p.data()){ 
                    return ev.run((void*)(&p));
                } 
            }else if(ev.getType() == (SQ::SQEvents::SQEVTYPE_P|SQ::SQEvents::SQEVTYPE_D)){                     
                 if(ev.getMessage() == SQ::PATCH::to_string((int)p.method())+'-'+SQ::PATCH::to_string((int)p.parameter())+'-'+p.data()){ 
                    return ev.run((void*)(&p));
                }                                     
            }else{
                throw SQException("[bind] The data type to track is not set.");
            }
        }
        return false;
    }
}
}