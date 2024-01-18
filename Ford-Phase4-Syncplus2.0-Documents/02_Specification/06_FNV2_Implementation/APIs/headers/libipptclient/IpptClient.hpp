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
 * IpptClient.hpp
 *
 *  Created on: Aug 17, 2018
 *      Author: Terry Park
 */

// API version : 1.0

#ifndef IPPTCLIENT_HPP
#define IPPTCLIENT_HPP

#include <string>
#include <soa/framework/soa_consumer.hpp>


namespace fnv {
namespace ipptclient {


/**
 * @brief
 * Enumeration of ECUs
 */
enum class EcuId : unsigned char {
    SYNC    = 1,
    TCU     = 2,
    ECG     = 3
};

/**
 * @brief
 * Enumeration of possible error codes returned by the methods in this library
 */
enum class IpptRetCode : unsigned char {
    NO_ERROR              = 0,
    ERR_INVALID_PARAMETER = 1,
    ERR_GENERAL           = 2,
    ERR_UNKNOWN_APPID     = 3
};

enum class IpptServerStatus : unsigned char {
    UNAVAILABLE = 0,
    AVAILABLE     = 1
};

enum class IpptRegisterResult : unsigned char {
    REGISTER_FAILED    = 0,
    REGISTER_SUCCEED   = 1
};

/**
 * @brief
 * Enumeration values indicating the status of access token request
 */
enum class TokenRequestStatus : unsigned char {
    UNKNOWN                         = 0,
    REQUEST_ID_ALLOCATED            = 1,  // The request id is allocated. Use this id to map the request and response
    IN_PROGRESS                     = 2,  // The request is in progress
    ERROR_CLIENT_NOT_REGISTERED     = 3,  // This client is not registered registered. The IPPT client should be registered to IPPT sever before sending the request
    ERROR_REQUEST_PARAM_VALIDATION  = 4,  // The parameter validation check is failed
    ERROR_REQUEST_DELIVERY_ERROR    = 5,  // The request delivery to the cloud is failed
    ERROR_REQUEST_TIMEOUT           = 6,  // The cloud doesn't send the corresponding response
    ERROR_REQUEST_FAILED            = 7   // failed by various reasons
};

/**
 * @brief
 * Enumeration values indicating the access token response error
 */
enum class TokenResponseError {
    // This error code is set when the Subscription got expired
    SUBSCRIPTION_EXPIRED = 1,
    // This error code is set when Feature Not enrolled
    NOT_ENROLLED = 2,
    // This error code is set when Invalid CCS
    INVALID_CCS =3,
    // This error code is set when Subscription got expired and Not enrolled
    EXPIRED_SUBSCRIPTION_AND_NOT_ENROLLED = 4,
    // This error code is set when Subscription expiration and Invalid CCS
    EXPIRED_SUBSCRIPTION_AND_INVALID_CCS = 5,
    // This error code is set when Not enrolled and Invalid CCS
    NOT_ENROLLED_AND_INVALID_CCS = 6,
    // This error code is set when Subscription expiration , Not enrolled  and Invalid CCS
    EXPIRED_SUBSCRIPTION_NOT_ENROLLED_AND_INVALID_CCS = 7,
    // This error code is set when cloud system down, Unable to check subscription, enrollment and CCS
    POLICY_VALIDATION_SYSTEM_DOWN = 8,
    // This error code is set when Content Provider system down
    PROVIDER_SYSTEM_DOWN = 9,
    // This error code is set when IPPT cloud system down
    IPPT_CLOUD_SYSTEM_DOWN = 10,
    // This error code is set when Refusal from content provider for a Token
    PROVIDER_TOKEN_REFUSAL = 11,
    // This error code is set when Mandatory Token fields are missing
    PAYLOAD_ERROR = 12,
    // This error code is set when URL in the Configuration is not located
    INVALID_URL = 13,
    // This error code is set when all other Errors occurs
    OTHER_ERRORS = 14
};

enum class TokenRevocationResult : unsigned char {
    REVOCATION_FAILED  = 0,
    REVOCATION_SUCCEED = 1
};

/**
 * @brief
 * The class to hold access token response data.
 */
class TokenResponseData {
    public:

