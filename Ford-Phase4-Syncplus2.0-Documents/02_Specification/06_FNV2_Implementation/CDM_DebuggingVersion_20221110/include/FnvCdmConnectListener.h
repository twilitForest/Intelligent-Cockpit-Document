#ifndef FNVCDMCONNECT_LISTENER_H
#define FNVCDMCONNECT_LISTENER_H

#include <string>

#include "FnvCdmConnectUtils.h"
#include "FnvCdmTypes.h"

namespace fnv {
namespace cdm {

class FnvCdmConnectListener {
public:
    FnvCdmConnectListener() {}
    virtual ~FnvCdmConnectListener() {}

    /**
     * A callback method which needs to be implemented by the application developer if
     * calling a SOA connection.
     *
     * @param result The CdmConnectResult enum value will be from a set of possible result
     *              from the specific SOA connection. Refer to enum defination.
     */
    virtual void onConnect(CdmConnectResult result) = 0;

    /**
     * A callback method which needs to be implemented by the application developer if
     * calling a remote call method.
     *
     * @param result SoaMessage will be a protobuf message.
     */
    virtual void onResponseReceived(SoaMessage::SharedPtr response) = 0;
};

} /* namespace cdm */
} /* namespace fnv */

#endif // FNVCDMCONNECT_LISTENER_H
