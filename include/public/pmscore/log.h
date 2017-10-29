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

#ifndef LOGGING_H
#define LOGGING_H

#include "LoggerImpl.h"
#include "PmsLogging.h"

/* Logging for PMS  context ********
 * The parameters needed are
 * msgid - unique message id
 * kvcount - count for key-value pairs
 * ... - key-value pairs and free text. key-value pairs are formed using PMLOGKS or PMLOGKFV
 * e.g.)
 * LOG_CRITICAL(msgid, 2, PMLOGKS("key1", "value1"), PMLOGKFV("key2", "%s", value2), "free text message");
 **********************************************/

#define LOG_CONTEXT_CLIENT "powermgr"
#define LOG_CONTEXT_DEBUG  "powermgr"

#define MSG_DEBUG(format, ...)                    PmsLog::PmsDebugLog(mpLog, PMS_CLASS, PMS_FUNCTION, format, ##__VA_ARGS__)
#define MSG_NOTICE(format, ...)                   PmsLog::PmsNoticeLog(mpLog, PMS_CLASS, PMS_FUNCTION, format, ##__VA_ARGS__)
#define MSG_INFO(format, ...)                     PmsLog::PmsInfoLog(mpLog, PMS_CLASS, PMS_FUNCTION, format, ##__VA_ARGS__)
#define MSG_WARNING(format, ...)                  PmsLog::PmsWarningLog(mpLog, PMS_CLASS, PMS_FUNCTION, format, ##__VA_ARGS__)
#define MSG_ERROR(err, format, ...)               PmsLog::PmsErrorLog(mpLog, err, DUP(err), PMS_CLASS, PMS_FUNCTION, format, ##__VA_ARGS__)

#define MSG_DEBUG2(category, format,...)          PmsLog::PmsLogNonClass(category, kPmLogLevel_Debug, format, 0, ##__VA_ARGS__)
#define MSG_NOTICE2(category, format,...)         PmsLog::PmsLogNonClass(category, kPmLogLevel_Notice, format, 0, ##__VA_ARGS__)
#define MSG_INFO2(category, format,...)           PmsLog::PmsLogNonClass(category, kPmLogLevel_Info, format, 0, ##__VA_ARGS__)
#define MSG_WARNING2(category, format,...)        PmsLog::PmsLogNonClass(category, kPmLogLevel_Warning, format, 0, ##__VA_ARGS__)
#define MSG_ERROR2(category, err, format,...)     PmsLog::PmsLogNonClass(category, kPmLogLevel_Error, format, err, ##__VA_ARGS__)

/** list of MSGID's pairs */

#define MSGID_SHUTDOWN_APPS_SIG_FAIL              "SHUTDOWN_APPS_SIG_FAIL"   // Could not send shutdown applications
#define MSGID_SHUTDOWN_SRVC_SIG_FAIL              "SHUTDOWN_SRVC_SIG_FAIL"   // Could not send shutdown Services
#define MSGID_SUSPEND_SIG_FAIL                    "SUSPEND_SIG_FAIL"         // Could not send suspend signal
#define MSGID_RESUME_SIG_FAIL                     "RESUME_SIG_FAIL"          // Could not send resume signal
#define MSGID_CATEGORY_REG_FAIL                   "CATEGORY_REG_FAIL"        // could not register category
#define MSGID_MEM_ALLOC_FAIL                      "MEM_ALLOC_FAIL"           // memory allocation is failed
#define MSGID_SET_KEEP_AWAKE                      "SET_KEEP_AWAKE"           // setKeepAwake method
#define MSGID_SCEMA_VAL_FAIL                      "SCHEMA_VALIDATION_FAIL"   // schema validation is failed
#define MSGID_SERVER_STATUS_REGISTRY_FAIL         "SERVER_STATUS_REGISTRY_FAIL"  //server status registry failed
#define MSGID_TIME_RESTORE_FAIL                   "TIME_RESTORE_FAIL"        // Could not restore the time on bootup
#define MSGID_INVALID_INPUT                       "INVALID_INPUT"            // Invalid inputs passed

/** config.c */
#define MSGID_CONFIG_FILE_LOAD_ERR                "CONFIG_FILE_LOAD_ERR"     //Could not load config file from specified path

