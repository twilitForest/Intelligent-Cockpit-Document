#ifndef FNVCDM_CONSUMER_H
#define FNVCDM_CONSUMER_H

#include "ConnectListenerProcessor.h"
#include "FnvCdmConnectUtils.h"
#include "FnvCdmTypes.h"

namespace fnv {
namespace cdm {

class FnvCdmConsumer {
public:
    FnvCdmConsumer(SoaMessageManager::SharedPtr soaMessageManager);
    virtual ~FnvCdmConsumer();

    /**
     * Initialize a SOA Consumer.
     *
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorOccurred if couldn't connect
     */
    CdmReturnCode initialize();

    /**
     * Subscribe SOA provider's status.
     *
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode subscribeProviderStatus();

    /**
     * Unsubscribe SOA provider's status.
     *
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode unsubscribeProviderStatus();

    /**
     * Observe the SOA status.
     *
     * @param listener A listener object that implements a callback method to be
     *         called when a status change message is received.
     * @return a bool result
     * @return true if successful
     * @return false if not
     */
    bool registerListener(FnvCdmConnectListener * listener);

    /**
     * Returns true if this has disconnected a SOA connection callback.
     *
     * @param listener
     * @return a bool result
     * @return true if successful
     * @return false if not
     */
    bool unregisterListener(FnvCdmConnectListener * listener);

    /**
     * Send an async remote all (getCdmProfile). The expected response will be received
     * by listener (FnvCdmConnectListener).
     *
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode getCdmProfile();

    /**
     * Send an async remote all (getCdmProfile). The expected response will be received
     * by listener (FnvCdmConnectListener).
     *
     * @param timeout a integer specifying the call/reply timeout in ms
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode getCdmProfile(int32_t timeout);

    /**
    * Request service and block until the response is received or timeout is reached.
    * If successful, a message is included in the return container object.
    *
    * @param profileNumber, Refer to CdmProfileNumberEnum
    * @param timeout a integer specifying the call/reply timeout in ms
    * @return SoaMessage The result will be delivered through FnvCdmConnectListener
    * @return CdmReturnCode::Success if successful
    * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
    * @return CdmReturnCode::ErrorOccurred if request result is null.
    */
    CdmReturnCode setActiveCdmProfile(CdmProfileNumberEnum profile, int32_t timeout);

   /**
    * Request service and block until the response is received or timeout is reached.
    * If successful, a message is included in the return container object.
    *
    * @param profileNumber, Refer to CdmProfileNumberEnum
    * @param featureObjectId, Refer to CdmFeatureObjectId
    * @param settingNumberEnum, Refer to CdmSettingNumberEnum
    * @param timeout a integer specifying the call/reply timeout in ms
    * @return SoaMessage The result will be delivered through FnvCdmConnectListener
    * @return CdmReturnCode::Success if successful
    * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
    * @return CdmReturnCode::ErrorOccurred if request result is null.
    */
    CdmReturnCode setCdmSettings(CdmProfileNumberEnum profile, CdmFeatureObjectId feature, CdmSettingNumberEnum setting, int timeout);

private:
    SoaMessageManager::SharedPtr mSoaMessageManager;
    SoaConsumer::SharedPtr mConsumer;

    class SoaConsumerListenerBase : public SoaConsumer::SoaInitializedConsumerListener,
                                    public SoaServiceStatusListener,
                                    public soa::framework::SoaActionResultListener<SoaSubscribeToServiceStatusContext>,
                                    public soa::framework::SoaActionResultListener<SoaUnsubscribeToServiceStatusContext> {
    public:
        SoaConsumerListenerBase() = default;
        virtual ~SoaConsumerListenerBase();
        // SoaInitializedConsumerListener
        void onInitialized(SoaErrorCode errorCode, const SoaConsumer * consumer) override;
        void onDeinitialized( SoaErrorCode errorCode, const SoaConsumer * consumer) override;
        // SoaServiceStatusListener
        void onStatusChanged(SoaErrorCode error, const SoaClientEndpoint::SharedPtr endpoint, SoaServiceStatus status) override;
        // SoaActionResultListener<SoaSubscribeToServiceStatusContext>
        virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<SoaSubscribeToServiceStatusContext> actionContext) override;
        // SoaActionResultListener<SoaUnsubscribeToServiceStatusContext>
        virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<SoaUnsubscribeToServiceStatusContext> actionContext) override;
    private:
        CdmConnectResult createServiceStatusResult(SoaServiceStatus status);
    }; // end SoaConsumerListenerBase

    typedef soa::framework::SoaListenerLifecycleHelper<
                                SoaConsumerListenerBase,
                                SoaConsumer::SoaInitializedConsumerListener,
                                SoaServiceStatusListener,
                                soa::framework::SoaActionResultListener<SoaSubscribeToServiceStatusContext>,
                                soa::framework::SoaActionResultListener<SoaUnsubscribeToServiceStatusContext>
                                > SoaConsumerListenerWrapper;

    SoaConsumerListenerWrapper mSoaConsumerListenerWrapper;

    SoaClientEndpoint::SharedPtr mServiceEndpoint;
    SoaMessage::SharedPtr createRequest(const std::string & commandId, const std::string & rawPayload);
    bool isConnected();

    class RemoteCallListenerBase : public SoaRemoteCallListener,
                                   public soa::framework::SoaActionResultListener<SoaRemoteCallContext> {
    public:
        RemoteCallListenerBase() = default;
        virtual ~RemoteCallListenerBase();

        virtual void onResponseReceived(SoaMessage::SharedPtr response) override;
        virtual void onActionCompleted(SoaErrorCode error, std::shared_ptr<SoaRemoteCallContext> actionContext) override;
    };

    typedef fnv::soa::framework::SoaListenerLifecycleHelper<RemoteCallListenerBase,
                                                            SoaRemoteCallListener,
                                                            soa::framework::SoaActionResultListener<SoaRemoteCallContext>
                                                           > RemoteCallListenerWrapper;
    RemoteCallListenerWrapper mRemoteCallListenerWrapper;
    Logger * mLog;
}; // end class FnvCdmConsumer

} /* namespace cdm */
} /* namespace fnv */

#endif // FNVCDM_CONSUMER_H
