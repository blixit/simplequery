#ifndef SQNETENTITY_H
#define SQNETENTITY_H

#include <memory>
#include <atomic>
#include <queue>
#include "SQCommunicator.h"
#include "SQEvents.h"

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
                        
            virtual bool on_read(SQ::SQPacket::SQPacket const& packet)=0;
            
            virtual void queryListener(SQFinalClient const& c)=0; 
            static bool bindExecute(SQ::SQPacket::SQPacket const& p, SQ::SQEvents::SQEventsList liste);  

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