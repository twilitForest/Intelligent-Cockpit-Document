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
 * GatewayErrorCode.hpp
 *
 *  Created on: March 19, 2018
 *      Author: frisache
 */

#ifndef GATEWAYIPCCONFIG_HPP_
#define GATEWAYIPCCONFIG_HPP_

#include <string>

namespace fnv {
namespace soa {
namespace framework {

class GatewayIpcConfig {
    public:
    GatewayIpcConfig();

    virtual ~GatewayIpcConfig() = default;
    /**
     * Gets the file descriptor name of the Gateway IPC interface (client)
     * @return std::string, the file name path
     */
    const std::string getClientPath();

    /**
     * Gets the file descriptor name of the Gateway IPC interface (server)
     * @return std::string, the file name path
     */
    const std::string getServerPath();

    /**
     * Gets the root path of the IPC file descriptors
     * @return std::string, the file name path
     */
    const std::string getRootPath();

    /**
     * @return std::string, the server component name
     */
    const std::string getServerComponentName();

    private:
        std::string m_rootPath;
        std::string m_clientPath;
        std::string m_serverPath;

        std::string getIpcRootPathFromSharedMemory();

        static constexpr const char* GW_SERVER_COMP = "gwIpcServer";
        static constexpr const char* GW_CLIENT_COMP = "gwIpcClient";
        const char * const k_className;

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* GATEWAYIPCCONFIG_HPP_ */
