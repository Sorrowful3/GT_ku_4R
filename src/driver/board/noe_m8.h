/*
 * noe_m8.h
 *
 *  Created on: 2022年4月21日
 *      Author: Administrator
 */

#ifndef __NOE_M8_H_
#define __NOE_M8_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "power.h"
#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_NOE_M8

//! UBX 数据帧的class及ID定义
#define MSG_CLASS_ACK 0X05
    #define MSG_ID_ACK_ACK 0x01
    #define MSG_ID_ACK_NAK 0x00

#define MSG_CLASS_CFG 0x06
    #define MSG_ID_CFG_ANT 0X13
    #define MSG_ID_CFG_CFG 0x09
    #define MSG_ID_CFG_DAT 0x06
    #define MSG_ID_CFG_DOSC 0x61
    #define MSG_ID_CFG_DYNSEED 0x85
    #define MSG_ID_CFG_ESRC 0x60
    #define MSG_ID_CFG_FIXSEED 0x84
    #define MSG_ID_CFG_GEOFENCE 0x69
    #define MSG_ID_CFG_GNSS 0x3E
    #define MSG_ID_CFG_INF 0x02
    #define MSG_ID_CFG_ITFM 0x39
    #define MSG_ID_CFG_LOGFILTER 0x47
    #define MSG_ID_CFG_MSG 0x01
    #define MSG_ID_CFG_NAV5 0x24
    #define MSG_ID_CFG_NAVX5 0x23
    #define MSG_ID_CFG_NMEA 0x17
    #define MSG_ID_CFG_ODO 0x1E
    #define MSG_ID_CFG_PM2 0x3B
    #define MSG_ID_CFG_PRT 0x00
    #define MSG_ID_CFG_PWR 0x57
    #define MSG_ID_CFG_RATE 0x08
    #define MSG_ID_CFG_RINV 0x34
    #define MSG_ID_CFG_RST 0x04
    #define MSG_ID_CFG_RXM 0x11
    #define MSG_ID_CFG_SBAS 0x16
    #define MSG_ID_CFG_SMGR 0x62
    #define MSG_ID_CFG_TMODE2 0x3D
    #define MSG_ID_CFG_TP5 0x31
    #define MSG_ID_CFG_TXSLOT 0x53
    #define MSG_ID_CFG_USB 0x1B

#define MSG_CLASS_INF 0x04
    #define MSG_ID_INF_DEBUG 0x04
    #define MSG_ID_INF_ERROR 0x00
    #define MSG_ID_INF_NOTICE 0x02
    #define MSG_ID_INF_TEST 0x03
    #define MSG_ID_INF_WARNING 0x01

#define MSG_CLASS_LOG 0x21
    #define MSG_ID_LOG_CREATE 0x07
    #define MSG_ID_LOG_ERASE 0x03
    #define MSG_ID_LOG_FINDTIME 0x0E
    #define MSG_ID_LOG_INFO 0x08
    #define MSG_ID_LOG_RETRIEVEPOSEXTRA 0x0F
    #define MSG_ID_LOG_RETRIEVEPOS 0x0B
    #define MSG_ID_LOG_RETRIEVESTRING 0x0d
    #define MSG_ID_LOG_RETRIEVE 0x09
    #define MSG_ID_LOG_STRING 0x04

