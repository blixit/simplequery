#ifndef SQEVENTS_H
#define SQEVENTS_H

#include <vector>
#include <functional>
#include <iostream>



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
#ifndef INCLUDE_SQUTILS_H
#define INCLUDE_SQUTILS_H

#include <sstream>
#include <string>
#include <vector> 
#include <utility>

namespace SQ{
    class PATCH
    {
        public :
         template < typename T > 
         static std::string to_string( const T& n )
        {
            std::ostringstream stm ;
            stm << n ;
            return stm.str() ;
        }


        static int split(std::vector<std::string>& vecteur, std::string chaine, char separateur);
    };
  
    enum QueryResultatStatus{

    };

    enum QueryDefaultClientId{
        SERVER_ID = -1, //<! to identiy the server
        BRCAST_ID = -2, //<! to broadcast
        ANY_ID = -3 //<! to identiy anyone
    };

    enum QueryMethod{
        post = 1,
        get = 2,
        session = 3,
        sms = 4,
        file = 5,
        del = 6,
        invite = 7,
        update = 8,
        status = 9 //<! to give back the status of a query
    };

    enum QueryParameter{
        PARAM_SHUT_DOWN = 1,
        /*' status de la requete */
        PARAM_Q_BUSY = 2,
        PARAM_Q_END = 3,
        PARAM_Q_OK = 4,
        PARAM_Q_FALL = 5,

        /*objet de la requete get ou post ou update*/
        PARAM_NBCLIENT = 6,
        PARAM_PSEUDO = 7,
        PARAM_ID = 8,
        PARAM_NOTIFY = 9,
        PARAM_LISTCLIENTS = 10,

        PARAM_FILE = 11,

        PARAM_SESSION_START = 12,
        PARAM_SESSION_END = 13,

        /* objet de la requete invite*/
        PARAM_INV_SEND = 14,
        PARAM_INV_ACCEPT = 15,// 'recoit une invitation
        PARAM_INV_DECLINE = 16 // 'recoit une invitation

    };


}

#endif //INCLUDE_SQUTILS_H
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

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SQPacket.h
 * Author: Blixit 
 *
 * Created on 1 novembre 2016, 22:56
 */

#ifndef SQPACKET_H
#define SQPACKET_H

namespace SQ{
namespace SQPacket{
    
    typedef unsigned char uchar; 
    constexpr int UCHAR_MAX = 65535;
    constexpr int HEADERLENGTH = 11;
    
class SQPacket {
public:
    SQPacket();
    SQPacket(const SQPacket& orig);
    virtual ~SQPacket();
private:

};

}
}

#endif /* SQPACKET_H */

#ifndef INCLUDE_SQ_H
#define INCLUDE_SQ_H

#include <vector>
#include <utility>
#include <sstream> 


namespace SQ{
namespace SQPacket{

  class SQStringPacket : public SQ::SQPacket::SQPacket{
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
#ifndef SQCOMMUNICATOR_H
#define SQCOMMUNICATOR_H

#include <memory>

#if defined(_WIN32) || defined(WIN32)
    #include <winsock2.h> //socket
    #include <unistd.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <errno.h>

    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1

    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#endif


namespace SQ{
namespace SQCommunicator{

    constexpr int BUFFER_MINSIZE = 32; //<! this size contains SQPacket::HEADERLENGTH
    constexpr int BUFFER_MEANSIZE = 1024; //<! the size to use by default
    constexpr int BUFFER_MAXSIZE = 4194304; //<! = 4Mo. This size allows small files transfer

	class SQCommunicator
	{
		private:
			SOCKET _sock; //<! socket
			int _time; //<! reception time
			int _size; //<! datagramm size
			int _is_server; //<! define server or client side
			std::string _datagram; //<! datagram
			//std::shared_ptr<SQ::SQPacket::SQPacket> _ptrPacket;
 

		public:
		    int _buffersize; //<! user chosen buffer size

			SQCommunicator(SOCKET const& s);
			~SQCommunicator();

			inline SOCKET sock() {return _sock; };
			inline void sock(SOCKET const& value) {_sock = value; };
			inline int time() {return _time; };
			inline void time(int const& value) {_time = value; };
			inline int size() {return _size; };
			inline void size(int const& value) {_size = value; };
			inline std::string datagram() {return _datagram; };
			inline void datagram(std::string const& value) {_datagram = value; };

			void build(SQ::SQPacket::SQStringPacket const& packet);
			void extract(SQ::SQPacket::SQStringPacket & packet); 

			void read(SQ::SQPacket::SQStringPacket & packet, int const& buffersize = BUFFER_MEANSIZE) ;
			void write(SQ::SQPacket::SQStringPacket const& packet, int const& buffersize = BUFFER_MEANSIZE) ;

            SQCommunicator &operator=(SQCommunicator const &) = delete;
	};

}
}

#endif
#ifndef SQNETENTITY_H
#define SQNETENTITY_H

#include <memory>
#include <atomic>
#include <queue>

namespace SQ{
namespace SQNetEntity{

    enum EntitySide{
        isClient = 0,
        isServer = 1
    }; 
    
    class SQFinalClient{
      public:
        int id;
        SOCKET sock;
        
        SQFinalClient(int const& i, SOCKET const& s) : id(i), sock(s){};
        SQFinalClient(SQFinalClient const& c) : id(c.getId()), sock(c.getSock()){};
        