/** main.c */
#define MSGID_NYX_DEVICE_OPEN_FAIL                "NYX_DEVICE_OPEN_FAIL"     //Failed to open nyx device
#define MSGID_SRVC_REGISTER_FAIL                  "SRVC_REGISTER_FAIL"       //luna-bus registraton to check charger status failed
#define MSGID_MAINLOOP_FAIL                       "MAINLOOP_FAIL"            //Unable to initiate g_main_loop

/** init.c */
#define MSGID_HOOKINIT_FAIL                       "HOOKINIT_FAIL"            //Failed to initialize
#define MSGID_NAMED_INIT_FUNC_OOM                 "NAMED_INIT_FUNC_OOM"      //Out of memory on initialization
#define MSGID_NAMED_HOOK_LIST_OOM                 "NAMED_HOOK_LIST_OOM"      //Out of memory on initialization

/** machine.c */
#define MSGID_FRC_SHUTDOWN                        "FRC_SHUTDOWN"             // Force Shutdown
#define MSGID_FRC_REBOOT                          "FRC_REBOOT"               // Force Reboot
#define MSGID_SHUTDOWN_DEBUG                      "SHUTDOWN_DEBUG"           // For shutdown debug

/** shutdown.c */
#define MSGID_SHUTDOWN_APPS_SIG_FAIL              "SHUTDOWN_APPS_SIG_FAIL"   // Could not send shutdown applications
#define MSGID_SHUTDOWN_SRVC_SIG_FAIL              "SHUTDOWN_SRVC_SIG_FAIL"   // Could not send shutdown Services
#define MSGID_SHUTDOWN_REPLY_FAIL                 "SHUTDOWN_REPLY_FAIL"      // Could not send shutdown success message
#define MSGID_LSMSG_REPLY_FAIL                    "LSMSG_REPLY_FAIL"         // Could not send reply to caller
#define MSGID_LSSUBSCRI_ADD_FAIL                  "LSSUBSCRI_ADD_FAIL"       // LSSubscriptionAdd failed

/** suspend.c */
#define MSGID_PTHREAD_CREATE_FAIL                 "PTHREAD_CREATE_FAIL"      // Could not create SuspendThread
#define MSGID_NYX_DEV_OPEN_FAIL                   "NYX_DEV_OPEN_FAIL"        // Unable to open the nyx device led controller
#define MSGID_SUSPEND_DEBUG                       "SUSPEND_STATE_DEBUG"      // For suspend state machine debug
#define MSGID_GENERAL                             "GENERAL_DEBUG"            // For general debug message

/** suspend_ipc.c */
#define MSGID_LS_SUBSCRIB_SETFUN_FAIL             "LS_SUBSCRIB_SETFUN_FAIL"  // Error in setting cancel function

/** sawmill_logger.c */
#define MSGID_READ_PROC_MEMINFO_ERR               "READ_PROC_MEMINFO_ERR"    // Error while reading /proc/meminfo
#define MSGID_READ_PROC_STAT_ERR                  "READ_PROC_STAT_ERR"       // Error while reading /proc/stat
#define MSGID_READ_PROC_DISKSTAT_ERR              "READ_PROC_DISKSTAT_ERR"   // Error while reading /proc/diskstats
#define MSGID_READ_PROC_LOADAVG_ERR               "READ_PROC_LOADAVG_ERR"    // Error while reading /proc/loadavg
#define MSGID_READ_PROC_NETDEV_ERR                "READ_PROC_NETDEV_ERR"     // Error while reading /proc/net/dev
#define MSGID_ASSERTION_FAIL                      "ASSERTION_FAIL"           //Assertion failed

/** list of logkey ID's */

#define ERRTEXT                   "ERRTEXT"
#define ERRCODE                   "ERRCODE"
#define PATH                      "PATH"
#define CAUSE                     "CAUSE"
#define COMMAND                   "COMMAND"
#define NYX_QUERY_TIME            "NYX_QUERY_TIME"
#define SRVC_NAME                 "SRVC_NAME"
#define APP_NAME                  "APP_NAME"

PmLogContext GetPmsPmLogContext();
PmLogContext GetPMSIPCLogContext();

#endif // LOGGING_H

