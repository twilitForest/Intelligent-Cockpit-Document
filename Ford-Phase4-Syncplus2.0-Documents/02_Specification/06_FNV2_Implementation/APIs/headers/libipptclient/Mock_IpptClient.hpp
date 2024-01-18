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
 * Mock_IpptClient.hpp
 *
 *  Created on: Jan 28, 2019
 *      Author: CCHAN34
 */

#ifndef IPPT_CLIENT_MOCK_IPPTCLIENT_HPP_
#define IPPT_CLIENT_MOCK_IPPTCLIENT_HPP_

#include <gmock/gmock.h>
#include <libipptclient/IpptClient.hpp>


namespace fnv {
namespace ipptclient {


class Mock_IpptClient : public IpptClient {
    public:

    Mock_IpptClient(const std::shared_ptr<fnv::soa::framework::SoaConsumer> & consumer,
                        const EcuId & ecuId, const std::string & applicationId)
        : IpptClient(consumer, ecuId, applicationId) {
};

    ~Mock_IpptClient() = default;

    MOCK_METHOD0(cleanup, void ());

    MOCK_METHOD0(registerClient, IpptRetCode ());

    MOCK_METHOD1(registerClientAsync, IpptRetCode (register_cb cb));

    MOCK_METHOD0(unregisterClient, IpptRetCode ());

    MOCK_METHOD3(setTokenRequestCallbacks, void (token_request_status_cb statusCb,
                                                token_response_cb responseCb,
                                                token_response_error_cb responseErrorCb));

    MOCK_METHOD1(setTokenRevocationCallback, void (token_revocation_cb revocationCb));

    MOCK_METHOD1(setServerStatusCallback, IpptRetCode (server_status_cb serverStatusCb));

    MOCK_METHOD4(sendTokenRequest, IpptRetCode (const std::string & transactionId,
                                                bool policyValidation,
                                                const std::vector<std::pair<std::string, std::string>> & configurations,
                                                const std::vector<std::string> & scopes));

    MOCK_METHOD1(sendRevocationResponse, IpptRetCode (const std::string & revocationId));

    MOCK_METHOD3(sendRevocationResult, IpptRetCode (const std::string & revocationId,
                                                    const TokenRevocationResult & result,
                                                    const std::string & errorDescription));
                                                    
};

 } /* namespace ipptclient */
 } /* namespace fnv */

#endif // IPPT_CLIENT_MOCK_IPPTCLIENT_HPP_
