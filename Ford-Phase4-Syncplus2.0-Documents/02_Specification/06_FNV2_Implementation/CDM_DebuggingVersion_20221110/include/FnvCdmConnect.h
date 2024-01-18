#ifndef FNVCDMCONNECT_H
#define FNVCDMCONNECT_H

#include "FnvCdmConnectUtils.h"
#include "FnvCdmTypes.h"

namespace fnv {
namespace cdm {

class FnvCdmConnect {
public:
    FnvCdmConnect();
    virtual ~FnvCdmConnect();

    /**
     * Connect a SOA connection.
     *
     * @return CdmReturnCode::Success if successful
     * @return CdmReturnCode::ErrorOccurred if couldn't connect
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode connect();

    /**
     * Disconnect a SOA connection.
     *
     * @return CdmReturnCode::Success if successful.
     * @return CdmReturnCode::ErrorOccurred if couldn't disconnect.
     * @return CdmReturnCode::ErrorNotInitialized if this was previously initialized failed.
     */
    CdmReturnCode disconnect();

    /**
     * Gets the SoaMessageManager object.
     *
     * @return SoaMessageManager
     */
    SoaMessageManager::SharedPtr getSoaMessageManager();

    /**
     * Observe the SOA connection status.
     *
     * @param listener Other module should implement a callback method. A callback will be
     *         called when a status change message is received.
     * @return a bool result
     * @return true if successful
     * @return false if not
     */
    bool registerListener(FnvCdmConnectListener * listener);

    /**
     * Unregister a SOA connection listener.
     *
     * @param listener
     * @return a bool result
     * @return true if successful
     * @return false if not
     */
    bool unregisterListener(FnvCdmConnectListener * listener);

private:
    void initialize();
    CdmReturnCode connectAsync();

    class SoaConnectListenerBase : public SoaConnectionListener,
                                   public SoaConnectionLostListener {
    public:
        SoaConnectListenerBase() = default;
        virtual ~SoaConnectListenerBase();

        void onConnect(SoaErrorCode errorCode);
        void onConnectionLost(const std::string & cause);
    }; // end SoaConnectListenerBase

    typedef soa::framework::SoaListenerLifecycleHelper<
                                SoaConnectListenerBase,
                                SoaConnectionListener,
                                SoaConnectionLostListener
                                > SoaConnectListenerWrapper;

    SoaConnectListenerWrapper mSoaConnectListenerWrapper; 
    SoaConnectionOptionsIpc::SharedPtr mConnOptionsIpc;

    SoaMessageManager::SharedPtr mSoaMessageManager;
    Logger * mLog;

}; // end class FnvCdmConnect

} /* namespace cdm */
} /* namespace fnv */

#endif // FNVCDMCONNECT_H
