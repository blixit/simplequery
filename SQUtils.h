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
