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

#include "SQPacket.h"

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

			void wsaInit();
            void wsaClean();

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

			void build(SQ::SQPacket::SQPacket const& packet);
			//void extract(SQ::SQPacket::SQPacket & packet){};
                        void extract(char* msg,SQ::SQPacket::SQPacket & packet);

			void read(SQ::SQPacket::SQPacket & packet, int const& buffersize = BUFFER_MEANSIZE) ;
			void write(SQ::SQPacket::SQPacket const& packet, int const& buffersize = BUFFER_MEANSIZE) ;

            SQCommunicator &operator=(SQCommunicator const &) = delete;
	};

}
}

#endif