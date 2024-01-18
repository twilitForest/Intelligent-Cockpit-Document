#ifndef CONNECT_LISTENER_PROCESSOR_H
#define CONNECT_LISTENER_PROCESSOR_H

#include <logger_api.hpp>

#include "FnvCdmConnectListener.h"
#include "FnvCdmTypes.h"

namespace fnv {
namespace cdm {

class ConnectListenerProcessor {
public:
    static ConnectListenerProcessor & getInstance();

    bool registerListener(FnvCdmConnectListener * listener);
    bool unregisterListener(FnvCdmConnectListener * listener);

    void notifyListener(CdmConnectResult result);
    void notifyResult(SoaMessage::SharedPtr response);

    const char * getResultName(CdmConnectResult type);

    bool isConnected();
private:
    ConnectListenerProcessor();
    virtual ~ConnectListenerProcessor();

    void setConnectStatus(CdmConnectResult status);
    Logger * mLog;

    std::mutex mListenerMutex;
    typedef std::vector<FnvCdmConnectListener *> Listeners;
    Listeners mListeners;

    CdmConnectResult mCdmConnectResult = CdmConnectResult::MSG_INIT;
}; // end class ConnectListenerProcessor

} /* namespace cdm */
} /* namespace fnv */

#endif // CONNECT_LISTENER_PROCESSOR_H
