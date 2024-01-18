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
 * soa_listener_lifecycle_helper.hpp
 *
 *  Created on: Oct 9, 2018
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_LISTENER_LIFECYCLE_HELPER_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_LISTENER_LIFECYCLE_HELPER_HPP_

#include <soa/utils/soa_resource_manager.hpp>

namespace fnv {
namespace soa {
namespace framework {


/**
 * SOA Framework library users are encouraged to use this template to wrap
 * their derived listener classes, especially if their classes use
 * multiple inheritance such that their listener class derives from multiple
 * SOA Framework listener classes to implement multiple callbacks within a
 * single class.
 *
 * This class template implements an automatic call to the pre-destructor
 * method in the base class for the user to make the lifecycle of instantiated
 * listener objects safe in the real-time multi-threaded run-time environment
 * of FNV.
 *
 * Without the use of this helper class, the SOA Library user must explicitly
 * call ...
 *   this->removeResource();
 * at the beginning of the destructor of the most derived listener class whose
 * object is passed to a SOA API.
 * In the case where the user listener class derives from more than one SOA
 * Listener class, the user must call removeResource() for each SOA Listener
 * class from which their class derives using the following syntax for each ...
 *   static_cast<Soa???Listener*>(this)->removeResource();
 * where Soa???Listener is replaced by the SOA Listener class name
 * (ex. SoaDataListener)
 *
 * Using this template to wrap their listener class creates a more derived
 * class derived from their class. The user needs to provide their own
 * listener class name, followed by each of the SOA Listener class names from
 * which their class derives, as template parameters..
 *
 * Ex.
 * / / user class definition
 * class MyDataListener :
 *    public SoaDataListener,
 *    public SoaActionResultListener<SoaSubscribeToDataContext>
 * {
 *   / / ... user class definition, including callbacks declarations
 *
 *   virtual void onDataReceived(SoaMessage::SharedPtr message);
 *
 *   virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<SoaSubscribeToDataContext> actionContext);
 * };
 *
 * / / A wrapped class is created as follows:
 * using MySafeDataListener = SoaListenerLifecycleHelper < MyDataListener, SoaDataListener, SoaActionResultListener<SoaSubscribeToDataContext> >;
 * / / or
 * typedef SoaListenerLifecycleHelper < MyDataListener, SoaDataListener, SoaActionResultListener<SoaSubscribeToDataContext> > MySafeDataListener;
 *
 * ===========================
 * This template uses template recursion to create the required methods to
 * remove each listener from the resource manager.
 */


template <class UserMultiInheritedListener, class FirstParent, class ... RestOfParents>
class SoaListenerLifecycleHelper :
                public UserMultiInheritedListener
{
    public:
        /**
         * Constructor template
         */
        template <class ... Args>
        SoaListenerLifecycleHelper(Args && ... args) :
            UserMultiInheritedListener(std::forward<Args>(args)...)
        {
            // nothing else to be done
        }
        /**
         * Destructor
         */
        virtual ~SoaListenerLifecycleHelper() {
           RemoveResources<true, FirstParent, RestOfParents ...>();
        }

        /**
         * recursive template method to iterate over Parents
         */
        template <bool, class Parent1, class ... OtherParents>  // unused bool type param is only needed for compiler template deduction rules
        void RemoveResources() {                           // because there are no function parameters
            RemoveResources<true, OtherParents...>();
            static_cast<Parent1*>(this)->removeResource();
        }

        /**
         * base case for recursive template method
         */
        template <bool>
        void RemoveResources() {
            /* base case for template recursion - do nothing */
        }
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_LISTENER_LIFECYCLE_HELPER_HPP_ */
