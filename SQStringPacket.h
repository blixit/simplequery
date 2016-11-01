#ifndef INCLUDE_SQ_H
#define INCLUDE_SQ_H

#include <vector>
#include <utility>
#include <sstream> 

#include "SQException.h"
#include "SQUtils.h"
#include "SQDataHeaders.h"

namespace SQ{
namespace SQPacket{
  typedef unsigned char uchar; 
	
	constexpr int UCHAR_MAX = 65535;

  constexpr int HEADERLENGTH = 11;

  class SQStringPacket{
  	private:
	    int _dest;
	    int _src;
	    uchar _method;
	    uchar _parameter; 
            SQDataHeaders _optionslist; //<! list of options for this query
	    std::string _data;
		 

    public :
	    SQStringPacket();
	    SQStringPacket(int const& dest, int const& src, uchar const& method, uchar const& parameter, SQDataHeaders list, std::string data );
	    SQStringPacket(SQStringPacket const& p);
	    ~SQStringPacket();

		inline int dest() const{return _dest;}
		inline void dest(int value) { _dest = value;}
		inline int src() const{return _src;}
		inline void src(int value) { _src = value;}
		inline uchar method() const{return _method;}
		inline void method(uchar value) { _method = value;}
		inline uchar parameter() const{return _parameter;}
		inline void parameter(uchar value) { _parameter = value;}
		inline int nbOptions() const{return _optionslist.size();}  
		inline SQDataHeaders getOptionsList() const{return _optionslist;}
		inline void setOptionsList(SQDataHeaders value) { _optionslist = value;}
		inline std::string data() const{return _data;}
		inline void data(std::string value) { _data = value;}
                
                SQStringPacket& clear();
                std::string toString(){
                    std::ostringstream ss;
                    ss << dest() << " " << src() << " " << (int)method() << " " << (int)parameter() << " " << data();
                    return ss.str();
                }




  };

}
}

#endif //INCLUDE_SQ_H