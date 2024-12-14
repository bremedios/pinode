//
// Created by Bradley Remedios on 11/24/24.
//
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

//#define __ENABLE_PINODE_DEBUG       1
//#define __ENABLE_PINODE_DEBUG_ALL   1
//#define __ENABLE_PINODE_TEMP_DEBUG  1
//#define __ENABLE_PINODE_CLIENT_DEBUG  1
#define __ENABLE_PINODE_HW_DEBUG    1

#define ERROR_MSG(msg)  { std::cerr << "ERROR:" <<__PRETTY_FUNCTION__ << ":" << msg << std::endl; }

#if defined(__ENABLE_PINODE_DEBUG)
    #if defined(__ENABLE_PINODE_DEBUG_ALL)
        #define DEBUG_MSG(msg)  { std::cout << "DEBUG:" <<  __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
    #else
        #define DEBUG_MSG(cmd)
    #endif

    #if defined(__ENABLE_PINODE_TEMP_DEBUG)
        #define DEBUG_TEMP_MSG(msg)  { std::cout << "DEBUG:" <<  __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
    #else
        #define DEBUG_TEMP_MSG(cmd)
    #endif

    #if defined(__ENABLE_PINODE_CLIENT_DEBUG)
        #define DEBUG_CLIENT_MSG(msg)  { std::cout << "DEBUG:" <<  __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
    #else
        #define DEBUG_CLIENT_MSG(cmd)
    #endif

    #if defined(__ENABLE_PINODE_HW_DEBUG)
        #define DEBUG_HW_MSG(msg)  { std::cout << "DEBUG:" <<  __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
    #else
        #define DEBUG_HW_MSG(cmd)
    #endif
#else
    #define DEBUG_MSG(cmd)
    #define DEBUG_TEMP_MSG(cmd)
    #define DEBUG_CLIENT_MSG(cmd)
    #define DEBUG_HW_MSG(cmd)
#endif // __ENABLE_PINODE_DEBUG

#endif //DEBUG_H