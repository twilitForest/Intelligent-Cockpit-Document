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
//  Mock_FciClient.hpp
//
//   Created on: Feb 16, 2018
//       Author: msande86
//

#ifndef FCI_CLIENT_MOCK_FCICLIENT_HPP_
#define FCI_CLIENT_MOCK_FCICLIENT_HPP_

#include <gmock/gmock.h>
#include <libfciclient/FciClient.hpp>


namespace fnv {
namespace fci {
namespace client {


class Mock_FciClient : public FciClient
{
public:

    MOCK_METHOD0(cleanup, void ());

    MOCK_METHOD3(setFtcpMessageListeners, bool (std::shared_ptr<FtcpMessageListener> cmdReceived,
                            std::shared_ptr<FtcpMessageListener> queryRespReceived,
                            std::shared_ptr<FtcpMessageListener> alertRespReceived));

    MOCK_METHOD1(setDeliveryStatusListener, bool (std::shared_ptr<DeliveryStatusListener> deliveryStatusReceived));

    MOCK_METHOD1(setDeliveryConfirmationListener, bool (std::shared_ptr<DeliveryConfirmationListener> deliveryConfirmed));

    MOCK_METHOD1(setBroadcastMessageListener, bool (std::shared_ptr<BroadcastMessageListener> messageReceived));

    MOCK_METHOD1(registerForFtcpCommands, FciErrorCode (const std::unordered_set<std::string> & cmdNames));

    MOCK_METHOD0(deregisterFromAllFTCPCommands, FciErrorCode ());

    MOCK_METHOD2(sendCommandResponse, FciErrorCode (const FciData & cmdReponse, bool deliveryConfirmation));

    MOCK_METHOD5(sendAlert, FciErrorCode (const FciData & alert,
                             bool deliveryConfirmation,
                             FciDrxConfig drxConfig,
                             FciQos qos,
                             bool expectAlertResponse));

    MOCK_METHOD2(sendQuery, FciErrorCode (const FciData & query, bool deliveryConfirmation));

    MOCK_METHOD5(requestConnectSdn, FciErrorCode(FciSdnConnectionInterfaceType interface,
                                const std::string & url,
                                const std::string & wifiSsid,
                                const std::string & wifiPassword,
                                bool withReceiptConfirmation));

    MOCK_METHOD2(requestDisconnectSdn, FciErrorCode (FciSdnConnectionInterfaceType interface, bool withReceiptConfirmation));

    MOCK_METHOD1(updateAuthorizationStatus, FciErrorCode (AuthorizationStatus authStatus));

    MOCK_METHOD1(updateAnonymizationStatus, FciErrorCode (AnonymizationStatus anonyStatus));

    MOCK_METHOD2(registerForFtcpCommandsAsync, FciErrorCode(const std::unordered_set<std::string> & cmdNames,
            std::shared_ptr<FtcpCommandRegistrationListener> registrationComplete));

};

} /* namespace client */
} /* namespace fci */
} /* namespace fnv */

#endif // FCI_CLIENT_MOCK_FCICLIENT_HPP_
