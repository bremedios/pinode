//
// Created by Bradley Remedios on 11/24/24.
//
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#define __ENABLE_NET_DEBUG    1

#define ERROR_MSG(msg)  { std::cerr << __PRETTY_FUNCTION__ << ":" << msg << std::endl; }

#if defined(__ENABLE_NET_DEBUG)
    #define DEBUG_MSG(msg)  { std::cout << __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
#else
    #define DEBUG_MSG(cmd)
#endif // __ENABLE_NET_DEBUG

#endif //DEBUG_H