#define MSG_CLASS_MGA 0x13
    #define MSG_ID_MGA_ACK 0x60
    #define MSG_ID_MGA_ANO 0x20
    #define MSG_ID_MGA_DBD 0x80
    #define MSG_ID_MGA_FLASH_DATA 0x21
    #define MSG_ID_MGA_FLASH_STOP 0x21
    #define MSG_ID_MGA_FLASH 0x21
    #define MSG_ID_MGA_GAL_EPH 0x02
    #define MSG_ID_MGA_GAL_ALM 0x02
    #define MSG_ID_MGA_GAL_TIMEOFFSET 0x02
    #define MSG_ID_MGA_GAL_UTC 0x02
    #define MSG_ID_MGA_GLO_EPH 0x06
    #define MSG_ID_MGA_GLO_ALM 0x06
    #define MSG_ID_MGA_GLO_TIMEOFFSET 0x06
    #define MSG_ID_MGA_GPS_EPH 0x00
    #define MSG_ID_MGA_GPS_ALM 0x00
    #define MSG_ID_MGA_GPS_UTC 0x00
    #define MSG_ID_MGA_GPS_IONO 0x00
    #define MSG_ID_MGA_INI_POS_XYZ 0x40
    #define MSG_ID_MGA_INI_POS_LLH 0x40
    #define MSG_ID_MGA_INI_TIME_UTC 0x40
    #define MSG_ID_MGA_INI_TIME_GNSS 0x40
    #define MSG_ID_MGA_INI_CLKD 0x40
    #define MSG_ID_MGA_INI_FREQ 0x40
    #define MSG_ID_MGA_INI_EOP 0x40
    #define MSG_ID_MGA_QZSS_EPH 0x05
    #define MSG_ID_MGA_QZSS_ALM 0x05
    #define MSG_ID_MGA_QZSS_HEALTH 0x05
    //Leaving for backward compatability
    #define MSG_ID_MGA_FLASH 0x21
    #define MSG_ID_MGA_GLO 0x06
    #define MSG_ID_MGA_GPS 0x00
    #define MSG_ID_MGA_INI 0x40
    #define MSG_ID_MGA_QZSS 0x05

#define MSG_CLASS_MON 0x0A
    #define MSG_ID_MON_GNSS 0x28
    #define MSG_ID_MON_HW2 0x0B
    #define MSG_ID_MON_HW 0x09
    #define MSG_ID_MON_IO 0x02
    #define MSG_ID_MON_MSGPP 0x06
    #define MSG_ID_MON_PATCH 0x27
    #define MSG_ID_MON_RXBUF 0x07
    #define MSG_ID_MON_RXR 0x21
    #define MSG_ID_MON_SMGR 0x2E
    #define MSG_ID_MON_TXBUF 0X08
    #define MSG_ID_MON_VER 0x04

#define MSG_CLASS_NAV 0x01
    #define MSG_ID_NAV_AOPSTATUS 0x60
    #define MSG_ID_NAV_CLOCK 0x22
    #define MSG_ID_NAV_DGPS 0x31
    #define MSG_ID_NAV_DOP 0x04
    #define MSG_ID_NAV_EOE 0x61
    #define MSG_ID_NAV_GEOFENCE 0x39
    #define MSG_ID_NAV_ODO 0x09
    #define MSG_ID_NAV_ORB 0x34
    #define MSG_ID_NAV_POSECEF 0x01
    #define MSG_ID_NAV_POSLLH 0x02
    #define MSG_ID_NAV_PVT 0x07
    #define MSG_ID_NAV_RESETODO 0x10
    #define MSG_ID_NAV_SAT 0x35
    #define MSG_ID_NAV_SBAS 0x32
    #define MSG_ID_NAV_SOL 0x06
    #define MSG_ID_NAV_STATUS 0x03
    #define MSG_ID_NAV_SVINFO 0x30
    #define MSG_ID_NAV_TIMEBDS 0x24
    #define MSG_ID_NAV_TIMEGAL 0x25
    #define MSG_ID_NAV_TIMEGLO 0x23
    #define MSG_ID_NAV_TIMEGPS 0x20
    #define MSG_ID_NAV_TIMELS 0x26
    #define MSG_ID_NAV_TIMEUTC 0x21
    #define MSG_ID_NAV_VELECEF 0x11
    #define MSG_ID_NAV_VELNED 0x12

#define MSG_CLASS_RXM 0x02
    #define MSG_ID_RXM_IMES 0x61
    #define MSG_ID_RXM_PMREQ 0x41
    #define MSG_ID_RXM_RAWX 0x15
    #define MSG_ID_RXM_RLM 0x59
    #define MSG_ID_RXM_SFRBX 0x13
    #define MSG_ID_RXM_SVSI 0x20

#define MSG_CLASS_SEC 0x27
    #define MSG_ID_SEC_SIGN 0x01
    #define MSG_ID_SEC_UNIQID 0x03

#define MSG_CLASS_TIM 0x0D
    #define MSG_ID_TIM_DOSC 0x11
    #define MSG_ID_TIM_FCHG 0x16
    #define MSG_ID_TIM_HOC 0x17
    #define MSG_ID_TIM_SMEAS 0x13
    #define MSG_ID_TIM_SVIN 0x04
    #define MSG_ID_TIM_TM2 0x03
    #define MSG_ID_TIM_TOS 0x12
    #define MSG_ID_TIM_TP 0x01
    #define MSG_ID_TIM_VCOCAL 0x15
    #define MSG_ID_TIM_VRFY 0x06

