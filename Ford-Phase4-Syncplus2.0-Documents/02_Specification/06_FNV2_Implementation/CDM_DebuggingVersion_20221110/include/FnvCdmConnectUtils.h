#ifndef FNVCDMCONNECT_UTILS_H
#define FNVCDMCONNECT_UTILS_H

#define TAG "FnvCdm"

#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_connection_options.hpp>
#include <soa/framework/soa_consumer.hpp>
#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_error_code_util.hpp>
#include <soa/framework/soa_message_manager.hpp>
#include <soa/framework/soa_remote_call_context.hpp>
#include <soa/framework/soa_remote_call_listener.hpp>
#include <soa/framework/soa_service_directory_manager.hpp>
#include <soa/framework/soa_service_status.hpp>
#include <soa/framework/soa_service_status_util.hpp>

#include <soa/ipc/soa_connection_options_ipc.hpp>

namespace framework = fnv::soa::framework;

using SoaClientEndpoint         = framework::SoaClientEndpoint;
using SoaConnectionListener     = framework::SoaConnectionListener;
using SoaConnectionLostListener = framework::SoaConnectionLostListener;
using SoaConnectionOptionsIpc   = framework::SoaConnectionOptionsIpc;
using SoaConsumer               = framework::SoaConsumer;

using SoaErrorCode      = framework::SoaErrorCode;
using SoaErrorCodeUtil  = framework::SoaErrorCodeUtil;

using SoaMessage        = framework::SoaMessage;
using SoaMessageManager = framework::SoaMessageManager;

using SoaRemoteCallListener = framework::SoaRemoteCallListener;
using SoaRemoteCallContext  = framework::SoaRemoteCallContext;

using SoaServiceStatus           = framework::SoaServiceStatus;
using SoaServiceStatusUtil       = framework::SoaServiceStatusUtil;
using SoaServiceDirectoryManager = framework::SoaServiceDirectoryManager;
using SoaServiceStatusListener   = framework::SoaServiceStatusListener;
using SoaSubscribeToServiceStatusContext   = framework::SoaSubscribeToServiceStatusContext;
using SoaUnsubscribeToServiceStatusContext = framework::SoaUnsubscribeToServiceStatusContext;

#endif // FNVCDMCONNECT_UTILS_H