        explicit TokenResponseData() = default;

        virtual ~TokenResponseData() = default;

        /**
         * @brief
         * Returns the access token - string representing an authorization issued.
         */
        virtual const std::string & getToken() const = 0;

        /**
         * @brief
         * Returns the expiry Time in seconds for the access token.
         */
        virtual int getTokenExpiryTime() const = 0;

        /**
         * @brief
         * Returns the type of token  e.g: Bearer, Mac. This is Oauth2 Standard parameter.
         */
        virtual const std::string & getTokenType() const = 0;

        /**
         * @brief
         * Returns the token being used to acquire additional access tokens when the current access token expires,
         * Refresh tokens are long-lived.
         */
        virtual const std::string & getRefreshToken() const = 0;

        /**
         * @brief
         * Returns the expiry Time in seconds for the refresh token.
         */
        virtual int getRefreshTokenExpiryTime() const = 0;

        /**
         * @brief
         * Returns what the access token can do and what resources it can access.
         */
        virtual const std::vector<std::string> & getScope() const = 0;

        /**
         * @brief For future usage
         * Returns the download URL that is provided by the content provider from where content can be accessed
         */
        virtual const std::string & getRedirectUrl() const = 0;

};


typedef std::function<void(IpptRegisterResult)> register_cb;
typedef std::function<void(const std::string & requestId, TokenRequestStatus status, const std::string & transactionId)> token_request_status_cb;
typedef std::function<void(const std::string & requestId, const std::shared_ptr<TokenResponseData> & tokenData)> token_response_cb;
typedef std::function<void(const std::string & requestId, TokenResponseError error, const std::string & errorDescription)> token_response_error_cb;
typedef std::function<void(const std::string & revocationId)> token_revocation_cb;
typedef std::function<void(IpptServerStatus)> server_status_cb;


class IpptClientImpl;

/**
 * @class IpptClient
 * Public library to facilitate the use of the IP Pass Through Interface.
 */
class IpptClient {
    public:
        /**
         * @brief
         * Creates a IpptClient object. Caller responsible for deletion.
         *
         * Requires properly initialized a SoaConsumer object. The caller has
         * ownership of the SoaConsumer object. IpptClient uses a weak pointer
         * to keep a SoaConsumer reference.
         *
         * @param consumer - the shared pointer of SoaConsumer
         * @param ecuId - ECU id
         * @param featureId - feature id MUST be unique among IPPT applications.
         *      This id will be sent to the cloud as the feature id.
         */
        explicit IpptClient(const std::shared_ptr<fnv::soa::framework::SoaConsumer> & consumer,
                        const EcuId & ecuId, const std::string & featureId);

        virtual ~IpptClient();

        /**
         * @brief
         * This method clears all callback handlers and unregisters IPPT Client.
         */
        virtual void cleanup();

        /**
         * @brief
         * Register IPPT Client to IPPT Server.
         * In order to send a token request and receive the request status/response,
         * IPPT Client must register to IPPT Server.
         * This is a synchronous call and must be called prior to calling other actions methods
         *
         * @return IpptRetCode
         */
        virtual IpptRetCode registerClient();

        /**
         * @brief
         * Register IPPT Client to IPPT Server asynchronously.
         * In order to send a token request and receive the request status/response,
         * IPPT Client must register to IPPT Server.
         * This is an asynchronous call and must be called prior to calling other actions methods.
         * The result will be passed through the callback, register_cb.
         * The app has to wait for the registration result before requesting a token.
         *
         * @param cb - This callback will be invoked when the IPPT Client registration is completed.
         * @return IpptRetCode
         */
        virtual IpptRetCode registerClientAsync(register_cb cb);

