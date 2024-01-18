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
 * soa_message_derived_template.hpp
 *
 *  Created on: Sep 30, 2017
 *      Author: preissne
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_SUBCLASS_TEMPLATE_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_SUBCLASS_TEMPLATE_HPP_


#include <soa/framework/soa_action_result.hpp>
#include <soa/framework/soa_message.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * This template is used to easily create message classes which derive
 * from the SoaMessage class. These derived classes can be used to
 * conceptually add header fields to the SoaMessage class by simulating
 * an extension to the SoaMessage protobuf, yet the added fields are
 * simply defined in a custom protobuf used to populate the SoaMessage
 * payload. The custom protobuf defines the extra fields
 * plus an embedded payload field.
 *
 * Structure of a SoaMessageDerivedTemplate template class
 *
 * WARNING: See IMPORTANT note below
 *
 * This _is_a_ SoaMessage whose payload is a user-defined protobuf that
 * includes some state variable (header) fields plus an embedded payload
 * The embedded payload is opaque to this class
 * As this class "is a" SoaMessage, the specialization comes by adding
 * new header fields with setters and getters for required additional
 * state variable fields, while the opaque payload concept of the SoaMessage
 * being the only element of the object that is an unknown blob of data,
 * is pushed down into the embedded "payload" field of the instantiating
 * GPB that makes up the payload of the parent SoaMessage class.
 * NOTE: The SoaMessage protobuf is in no way modified or impacted by this
 * structure.
 *
 * SoaMessage  GPB                ->      |               SoaMessageProto                 |
 * SoaMessage part                ->      | header fields |     SoaMessage::payload       |
 * SoaMessageDerivedTemplate<GPB>  GPB ->                 |               GPB             |
 * SoaMessageDerivedTemplate<GPB> part ->                 | GPB fields |  GPB::payload    |
 *
 * The "payload" of the SoaMessageDerivedTemplate is an arbitrary blob of data
 *
 * SoaMessageDerivedTemplate<GPB>  - - -> | header fields | GPB fields |  GPB::payload    |
 *
 * !!! IMPORTANT !!!
 * This class template assumes the name of the embedded payload defined in the
 * GPB is "payload".
 * Users are free call the payload field by any arbitrary name
 * as long as they instantiate this class template with a class that
 * implements the following public methods:
 *
 *      std::string payload()
 *      bool has_payload()
 *      void set_payload(const std::string & payload)
 *      void clear_payload()
 *
 * This can be accomplished by defining an adapter class that inherits the GPB class,
 * and adds these methods which to access their payload field.
 *
 * Ex.
 *
 * class GPBAdapter :
 *     public GPB
 * {
 *     public:
 *         GPBAdapter() = default;
 *         virtual ~GPBAdapter() = default;
 *
 *         std::string payload() { return yourpayloadfieldname(); }
 *         bool has_payload() { return has_yourpayloadfieldname(); }
 *         void set_payload(const std::string & payload) { set_yourpayloadfieldname(payload); }
 *         void clear_payload() { clear_yourpayloadfieldname(); }
 * };
 *
 * ########################################################################
 * Using this template:
 * The user wants to create a message structured like this ...
 *
 * | header fields | GPB fields |  GPB::payload    |
 *
 * so they need to create a class derived from SoaMessageDerivedTemplate<GPB>
 * where the only functionality that needs to be added is
 * setters and getters to the GPB fields, plus
 * the following static methods:
 *
 *    This method creates a new message object with all of the parameters needs
 *    to populate the required fields
 * static SoaOnDemandRequestMessage::SharedPtr createMessage( ... );
 *
 *    This method creates a new message object populated by the provided string
 *    of raw data. It should verify all of the required fields are present.
 *    This method can call the parent (an instantiation of this template)
 *    version of this method to parse the string and then downcast the object.
 * static SoaActionResult<SoaOnDemandRequestMessage>::SharedPtr parseFromString(const std::string & message);
 *
 *    This method creates a new message object populated by the provided SoaMessage
 *    NOTE: A message received from the network and parsed into a SoaMessage object
 *    using only the SoaMessage protobuf CANNOT be downcast to the expected child class.
 *    The derived message type MUST be parsed from the original string of bytes.
 *    This method needs to serialize the SoaMessage and then parse from that serialized data.
 *    It should verify all of the required fields are present.
 *    This method should not call the parent (an instantiation of this template)
 *    version of this method to parse the SoaMessage. Once the message is serialized,
 *    the string can be applied to the derived class' static method parseFromString(string)
 *  static SoaActionResult<SoaOnDemandRequestMessage>::SharedPtr parseFromMessage(const SoaMessage::SharedPtr soaMessage);
 *
 */
