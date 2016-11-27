/* 
 * File:   SQPacket.cpp
 * Author: Blixit 
 * 
 * Created on 1 novembre 2016, 22:56
 */

#include "SQPacket.h"


namespace SQ{
namespace SQPacket{
/*     
SQPacket::SQPacket<T>() {
}*/

    template<class T>
SQPacket<T>::SQPacket(int const& d, int const& s, uchar const& m, uchar const& p, SQDataHeaders const& liste, T data){
    //
}

template<typename T>
SQPacket::SQPacket(const SQPacket<T>& orig) {
}

SQPacket::~SQPacket() {
}

}
}

