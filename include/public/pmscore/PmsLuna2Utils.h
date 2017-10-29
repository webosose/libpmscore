// @@@LICENSE
//
//      Copyright (c) 2017 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef LS2_UTILS_H
#define LS2_UTILS_H

#include <luna-service2/lunaservice.hpp>
#include <pbnjson.hpp>
#include <string>

#define LS_CATEGORY_TABLE_NAME(name) name##_table

#define LS_CREATE_CATEGORY_BEGIN(cl, name) \
    typedef cl cl_t; \
    constexpr static const LSMethod LS_CATEGORY_TABLE_NAME(name)[] = {

#define LS_CATEGORY_MAPPED_METHOD(name, func) { #name, \
    &LS::Handle::methodWraper<cl_t, &cl_t::func>, \
    static_cast<LSMethodFlags>(0) },

#define LS_CATEGORY_CLASS_METHOD(cls, name) { #name, \
    &LS::Handle::methodWraper<cls, &cls::name>, \
    static_cast<LSMethodFlags>(0) },

#define LS_CREATE_CATEGORY_END \
{ nullptr, nullptr } \
    }; \

// Build a schema as a const char * string without any execution overhead
#define SCHEMA_ANY                                    "{}"
#define SCHEMA_1(param)                               "{\"type\":\"object\",\"properties\":{" param "},\"additionalProperties\":false}"

#define PROPS_1(p1)                                   ",\"properties\":{" p1 "}"
#define PROPS_2(p1, p2)                               ",\"properties\":{" p1 "," p2 "}"
#define PROPS_3(p1, p2, p3)                           ",\"properties\":{" p1 "," p2 "," p3 "}"
#define PROPS_4(p1, p2, p3, p4)                       ",\"properties\":{" p1 "," p2 "," p3 "," p4 "}"
#define PROPS_5(p1, p2, p3, p4, p5)                   ",\"properties\":{" p1 "," p2 "," p3 "," p4 "," p5 "}"
#define PROPS_6(p1, p2, p3, p4, p5, p6)               ",\"properties\":{" p1 "," p2 "," p3 "," p4 "," p5 "," p6 "}"
#define PROPS_7(p1, p2, p3, p4, p5, p6, p7)           ",\"properties\":{" p1 "," p2 "," p3 "," p4 "," p5 "," p6 "," p7 "}"
#define PROPS_8(p1, p2, p3, p4, p5, p6, p7, p8)       ",\"properties\":{" p1 "," p2 "," p3 "," p4 "," p5 "," p6 "," p7 "," p8 "}"
#define PROPS_9(p1, p2, p3, p4, p5, p6, p7, p8, p9)   ",\"properties\":{" p1 "," p2 "," p3 "," p4 "," p5 "," p6 "," p7 "," p8 "," p9 "}"
#define REQUIRED_1(p1)                                ",\"required\":[\"" #p1 "\"]"
#define REQUIRED_2(p1, p2)                            ",\"required\":[\"" #p1 "\",\"" #p2 "\"]"
#define REQUIRED_3(p1, p2, p3)                        ",\"required\":[\"" #p1 "\",\"" #p2 "\",\"" #p3 "\"]"
#define REQUIRED_4(p1, p2, p3, p4)                    ",\"required\":[\"" #p1 "\",\"" #p2 "\",\"" #p3 "\",\"" #p4 "\"]"
#define STRICT_SCHEMA(attributes)                     "{\"type\":\"object\"" attributes ",\"additionalProperties\":false}"
#define RELAXED_SCHEMA(attributes)                    "{\"type\":\"object\"" attributes ",\"additionalProperties\":true}"

// Macros to use in place of the parameters in the SCHEMA_xxx macros above
#define PROP(name, type)                              "\"" #name "\":{\"type\":\"" #type "\"}"
#define PROP_WITH_VAL_1(name, type, v1)               "\"" #name "\":{\"type\":\"" #type "\", \"enum\": [" #v1 "]}"
#define PROP_WITH_VAL_2(name, type, v1, v2)           "\"" #name "\":{\"type\":\"" #type "\", \"enum\": [" #v1 ", " #v2 "]}"
#define ARRAY(name, type)                             "\"" #name "\":{\"type\":\"array\", \"items\":{\"type\":\"" #type "\"}}"
#define OBJARRAY(name, objschema)                     "\"" #name "\":{\"type\":\"array\", \"items\": " objschema "}"
#define OBJSCHEMA_1(param)                            "{\"type\":\"object\",\"properties\":{" param "}}"
#define OBJSCHEMA_2(p1, p2)                           "{\"type\":\"object\",\"properties\":{" p1 "," p2 "}}"
#define OBJSCHEMA_3(p1, p2, p3)                       "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 "}}"
#define OBJSCHEMA_4(p1, p2, p3, p4)                   "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 ", " p4 "}}"
#define OBJSCHEMA_5(p1, p2, p3, p4, p5)               "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 ", " p4 ", " p5 "}}"
#define OBJSCHEMA_6(p1, p2, p3, p4, p5, p6)           "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 ", " p4 ", " p5 ", " p6 "}}"
#define OBJSCHEMA_7(p1, p2, p3, p4, p5, p6, p7)       "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 ", " p4 ", " p5 ", " p6 ", " p7 "}}"
#define OBJSCHEMA_8(p1, p2, p3, p4, p5, p6, p7, p8)   "{\"type\":\"object\",\"properties\":{" p1 "," p2 ", " p3 ", " p4 ", " p5 ", " p6 ", " p7 ", " p8 "}}"
#define OBJECT(name, objschema)                       "\"" #name "\":" objschema

