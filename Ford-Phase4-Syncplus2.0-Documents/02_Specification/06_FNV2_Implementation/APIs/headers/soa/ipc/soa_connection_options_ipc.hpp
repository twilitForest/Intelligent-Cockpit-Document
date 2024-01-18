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
 * soa_connection_options_ipc.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: drati
 */

#ifndef CPP_SOA_CONNECTIONOPTIONS_IPC_HPP_
#define CPP_SOA_CONNECTIONOPTIONS_IPC_HPP_



#include <soa/framework/soa_message.hpp>
#include <soa/framework/soa_connection_options_common.hpp>

namespace fnv {
namespace soa {
namespace framework {



/**
 * Class used to set connection options.
 */
class SoaConnectionOptionsIpc : public SoaConnectionOptionsCommon{

public:

    /**
     * Typedef of scoped shared pointer type to this class
     */
    typedef std::shared_ptr<SoaConnectionOptionsIpc> SharedPtr;

    /**
     * Create container of IPC default connection options
     */
    static SharedPtr createConnectionOptionsIpc();

    /**
     * @DEPRECATED
     *
     * Set the Client Path for the Gateway Client. This method is deprecated as the clientPath is now retrieved
     * from the GatewayIpcConfig
     * @param Client Path
     */
    virtual void setClientPath(const std::string & clientPath) = 0;

    /**
     * @DEPRECATED
     *
     * Get the Client Path for the Gateway Client
     * @param Client Path
     */
    virtual std::string getClientPath() = 0;

    /**
     * @DEPRECATED
     *
     * Set the Server Path for the Gateway. This method is deprecated as the serverPath is now retrieved
     * from the GatewayIpcConfig
     * @param Server Path
     */
    virtual void setServerPath(const std::string & serverPath) = 0;

    /**
     * @DEPRECATED
     *
     * Get Server Path for the Gateway
     * @return the server path
     */
    virtual std::string getServerPath() = 0;

    /*
     * Get Option type
     * @return option type: MQTT or IPC
     */
    SoaConnectionOptionsCommon::optionType getOptionType() override = 0;

    /**
     * get LWT message
     * @return SOA Message to be sent as LWT message when connection dies
     */
    virtual SoaMessage::SharedPtr getLwtMessage() = 0;


};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */


#endif /* CPP_SOA_CONNECTIONOPTIONS_IPC_HPP_ */
