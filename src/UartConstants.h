// @@@LICENSE
//
//      Copyright (c) 2022 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef _UARTCONSTANTS_H
#define _UARTCONSTANTS_H

#define CMD_SUCCESS  0x01
#define CMD_FAILURE  0x00
#define UART_DATA_SIZE  35
#define DEVICENAME_LENGTH  25
#define CRC_MSB  0x0a
#define CRC_LSB  0x0d

enum CommandId {
    HOST_CMD_MODULE_START = 0x01,
    HOST_CMD_MODULE_STOP = 0x02,
    HOST_REQ_MODULE_INFO = 0x03,
    HOST_REQ_PAIRING_INFO = 0x04,
    HOST_SET_DEVICE_TYPE = 0x05,
    HOST_SET_DEVICE_MODE = 0x06,
    HOST_SET_MEASUREMENT_CONFIG = 0x07,
    HOST_SET_BYPASS_DATA = 0x08,
    HOST_SET_TRANSFER_DATA = 0x09,
    HOST_SET_SCAN_TIME = 0x0A,
    HOST_REQ_DEVICE_DISCOVERY = 0x0B,
    HOST_REQ_DISCOVERY_STOP = 0x0C,
    HOST_REQ_ADVERTISING = 0x1A,
    HOST_REQ_PAIRING_REQUEST = 0x0E,
    HOST_REQ_PAIRING_REQUEST_CANCEL = 0x0F,
    HOST_REQ_PAIRING_CANCEL = 0x10,
    HOST_REQ_FACTORY_RESET = 0x11,
    HOST_REQ_START_RANGING_SESSION = 0x12,
    HOST_REQ_STOP_RANGING_SESSION = 0x13,
    HOST_REQ_START_TRANSFER_SESSION = 0x14,
    HOST_REQ_STOP_TRANSFER_SESSION = 0x15,
    HOST_SET_DEVICE_NAME = 0x16,
    HOST_REQ_DEVICE_NAME = 0x17,
    HOST_SET_DIST_OFFSET = 0x18,
    HOST_SET_ANGLE_OFFSET = 0x19,
    PREAMBLE = 0xCC,
};

enum EventId {
    HOST_STATUS_MODULE_INFO = 0x3C,
    HOST_STATUS_PAIRING_INFO = 0x3D,
    HOST_EVT_MEASUREMENT = 0x3E,
    HOST_EVT_MEASUREMENT_BYPASS = 0x3F,
    HOST_EVT_TRANSFER_DATA = 0x40,
    HOST_EVT_SCAN_RESULT = 0x41,
    HOST_EVT_DISCONNECT = 0x42,
    HOST_EVT_CONNECT = 0x43,
    HOST_EVT_CMD = 0x44,
    HOST_EVT_DEVICE_NAME = 0x45,
    HOST_ACK_MODULE = 0x46,
    HOST_ACK_SERVICE = 0x47,
};


#endif
