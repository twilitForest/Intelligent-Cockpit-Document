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
 * soa_message.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAMESSAGE_HPP_
#define FNVSOA_FRAMEWORK_SOAMESSAGE_HPP_

#include <memory>
#include <string>
#include <stdint.h>
#include <soa/framework/soa_error_code.hpp>
#include <soa/framework/soa_payload_parser.hpp>
#include <soa/framework/soa_client_endpoint.hpp>
#include <soa/framework/soa_message_options.hpp>
#include <soa/framework/soa_action_result.hpp>
#include <soa/idl/soa_message_proto.pb.h>

namespace fnv {
namespace soa {
namespace framework {

/**
 * SOA message envelop class.
 * Contains SOA message header and a payload.
 */
class SoaMessage {
    public:
        /**
         * Enum to report transaction error if any.
         * An example can be version mismatch between RPC provider and consumer.
         */
        enum class TransactionError {
            /**
             * No error
             */
            NO_ERROR = SoaMessageProto_TransactionError_NO_ERROR,
            /**
             * General error, allows to report errors not listed in the enum
             */
            GENERAL_ERROR = SoaMessageProto_TransactionError_GENERAL_ERROR,
            /**
             * Version mismatch between RPC provider and consumer.
             */
            VERSION_MISMATCH = SoaMessageProto_TransactionError_VERSION_MISMATCH,
            /**
             * Service does not support the requested command
             */
            COMMAND_NOT_SUPPORTED = SoaMessageProto_TransactionError_COMMAND_NOT_SUPPORTED,
            /**
             * Publisher no longer publishing messages
             */
            PROVIDER_CLOSED = SoaMessageProto_TransactionError_PROVIDER_CLOSED,
            /**
             * Publisher no longer publishing messages
             */
            PROVIDER_DISCONNECTED = SoaMessageProto_TransactionError_PROVIDER_DISCONNECTED
        };

        static std::string transactionErrorToString(TransactionError error);
        /**
         * Typedef of scoped shared pointer type to this class
         */
        typedef std::shared_ptr<SoaMessage> SharedPtr;

        /**
         * Typedef of scoped shared pointer type to this class that disallows object modification
         */
        typedef std::shared_ptr<const SoaMessage> ConstSharedPtr;

        /**
         * SOAMessage constructor
         */
        SoaMessage();

        /**
         * SOAMessage destructor
         */
        virtual ~SoaMessage();

        /**
         * Max message size
         */
        static size_t kMaxMessageSize();

        /**
         * Factory method to create a remote call (RPC) request message.
         * The factory will fail to create a message object if any of the arguments are
         * invalid.
         * @param providerEndpoint a shared pointer to the service provider endpoint
         * @param commandID The command ID
         * @param version The service version
         * @param transactionID The transaction ID
         * @return a shared pointer to a new SoaMessage object, or nullptr if an argument is invalid
         */
        static SoaMessage::SharedPtr createRequestMessage(
                        SoaClientEndpoint::SharedPtr providerEndpoint,
                        const std::string & commandID,
                        const std::string & version,
                        const std::string & transactionID);

        /**
         * @deprecated { use the version which does not take a consumer endpoint }
         */
        static SoaMessage::SharedPtr createRequestMessage(
                        SoaClientEndpoint::SharedPtr consumerEndpoint,
                        SoaClientEndpoint::SharedPtr providerendpoint,
                        const std::string & commandID,
                        const std::string & version,
                        const std::string & transactionID);

        /**
         * Factory method to create a remote call (RPC) response message using
         * the transaction context provided in the request.
         * The consumer endpoint is NOT copied from the request message and is nullptr in the
         * resulting response message.
         * The factory will fail to create a message object if any of the arguments are
         * invalid. If the request message has an invalid provider endpoint, nullptr is returned.
         * @param requestMessage a shared pointer to the request message
         * @param version The service provider version.
         * @return a shared pointer to a new SoaMessage object, or nullptr if an argument is invalid
         */
        static SoaMessage::SharedPtr createResponseMessage(
                        SoaMessage::SharedPtr requestMessage,
                        const std::string& version);

