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
    void SQNetEntity::on_read(SQ::SQPacket::SQPacket const& packet){   }
    
    /**
     * This function listens messages from the network while the server is running.
     */
    void SQNetEntity::queryListener(){
      if(_com.get() == nullptr){
        throw SQException("[queryListener] The NetEntity communicator agent has not been initialized. You should launch an connection on client side or start the server.");
      }
      while(isRunning()){
        try{
          //packet p
          //comm.read(p);
          //cout << comm.requete() << endl;
          /*if(_bind)
            comm.bind(p);  */  
					
        }catch(SQException &e){
          //if(comm.requete().length()==0)
            isRunning(false); 
        }catch(std::exception &e){
          Log(std::string(e.what()),DEBUGSTATE); 
        }
      }
  }

}
}