template <class GPB>
class SoaMessageSubclassTemplate : public SoaMessage {
    public:
        SoaMessageSubclassTemplate() = default;
        virtual ~SoaMessageSubclassTemplate() = default;

        typedef std::shared_ptr<SoaMessageSubclassTemplate<GPB>> SharedPtr;

        /**
         * Parses a message from raw protobuf string
         * Possible error codes are:
         * SoaErrorCode::No_ERROR if successful
         * SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the message reference is on a null pointer
         * SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @param message The raw protobuf string
         * @return The action result containing action error code and SoaMessage derived-type object.
         */
        template <class Derived>
        static typename SoaActionResult<Derived>::SharedPtr parseFromString(const std::string & message);

        /**
         * Parses a derived message from SoaMessage
         * Possible error codes are:
         * SoaErrorCode::No_ERROR if successful
         * SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the message reference is on a null pointer
         * SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @param soaMessage A shared_ptr to a SoaMessage with a GPB payload
         * @return The action result containing action error code and SoaMessage derived-type object.
         */
        template <class Derived>
        static typename SoaActionResult<Derived>::SharedPtr parseFromMessage(const SoaMessage::SharedPtr soaMessage);

        /**
         * The payload is the user level request message as a string
         *
         * Set payload from a raw string of bytes wrapped by the string type
         * If the argument is a null reference, nothing is set, no exception occurs
         * @param rawPayload A std::string wrapped binary byte sequence for the message payload
         */
        virtual void setPayload(const std::string & rawPayload);

        /**
         * The payload is the user level request message as a string
         *
         * Set payload from a Google Protocol Buffer object
         * If the argument is a null reference, nothing is set, no exception occurs
         * @param payload A protocol buffer object for the message payload
         * @return SoaErrorCode::NO_ERROR if successful
         * @return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the argument is a null reference
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED if an error occurred
         *                            while serializing the data in the SoaMessage object
         */
        virtual SoaErrorCode setPayload(const ::google::protobuf::Message & payload);

        /**
         * The payload is the user level request message as a string
         *
         * Convenience method to parse a Google protocol message payload.
         * @param payloadObject Google protobuf object representing the payload
         * @param version API version
         * @return SoaErrorCode::No_ERROR if successful
         * @return SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED if protobuf parsing failed
         * @return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE if the argument is a null reference
         */
        virtual SoaErrorCode parsePayload(::google::protobuf::Message & payloadObject);

        /**
         * The payload is the user level request message as a string
         *
         * Get the raw payload bytes.
         * @return the raw payload bytes wrapped by a string object for convenience
         */
        virtual std::string getRawPayload();

        /**
         * Returns an action result including the action status and serialized message
         */
        virtual SoaActionResult<std::string>::SharedPtr serializeAsString();

        /**
         * Generates a human readable form of this message, useful for debugging and other purposes.
         * @return std::string
         */
        virtual std::string getDebugString();

        /**
         * Does the message have a payload?
         * @return true if the payload is populated, false otherwise
         */
        virtual bool hasPayload();

    protected:
        GPB m_extensionProto;

        /**
         * Parses m_messageProtobuf and m_extensionProto from string
         * @param message Serialized message
         * @return SOAErrorCode
         */
        virtual SoaErrorCode parseHeaderFromString(const std::string & message);

        /**
         * Validate the message has all required fields populated
         * @return always true here in the base class
         */
        virtual inline bool validate() {
            return true;
        }

    public:
        SoaMessageSubclassTemplate(const SoaMessageSubclassTemplate &) = default;
        SoaMessageSubclassTemplate & operator=(const SoaMessageSubclassTemplate &) = default;
};

// static
template <class GPB>
template <class Derived>
typename SoaActionResult<Derived>::SharedPtr SoaMessageSubclassTemplate<GPB>::parseFromString(const std::string & message) {
    if ( &message == nullptr ) {
        return SoaActionResult<Derived>::createResult(SoaErrorCode::ERROR_PARAM_NULL_REFERENCE);
    }
    std::shared_ptr<Derived> derivedMessage = std::make_shared<Derived>();
    SoaErrorCode error = derivedMessage->parseHeaderFromString(message);
    if (error != SoaErrorCode::NO_ERROR) {
        derivedMessage = nullptr;
    } else if ( ! derivedMessage->validate() ) {
        error = SoaErrorCode::ERROR_REQUIRED_FIELD_MISSING;
        derivedMessage = nullptr;
    } else {
        // no error, no adjustments needed
    }
    return SoaActionResult<Derived>::createResult(derivedMessage, error);
}