#define MSG_CLASS_UPD 0x09
    #define MSG_ID_UPD_SOS 0x14

//帧头定义
#pragma pack(1)
typedef struct {
    uint8_t sync1;           //!< start of packet first byte (0xB5)
    uint8_t sync2;           //!< start of packet second byte (0x62)
    uint8_t message_class;   //!< Class that defines basic subset of message (NAV, RXM, etc.)
    uint8_t message_id;      //!< Message ID
    uint16_t payload_length; //!< length of the payload data, excluding header and checksum
}ublox_header_t;
#pragma pack()


//UBX-CFG-PRT (0x06 0x00)数据帧定义(Port configuration for UART ports)
#pragma pack(1)
typedef struct {
    ublox_header_t header;  //!< Ublox header
    uint8_t port_id;        //!< port identifier (0 or 1 for USART or 3 for USB)
    uint8_t reserved;       //!< reserved
    uint16_t tx_ready;      //!< transmit ready status
    uint32_t mode;          //!< reserved
    uint32_t baud_rate;     //!< reserved
    uint16_t input_mask;    //!< input protocol mask
    uint16_t output_mask;   //!< output protocol mask
    uint16_t flags;         //!< reserved
    uint16_t reserved2;     //!< reserved
    uint8_t checksum[2];
}cfg_prt_uart_t;
#pragma pack()

#pragma pack(1)
typedef struct{
    int      latitude;
    int      longitude;
    uint8_t   height[3];
    int byte4Height;

    uint32_t gnss_latitude_h;
    uint32_t gnss_latitude_l;

    uint32_t gnss_longitude_h;
    uint32_t gnss_longitude_l;

    uint32_t gnss_height_h;
    uint32_t gnss_height_l;

    double gnss_latitude;
    double gnss_longitude;
    double gnss_height;

    uint8_t  ParmCount;

    uint8_t ParmCount1byte;

    int16_t roll;
    int16_t pitch;
    uint16_t azimuth;
    uint16_t gnss_azimuth;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t time_vaild;

    uint32_t imu_time;
    uint32_t week_time_init;
    uint64_t week_time;
    uint8_t week_day;
    int16_t temp;


    uint64_t imu_time_h_bit;
    uint32_t imu_time_4bety;
    uint32_t iTOW;          // GPS ms time of week
    int32_t ftow;           // fractional nanoseconds remainder
    uint16_t week;           // GPS week

    uint8_t outflag;

    uint8_t loc_flag;
    uint8_t week_loc_flag;
    uint8_t Pos_type;   // gnss vain

}xi_an_imu_t;
#pragma pack()

//UBX-CFG-RATE (0x06 0x08)数据帧定义
#pragma pack(1)
typedef struct {
    ublox_header_t header;
    uint16_t meas_rate;      //< GPS measurement rate (ms)
    uint16_t nav_rate;       //< Navigation rate (ALWAYS = 1) (cycles)
    uint16_t time_ref;       //< Alignment to reference time (0=UTC Time, 1=GPS Time)
    uint8_t checksum[2];
}cfg_rate_t;
#pragma pack()

//UBX-CFG-CFG (0x06 0x09)数据帧定义
#pragma pack(1)
typedef struct {
    ublox_header_t header;      //!< Ublox header
    uint32_t clear_mask;        //!< clear mask
    uint32_t save_mask;         //!< save mask
    uint32_t load_mask;         //!< load mask
    uint8_t  device_mask;       //!< device mask
    uint8_t checksum[2];        //!< Checksum
}cfg_cfg_t;
#pragma pack()

//UBX-CFG-MSG (0x06 0x01)数据帧定义(Set message rate)
#pragma pack(1)
typedef struct{
    ublox_header_t header;    //!< Ublox header
    uint8_t message_class;    //!< class of message to request
    uint8_t message_id;       //!< id of message to request
    uint8_t rate;             //!< rate message will be sent on current port
    uint8_t checksum[2];
}cfg_msg_rate_t;
#pragma pack()

