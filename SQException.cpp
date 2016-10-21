#include <fstream>

#include "SQUtils.h"
#include "SQException.h"

namespace SQ{
  
  std::string SQException::LOGSTR(std::string str, std::string const& function, int const& line, std::string const& file){
    return  file + ":" + SQ::PATCH::to_string(line) + " : " + function + " :  " + str + "\n";
  }
  
  void SQException::LOG( std::string str, std::string const& OUT){
    std::ofstream of(OUT, std::ofstream::out | std::ofstream::app);
    if(of.is_open()){
      of << str;
      of.close();
    }else{
      throw SQException(LOGSTR("The log operation into " + OUT + " failed.").c_str());
    }
  }
}