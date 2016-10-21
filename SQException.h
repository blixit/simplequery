#ifndef SQEXCEPTION_H
#define SQEXCEPTION_H

#include <iostream>
#include <exception>

#define  Log(XXX,state) SQ::SQException::LOG(SQ::SQException::LOGSTR(XXX,__FUNCTION__, __LINE__, __FILE__)); \
                        if(state) std::cout << SQ::SQException::LOGSTR(XXX,__FUNCTION__, __LINE__, __FILE__) << std::endl;

namespace SQ{ 
  const std::string DEFAULTLOG = "sqlog"; 
  
    class SQException : public std::exception{
        public: 
            
            SQException(const char* message) throw(): _msg(message) {
                //ctor
            }
            virtual ~SQException() throw(){
                //dtor
            }
            virtual const char * what() const throw() {
                return _msg.c_str();
            }
      
            static std::string LOGSTR(std::string str, std::string const& function = __FUNCTION__, int const& line = __LINE__, std::string const& file = __FILE__);
            static void LOG( std::string str, std::string const& OUT = DEFAULTLOG);


        protected:

        private:
            std::string _msg;
    };
}

#endif // SQEXCEPTION_H