        /**
         * Factory method to create a remote call (RPC) response message using
         * the transaction context provided in the request.
         * The consumer endpoint is NOT copied from the request message and is nullptr in the
         * resulting response message.
         * The factory will fail to create a message object if any of the arguments are
         * invalid. If the request message has an invalid provider endpoint, nullptr is returned.
         * @param requestMessage a shared pointer to the request message
         * @param version The service provider version.
         * @param error The transaction error.
         * @return a shared pointer to a new SoaMessage object, or nullptr if an argument is invalid
         */
        static SoaMessage::SharedPtr createResponseMessage(
                        SoaMessage::SharedPtr requestMessage,
                        const std::string& version, TransactionError error);

        /**
         * Parses a message from raw protobuf string
         * Possible error codes are:
         * SoaErrorCode::No_ERROR if successful
         * SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the message reference is on a null pointer
         * SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @param message The raw protobuf string
         * @return The action result containing action error code and SoaMessage object.
         */
        static SoaActionResult<SoaMessage>::SharedPtr parseFromString(
                        const std::string & message);

        /**
         * Parses a message from raw protobuf byte array
         * Possible error codes are:
         * SoaErrorCode::No_ERROR if successful
         * SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the message reference is on a null pointer
         * SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @param soaMessage OUT a reference to a SoaMessage object to be populated from the byte array
         * @param serializedMessage IN pointer to the byte array containing the raw protobuf
         * @param size length of byte array
         * @return a SoaErrorCode as described above
         */
        static SoaErrorCode parseFromArray(
                        SoaMessage & soaMessage,            // out
                        const char * serializedMessage,     // in
                        int size);                          // in

        /**
         * Test a string to know if it can be parsed to a SoaMessage
         * @param rawString - the string to test
         * @return true if the string can be parsed to a SoaMessage, false otherwise
         */
        static bool isSoaMessage(const std::string & rawString);

        /**
         * Sets data endpoint.
         * "Data endpoint" message field should be used to deliver data multicast endpoint to a listener.
         * Setting the endpoint to the nullptr will clear it from the message.
         * @param endpoint a shared pointer to a data endpoint object
         */
        void setDataEndpoint(SoaClientEndpoint::SharedPtr endpoint);

        /**
         * Gets data endpoint.
         * "Data endpoint" message field should be used to deliver data multicast endpoint to a listener.
         * @return a shared pointer to a data endpoint object
         *         returns nullptr if the message does not include a data endpoint
         */
        SoaClientEndpoint::SharedPtr getDataEndpoint() const;

        /**
         * Does the SoaMessage include a data endpoint field?
         * @return true if the field is populated in the message
         */
        bool hasDataEndpoint() const;

        /**
         * Sets consumer endpoint
         * Setting the endpoint to the nullptr will clear it from the message.
         * @param endpoint a shared pointer to a consumer endpoint object
         */
        void setConsumerEndpoint(SoaClientEndpoint::SharedPtr endpoint);

        /**
         * Gets consumer endpoint
         * @return a shared pointer to the consumer endpoint object
         *         returns nullptr if the message does not include a consumer endpoint
         */
        SoaClientEndpoint::SharedPtr getConsumerEndpoint() const;

        /**
         * Does the SoaMessage include a consumer endpoint field?
         * @return true if the field is populated in the message
         */
        bool hasConsumerEndpoint() const;
        /**
         * Sets service provider endpoint.
         * Setting the endpoint to the nullptr will clear it from the message.
         * @param a shared pointer to the service provider endpoint
         */
        void setProviderEndpoint(SoaClientEndpoint::SharedPtr id);

        /**
         * Gets service provider endpoint
         * @return a shared pointer to the service provider endpoint
         *         returns nullptr if the message does not include a provider endpoint
         */
        SoaClientEndpoint::SharedPtr getProviderEndpoint() const;

        /**
         * Does the SoaMessage include a provider endpoint field?
         * @return true if the field is populated in the message
         */
        bool hasProviderEndpoint() const;

        /**
         * Sets service command id
         * If the argument is a null reference, no change is made, no exception occurs
         * Setting an empty string will remove this element from the message
         * @param id The id
         */
        void setCommandId(const std::string & id);

        /**
         * Gets service command ID
         * @return : The ID
         */
        std::string getCommandId() const;

        /**
         * Does the SoaMessage include a command ID field?
         * @return true if the field is populated in the message
         */
        bool hasCommandId() const {
            return m_messageProtobuf.has_commandid();
        }