        /**
         * @brief
         * Unregister IPPT Client from IPPT Server.
         * This is a synchronous call.
         *
         * @return IpptRetCode
         */
        virtual IpptRetCode unregisterClient();

        /**
         * @brief
         * Set callback handlers to receive the progress status after requesting a token and
         * receive a token response sent by the cloud.
         * You can set nullptr to remove the existing callback.
         *
         * @param statusCb - This callback will be invoked when the IPPT Server sends the progress status.
         *                  The callback will pass the token request id, transaction id and TokenRequestStatus.
         *
         * @param responseCb - This callback will be invoked when the token response is arrived from the cloud.
         *                  The callback will pass the token request id and TokenResponseData.
         *
         * @param responseErrorCb - This callback will be invoked when the token response error is arrived from the cloud.
         *                  The callback will pass the token request id and TokenResponseError.
         */
        virtual void setTokenRequestCallbacks(token_request_status_cb statusCb,
                                              token_response_cb responseCb,
                                              token_response_error_cb responseErrorCb);

        /**
         * @brief
         * Set a callback handler to receive a token revocation sent by the cloud.
         * You can set nullptr to remove the existing callback.
         *
         * @param revocationCb - This callback will be invoked when the token revocation is arrived from the cloud.
         *                  The callback will pass the token revocation id.
         *
         */
        virtual void setTokenRevocationCallback(token_revocation_cb revocationCb);

        /**
         * @brief
         * Set callback handler to receive the IPPT server status change.
         * You can set nullptr to remove the existing callback.
         *
         * @param serverStatusCb - This callback will be invoked when the IPPT Server status is updated.
         * @return IpptRetCode
         */
        virtual IpptRetCode setServerStatusCallback(server_status_cb serverStatusCb);

        /**
         * @brief
         * Send an access token request.
         * @param transactionId - This transation id will be passed through token_request_status_cb callback with the status, TokenRequestStatus::REQUEST_ID_ALLOCATED,
         *                        when the requestId is allocated. It allows the token request/request-status and request/request-response mapping.
         * @param policyValidation - This indicates whether the cloud has to check a policy validation or not before processing an access token request.
         * @param configurations - Any type of configuration (parameters) provided as key/value pair. E.g.) Key = url Value= example.com/radio
         * @param scopes - Scope of the Access Token
         * @return IpptRetCode
         */
        virtual IpptRetCode sendTokenRequest(const std::string & transactionId,
                                    bool policyValidation,
                                    const std::vector<std::pair<std::string, std::string>> & configurations,
                                    const std::vector<std::string> & scopes);


        /**
        * @brief
        * Send the response of token revocation.
        * The app must send the response to the cloud on receiving the revocation request through token_revocation_cb.
        * The app must call this API from outside of the callback thread.
        * The app sends the response to confirm the arrival of the revocation, then sends the result after
        * the app revokes the tokens.
        *
        * @param revocationId - id passed through token_revocation_cb
        * @return IpptRetCode
        */
        virtual IpptRetCode sendRevocationResponse(const std::string & revocationId);

        /**
        * @brief
        * Send the result of token revocation.
        * The app must process the token revocation after received the revocation request through token_revocation_cb
        * and send the result to the cloud.
        * The app must call this API from outside of the callback thread.
        *
        * @param revocationId - id passed through token_revocation_cb
        * @param result - revocation result
        * @param errorDescription - set a brief description for the failure if failed, don't set otherwise.
        * @return IpptRetCode
        */
        virtual IpptRetCode sendRevocationResult(const std::string & revocationId,
                                                 const TokenRevocationResult & result,
                                                 const std::string & errorDescription = "");

    private:
        std::unique_ptr<IpptClientImpl> m_pimpl;

};

} /* namespace ipptclient */
} /* namespace fnv */

#endif /* IPPTCLIENT_HPP */
