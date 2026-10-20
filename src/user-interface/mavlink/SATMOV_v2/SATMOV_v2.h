/** @file
 *  @brief MAVLink comm protocol generated from SATMOV_v2.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_SATMOV_V2_H
#define MAVLINK_SATMOV_V2_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_SATMOV_V2.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 0

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{1, 163, 129, 129, 0, 0, 0}, {2, 143, 1, 1, 0, 0, 0}, {3, 249, 1, 1, 0, 0, 0}, {4, 24, 1, 1, 0, 0, 0}, {5, 140, 13, 13, 0, 0, 0}, {6, 206, 1, 1, 0, 0, 0}, {7, 31, 1, 1, 0, 0, 0}, {8, 68, 129, 129, 0, 0, 0}, {9, 132, 1, 1, 0, 0, 0}, {10, 45, 1, 1, 0, 0, 0}, {11, 125, 1, 1, 0, 0, 0}, {12, 180, 1, 1, 0, 0, 0}, {13, 78, 130, 130, 0, 0, 0}, {14, 108, 3, 3, 0, 0, 0}, {15, 142, 1, 1, 0, 0, 0}, {16, 128, 5, 5, 0, 0, 0}, {17, 55, 1, 1, 0, 0, 0}, {18, 30, 10, 10, 0, 0, 0}, {19, 227, 1, 1, 0, 0, 0}, {20, 189, 10, 10, 0, 0, 0}, {21, 145, 1, 1, 0, 0, 0}, {22, 127, 18, 18, 0, 0, 0}, {23, 126, 1, 1, 0, 0, 0}, {24, 183, 1, 1, 0, 0, 0}, {25, 240, 201, 201, 0, 0, 0}, {26, 39, 1, 1, 0, 0, 0}, {27, 176, 1, 1, 0, 0, 0}, {28, 1, 1, 1, 0, 0, 0}, {29, 165, 1, 1, 0, 0, 0}, {30, 33, 2, 2, 0, 0, 0}, {31, 224, 1, 1, 0, 0, 0}, {32, 235, 64, 64, 0, 0, 0}, {33, 218, 1, 1, 0, 0, 0}, {34, 234, 1, 1, 0, 0, 0}, {35, 18, 1, 1, 0, 0, 0}, {40, 44, 1, 1, 0, 0, 0}, {41, 108, 62, 62, 0, 0, 0}, {42, 15, 1, 1, 0, 0, 0}, {43, 159, 1, 1, 0, 0, 0}, {44, 242, 77, 77, 0, 0, 0}, {45, 174, 1, 1, 0, 0, 0}, {46, 42, 1, 1, 0, 0, 0}, {47, 187, 152, 152, 0, 0, 0}, {48, 111, 1, 1, 0, 0, 0}, {49, 99, 128, 128, 0, 0, 0}, {50, 167, 1, 1, 0, 0, 0}, {51, 91, 13, 13, 0, 0, 0}, {60, 112, 22, 22, 0, 0, 0}, {61, 110, 145, 145, 0, 0, 0}, {62, 105, 23, 23, 0, 0, 0}, {63, 90, 23, 23, 0, 0, 0}, {64, 173, 22, 22, 0, 0, 0}, {65, 63, 145, 145, 0, 0, 0}, {70, 222, 36, 36, 0, 0, 0}, {71, 133, 1, 1, 0, 0, 0}, {72, 247, 4, 4, 0, 0, 0}, {73, 73, 32, 32, 0, 0, 0}, {74, 144, 40, 40, 0, 0, 0}, {75, 199, 1, 1, 0, 0, 0}, {76, 217, 1, 1, 0, 0, 0}, {77, 48, 1, 1, 0, 0, 0}, {101, 82, 14, 14, 0, 0, 0}, {102, 250, 51, 51, 0, 0, 0}, {103, 55, 46, 46, 0, 0, 0}, {104, 86, 25, 25, 0, 0, 0}, {105, 196, 112, 112, 0, 0, 0}, {106, 47, 44, 44, 0, 0, 0}, {120, 196, 201, 201, 0, 0, 0}, {130, 165, 201, 201, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_SATMOV_V2

// ENUM DEFINITIONS



// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_firmware_reday.h"
#include "./mavlink_msg_correct_imu.h"
#include "./mavlink_msg_correct_imu_result.h"
#include "./mavlink_msg_request_imu_drift.h"
#include "./mavlink_msg_imu_drift.h"
#include "./mavlink_msg_restart.h"
#include "./mavlink_msg_restart_result.h"
#include "./mavlink_msg_update_firmware.h"
#include "./mavlink_msg_update_firmware_result.h"
#include "./mavlink_msg_switch_firmware.h"
#include "./mavlink_msg_switch_firmware_result.h"
#include "./mavlink_msg_read_version.h"
#include "./mavlink_msg_report_version.h"
#include "./mavlink_msg_switch_antenna.h"
#include "./mavlink_msg_switch_antenna_result.h"
#include "./mavlink_msg_set_report.h"
#include "./mavlink_msg_set_report_result.h"
#include "./mavlink_msg_set_beacon.h"
#include "./mavlink_msg_set_beacon_result.h"
#include "./mavlink_msg_set_transducer.h"
#include "./mavlink_msg_set_transducer_result.h"
#include "./mavlink_msg_set_beam_point.h"
#include "./mavlink_msg_set_beam_point_result.h"
#include "./mavlink_msg_self_check.h"
#include "./mavlink_msg_self_check_result.h"
#include "./mavlink_msg_start_tracking.h"
#include "./mavlink_msg_start_tracking_result.h"
#include "./mavlink_msg_stop_tracking.h"
#include "./mavlink_msg_stop_tracking_result.h"
#include "./mavlink_msg_set_monitor.h"
#include "./mavlink_msg_set_monitor_result.h"
#include "./mavlink_msg_set_product_info.h"
#include "./mavlink_msg_set_product_info_result.h"
#include "./mavlink_msg_new_log_file.h"
#include "./mavlink_msg_new_log_file_result.h"
#include "./mavlink_msg_request_beam_parameter.h"
#include "./mavlink_msg_beam_parameter.h"
#include "./mavlink_msg_set_beam_parameter_result.h"
#include "./mavlink_msg_request_tracking_parameter.h"
#include "./mavlink_msg_tracking_parameter.h"
#include "./mavlink_msg_set_tracking_parameter_result.h"
#include "./mavlink_msg_request_work_parameter.h"
#include "./mavlink_msg_work_parameter.h"
#include "./mavlink_msg_set_work_parameter_result.h"
#include "./mavlink_msg_update_tle.h"
#include "./mavlink_msg_update_tle_result.h"
#include "./mavlink_msg_issue_earth_point_local.h"
#include "./mavlink_msg_passthrough_22b_pack.h"
#include "./mavlink_msg_passthrough_286b_pack.h"
#include "./mavlink_msg_beacon_pack.h"
#include "./mavlink_msg_transducer_pack.h"
#include "./mavlink_msg_arm_22b_pack.h"
#include "./mavlink_msg_arm_286b_pack.h"
#include "./mavlink_msg_currect_yaw.h"
#include "./mavlink_msg_currect_yaw_result.h"
#include "./mavlink_msg_currect_yaw_complete.h"
#include "./mavlink_msg_issue_earth_point.h"
#include "./mavlink_msg_save_earth_point.h"
#include "./mavlink_msg_save_earth_point_result.h"
#include "./mavlink_msg_clear_earth_point.h"
#include "./mavlink_msg_clear_earth_point_result.h"
#include "./mavlink_msg_report_beacon_state.h"
#include "./mavlink_msg_report_imu_state.h"
#include "./mavlink_msg_report_antenna_state.h"
#include "./mavlink_msg_report_transducer_state.h"
#include "./mavlink_msg_report_machine_state.h"
#include "./mavlink_msg_report_satellite_info.h"
#include "./mavlink_msg_log_string.h"
#include "./mavlink_msg_openamip_string.h"

// base include


#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 0

#if MAVLINK_THIS_XML_IDX == MAVLINK_PRIMARY_XML_IDX
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_FIRMWARE_REDAY, MAVLINK_MESSAGE_INFO_CORRECT_IMU, MAVLINK_MESSAGE_INFO_CORRECT_IMU_RESULT, MAVLINK_MESSAGE_INFO_REQUEST_IMU_DRIFT, MAVLINK_MESSAGE_INFO_IMU_DRIFT, MAVLINK_MESSAGE_INFO_RESTART, MAVLINK_MESSAGE_INFO_RESTART_RESULT, MAVLINK_MESSAGE_INFO_UPDATE_FIRMWARE, MAVLINK_MESSAGE_INFO_UPDATE_FIRMWARE_RESULT, MAVLINK_MESSAGE_INFO_SWITCH_FIRMWARE, MAVLINK_MESSAGE_INFO_SWITCH_FIRMWARE_RESULT, MAVLINK_MESSAGE_INFO_READ_VERSION, MAVLINK_MESSAGE_INFO_REPORT_VERSION, MAVLINK_MESSAGE_INFO_SWITCH_ANTENNA, MAVLINK_MESSAGE_INFO_SWITCH_ANTENNA_RESULT, MAVLINK_MESSAGE_INFO_SET_REPORT, MAVLINK_MESSAGE_INFO_SET_REPORT_RESULT, MAVLINK_MESSAGE_INFO_SET_BEACON, MAVLINK_MESSAGE_INFO_SET_BEACON_RESULT, MAVLINK_MESSAGE_INFO_SET_TRANSDUCER, MAVLINK_MESSAGE_INFO_SET_TRANSDUCER_RESULT, MAVLINK_MESSAGE_INFO_SET_BEAM_POINT, MAVLINK_MESSAGE_INFO_SET_BEAM_POINT_RESULT, MAVLINK_MESSAGE_INFO_SELF_CHECK, MAVLINK_MESSAGE_INFO_SELF_CHECK_RESULT, MAVLINK_MESSAGE_INFO_START_TRACKING, MAVLINK_MESSAGE_INFO_START_TRACKING_RESULT, MAVLINK_MESSAGE_INFO_STOP_TRACKING, MAVLINK_MESSAGE_INFO_STOP_TRACKING_RESULT, MAVLINK_MESSAGE_INFO_SET_MONITOR, MAVLINK_MESSAGE_INFO_SET_MONITOR_RESULT, MAVLINK_MESSAGE_INFO_SET_PRODUCT_INFO, MAVLINK_MESSAGE_INFO_SET_PRODUCT_INFO_RESULT, MAVLINK_MESSAGE_INFO_NEW_LOG_FILE, MAVLINK_MESSAGE_INFO_NEW_LOG_FILE_RESULT, MAVLINK_MESSAGE_INFO_REQUEST_BEAM_PARAMETER, MAVLINK_MESSAGE_INFO_BEAM_PARAMETER, MAVLINK_MESSAGE_INFO_SET_BEAM_PARAMETER_RESULT, MAVLINK_MESSAGE_INFO_REQUEST_TRACKING_PARAMETER, MAVLINK_MESSAGE_INFO_TRACKING_PARAMETER, MAVLINK_MESSAGE_INFO_SET_TRACKING_PARAMETER_RESULT, MAVLINK_MESSAGE_INFO_REQUEST_WORK_PARAMETER, MAVLINK_MESSAGE_INFO_WORK_PARAMETER, MAVLINK_MESSAGE_INFO_SET_WORK_PARAMETER_RESULT, MAVLINK_MESSAGE_INFO_UPDATE_TLE, MAVLINK_MESSAGE_INFO_UPDATE_TLE_RESULT, MAVLINK_MESSAGE_INFO_ISSUE_EARTH_POINT_LOCAL, MAVLINK_MESSAGE_INFO_PASSTHROUGH_22B_PACK, MAVLINK_MESSAGE_INFO_PASSTHROUGH_286B_PACK, MAVLINK_MESSAGE_INFO_BEACON_PACK, MAVLINK_MESSAGE_INFO_TRANSDUCER_PACK, MAVLINK_MESSAGE_INFO_ARM_22B_PACK, MAVLINK_MESSAGE_INFO_ARM_286B_PACK, MAVLINK_MESSAGE_INFO_CURRECT_YAW, MAVLINK_MESSAGE_INFO_CURRECT_YAW_RESULT, MAVLINK_MESSAGE_INFO_CURRECT_YAW_COMPLETE, MAVLINK_MESSAGE_INFO_ISSUE_EARTH_POINT, MAVLINK_MESSAGE_INFO_SAVE_EARTH_POINT, MAVLINK_MESSAGE_INFO_SAVE_EARTH_POINT_RESULT, MAVLINK_MESSAGE_INFO_CLEAR_EARTH_POINT, MAVLINK_MESSAGE_INFO_CLEAR_EARTH_POINT_RESULT, MAVLINK_MESSAGE_INFO_REPORT_BEACON_STATE, MAVLINK_MESSAGE_INFO_REPORT_IMU_STATE, MAVLINK_MESSAGE_INFO_REPORT_ANTENNA_STATE, MAVLINK_MESSAGE_INFO_REPORT_TRANSDUCER_STATE, MAVLINK_MESSAGE_INFO_REPORT_MACHINE_STATE, MAVLINK_MESSAGE_INFO_REPORT_SATELLITE_INFO, MAVLINK_MESSAGE_INFO_LOG_STRING, MAVLINK_MESSAGE_INFO_OPENAMIP_STRING}
# define MAVLINK_MESSAGE_NAMES {{ "ARM_22B_PACK", 64 }, { "ARM_286B_PACK", 65 }, { "BEACON_PACK", 62 }, { "BEAM_PARAMETER", 41 }, { "CLEAR_EARTH_POINT", 76 }, { "CLEAR_EARTH_POINT_RESULT", 77 }, { "CORRECT_IMU", 2 }, { "CORRECT_IMU_RESULT", 3 }, { "CURRECT_YAW", 70 }, { "CURRECT_YAW_COMPLETE", 72 }, { "CURRECT_YAW_RESULT", 71 }, { "FIRMWARE_REDAY", 1 }, { "IMU_DRIFT", 5 }, { "ISSUE_EARTH_POINT", 73 }, { "ISSUE_EARTH_POINT_LOCAL", 51 }, { "LOG_STRING", 120 }, { "NEW_LOG_FILE", 34 }, { "NEW_LOG_FILE_RESULT", 35 }, { "OPENAMIP_STRING", 130 }, { "PASSTHROUGH_22B_PACK", 60 }, { "PASSTHROUGH_286B_PACK", 61 }, { "READ_VERSION", 12 }, { "REPORT_ANTENNA_STATE", 103 }, { "REPORT_BEACON_STATE", 101 }, { "REPORT_IMU_STATE", 102 }, { "REPORT_MACHINE_STATE", 105 }, { "REPORT_SATELLITE_INFO", 106 }, { "REPORT_TRANSDUCER_STATE", 104 }, { "REPORT_VERSION", 13 }, { "REQUEST_BEAM_PARAMETER", 40 }, { "REQUEST_IMU_DRIFT", 4 }, { "REQUEST_TRACKING_PARAMETER", 43 }, { "REQUEST_WORK_PARAMETER", 46 }, { "RESTART", 6 }, { "RESTART_RESULT", 7 }, { "SAVE_EARTH_POINT", 74 }, { "SAVE_EARTH_POINT_RESULT", 75 }, { "SELF_CHECK", 24 }, { "SELF_CHECK_RESULT", 25 }, { "SET_BEACON", 18 }, { "SET_BEACON_RESULT", 19 }, { "SET_BEAM_PARAMETER_RESULT", 42 }, { "SET_BEAM_POINT", 22 }, { "SET_BEAM_POINT_RESULT", 23 }, { "SET_MONITOR", 30 }, { "SET_MONITOR_RESULT", 31 }, { "SET_PRODUCT_INFO", 32 }, { "SET_PRODUCT_INFO_RESULT", 33 }, { "SET_REPORT", 16 }, { "SET_REPORT_RESULT", 17 }, { "SET_TRACKING_PARAMETER_RESULT", 45 }, { "SET_TRANSDUCER", 20 }, { "SET_TRANSDUCER_RESULT", 21 }, { "SET_WORK_PARAMETER_RESULT", 48 }, { "START_TRACKING", 26 }, { "START_TRACKING_RESULT", 27 }, { "STOP_TRACKING", 28 }, { "STOP_TRACKING_RESULT", 29 }, { "SWITCH_ANTENNA", 14 }, { "SWITCH_ANTENNA_RESULT", 15 }, { "SWITCH_FIRMWARE", 10 }, { "SWITCH_FIRMWARE_RESULT", 11 }, { "TRACKING_PARAMETER", 44 }, { "TRANSDUCER_PACK", 63 }, { "UPDATE_FIRMWARE", 8 }, { "UPDATE_FIRMWARE_RESULT", 9 }, { "UPDATE_TLE", 49 }, { "UPDATE_TLE_RESULT", 50 }, { "WORK_PARAMETER", 47 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_SATMOV_V2_H
