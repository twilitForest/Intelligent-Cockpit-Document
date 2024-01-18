//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2018.  Ford Motor Company owns all rights to this work and intends
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
 * soa_subscription_tracker.hpp
 *
 *  Created on: Feb 8, 2018
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_UTILS_SOA_SUBSCRIPTION_TRACKER_HPP_
#define INCLUDE_SOA_UTILS_SOA_SUBSCRIPTION_TRACKER_HPP_

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/utils/soa_resource_manager.hpp>
#include <forward_list>
#include <mutex>
#include <string>

namespace fnv {
namespace soa {
namespace framework {

// forward reference
class SoaListenerUser;

/**
 * A container struct to aggregate the details that define a subscription binding
 * a topic pattern and a listener
 */
struct SoaListenerTuple : std::tuple<SoaClientEndpoint::SharedPtr, bool, SoaListenerUser *> {
    /**
     * Constructor
     */
    SoaListenerTuple(const SoaClientEndpoint::SharedPtr & endpoint, bool unique, SoaListenerUser * user) :
        std::tuple<SoaClientEndpoint::SharedPtr, bool, SoaListenerUser *>(endpoint, unique, user) {}

    /**
     *  predicate method used to find tuple in a list
     */
    bool operator() (const SoaListenerTuple & other) const;

    /**
     * Destructor
     */
    virtual ~SoaListenerTuple() = default;

    /**
     * tuple field descriptors
     */
    enum { ENDPOINT = 0, UNIQUE, USER };

    /**
     * Get a reference to the endpoint shared_ptr value
     * @return a reference to the endpoint shared_ptr
     */
    inline const SoaClientEndpoint::SharedPtr &
        endpoint() const { return std::get<ENDPOINT>(*this); }
    /**
     * Get the uniqueness value
     * @return the subscription uniqueness value
     */
    inline bool
        unique() const { return std::get<UNIQUE>(*this); }
    /**
     * Get get the user pointer value
     * @return a raw pointer to the listener user
     */
    inline SoaListenerUser *
        user() const { return std::get<USER>(*this); }

    SoaListenerTuple() = default;
    SoaListenerTuple(const SoaListenerTuple & other) = default;
    SoaListenerTuple & operator=(const SoaListenerTuple & other) = default;
};

/**
 * Parent class of a message listener used to track all subscriptions the listener
 * is bound in
 */
class SoaSubscriptionTracker {
    public:
        friend SoaListenerUser;

        /**
         * Constructor
         */
        SoaSubscriptionTracker();

        /**
         * Destructor
         * This is where much of the functionality of this class is implemented.
         * When the message listener associated with an MQTT subscription is destroyed,
         * the caller of the listener is notified to remove the reference to the
         * listener and perform any clean up work, such as unsubscribing.
         */
        virtual ~SoaSubscriptionTracker();

    private:
        /**
         * Used by SOA Framework internal code to handle listener lifecycle transitions
         */
        virtual void internalTrackSubscribe(const SoaClientEndpoint::SharedPtr & endpoint, bool unique, SoaListenerUser * user) final;
        virtual void internalTrackUnsubscribe(const SoaClientEndpoint::SharedPtr & endpoint, bool unique, SoaListenerUser * user) final;

        /**
         * Short name abstraction of container type being used
         */
        typedef std::forward_list<SoaListenerTuple> SubscriptionTrackingDetails;

        SubscriptionTrackingDetails m_trackingDetails;    ///< container of subscription tracking info
        std::mutex m_detailsMutex; ///< to serialize modifications to the tracking details container
        const char * const k_className;
};


} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_SUBSCRIPTION_TRACKER_HPP_ */