#pragma pack(1)
typedef struct{
    uint8_t gnssId;             // GNSS identifier (see Satellite Numbering)
    uint8_t resTrkCh;           // Number of reserved (minimum) tracking channels for this GNSS system
    uint8_t maxTrkCh;           // Maximum number of tracking channels used for this GNSS system (>=resTrkChn)
    uint8_t reserved1;          // Reserved
    uint32_t flags;             // Bitfield of flags (see graphic in datasheet)
}cfg_gnss_repeated_t;
#pragma pack()
//UBX-CFG-GNSS (0x06 0x3E)数据帧定义
#pragma pack(1)
typedef struct {
    ublox_header_t header;
    uint8_t msgVer;        // Message version (=0 for this version)
    uint8_t numTrkChHw;        // Number of tracking channels available in hardware (read only)
    uint8_t numTrkChUse;        // Number of tracking channels to use (<=numTrkChHw)
    uint8_t numConfigBlocks;        // Number of configuration blocks following
    cfg_gnss_repeated_t repeated_block;
    cfg_gnss_repeated_t repeated_block1;
    cfg_gnss_repeated_t repeated_block2;
    cfg_gnss_repeated_t repeated_block3;
    cfg_gnss_repeated_t repeated_block4;
    cfg_gnss_repeated_t repeated_block5;
    cfg_gnss_repeated_t repeated_block6;
    uint8_t checksum[2];
}cfg_gnss_t;
#pragma pack()

//UBX-CFG-RST (0x06 0x04)
#pragma pack(1)
typedef struct {
    ublox_header_t header;      //!< Ublox header
    uint16_t nav_bbr_mask;      //!< Nav data to clear: 0x0000 = hot start, 0x0001 = warm start, 0xFFFF=cold start
    uint8_t  reset_mode;        //!< Reset mode
    uint8_t  reserved;          //!< reserved
    uint8_t checksum[2];
}cfg_rst_t;
#pragma pack()

//UBX-NAV-PVT (0x01 0x07)数据帧定义
#pragma pack(1)
typedef struct {
        ublox_header_t header;
        uint32_t iTOW;         // GPS time of week of the navigation epoch. See the description of iTOW for details.
        uint16_t year;         // Year (UTC)
        uint8_t month;         // Month, range 1..12 (UTC)
        uint8_t day;           // Day of month, range 1..31 (UTC)
        uint8_t hour;          // Hour of day, range 0..23 (UTC)
        uint8_t min;           // Minute of hour, range 0..59 (UTC)
        uint8_t sec;           // Seconds of minute, range 0..60 (UTC)
        uint8_t valid;         // Validity Flags (see graphic below)
        uint32_t tAcc;         // Time accuracy estimate (UTC)
        int32_t nano;          // Fraction of second, range -1e9 .. 1e9 (UTC)
        uint8_t fixType;       // GNSS fix Type, range 0..5
                                   // 0 = No Fix
                                   // 1 = Dead Reckoning only
                                   // 2 = 2D-Fix
                                   // 3 = 3D-Fix
                                   // 4 = GNSS + dead reckoning combined
                                   // 5 = Time only fix
        uint8_t flags;         // Fix Status Flags (see graphic below)
        uint8_t flags2;        // Reserved
        uint8_t numSV;         // Number of satellites used in Nav Solution
        int32_t lon;           // Longitude
        int32_t lat;           // Latitude
        int32_t height;        // Height above ellipsoid
        int32_t hMSL;          // Height above mean sea level
        uint32_t hAcc;         // Horizontal accuracy estimate
        uint32_t vAcc;         // Vertical accuracy estimate
        int32_t velN;          // NED north velocity
        int32_t velE;          // NED east velocity
        int32_t velD;          // NED down velocity
        int32_t gSpeed;        // Ground Speed (2-D)
        int32_t headMot;       // Heading of motion (2-D)
        uint32_t sAcc;         // Speed accuracy estimate
        uint32_t headAcc;      // Heading accuracy estimate (both motion and vehicle)
        uint16_t pDOP;         // Position DOP
        uint16_t flags3;       // Reserved
        uint8_t reserved1[4];  // Reserved
        int32_t headVeh;       // Heading of vehicle (2-D)
        uint16_t magDec;
        uint16_t magAcc;
        uint8_t checksum[2];
}nav_pvt_t;
#pragma pack()