        bool operator==(SQFinalClient const& rhs) const { return this->id == rhs.id && this->sock == rhs.sock; };
        
        int getId() const { return id;}
        void setId(int id) { this->id = id;}
        SOCKET getSock() const { return sock;}
        void setSock(SOCKET s) { this->sock = s;}
        
    };
    
    class SQNetEntity
    {
        public:
            SQNetEntity();
            virtual ~SQNetEntity();
            
            static void initializer();
            static void destroyer();

            virtual bool is_server();

            inline bool isRunning(){return static_cast<bool>(_is_running.load(std::memory_order_relaxed)); }
            inline void isRunning(bool const& value){ _is_running.store(static_cast<int>(value),std::memory_order_relaxed); }
            inline SQ::SQEvents::SQEventsList getEvents() {return events; } 
            inline void setEvents(SQ::SQEvents::SQEventsList const& value) { events = value; }
            std::shared_ptr<SQCommunicator::SQCommunicator> com(){ return _com ; }
            SQCommunicator::SQCommunicator* ptrCom(){ return _com.get(); }
                        
            virtual bool on_read(SQ::SQPacket::SQStringPacket const& packet)=0;
            
            virtual void queryListener(SQFinalClient const& c)=0; 
            static bool bindExecute(SQ::SQPacket::SQStringPacket const& p, SQ::SQEvents::SQEventsList liste);  

            bool DEBUGSTATE = false;

        protected:
            std::shared_ptr<SQCommunicator::SQCommunicator> _com;

        private:
            static int _initializer;
            std::atomic<int> _is_running;
            std::queue<int> _receivedPackets; 
            SQ::SQEvents::SQEventsList events;
    };
}
}
#endif // SQNETENTITY_H
#ifndef SQCLIENT_H
#define SQCLIENT_H


namespace SQ{
namespace SQNetEntity{
    class SQClient : public SQNetEntity
    {
        public:
            SQClient() ;
            virtual ~SQClient();
            SQClient(const SQClient& other);
            SQClient& operator=(const SQClient& other);

            virtual bool is_server();

            void sconnect(std::string const& ip, int const& port);
            void sdisconnect();
            
            SQFinalClient getAuthId(SOCKET const& s);
            
            virtual void on_connect(SQFinalClient const& c)=0;
            virtual void on_leave(SOCKET const& s)=0;
            virtual bool on_read(SQ::SQPacket::SQStringPacket const& packet){return true; };

        protected:

        private:
            void queryListener(SQFinalClient const& c);
    };
}
}
#endif // SQCLIENT_H
#ifndef SQSERVER_H
#define SQSERVER_H

#include <atomic>
#include <mutex>
#include <queue>
#include <random> 


namespace SQ{
namespace SQNetEntity{
    constexpr int DEFAULT_MAX_CLIENT = 1024; //<! arbitrary max value
    constexpr int DEFAULT_CLIENT_NUMBER = 1; //<! arbitrary value, can be changed
  
    constexpr int DEFAULT_PORT = 1607; //<! arbitrary value, can be changed  
  
    class SQServer : public SQNetEntity 
    {
        public:
            SQServer(int const& portnum = DEFAULT_PORT);
            virtual ~SQServer();
            SQServer(const SQServer& other);
            SQServer& operator=(const SQServer& other);

            virtual bool is_server();
            
            inline int port() const{ return _port;}
            inline void port(int const& value) { _port = value;}
            inline SOCKET sock() const{ return _sock;}
            
            inline int maxClients() const{ return _maxClients;}
            inline void maxClients(int const& value) { _maxClients = value > DEFAULT_MAX_CLIENT ? DEFAULT_MAX_CLIENT : (value < DEFAULT_CLIENT_NUMBER ? DEFAULT_CLIENT_NUMBER : value ) ;}
            inline int nbClients(){return _nbClients.load(std::memory_order_relaxed); }
            std::vector<SQFinalClient> getClients(){ return this->_clients; }
            void setClients(std::vector<SQFinalClient> c){ this->_clients = std::move(c); }

            int startService();
            void stopService();            
            
            virtual void on_connect(SQFinalClient const& c)=0;
            virtual void on_leave(SOCKET const& s)=0;
            virtual bool on_read(SQ::SQPacket::SQStringPacket const& packet){return true; };
            

        protected:

        private:   
                        
            int _maxClients;
            std::atomic<int> _nbClients;
            std::queue<int> _ticketQueue;
            std::vector<SQFinalClient> _clients;
            
            std::mutex _mutex; 
            
            int _port;
            SOCKET _sock;
                         
            inline void sock(SOCKET const& value) { _sock = value;}
            inline void nbClients(int const& value){ _nbClients.store(value,std::memory_order_relaxed); }
            
            inline bool addClientToQueue(){ if(nbClients()<DEFAULT_MAX_CLIENT) {_nbClients.store(nbClients()+1,std::memory_order_relaxed); return true; }else{return false;} }
            inline void delClientToQueue(){ if(nbClients()>0) _nbClients.store(nbClients()-1,std::memory_order_relaxed); }
            
            SOCKET waitClient();
            void getTicket(); 
            void queryListener(SQFinalClient const& c); 
            
            void sdisconnect(SOCKET const& s);
            void authentify(SQFinalClient const& client);
    };
}
}
#endif // SQSERVER_H
