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
 * soa_allocator_cache.hpp
 *
 *  Created on: Jan 18, 2019
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_UTILS_SOA_ALLOCATOR_CACHE_HPP_
#define INCLUDE_SOA_UTILS_SOA_ALLOCATOR_CACHE_HPP_

#include <mutex>
#include <queue>
#include <memory>
#include <string>
#include <unordered_set>
#include <soa/utils/soa_typeid_demangle.hpp>
#include <soa/utils/soa_frameworks_logging.hpp>
#include <thread-util/Fnv2log.hpp>

namespace fnv {
namespace soa {

/**
 * This class template implements an object cache container which grows on
 * demand for arbitrary object types.
 * This cache is used to minimize dynamic memory allocation on the heap (new
 * and delete operations) by allocating from heap memory only when needed for
 * user requests, and caching the object sized chunks of memory for subsequent
 * requests when released by the user code. When the cache is destroyed, all
 * memory allocated on the heap is freed.
 * Use this cache for frequently allocated and freed objects of a particular
 * type, such as context objects used briefly for quick operations.
 * The API will allocate and construct the object of the template instantiation
 * argument class when needed, and destroy (call the destructor) the object
 * when released.
 *
 * IMPORTANT #1:
 * As this cache is designed to improve performance for frequently allocated
 * and freed memory blocks, the expectation is that there is a typical normal
 * runtime limit on the number of concurrently allocated blocks.
 * For example, the normal runtime limit may be 10 blocks in use at any time.
 * But there may be times when bursts of allocations drive the number of
 * concurrent allocations to an exceptionally high number. For example, due to
 * inter-thread message queuing, the number of in-use blocks may exceed 500 or
 * 1000 or even 2000. If all new allocations were unconditionally cached when
 * freed (which is how this cache fundamentally works, the cache runs the risk
 * of being a memory hog.
 * For this reason, each type cache has a maximum limit specified by a static
 * maximum scoped within the class. This maximum must be defined in a .cpp file
 * as follows, where you will replace n with a hard coded integer (ex. 10)
 *
 *    template <> const size_t DynamicObjectCache<Type>::MAX_CACHE_SIZE = n
 *
 * API:
 * Type * construct(Args ...) is used to replace the new operator
 * void destroy(Type * ptr)   is used to replace the delete operator
 *
 * See below for replacements for new, delete and std::make_shared as in ...
 *   Type * tp = new Type(...)
 *   delete tp
 *   std::shared_ptr<Type> tsp = std::make_shared<Type>(...)
 *
 */

template <class Type>
class DynamicObjectCache {
    public:
        static const size_t MAX_CACHE_SIZE;
        static const std::string TAG;
        /**
         * Get memory for- and construct an object of type Type
         */
        template <class ... Args>
        Type * construct(Args && ... args) {  // NOSONAR 20190122 PR Needed for speed
            std::unique_lock<std::mutex> mlock(m_cacheGuard);
            Type * objPtr;
            if (m_freeCache.empty()) {
                // no objects available, create a new one
                objPtr = static_cast<Type *>(m_alloc.allocate(1));
                FNV2TRACE(fnv::soa::framework::getSoaFrmwkLogger(), "%s::%s<%s> at 0x%p, in use %lu", CLASS_NAME, __FUNCTION__, m_name.c_str(), objPtr, (long unsigned int)(m_inUseSet.size()) + 1 );
            } else {
                // reuse the first available object in the cache
                objPtr = m_freeCache.front();
                m_freeCache.pop();
                FNV2TRACE(fnv::soa::framework::getSoaFrmwkLogger(), "%s::%s<%s> reuse at 0x%p, free %lu used %lu", CLASS_NAME, __FUNCTION__, m_name.c_str(), objPtr, (long unsigned int)(m_freeCache.size()), (long unsigned int)(m_inUseSet.size()) + 1);
            }
            m_alloc.construct(objPtr, std::forward<Args>(args)...);
            m_inUseSet.insert( objPtr );
            return objPtr;
        }

        /**
         * Destroy object and free the memory (return to cache)
         */
        void destroy(Type * objPtr) {
            std::unique_lock<std::mutex> mlock(m_cacheGuard);
            auto it = m_inUseSet.find( objPtr );
            if ( it != m_inUseSet.end() ){
                m_alloc.destroy( *it );
                if(sizeof(Type) >= 4 ){
                    unsigned char * fillPattern = (unsigned char *)(void * )objPtr;
                    fillPattern[0]=5;
                    fillPattern[1]=5;
                    fillPattern[2]=5;
                    fillPattern[3]=5;
                }
                if (m_freeCache.size() < MAX_CACHE_SIZE) {  // cache not at max yet
                    m_freeCache.push( *it );
                    FNV2TRACE(fnv::soa::framework::getSoaFrmwkLogger(), "%s::%s<%s> returned 0x%p to cache, free %lu used %lu", CLASS_NAME, __FUNCTION__, m_name.c_str(), objPtr, (long unsigned int)(m_freeCache.size()), (long unsigned int)(m_inUseSet.size()) - 1);
                } else {        // cache at max, give back to OS
                    m_alloc.deallocate( *it, 1 );
                    FNV2TRACE(fnv::soa::framework::getSoaFrmwkLogger(), "%s::%s<%s> returned 0x%p to OS, free %lu used %lu", CLASS_NAME, __FUNCTION__, m_name.c_str(), objPtr, (long unsigned int)(m_freeCache.size()), (long unsigned int)(m_inUseSet.size()) - 1);
                }
                m_inUseSet.erase( it );
            } else {
                FNV2LOGW(fnv::soa::framework::getSoaFrmwkLogger(), "%s::%s<%s> tried to destroy unused object at 0x%p", CLASS_NAME, __FUNCTION__, m_name.c_str(), objPtr );
            }
        }