#define POWER2_BUSY                      1
#define INVALID_JSON_FORMAT              2
#define CLIENT_NOT_REGISTERED            3
#define CLIENT_ALREADY_REGISTERED        4
#define RESPONSE_TOO_LATE                5
#define TIMER_NOT_RUNNING                6
#define UNKNOWN_ERROR                    7

const std::string errorPower2Busy("com.webos.service.power2 is busy");
const std::string errorInvalidJsonFormat("Invalid JSON format");
const std::string errorClientNotRegistered("The client is not registered");
const std::string errorCleintAlreadyRegistered("The client is already registered");
const std::string errorResponseTooLate("The response is too late");
const std::string errorTimerNotRunning("The timer is not running");

#define JSON_PARSE_SCHEMA_ERROR 1

const std::string errorInvalidParam("Invalid parameters");
const std::string errorUnknown("Unknown error");
const std::string errorMalformedJson("Malformed json");
const std::string errorParseFailed("parsePayload failed");
const std::string errorInvalidClient("Invalid client ID");
const std::string errorKeepAwakeSet("keepawake is already set");
const std::string errorInternalError("internal error");

namespace LSUtils {

inline bool generatePayload(const pbnjson::JValue &object, std::string &payload)
{
    pbnjson::JGenerator serializer(nullptr);
    return serializer.toString(object, pbnjson::JSchema::AllSchema(), payload);
}

inline bool parsePayload(const std::string &payload, pbnjson::JValue &object)
{
    pbnjson::JSchema parseSchema = pbnjson::JSchema::AllSchema();
    pbnjson::JDomParser parser;

    if (!parser.parse(payload, parseSchema)) {
        return false;
    }

    object = parser.getDom();

    return true;
}


inline bool parsePayload(const std::string &payload, pbnjson::JValue &object, const std::string &schema, int *error)
{
    pbnjson::JSchema parseSchema = pbnjson::JSchema::AllSchema();

    if (schema.length() > 0) {
        parseSchema = pbnjson::JSchemaFragment(schema);
    }

    pbnjson::JDomParser parser;

    if (!parser.parse(payload, parseSchema)) {
        if (strstr(parser.getError(), "Schema error") != '\0') {
            // notify this is a schema error, so that caller can make further
            // checks for throwing custom errors (particular key missing, etc)
            pbnjson::JSchema parseSchema = pbnjson::JSchema::AllSchema();

            if (parser.parse(payload, parseSchema)) {
                *error = JSON_PARSE_SCHEMA_ERROR;
                object = parser.getDom();
            }
        }

        return false;
    }

    object = parser.getDom();
    return true;
}

inline void respondWithError(LS::Message &message, const std::string &errorText, unsigned int errorCode = -1,
                             bool failedSubscription = false)
{
    pbnjson::JValue responseObj = pbnjson::Object();

    if (failedSubscription) {
        responseObj.put("subscribed", false);
    }

    responseObj.put("returnValue", false);
    responseObj.put("errorText", errorText);
    responseObj.put("errorCode", (int) errorCode);

    std::string payload;
    generatePayload(responseObj, payload);

    message.respond(payload.c_str());
}

inline void respondWithErrorText(LS::Message &message, const std::string &errorText)
{
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", false);
    responseObj.put("errorText", errorText);

    std::string payload;
    generatePayload(responseObj, payload);

    message.respond(payload.c_str());
}

inline void respondWithError(LSMessage *message, const std::string &errorText, unsigned int errorCode = -1)
{
    LS::Message msg(message);
    respondWithError(msg, errorText, errorCode);
}


inline void postToSubscriptionPoint(LS::SubscriptionPoint *subscriptionPoint, pbnjson::JValue &object)
{
    std::string payload;
    LSUtils::generatePayload(object, payload);

    subscriptionPoint->post(payload.c_str());
}

inline void postToClient(LS::Message &message, pbnjson::JValue &object)
{
    std::string payload;
    LSUtils::generatePayload(object, payload);

    try {
        message.respond(payload.c_str());
    } catch (LS::Error &error) {
        // to put debug log
    }
}

inline void postToClient(LSMessage *message, pbnjson::JValue &object)
{
    if (!message) {
        return;
    }

    LS::Message request(message);
    postToClient(request, object);
}

} // namespace LSUtils
#endif // LS2_UTILS_H

