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
 * soa_debug_commands.hpp
 *
 *  Created on: Jun 19, 2018
 *      Author: frisache
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_EDT_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_EDT_HPP_

#include <thread-util/BaseCommand.hpp>
#include <string>
#include <soa/framework/soa_provider.hpp>
#include <soa/framework/soa_message_manager.hpp>
#include <soa/framework/soa_consumer.hpp>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_action_result_listener.hpp>
#include <chrono>
#include <thread-util/MonotonicCondVar.hpp>
#include <mutex>

namespace fnv {
namespace soa {
namespace framework {
/*
 * This class is the main class to define all the SOA related debug commands
 */
class SoaDebugCommands: public fnv::base::BaseCommand {
public:
    /**
     * constructor
     * @param SoaMessageManager::SharedPtr The reference to the SoaMessageManager object
     */
    explicit SoaDebugCommands(const fnv::soa::framework::SoaMessageManager::SharedPtr& msgMgr);
    virtual ~SoaDebugCommands() = default;
};

class SoaPublishCommand: public fnv::base::BaseCommand {
public:
    explicit SoaPublishCommand(const fnv::soa::framework::SoaMessageManager::SharedPtr& msgMgr);
    virtual ~SoaPublishCommand() = default;
    virtual fnv::soa::framework::SoaProvider::SharedPtr createSoaProvider(SoaMessageManager::SharedPtr msgManager);
protected:
    bool executeCommand(std::ostringstream& ostr, char** paramarray, int num) override;
private:
    bool sendPublish(std::ostringstream& ostr, std::string& endpoint, std::string& test_msg);
    std::chrono::milliseconds TIMEOUT;
    fnv::soa::framework::SoaMessageManager::SharedPtr m_msgMgr;
};

class SoaConnectionCommand: public fnv::base::BaseCommand {
public:
    explicit SoaConnectionCommand(const fnv::soa::framework::SoaMessageManager::SharedPtr& msgMgr);
    ~SoaConnectionCommand() = default;
protected:
    bool executeCommand(std::ostringstream& ostr, char** paramarray, int num);
private:
    fnv::soa::framework::SoaMessageManager::SharedPtr m_msgMgr;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_EDT_HPP_ */
