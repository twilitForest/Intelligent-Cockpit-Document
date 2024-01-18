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
 * soa_analytics.hpp
 *
 *  Created on: Apr 27, 2018
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_ANALYTICS_HPP_
#define INCLUDE_SOA_UTILS_SOA_ANALYTICS_HPP_

#include <string>
#include <device_abstraction_factory.hpp>

namespace fnv {
namespace soa {
namespace framework {

class SoaDiagnosticsEvent {
public:

    // Supported diagnostics event types.
    enum class Type {
        GW_MQTT_CONNECTION_FAILED   = (int)fnv::soa::deviceabstraction::DeviceAnalytics::EventType::GW_MQTT_CONNECTION_FAILED,
        GW_IPC_ERROR                = (int)fnv::soa::deviceabstraction::DeviceAnalytics::EventType::GW_IPC_ERROR,
        GW_ERROR                    = (int)fnv::soa::deviceabstraction::DeviceAnalytics::EventType::GW_ERROR
    };

    // Supported diagnostics severity.
    enum class Severity {
        CRITICAL = (int)fnv::soa::deviceabstraction::DeviceAnalytics::Severity::CRITICAL
    };

    // Functions to send logworthy with comment containing no argument.
    bool sendLogworthy(const char* filename, int linenumber, const std::string& lwcomment) const;
    bool sendLogworthy(const char* filename, int linenumber, const char* lwcomment) const;

    // Functions to send logworthy with comment containing arguments.
    template<typename... Args>
    void sendLogworthy(const char* filename, int linenumber, const std::string& lwcomment, Args... args) const {
        char comment[commentMaxSize];
        snprintf(comment, sizeof(comment), lwcomment.c_str(), args ..., "");
        send(filename, linenumber, comment);
    }
    template<typename Format, typename... Args>
    void sendLogworthy(const char* filename, int linenumber, Format lwcomment, Args... args) const {
        char comment[commentMaxSize];
        snprintf(comment, sizeof(comment), lwcomment, args ..., "");
        send(filename, linenumber, comment);
    }

protected:

    explicit SoaDiagnosticsEvent(
            SoaDiagnosticsEvent::Type eventType,
            SoaDiagnosticsEvent::Severity severity);

private:

    SoaDiagnosticsEvent::Type       m_eventType;
    SoaDiagnosticsEvent::Severity   m_severity;
    const char * const              k_className;

    bool send(const char* filename, int linenumber, const std::string& lwcomment) const;

    // The maximum size (in bytes) of a comment.
    const static int commentMaxSize = 256;
};

class SoaAnalyticsEvent {
public:

    // Supported analytics event types.
    enum class Type {
        GW_MQTT_CONNECTION_LOST     = (int)fnv::soa::deviceabstraction::DeviceAnalytics::EventType::GW_MQTT_CONNECTION_LOST,
    };

    // Supported analytics severity.
    enum class Severity {
        CRITICAL = (int)fnv::soa::deviceabstraction::DeviceAnalytics::Severity::CRITICAL
    };

    // Functions to send analytics event with comment containing no argument.
    bool sendAnalytics(const char* filename,
                        int linenumber,
                        std::map<std::string, std::string>& attributes,
                        const std::string& anadescription) const;
    bool sendAnalytics(const char* filename,
                        int linenumber,
                        std::map<std::string, std::string>& attributes,
                        const char* anadescription) const;

    // Functions to send analytics event with comment containing arguments.
    template<typename... Args>
    void sendAnalytics(const char* filename,
                        int linenumber,
                        std::map<std::string, std::string>& attributes,
                        const std::string& anadescription, Args... args) const {
        char comment[commentMaxSize];
        snprintf(comment, sizeof(comment), anadescription.c_str(), args ..., "");
        send(filename, linenumber, attributes, comment);
    }
    template<typename Format, typename... Args>
    void sendAnalytics(const char* filename,
                        int linenumber,
                        std::map<std::string, std::string>& attributes,
                        Format anadescription,
                        Args... args) const {
        char comment[commentMaxSize];
        snprintf(comment, sizeof(comment), anadescription, args ..., "");
        send(filename, linenumber, attributes, comment);
    }

protected:

    explicit SoaAnalyticsEvent(
            std::string category,
            std::string action,
            SoaAnalyticsEvent::Severity severity);

private:

    std::string                     m_category;
    std::string                     m_action;
    SoaAnalyticsEvent::Severity     m_severity;
    const char * const              k_className;

    bool send(const char* filename,
                int linenumber,
                std::map<std::string, std::string>& attributes,
                const std::string& anadescription) const;

    // The maximum size (in bytes) of a comment.
    const static int commentMaxSize = 256;
};
/*
 * SOA wrapper for diagnostics and analytics events
 */
class SoaAnalytics {
public:
    /**
     * Builds and asynchronously sends a logworthy event
     * @param eventType an event type
     * @param severity an event severity
     * @param attributeKey attribute key
     * @param attributeValue attribute value
     * @return true if the event was build successfully
     */
    static bool sendLogworthyEvent(
            SoaDiagnosticsEvent::Type eventType,
            SoaDiagnosticsEvent::Severity severity,
            const std::string& attributeKey,
            const std::string& attributeValue);
    /**
     * Builds and asynchronously sends an analytics event
     * @param category an event category
     * @param action an event action
     * @param severity an event severity
     * @param attributes event attributes
     * @param description an event description
     * @return true if the event was build successfully
     */
    static bool sendAnalyticsEvent(
            const std::string& category,
            const std::string& action,
            SoaAnalyticsEvent::Severity severity,
            const std::map<std::string, std::string>& attributes,
            const std::string& description = std::string());
private:
    const static fnv::soa::deviceabstraction::DeviceAnalytics::SharedPtr m_analytics;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_ANALYTICS_HPP_ */