        /**
         * Sets service version
         * If the argument is a null reference, no change is made, no exception occurs
         * Setting an empty string will remove this element from the message
         * The version
         * @param version
         */
        void setServiceVersion(const std::string & version);

        /**
         * Gets the service version
         * @return : The service version
         */
        std::string getServiceVersion() const;

        /**
         * Does the SoaMessage include a service version field?
         * @return true if the field is populated in the message
         */
         bool hasServiceVersion() const {
            return m_messageProtobuf.has_serviceversion();
        }

        /**
         * Sets transaction ID
         * If the argument is a null reference, no change is made, no exception occurs
         * Setting an empty string will remove this element from the message
         * @param id The ID
         */
        void setTransactionId(const std::string & id);

        /**
         * Gets transaction id.
         * @return : The message id. Allows to resquest/response mapping.
         */
        std::string getTransactionId() const;

        /**
         * Does the SoaMessage include a transaction id field?
         * @return true if the field is populated in the message
         */
        bool hasTransactionId() const {
            return m_messageProtobuf.has_transactionid();
        }

        /**
         * Use the method to set an error if the transaction cannot be completed
         * @param error The error
         */
        void setTransactionError(TransactionError error);

        /**
         * Returns transaction error
         * @return The error
         */
        TransactionError getTransactionError();

        /**
         * Does the SoaMessage include a transaction error field?
         * @return true if the field is populated in the message
         */
        bool hasTransactionError() const {
            return m_messageProtobuf.has_transactionerror();
        }

        /**
         * Set any additional transaction error details
         * If the argument is a null reference, no change is made, no exception occurs
         * Setting an empty string will remove this element from the message
         * @param errorDetails The details
         */
        void setTransactionErrorDetails(const std::string & errorDetails);

        /**
         * Returns transaction error details
         * @return The error description
         */
        std::string getTransactionErrorDetails();

        /**
         * Does the SoaMessage include a transaction error details field?
         * @return true if the field is populated in the message
         */
        bool hasTransactionErrorDetails() const {
            return m_messageProtobuf.has_transactionerrordetailes();
        }

        /**
         * Set payload from a raw string of bytes wrapped by the string type
         * If the argument is a null reference, no change is made, no exception occurs
         * Setting an empty string will remove this element from the message
         * @param rawPayload A std::string wrapped binary byte sequence for the message payload
         */
        virtual void setPayload(const std::string & rawPayload);

        /**
         * Set payload from a Google Protocol Buffer object
         * If the argument is a null reference, no change is made, no exception occurs
         * @param payload A protocol buffer object for the message payload
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the argument is a null reference
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if an error occurred
         *                            while serializing the data in the SoaMessage object
         */
        virtual SoaErrorCode setPayload(
                        const ::google::protobuf::Message & payload);

        /**
         * Convenience method to parse a Google protocol message payload.
         * @param payloadObject Google protobuf object representing the payload
         * @param version API version
         * @return SoaErrorCode::No_ERROR if successful
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the argument is a null reference
         */
        virtual SoaErrorCode parsePayload(
                        ::google::protobuf::Message & payloadObject);

        /**
         * Get the raw payload bytes.
         * @return the raw payload bytes wrapped by a string object for convenience
         */
        virtual std::string getRawPayload();

        /**
         * Does the SoaMessage include a payload field?
         * @return true if the field is populated in the message
         */
        virtual bool hasPayload() const;

        /**
         * Returns an action result including the action status and serialized message
         */
        virtual SoaActionResult<std::string>::SharedPtr serializeAsString();

        /**
         * Returns the serialized message in a string object
         * If serialization fails, the returned string is empty
         */
        std::string serializeToString() const;


        /**
         * Set retain flag to true or false
         */
        void setRetain(bool retain);

        /**
         * Return true when this message should be retained, false when should not.
         * @return bool
         */
        bool shouldRetain();

        /**
         * Generates a human readable form of this message, useful for debugging and other purposes.
         * @return std::string
         */
        virtual std::string getDebugString();

        /**
         * Copy constructor
         */
        SoaMessage(const SoaMessage & that) = default;

        /**
         * Assignment operator
         */
        SoaMessage & operator=(const SoaMessage & that) = default;

