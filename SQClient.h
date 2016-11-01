#ifndef SQCLIENT_H
#define SQCLIENT_H

#include "SQNetEntity.h"

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