        /**
         * These methods return the number of free and in-use
         * blocks of memory allocated. These methods are used by unit test
         * code to validate the behaviour of this class template.
         */
        size_t sizeFree() {
            std::unique_lock<std::mutex> mlock(m_cacheGuard);
            return m_freeCache.size();
        }
        size_t sizeUsed() {
            std::unique_lock<std::mutex> mlock(m_cacheGuard);
            return m_inUseSet.size();
        }

    protected:
        /**
         * cache container constructor
         */
        DynamicObjectCache()
            : m_name( typeid_demangle( typeid(Type).name() ) )
        {
            // nothing else to do
        }
        /**
         * cache container destructor
         */
        virtual ~DynamicObjectCache() {
            std::unique_lock<std::mutex> mlock(m_cacheGuard);
            // call destructor on all initialized objects and move to free list
            for(auto it = m_inUseSet.begin(); it != m_inUseSet.end(); ) {
                m_freeCache.push( *it );
                m_alloc.destroy( *it );
                it = m_inUseSet.erase( it );
            }
            // deallocate memory for each pointer on free list
            for(; ! m_freeCache.empty(); ) {
                Type * objPtr = m_freeCache.front();
                m_freeCache.pop();
                m_alloc.deallocate( objPtr, 1 );
            }
        }

    private:
        std::string                 m_name;         ///< name of Type, used for logging
        std::allocator<Type>        m_alloc;        ///< used to separate construction from allocation, etc
        std::queue<Type*>           m_freeCache;    ///< list of allocated but available objects
        std::unordered_set<Type*>   m_inUseSet;     ///< set of constructed objects
        std::mutex                  m_cacheGuard;   ///< mutex for modifying the cache containers
        static constexpr auto CLASS_NAME = "DynamicObjectCache";
};


/**
 * Only one allocator cache should exist within the process per
 * allocated object type, so
 * the instantiated dynamic object cache class template
 * is wrapped via inheritance to implement a singleton
 * pattern where the singleton is allocated on the heap.
 */
template <class Type>
class AllocatorCache :
        public DynamicObjectCache<Type> {
    public:
        /**
         * Get a reference to the allocator cache singleton
         */
        static AllocatorCache & getInstance() {
            static AllocatorCache & cache = * (new AllocatorCache());
            return cache;
        }

        /**
         * Destructor - as a heap-based singleton, this is not called
         */
        virtual ~AllocatorCache() = default;

    private:
        /**
         * Constructor
         */
        AllocatorCache()
           : DynamicObjectCache<Type>() {
            // nothing else
        }
};


// The following function templates require that the
// AllocatorCache<Type> class tmplate is instantiated in a source (.cpp)
// file. Example:
// template class AllocatorCache< MyType >

/**
 * Replacement for the new Type(...) operator used to allocate
 * and construct objects through this cache
 *
 * IMPORTANT:
 * Objects allocated by this method SHOULD NOT be deleted using the
 * default C++ delete operator. If that's what you intend to do, then
 * simply use the default C++ new operator to allocate your object.
 *
 * @param args ... Any arguments which can be passed to a Type constructor
 * @return a raw pointer to a constructed, newly heap-allocated Type object
 */
template <class Type, class ... Args>
static Type * cached_new(Args && ... args) { // NOSONAR 20190122 PR Needed for speed
    return AllocatorCache<Type>::getInstance().construct(std::forward<Args>(args)...);
}

/**
 * Custom deleter used for objects allocated via
 * this cache.
 *
 * IMPORTANT:
 * This method MUST NOT be used on objects not allocated by
 * cached_new<Type>(...) as it will ignore the request resulting
 * in a memory leak, where you may have expected the object to be
 * added to the cache or deleted using the default delete operator.
 * Arbitrary objects will not be placed in the cache using this method.
 * A warning is logged for any such attempts.
 *
 * This function will not crash if a pointer is deleted more once, or
 * if the pointer is the nullptr.
 *
 * @param pObject - raw pointer to object to be deleted
 */
template <class Type>
static void cached_delete( Type * pObject ) {
    AllocatorCache<Type>::getInstance().destroy( pObject );
}

/**
 * Replacement for std::make_shared<Type>(...) which will allocate
 * objects on the heap only when needed. Otherwise, free storage is
 * used from the cache.
 * @param args ... Any arguments which can be passed to a Type constructor
 * @return a std::shared_ptr<Type> (containing a non-null raw pointer to a
 *                                  constructed, newly heap-allocated Type
 *                                  object)
 */
template <class Type, class ... Args>
static std::shared_ptr<Type> cached_make_shared(Args && ... args) { // NOSONAR 20190122 PR Needed for speed
        Type * pType = AllocatorCache<Type>::getInstance().construct(std::forward<Args>(args)...);
        return std::shared_ptr<Type>( pType, cached_delete<Type> );
}

} // namespace soa
} // namespace fnv

#endif /* INCLUDE_SOA_UTILS_SOA_ALLOCATOR_CACHE_HPP_ */
