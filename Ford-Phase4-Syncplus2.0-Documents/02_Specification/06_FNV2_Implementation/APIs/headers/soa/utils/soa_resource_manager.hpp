//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2017.  Ford Motor Company owns all rights to this work and intends
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
 * soa_resource_manager.hpp
 *
 *  Created on: Aug 14, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_UTILS_SOA_RESOURCE_MANAGER_HPP_
#define INCLUDE_UTILS_SOA_RESOURCE_MANAGER_HPP_

#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>
#include <functional>
#include <thread-util/MonotonicCondVar.hpp>
#include <soa/framework/soa_listener_lifecycle_helper.hpp> // made available to users
#include <soa/utils/soa_singleton_factory.hpp>
#include <pthread.h>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Forward declaration of entry type
 */
struct ResourceMapEntry;

/**
 * Class container for alive SoaResourceManager::Resource resources.
 * A "Resource" instance adds itself to the container when created and deletes when deleted.
 * To be trackable a resource class should derive from SoaResourceManager::Resource
 */
class SoaResourceManager {
public:
    /**
     * Trackable resource class
     *
     * IMPORTANT !!!
     * User defined classes that derive from this class MUST call
     * this->removeResource()
     * in their destructor to ensure there are no race conditions with respect
     * to accesses to the object and the destruction of the object.
     * Any derived user class that doesn't will cause a warning log to be generated
     * by the parent's destructor indicating the non-compliance. Non-compliance can be
     * detected, but this is too late to be effective other than logging the non-compliance.
     * This is the first level of warning to the user code developer to comply.
     * Failure to comply can lead to ancestor callback definitions to be called after the
     * most derived callback object is destroyed. These will generate critical error logs
     * indicating potential crash or system data corruption. These are the second level
     * warnings to the user code developer.
     * First level warnings will always be generated. Second level warnings may or may not
     * be generated, but are provided as an indication of the urgent need to comply.
     * Ex.
     * The virtual callback method SoaRemoteCallListener::onResponseReceived(SoaMessage::SharedPtr)
     * is overridden by the user's derived listener class. Therefore, the parent method
     * SoaRemoteCallListener::onResponseReceived(SoaMessage::SharedPtr) will never be executed.
     * This is guaranteed if the user calls removeResource() in the most derived destructor.
     * However, if not, due to asynchronous callbacks triggered by incoming messages on a background
     * thread while the user thread destroys the callback object, combined with C++'s order of
     * inherited destructor execution (most derived- to base class), instead of no callback
     * being called, the parent callback implementation is called. If the parent callback were
     * pure-virtual, the process would crash. Instead, parent callbacks have been implemented as
     * empty methods, except for a critical error log generation indicating this has happened.
     *
     * Examples of such classes include ALL classes derived from Soa*Listener classes.
     * Example, if a class is derived from the SoaDataListener class, then it MUST
     * call removeResource( this ) in its destructor to guaranty no crashes due
     * to access race conditions.
     * Note, if user code defines a class which derives from two or more Soa*Listener classes,
     * then the destructor MUST call removeResource(...) for each parent Soa*Listener class
     * with the "this" pointer cast to the respective Soa*Listener class using static_cast<...>(this)
     * Ex. user class derives from SoaDataListener and SoaActionResultListener<SoaSubscribeToDataContext>
     * virtual ~DerivedUserListener() {
     *   static_cast<SoaDataListener*>(this)->removeResource()
     *   static_cast<SoaActionResultListener<SoaSubscribeToDataContext>*>(this)->removeResource()
     * }
     */
    class Resource {
    public:
        /**
         * User implementations of classes which ultimately derive from this class
         * must call removeResource() in their destructor to avoid crashes due to
         * race conditions between destruction of the object and asynchronous calls
         * to the callback method. Prevalent examples include all listener classes,
         * where the asynchronous calls to callbacks are due to incoming messages.
         *
         * Removes this resource from the resource manager tracking table to
         * prevent crashes during destruction of the derived resource.
         */
        virtual void removeResource(void) final;
    protected:
        explicit Resource(const std::string & name);
        virtual ~Resource(void);

        /**
         * User code must not call this method
         */
        virtual void internalRemoveResource(void) final;

    private:
        friend SoaResourceManager;
        mutable bool m_tracked;   ///< true if this resource was added to rsrc mgr table and not yet removed
    };
    /**
     * SoaResourceManager singleton implementation.
     * Returns a single SoaResourceManager instance
     * @return SoaResourceManager
     */
    static SoaResourceManager& instance(void);

    static bool isResourceAvailable(const Resource * resource);

    static bool isResourceAvailable(const Resource & resource);

    /**
     * static method
     * Wrapped access to the resource to prevent destruction of the resource
     * while being accessed.
     * @param resource a pointer to the resource
     * @param func a user provided function to perform a custom action using the resource
     *             The caller is responsible for the relationship between the resource and the function
     *             This method simply protects the resource from being destroyed for the duration of the
     *             function.
     * @return true if resource was available and function was executed, false otherwise
     */
    static bool accessResource( const Resource * resource, const std::function<void()> & func );

    /**
     * Removes a resource from internal hash map
     * Convenience API. As a static method, no need to get instance of singleton to call
     * Application defined classes that uses the SOA library which derive from the Resource class
     * MUST call this function in each class' destructor
     * @param resource A resource
     */
    static void removeResource(const Resource * resource);

    /**
     * Destructor
     */
    virtual ~SoaResourceManager(void);

    /**
     * Removes a resource from internal hash set
     * @param resource A resource
     * @param internal reserved for internal use
     */
    void deleteResource(const Resource * resource, bool internal = false);

    /**
     * Returns registered resource count
     * @return size_t
     */
    size_t resourceCount(void);
private:
    friend Resource;
    /**
     * Adds a Resource to internal hash set
     * @param resource A resource
     */
    void addResource(const Resource * resource, const std::string & resourceName);

    /**
     * Returns true if the internal hash set contains a resource
     * This method acquires the map mutex. It must not be already
     * owned by the same thread.
     * @param resource A resource
     * @return bool
     */
    bool safeContainsResource(const Resource * resource);

    const char * const k_className;
    std::unordered_map<const Resource *, std::shared_ptr<ResourceMapEntry>> m_resources;
    std::mutex m_mapMutex;;
    fnv::base::MonotonicCondVar m_cv;       ///< cond var for the resource use count
    std::mutex                  m_cvMutex;  ///< mutex for the cv

    friend class SingletonFactory<SoaResourceManager>;
    SoaResourceManager(void);

    /**
     * Returns true if the internal hash set contains a resource
     * $$$$ The map mutex MUST be owned before calling this method
     * @param resource A resource
     * @return bool
     */
    bool containsResource(const Resource * resource);

    /**
     * member method
     * Wrapped access to the resource to prevent destruction of the resource
     * while being accessed.
     * @param resource a pointer to the resource
     * @param func a user provided function to perform a custom action using the resource
     *             The caller is responsible for the relationship between the resource and the function
     *             This method simply protects the resource from being destroyed for the duration of the
     *             function.
     * @return true if resource was available and function was executed, false otherwise
     */
    bool safeAccessResource( const Resource * resource, const std::function<void()> & func );
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_UTILS_SOA_RESOURCE_MANAGER_HPP_ */
