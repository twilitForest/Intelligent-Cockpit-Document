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
 * soa_payload_parser.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: ASERGEE2
 */

#ifndef FNVSOA_FRAMEWORK_SOAPAYLOADPARSER_HPP_
#define FNVSOA_FRAMEWORK_SOAPAYLOADPARSER_HPP_

#include <string>
#include <soa/framework/soa_error_code.hpp>

namespace fnv {
namespace soa {
namespace framework {

/*
 * This interface should be used when there is no way to use standard protobuf parsing methods
 * The Payload class can be an arbitrary class defined by the application developer, where
 * the developer also defines the parsing function
 * 
 * Usage example:
 * class MyClass { ... };
 * class MyClassParser : public SoaPayloadParser<MyClass> {
 *    public:
 *       MyClassParser() {}
 *       virtual ~MyClassParser() {}
 *       virtual SoaErrorCode parse(MyClass & payload, const string& rawPayload, const string& version) {
 *          / / process/decode rawPayload according to its custom encoding
 *          payload.field1 = ...
 *			return NO_ERROR;
 *		 }
 *  }
 * 
 *  void processMessage(SoaMessage msg) {
 *     MyClass myObject;	/ / allocate/instantiate the object to populate
 *     MyClassParser myObjectParser;  / / instantiate the parser
 *     SoaErrorCode result = msg.parsePayloadFromString( myObject, VERSION, myObjectParser);
 *     / / the message is now parse and myObject is not populated
 *     ...
 * 
 */
template <class Payload>
class SoaPayloadParser {
public:
	SoaPayloadParser();
	virtual ~SoaPayloadParser();
	/**
	 * Parses SOA Message payload - This method must be defined by the application developer
	 * This method can be provided by the application developer to be executed by the
	 * SoaMessage convenience parse method parsePayloadFromString(...)
	 * @param payloadObject Object of an arbitrary user defined class to be populated with the parsed values
	 * @param payloadStream Serialized payload stream
	 * @param version SOA API version
	 * @return Parsing result
	 */
	virtual SoaErrorCode parse(Payload& payloadObject, const std::string& payloadStream, const std::string& version) = 0;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* FNVSOA_FRAMEWORK_SOAPAYLOADPARSER_HPP_ */
