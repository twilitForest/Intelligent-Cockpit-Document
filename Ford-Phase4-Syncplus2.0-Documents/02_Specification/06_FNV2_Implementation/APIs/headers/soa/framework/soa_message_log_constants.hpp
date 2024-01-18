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
 * log_constants.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_LOG_CONSTANTS_HPP_
#define INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_LOG_CONSTANTS_HPP_

#include <string>

namespace fnv {
namespace soa {
namespace framework {

extern const std::string MD_EMPTY;
extern const std::string MD_NULL_PTR;
extern const std::string MD_STR_EMPTY;
extern const std::string MD_DUPLICATE;
extern const std::string MD_PARSING_ERROR;
extern const std::string PD_PARSING_ERROR;
extern const std::string ENDPOINT_NULL_PTR;
extern const std::string ENDPOINT_DUPLICATE;
extern const std::string COMMAND_EMPTY;
extern const std::string CLIENT_ID_EMPTY;
extern const std::string COMMAND_DUPLICATE;
// Format to be used with Logger log methods to report an empty collection:
// SoaLogger::logError(EMPTY_LIST_ERROR_FORMAT, "listVariableName");
// where listVariableName is a list variable name
extern const std::string LIST_EMPTY_ERROR_FORMAT;
// Format to be used with Logger log methods to report that a collection contains a nullptr element:
// SoaLogger::logError(NULLPTR_LIST_ELEMET_ERROR_FORMAT, "listVariableName");
// where listVariableName is a list variable name
extern const std::string LIST_ELEMENT_NULLPTR_ERROR_FORMAT;
// Format to be used with Logger log methods to report that a parameter is a nullptr:
// SoaLogger::logError(PARAM_NULLPTR_ERROR_FORMAT, "parameterName");
// where parameterName is a parameter name
extern const std::string PARAM_NULLPTR_ERROR_FORMAT;
// Format to be used with Logger log methods to report an empty string:
// SoaLogger::logError(STRING_EMPTY_ERROR_FORMAT, "stringVariableName");
// where stringVariableName is the name of a string param or variable
extern const std::string STRING_EMPTY_ERROR_FORMAT;
// Format to be used with Logger log methods to report a protobuf class parsing error:
// SoaLogger::logError(STRING_EMPTY_ERROR_FORMAT, "theError", message);
// where 	"theError" is the error to log
// 			message is a raw protobuf string, DebugString() method can be used to
//			convert proto instance to human readable string.
// 			WARNIG!!! DebugString() can return long strings.
extern const std::string MESSAGE_PARSING_FAILED_FORMAT;

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_FRAMEWORK_SOA_MESSAGE_LOG_CONSTANTS_HPP_ */
