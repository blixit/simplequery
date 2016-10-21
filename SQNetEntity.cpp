#include "SQNetEntity.h"

namespace SQ{
namespace SQNetEntity{ 
	
    SQNetEntity::SQNetEntity()  {
        //ctor
    }

    SQNetEntity::~SQNetEntity()  {
        //dtor
    }

    bool SQNetEntity::is_server()  {

        throw SQException("[is_server] : This method can't be called from an SQNetEntity object.");

        return false; //this line void compilation failure
    }
  
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