//UBX-NAV-TIMEGPS(0x01 0x20)
#pragma pack(1)
typedef struct{
    ublox_header_t header;
    uint32_t iTOW;          // GPS ms time of week
    int32_t ftow;           // fractional nanoseconds remainder
    int16_t week;           // GPS week
    int8_t leapsecs;        // GPS UTC leap seconds
    uint8_t valid;          // validity flags
    uint32_t tacc;          // time accuracy measurement (nanosecs)
    uint8_t checksum[2];
}nav_timegps_t;
#pragma pack()

//配置结构体
typedef struct {
    const char  *uart_name; //noe_m8所连接的串口名称
    const char  *dev_name;  //noe_m8设备名称
    uint32_t    power;      //电源
}noe_m8_config_t;

typedef struct {
    uint16_t current_lenght;    //当前接收到的字节数
    uint16_t payload_lenght;      //当前帧的长度
    uint8_t msg_class;          //命令类型
    uint8_t msg_id;             //命令id
    uint8_t payload[256];       //命令载荷
}ubx_msg_t;

typedef struct{
    uint32_t    pos_type;
    double      latitude;
    double      longitude;
    double      height;
    float undulation;
    double north_velocity;
    double east_velocity;
    double up_velocity;
    double roll;
    double pitch;
    double azimuth;
    uint8_t loc_flag;
}ms6110_t;

//设备结构体
typedef struct {
    device_t            device;             //继承device对象

    noe_m8_config_t     *config;            //noe_m8配置信息
    device_t            *uart;              //noe_m8所使用的串口
    nav_pvt_t           nav_pvt;            //noe_m8的nav_pvt数据
    nav_timegps_t       nav_timegps;        //noe_m8的nav_timegps数据
    ubx_msg_t           msg_buffer;         //解包所使用的消息缓存

    int32_t             manual_gps;         //手动GPS标志
    float               manual_longitude;   //经度
    float               manual_latitude;    //纬度
    float               manual_height;      //高度

    SemaphoreHandle_t   update;             //数据更新标志
    SemaphoreHandle_t   lock;               //配置互斥锁
    SemaphoreHandle_t   cfg_sync;           //配置同步信号量
    int32_t             cfg_result;         //配置结果

     ms6110_t            ms6110;
}noe_m8_t;

typedef struct{
    uint32_t    valid_flag; //有效标志，0 GPS无效，1 GPS定位成功
    float       longitude;  //经度
    float       latitude;   //纬度
    float       height;     //高度
    float       speed;      //地速
}noe_m8_gps_t;

typedef struct{
    uint32_t    valid_flag; //有效标志，0 无效，1 定位成功
    int32_t     second;     //GPS周秒,ms
    uint16_t    week;       //GPS周
    uint32_t    leap;       //GPS闰秒
    uint32_t    time;       //GPS时间，采用UNIX格式
}noe_m8_time_t;

#define Le_1 0.1f
#define Le_2 0.01f
#define Le_7 0.0000001f

int32_t noe_m8_init(void);
int32_t noe_m8_set_cfg_prt_uart(device_t *device, uint8_t port_id, uint32_t baud_rate, uint16_t io_mask);
int32_t noe_m8_set_cfg_rate(device_t *device, uint16_t meas_rate);
int32_t noe_m8_set_cfg_cfg_save(device_t *device);
int32_t noe_m8_set_cfg_msg(device_t *device, uint8_t class_id, uint8_t msg_id, uint8_t rate);
int32_t noe_m8_set_cfg_GNSS_GB(device_t *device);
int32_t noe_m8_reset(device_t *device, uint16_t nav_bbr_mask, uint8_t reset_mode);
int32_t noe_m8_read_gps(device_t *device, noe_m8_gps_t *gps);
int32_t noe_m8_manual_gps(device_t *device, int32_t flag, float longitude, float latitude, float height);
int32_t noe_m8_read_time(device_t *device, noe_m8_time_t *noe_m8_time);
int32_t noe_m8_self_check(device_t *device);
int32_t noe_m8_produce_cfg(device_t *device);

int32_t noe_m8_read_ms6110(device_t *device, ms6110_t *ms);

#endif /* USING_NOE_M8 */

#ifdef __cplusplus
}
#endif

#endif /* __NOE_M8_H_ */
