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

        protected:

        private:
    };
}
}
#endif // SQCLIENT_H