//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2019.  Ford Motor Company owns all rights to this work and intends
// to maintain it in confidence to preserve its trade secret status.
// Ford Motor Company reserves the right to protect this work as an
// unpublished copyrighted work in the event of an inadvertent or
// deliberate unauthorized publication.  Ford Motor Company also
// reserves its rights under the copyright laws to protect this work
// as a published work.  Those having access to this work may not copy
// it, use it, or disclose the information contained in it without
// the written authorization of Ford Motor Company.
//
/*
 * soa_typeid_demangle.hpp
 *
 *  Created on: Mar 19, 2019
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_UTILS_SOA_TYPEID_DEMANGLE_HPP_
#define INCLUDE_SOA_UTILS_SOA_TYPEID_DEMANGLE_HPP_

#include <string>
#include <memory>
#include <cstdlib>
#include <cxxabi.h>

namespace fnv {
namespace soa {

/**
 * demangle a mangled class name
 * This is defined as a template so that it can be
 * implicitly instantiated as often as needed with
 * a common implementation
 * @param name mangled name
 * @return the demangled name with namespace removed
 */
template <bool = true>
std::string typeid_demangle( const char * name ) {
    // adapted from https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#if __GNUG__  // this applies to the GCC C++ compiler
    int status = 1;  // the value is arbitrary, just to calm static checkers
    std::unique_ptr<char, void(*)(void*)> pretty {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };
    std::string strname( !status ? pretty.get() : name );
    // remove namespace, with support for template instantiation types
    std::string front;
    size_t anglePos = std::string::npos;
    do {    // remove namespace from template names and concrete type names
        // look for an angle bracket or comma, remove namespace before it
        anglePos = strname.find('<');
        size_t commaPos = strname.find(',');
        if( commaPos < anglePos ) {
            anglePos = commaPos;
        }
        size_t colonPos = strname.rfind(':', anglePos);
        if( colonPos != std::string::npos ) {           // : found
            strname = strname.substr( colonPos + 1 );
        }
        // special case if angle bracket found
        if(anglePos != std::string::npos) {             // < found
            size_t searchPos;
            if(colonPos != std::string::npos) {         // : found
                searchPos = anglePos - colonPos;
            } else {                                    // no : found
                searchPos = anglePos + 1;
            }
            // build and save identifier left of the angle bracket
            front = front + strname.substr(0, searchPos);
            // continue removing namespaces right of angle bracket
            strname = strname.substr(searchPos);
        }
    } while (anglePos != std::string::npos);
    strname = front + strname;
#else
    std::string strname( name );
#endif
    return strname;
}


} // namespace soa
} // namespace fnv

#endif /* INCLUDE_SOA_UTILS_SOA_TYPEID_DEMANGLE_HPP_ */
