#ifndef SQSERVER_H
#define SQSERVER_H

#include <atomic>
#include <mutex>
#include <queue>
#include <random> 

#include "SQNetEntity.h"

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