// static
template <class GPB>
template <class Derived>
typename SoaActionResult<Derived>::SharedPtr SoaMessageSubclassTemplate<GPB>::parseFromMessage(const SoaMessage::SharedPtr soaMessage) {
    if ( !soaMessage ) {
        return SoaActionResult<Derived>::createResult(SoaErrorCode::ERROR_PARAM_BAD);
    }
    SoaActionResult<std::string>::SharedPtr serializeResult = soaMessage->serializeAsString();
    if (serializeResult->getError() != SoaErrorCode::NO_ERROR) {
        return SoaActionResult<Derived>::createResult(serializeResult->getError());
    } else {
        return parseFromString<Derived>( *(serializeResult->getResult()) );
    }
}

template <class GPB>
SoaErrorCode SoaMessageSubclassTemplate<GPB>::parseHeaderFromString(const std::string & message) {
    // this call will check if message is a null reference
    // First parse the string as a simple SoaMessage
    SoaErrorCode error = SoaMessage::parseHeaderFromString(message);
    if (error == SoaErrorCode::NO_ERROR) {
        // now parse the payload of the simple SoaMessage layer as the SoaMessageDerivedTemplate extension
        error = m_extensionProto.ParseFromString( SoaMessage::getRawPayload() )
                 ? SoaErrorCode::NO_ERROR
                 : SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED;
    }
    return error;
}

template <class GPB>
SoaActionResult<std::string>::SharedPtr SoaMessageSubclassTemplate<GPB>::serializeAsString() {
    // First serialize the SoaMessageDerivedTemplate extension into the simple SoaMessage payload
    std::string serialized = m_extensionProto.SerializeAsString();
    if (!serialized.empty()){
        SoaMessage::setPayload( serialized );
        // now serialize the entire SoaMessage
        return SoaMessage::serializeAsString();
    } else {
        std::shared_ptr<std::string> emptyString = std::make_shared<std::string>();
        return SoaActionResult<std::string>::createResult(emptyString, SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED);
    }
}


template <class GPB>
void SoaMessageSubclassTemplate<GPB>::setPayload(const std::string & rawPayload) {
    if ( &rawPayload != nullptr ) {
        if ( !rawPayload.empty() ) {
            m_extensionProto.set_payload(rawPayload);
        } else {
            m_extensionProto.clear_payload();
        }
    }
}

template <class GPB>
SoaErrorCode SoaMessageSubclassTemplate<GPB>::setPayload(const ::google::protobuf::Message & payload) {
    if ( &payload == nullptr ) {
        return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE;
    }
    std::string serialized = payload.SerializeAsString();
    if (!serialized.empty()){
        setPayload(serialized);
        return SoaErrorCode::NO_ERROR;
    } else {
        return SoaErrorCode::ERROR_SOA_MESSAGE_SERIALIZATION_FAILED;
    }
}

template <class GPB>
SoaErrorCode SoaMessageSubclassTemplate<GPB>::parsePayload(::google::protobuf::Message & payloadObject) {
    if ( &payloadObject != nullptr ) {
        return payloadObject.ParseFromString(m_extensionProto.payload()) ? SoaErrorCode::NO_ERROR : SoaErrorCode::ERROR_SOA_MESSAGE_PARSING_FAILED;
    } else {
        return SoaErrorCode::ERROR_PARAM_NULL_REFERENCE;
    }
}


template <class GPB>
std::string SoaMessageSubclassTemplate<GPB>::getRawPayload() {
    return m_extensionProto.payload();
}

/**
 * Generates a human readable form of this message, useful for debugging and other purposes.
 * @return std::string
 */
template <class GPB>
std::string SoaMessageSubclassTemplate<GPB>::getDebugString() {
    return (SoaMessage::getDebugString() + "\n" + m_extensionProto.DebugString());
}

template <class GPB>
bool SoaMessageSubclassTemplate<GPB>::hasPayload() {
    return m_extensionProto.has_payload();
}

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */


#endif /* INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_SUBCLASS_TEMPLATE_HPP_ */
