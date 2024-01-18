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
 * soa_connection_state.hpp
 *
 *  Created on: Feb 21, 2018
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_CONNECTION_STATE_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_CONNECTION_STATE_HPP_

namespace fnv {
namespace soa {
namespace framework {

/**
 * Values defining the connection state of the message manager
 */
enum class SoaConnectionState {
        UNKNOWN,
        CONNECTED,
        DISCONNECTED,
        CONNECTING,
        CONNECTION_LOST,
        REGISTERED_TO_GW,   // applies only when using IPC connection manager
        REGISTERING_TO_GW   // applies only when using IPC connection manager
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_CONNECTION_STATE_HPP_ */
