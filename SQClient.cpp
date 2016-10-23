#include <string> //to_string

#include "SQClient.h"


namespace SQ{
namespace SQNetEntity{

    SQClient::SQClient()   {
        isRunning(false);
    }

    SQClient::~SQClient()    {
    }

    SQClient::SQClient(const SQClient& other)  {
        //copy ctor
    }

    SQClient& SQClient::operator=(const SQClient& rhs)    {
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        return *this;
    }

    bool SQClient::is_server() { return static_cast<bool>(SQ::SQNetEntity::isClient); }

    /**
        \brief Connection to a server
        \warning this function use sock_stream-af_inet socket
    */
    void SQClient::sconnect(std::string const& ip, int const& port) {

        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        SOCKADDR_IN sin;

        if(s == INVALID_SOCKET)
			throw SQException("The client initialization failed.");

        _com = std::shared_ptr<SQCommunicator::SQCommunicator>(new SQCommunicator::SQCommunicator(s));

		sin.sin_addr.s_addr = inet_addr(ip.c_str()); // * l'adresse se trouve dans le champ h_addr de la structure hostinfo * /
		sin.sin_port = htons(port); // * on utilise htons pour le port * /
		sin.sin_family = AF_INET;

		if(connect(s,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR){
            std::string msg = "The connection to [" + ip + ":" + SQ::PATCH::to_string(port) + "] failed.";
			throw SQException(msg.c_str());
		}

		isRunning(true);

	}

    void SQClient::sdisconnect(){
        if(_com.get()){
        close(_com.get()->sock());
        _com.get()->sock(-1);
        }
    }
    
    void SQClient::queryListener(SQFinalClient const& c){
        //
    }
}
}