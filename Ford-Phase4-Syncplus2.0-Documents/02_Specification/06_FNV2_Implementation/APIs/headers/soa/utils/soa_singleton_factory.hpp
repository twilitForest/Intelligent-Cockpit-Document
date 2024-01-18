/*
 * CONFIDENTIAL - FORD MOTOR COMPANY
 *
 * This is an unpublished work, which is a trade secret, created in
 * 2019.  Ford Motor Company owns all rights to this work and intends
 * to maintain it in confidence to preserve its trade secret status.
 * Ford Motor Company reserves the right to protect this work as an
 * unpublished copyrighted work in the event of an inadvertent or
 * deliberate unauthorized publication.  Ford Motor Company also
 * reserves its rights under the copyright laws to protect this work
 * as a published work.  Those having access to this work may not copy
 * it, use it, or disclose the information contained in it without
 * the written authorization of Ford Motor Company.
 */
/*
 * soa_singleton_factory.hpp
 *
 *  Created on: Jun 20, 2019
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_UTILS_SOA_SINGLETON_FACTORY_HPP_
#define INCLUDE_SOA_UTILS_SOA_SINGLETON_FACTORY_HPP_

#include <type_traits>
#include <utility>

namespace fnv {
namespace soa {
namespace framework {


/*
 * The template class (actually a struct) SingletonFactory
 * is used to create a singleton object using memory allocated at compile time,
 * where the object will not be destroyed when the process terminates.
 * This provides the benefit of a heap-based singleton without the need
 * to use dynamic memory.
 * This implementation uses placement new to call the constructor
 * This implementation is MISRA C++:2008 compliant.
 *
 * To use this class, code the singleton's instance static getter like this ...
 *
 * MyType & MyType::instance(void) {
 *     static ::fnv::soa::framework::SingletonFactory<MyType> singletonFactory
 *     static MyType & instance = singletonFactory.create()
 *     return instance
 * }
 *
 * As a singleton implementation, MyType should have a private constructor.
 * You'll need to make the allocator created by this template a friend of
 * your type. Add the following line in the class definition for MyType
 *
 *     friend class ::fnv::soa::framework::SingletonFactory<MyType>
 *
 */

template <class Type>
struct SingletonFactory {
    template <class ... Args>
    Type & create(Args && ... args) { /* NOSONAR 20190620 PR rvalue reference to support MISRA requirements */
        /* memory allocation for the singleton */
        typedef typename std::aligned_storage<sizeof(Type)>::type memory_block_t;
        static memory_block_t memoryBlock;
        return * ::new (static_cast<void*>(&memoryBlock)) Type(std::forward<Args>(args)...); /* NOSONAR 20190620 PR placement new to support MISRA requirements */
    }
};

} /* namespace framework */
} /* namespace soa       */
} /* namespace fnv       */


#endif /* INCLUDE_SOA_UTILS_SOA_SINGLETON_FACTORY_HPP_ */
