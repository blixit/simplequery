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

#include "SQDataHeaders.h"


namespace SQ{
namespace SQPacket{
    
    typedef unsigned char uchar; 
    constexpr int UCHAR_MAX = 65535;
    constexpr int HEADERLENGTH = 11;

    template<typename T>
    class SQPacket {
    private:
        int _dest;
        int _src;
        uchar _method;
        uchar _parameter; 
        SQDataHeaders _optionslist; //<! list of options for this query
        T _data;
        
    public:
        template<T>
        SQPacket(){};
        template<T>
        SQPacket(int const& d, int const& s, uchar const& m, uchar const& p, SQDataHeaders const& liste, T data);
        template<T>
        SQPacket(const SQPacket<T>& orig);
        virtual ~SQPacket(); 

    };

}
}

#endif /* SQPACKET_H */

