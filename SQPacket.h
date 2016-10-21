#ifndef INCLUDE_SQ_H
#define INCLUDE_SQ_H

#include <vector>
#include <utility>

#include "SQException.h"
#include "SQUtils.h"

namespace SQ{
namespace SQPacket{
  typedef unsigned char uchar;
  typedef std::vector< std::pair<std::string,std::string> > OptionsList;
	
	constexpr int UCHAR_MAX = 65535;

  constexpr int HEADERLENGTH = 11;

  class SQPacket{
  	private:
	    int _dest;
	    int _src;
	    uchar _method;
	    uchar _parameter; 
            OptionsList _optionslist; //<! list of options for this query
	    std::string _data;
		 

    public :
	    SQPacket();
	    SQPacket(int const& dest, int const& src, uchar const& method, uchar const& parameter, OptionsList list, std::string data );
	    SQPacket(SQPacket const& p);
	    ~SQPacket();

		inline int dest() const{return _dest;}
		inline void dest(int value) { _dest = value;}
		inline int src() const{return _src;}
		inline void src(int value) { _src = value;}
		inline uchar method() const{return _method;}
		inline void method(uchar value) { _method = value;}
		inline uchar parameter() const{return _parameter;}
		inline void parameter(uchar value) { _parameter = value;}
		inline int nbOptions() const{return _optionslist.size();}  
		inline OptionsList getOptionsList() const{return _optionslist;}
		inline void setOptionsList(OptionsList value) { _optionslist = value;}
		inline std::string data() const{return _data;}
		inline void data(std::string value) { _data = value;}
                
                SQPacket& clear();




  };

}
}

#endif //INCLUDE_SQ_H