        /**
         * Injects the sender ecu id and uid into the message.
         */
        void injectSenderCredentials();
        /**
         * Sender ECU ID getter method
         * @return The sender ecu ID
         */
        const std::string& getSenderEcuId() const;
        /**
         * Sender EUID getter method
         * @return The sender uid
         */
        const std::string& getSenderUid() const;

        /**
         * Seralizes the payload and sets the payload version to the message header
         * @param payload The payload
         * @return  SoaErrorCode::NO_ERROR in case of success
         *          SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the ayload in NULL
         *          SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if tha payload cannot be serialized
         */
        template <class Proto>
        SoaErrorCode setPayloadWithVersion(const Proto& payload) {
            setServiceVersion(makeVersion(payload.__api_version()));
            return setPayload(payload);
        }

        /**
         * @brief Parses the payload and performs the payload version check.
         * The method compares version in the message header with the current Proto version.
         * The API is in permissive mode: does not return an error in case of version mismatch
         * @param[out] payload an instance of a payload to parse the message to
         * @return  SoaErrorCode::NO_ERROR in case of success
         *          SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED payload parsing failed while the version is compatible
         *          SoaErrorCode::ERROR_PROTO_MAJOR_VERSION_MISMATCH payload parsing failed and version is not compatible
         *          SoaErrorCode::WARNING_PROTO_MAJOR_VERSION_MISMATCH - version is not supported, but parsing succeeded
         */
        template <class Proto>
        SoaErrorCode parsePayloadWithVersion(Proto& payload) {
            // we are in version permissive mode and do not check version compatibility, just log.
            // We are OK if we can parse the protobuf
            SoaErrorCode res = SoaErrorCode::NO_ERROR;
            bool versionCheck = checkVersion(makeVersion(payload.__api_version()));
            if (!payload.ParseFromString(getRawPayload())) {
                if (versionCheck) {
                    res = SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED;
                } else {
                    res = SoaErrorCode::ERROR_PROTO_MAJOR_VERSION_MISMATCH;
                }
            } else {
                if (!versionCheck) {
                    res = SoaErrorCode::WARNING_PROTO_MAJOR_VERSION_MISMATCH;
                }
            }
            return res;
        }

    protected:
        static const std::string& versionDelimiter();
        /**
         * Parses m_messageProtobuf from string
         * @param message Serialized message
         * @return SOAErrorCode
         */
        virtual SoaErrorCode parseHeaderFromString(const std::string & message);

        /**
         * Parses m_messageProtobuf from an array of bytes
         * @param message Serialized message
         * @param size length of serialized message
         * @return SOAErrorCode
         */
        virtual SoaErrorCode parseHeaderFromArray(const char * message, int size);

        /**
         * Get the SoaContext data as unformatted data
         * @return a string object representing raw data bytes for the SoaContext data
         */
        std::string getSoaContext() const;

        /**
         * Set the SoaContext data as unformatted data
         * Providing an empty string will clear the context from the message
         * @param a string object representing raw data bytes for the SoaContext data
         * @return true if the context data was applied to the message, false otherwise
         */
        bool setSoaContext(const std::string & context);

        /**
         * Does the SoaMessage include a SoaContext field?
         * @return true only if the field is populated in the message
         */
        bool hasSoaContext() const {
            return m_messageProtobuf.has_context();
        }
        template <class Version>
        std::string makeVersion(Version version) {
            return std::to_string((int)Version::major) + versionDelimiter() +
                std::to_string((int)Version::minor) + versionDelimiter() +
                std::to_string((int)Version::file_update);
        }
        /**
         * Compares version in the message header to the version passed to the method
         * @param version The version
         * @return true if mAJOR version fields are equal, false otherwize
         */
        bool checkVersion(const std::string& version);

    private:
        /**
         * Convenience method private to this class to create an endpoint object from a string
         * @param a string reference
         * @return an endpoint object whose string version equals the provided string
         */
        SoaClientEndpoint::SharedPtr endpointFromString(
                        const std::string & endpoint) const;

        /**
         * The entire contents of the SoaMessage are contained in this protobuf
         */
        SoaMessageProto m_messageProtobuf;
        bool m_retain;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAMESSAGE_HPP_ */
