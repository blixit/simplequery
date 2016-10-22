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
            inline SQ::SQEvents::SQEventsList getEvents() { return std::move(events); } 
            inline void setEvents(SQ::SQEvents::SQEventsList const& value) { events = value; }
            std::shared_ptr<SQCommunicator::SQCommunicator> com(){ return _com ; }
            SQCommunicator::SQCommunicator* ptrCom(){ return _com.get(); }
                        
            virtual void on_read(SQ::SQPacket::SQPacket const& packet)=0;
            
            void queryListener();

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