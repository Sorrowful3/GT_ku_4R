/*
 * noe_m8.c
 *
 *  Created on: 2022年4月21日
 *      Author: Administrator
 */

#include "noe_m8.h"

#define DEBUG_TAG "noe_m8,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../core/debug.h"

#ifdef USING_NOE_M8

#define UBX_SYNC_BYTE_1 0xB5
#define UBX_SYNC_BYTE_2 0x62

#define MSG_CFG_ACK     0x01
#define MSG_CFG_NAK     0x00

#define GPS_SCALING         0.0000001f
#define GPS_HIGH_SCALING    0.001f
#define at  0.001f

//GPS时间起点;1980-1-6 0:0:0
#define GPS_TIME_START  315936000LL

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive((x)->lock, portMAX_DELAY) != pdPASS) {       \
        LOG_E("lock failed");                                               \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
    (x)->cfg_result = MSG_CFG_NAK;                                          \
    xSemaphoreTake((x)->cfg_sync, 0);                                       \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive((x)->lock) != pdPASS) {                      \
        LOG_E("unlock failed");                                             \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
}while(0)

static noe_m8_config_t noe_m8_config[1] = {
#ifdef NOE_M8_0_CONFIG
    NOE_M8_0_CONFIG,
#endif
#ifdef NOE_M8_1_CONFIG
    NOE_M8_1_CONFIG,
#endif
};

static noe_m8_t noe_m8[sizeof(noe_m8_config)/sizeof(noe_m8_config[0])];

typedef enum {
    UBX_PARSE_OK = 0,          //解析完成
    UBX_PARSE_INCOMPLETE = 1,  //正在解析
    UBX_PARSE_BAD = 2,         //解析出错
}ubx_parse_state_t;


u16 GetCheckCode(unsigned char *ptr, unsigned char len)
{
	u16 Code;
	u8 i,j;
	Code = 0xffff;
	for(i=0;i<len;i++)
	{
		Code ^= ptr[i];
		for(j=0;j<8;j++)
		{
			if(Code&0x0001)
			{
				Code >>= 1;
				Code ^= 0xa001;
			}
			else
			{
				Code >>= 1;
			}
		}
	}
	u8 parm=(Code&(0xff00))>>8;
		Code=(Code<<8)|parm;
	return (Code);
}

static void ubx_checksum(const uint8_t *in, size_t length, uint8_t *out)
{
    uint8_t i;
    uint8_t a = 0;
    uint8_t b = 0;

    for(i=0; i<length; i++) {
        a = a + in[i];
        b = b + a;
    }

    out[0] = (a & 0xFF);
    out[1] = (b & 0xFF);
}

#if (DEBUG_LEVEL >= DEBUG_OUTPUT)
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void dump_hex(const uint8_t *ptr, size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    for (i = 0; i < buflen; i += 16)
    {
        LOG_D_RAW("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                LOG_D_RAW("%02X ", buf[i + j]);
            else
                LOG_D_RAW("   ");
        LOG_D_RAW(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                LOG_D_RAW("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
        LOG_D_RAW("\n");
    }
}
#endif

#define CRC32_POLYNOMIAL 0xEDB88320L
/* --------------------------------------------------------------------------
Calculate a CRC value to be used by CRC calculation functions.
-------------------------------------------------------------------------- */
unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for ( j = 8 ; j > 0; j-- )
	{
		if ( ulCRC & 1 )
			ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

/* --------------------------------------------------------------------------
Calculates the CRC-32 of a block of data all at once
ulCount - Number of bytes in the data block
ucBuffer - Data block
-------------------------------------------------------------------------- */
unsigned long CalculateBlockCRC32( unsigned long ulCount, unsigned char *ucBuffer )
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while ( ulCount-- != 0 )
	{
		ulTemp1 = ( ulCRC >> 8 ) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value( ((int) ulCRC ^ *ucBuffer++ ) & 0xFF );
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return( ulCRC );
}


ubx_parse_state_t noe_m8_parse_byte(noe_m8_t *ublox, ubx_msg_t *msg, uint8_t data)
{
    uint8_t checksum[2];

    //msg最大长度为256B
    if(msg->current_lenght >= sizeof(msg->payload)) {
        return UBX_PARSE_BAD;
    }

    msg->payload[msg->current_lenght] = data;
    msg->current_lenght++;


#ifdef XIAN_IMU_PARES
    uint16_t crc;
    switch(msg->current_lenght) {
      		case 1:                                                         //帧头第1字节
    	        if(msg->payload[0] == 0xDA) {
    	            return  UBX_PARSE_INCOMPLETE;
    	        }
    	        return UBX_PARSE_BAD;
    	    case 2:                                                         //帧头第2字节
    	        if(msg->payload[1] == 0x61) {
    	        	msg->payload_lenght=78;
    	            return UBX_PARSE_INCOMPLETE;
    	        }
    	        return UBX_PARSE_BAD;
      		default:
      			if (msg->payload_lenght == msg->current_lenght) {
      				memcpy(&crc, &msg->payload[msg->current_lenght - 2], 2);
    				//if (CalculateBlockCRC32(msg->current_lenght - 2, msg->payload) == crc)
   					if(GetCheckCode(msg->payload,msg->current_lenght - 2)==crc)
    					return UBX_PARSE_OK;
    				else
    					return UBX_PARSE_BAD;
      			}
      			return UBX_PARSE_INCOMPLETE;
      		break;
      	}
#endif

#if  ANTENNA_IMU_SCAN
    uint32_t crc;
  	switch(msg->current_lenght) {
  		case 1:                                                         //帧头第1字节
	        if(msg->payload[0] == 0xaa) {
	            return  UBX_PARSE_INCOMPLETE;
	        }
	        return UBX_PARSE_BAD;
	    case 2:                                                         //帧头第2字节
	        if(msg->payload[1] == 0x44) {
	            return UBX_PARSE_INCOMPLETE;
	        }
	        return UBX_PARSE_BAD;
	    case 3:
	    	if(msg->payload[2] == 0x12) {
	    		msg->payload_lenght = 158;
				return UBX_PARSE_INCOMPLETE;
			}
	    	else if (msg->payload[2] == 0xbd) {
	    		msg->payload_lenght = 124;
	    		return UBX_PARSE_INCOMPLETE;
	    	}
			return UBX_PARSE_BAD;
//	    case 4:
//			if(msg->payload[3] == 0x1c) {
//				return UBX_PARSE_INCOMPLETE;
//			}
//			return UBX_PARSE_BAD;
//	    case 5:
//			if(msg->payload[4] == 0xb9) {
//				return UBX_PARSE_INCOMPLETE;
//			}
//			return UBX_PARSE_BAD;
//	    case 6:
//			if(msg->payload[5] == 0x5) {
//				return UBX_PARSE_INCOMPLETE;
//			}
//			return UBX_PARSE_BAD;
  		default:
  			if (msg->payload_lenght == msg->current_lenght) {
  				memcpy(&crc, &msg->payload[msg->current_lenght - 4], 4);
				if (CalculateBlockCRC32(msg->current_lenght - 4, msg->payload) == crc)
					return UBX_PARSE_OK;
				else
					return UBX_PARSE_BAD;
  			}
  			return UBX_PARSE_INCOMPLETE;
  		break;
  	}
#endif

//    switch(msg->current_lenght) {
//    case 1:                                                         //帧头第1字节
//        if(msg->payload[0] == 0xB5) {
//            return  UBX_PARSE_INCOMPLETE;
//        }
//        return UBX_PARSE_BAD;
//    case 2:                                                         //帧头第2字节
//        if(msg->payload[1] == 0x62) {
//            return UBX_PARSE_INCOMPLETE;
//        }
//        return UBX_PARSE_BAD;
//    case 3:                                                         //class
//        msg->msg_class = msg->payload[2];
//        return UBX_PARSE_INCOMPLETE;
//    case 4:                                                         //id
//        msg->msg_id = msg->payload[3];
//        return UBX_PARSE_INCOMPLETE;
//    case 5:                                                         //帧长第字节（低8位）
//        msg->payload_lenght = (uint16_t)((msg->payload[4]));
//        return UBX_PARSE_INCOMPLETE;
//    case 6:                                                         //帧长第2字节（高8位）
//        msg->payload_lenght |= (uint16_t)(msg->payload[5] << 8);
//        if(msg->payload_lenght <= sizeof(msg->payload)) {
//            return UBX_PARSE_INCOMPLETE;
//        }
//        return UBX_PARSE_BAD;
//    default:
//        if(msg->current_lenght < (6+msg->payload_lenght+2)) {
//            return UBX_PARSE_INCOMPLETE;
//        }
//        //和校验
//        ubx_checksum(&msg->payload[2], msg->payload_lenght+4, checksum);
//        if((checksum[0] == msg->payload[msg->current_lenght-2]) && (checksum[1] == msg->payload[msg->current_lenght-1])) {
//            return UBX_PARSE_OK;
//        }
//        else {
//            return UBX_PARSE_BAD;
//        }
//    }
}


static u16 EndianSwitchU16(u16 value)
{
	return ((value&0x00ff)<<8)|((value&0xff00)>>8);
}

static u8 EndianSwitchU24(u8 *value)
{
	//*value=(((*value)&0x0000ff)<<16)|(((*value)&0x00ff00)<<8)|(((*value)&0xff0000)>>16);
	u8 data[3]={0};

	data[0]=value[2];
	data[1]=value[1];
	data[2]=value[0];
	memcpy(value,data,3);
	return 0;
}


static s32 EndianSwitchS32(s32 value)
{
	return ((value&0x000000ff)<<24)|((value&0x0000ff00)<<8)|
			((value&0x00ff0000)>>8)|((value&0xff000000)>>24);
}

static u32 EndianSwitchU32(u32 value)
{
	return ((value&0x000000ff)<<24)|((value&0x0000ff00)<<8)|
			((value&0x00ff0000)>>8)|((value&0xff000000)>>24);
}

static u64 EndianSwitchU64(u64 value)
{
	return ((value&0x00000000000000ff)<<56)|((value&0x000000000000ff00)<<40)|
			((value&0x0000000000ff0000)<<24)|((value&0x00000000ff000000)<<8)|
			((value&0x000000ff00000000)>>8)|((value&0x0000ff0000000000)>>24)|
			((value&0x00ff000000000000)>>40)|((value&0xff00000000000000)>>56);
}

//计算平年还是润年
static int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

static int week_parm( xi_an_imu_t *new_imu)
{
	if(new_imu->Pos_type==0)
	{
		return -1;
	}

	if(new_imu->week_loc_flag==1)
	{
		return new_imu->week;
	}

	if(new_imu->time_vaild==0)
	{
		return -1;
	}

	 // 计算周数
	   int week = (new_imu->year - 1980) * 52-1;
	   int day=1;
	   int days[13] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // 每个月的天数
	    int year_day_new=0;


	   // 加上润年的周数
	    for (int i = 1980; i < new_imu->year; i++) {
	    	if(isLeapYear(i)) {
	    	    		days[2]=29;
	    	 }
	    	else
	    	{
	    				days[2]=28;
	    	}
	    	for(int month_parm=1;month_parm<=12;month_parm++)
	    	{
	    		year_day_new+=days[month_parm];
	    	}

	    }

	    if(isLeapYear(new_imu->year)) {
	    	days[2]=29;
	    }
	    else
	    {
	    	days[2]=28;
	    }

	    for(int month_parm1=1;month_parm1<new_imu->month;month_parm1++)
	    {
	    	year_day_new+=days[month_parm1];
	    }

	    year_day_new+=new_imu->day;

	    week=year_day_new/7;

	    new_imu->week_day=(uint8_t)(year_day_new%7)*7;

	    new_imu->week_loc_flag=1;

	    new_imu->week_time_init=new_imu->week_day*24*60*60+new_imu->hour*60*60+new_imu->minute*60+new_imu->second-1*0.001f;

	    return week;
}


static void iTime(xi_an_imu_t *new_imu)
{
	new_imu->week_time+=new_imu->week_time_init;
	new_imu->iTOW=new_imu->week_time;
}

int convert_3byte_to_4byte(uint8_t *value_3byte)
{
	uint8_t value_4byte[4] = {0};
	int data;
    uint8_t* p = (uint8_t*)value_3byte;
    uint8_t sign = (p[2] & 0x80) ? 0x80 : 0x00;
    value_4byte[0] = sign;
    value_4byte[1] = p[2]&0x7f;
    value_4byte[2] = p[1];
    value_4byte[3] = p[0];
    data=(value_4byte[0]<<24)|(value_4byte[1]<<16)|(value_4byte[2]<<8)|(value_4byte[3]);
    return data;
}

int XA_Gnss_Parm(xi_an_imu_t *new_imu,ubx_msg_t *msg_buffer)
{
	switch(new_imu->ParmCount) {
	case(1):{
		memcpy(&new_imu->outflag, &msg_buffer->payload[67], 1);
		break;
	}
	case(2):{
		// memcpy(&new_imu.outflag, &msg_buffer->payload[68+xian_HEAD_LEN], 1);
		break;
	}
	case(3):{
		 memcpy(&new_imu->gnss_longitude_h, &msg_buffer->payload[67], 4);
		 new_imu->gnss_longitude_h= EndianSwitchU32(new_imu->gnss_longitude_h);
		break;
	}
	case(4):{
		memcpy(&new_imu->gnss_longitude_l, &msg_buffer->payload[67], 4);
		new_imu->gnss_longitude_l= EndianSwitchU32(new_imu->gnss_longitude_l);
		uint64_t longitude_h_data=(new_imu->gnss_longitude_h<<24)&0xffffffff00000000;
		new_imu->gnss_longitude=(double)(longitude_h_data|new_imu->gnss_longitude_l);
		break;
	}
	case(5):{
		 memcpy(&new_imu->gnss_latitude_h, &msg_buffer->payload[67], 4);
		 new_imu->gnss_latitude_h= EndianSwitchU32(new_imu->gnss_latitude_h);
		break;
	}
	case(6):{
		memcpy(&new_imu->gnss_latitude_l, &msg_buffer->payload[67], 4);
		new_imu->gnss_latitude_l= EndianSwitchU32(new_imu->gnss_latitude_l);
		uint64_t longitude_h_data=(new_imu->gnss_latitude_h<<24)&0xffffffff00000000;
		new_imu->gnss_longitude=(double)(longitude_h_data|new_imu->gnss_latitude_l);
		break;
	}
	case(7):{
		 memcpy(&new_imu->gnss_height_h, &msg_buffer->payload[67], 4);
		break;
	}
	case(8):{
		memcpy(&new_imu->gnss_height_l, &msg_buffer->payload[67], 4);
		uint64_t longitude_h_data=(new_imu->gnss_height_h<<24)&0xffffffff00000000;
		new_imu->gnss_longitude=(double)(longitude_h_data|new_imu->gnss_height_l);
		break;
	}
	case(12):{
		memcpy(&new_imu->gnss_azimuth, &msg_buffer->payload[67], 2);
		new_imu->gnss_azimuth=EndianSwitchU16(new_imu->gnss_azimuth);
		break;
	}
	case(16):{
		memcpy(&new_imu->time_vaild, &msg_buffer->payload[67], 1);
		memcpy(&new_imu->year, &msg_buffer->payload[68], 2);
		new_imu->year=EndianSwitchU16(new_imu->year);
		memcpy(&new_imu->month, &msg_buffer->payload[70], 1);
		break;
	}
	case(17):{
		memcpy(&new_imu->day, &msg_buffer->payload[67], 1);
		memcpy(&new_imu->hour, &msg_buffer->payload[68], 1);
		memcpy(&new_imu->minute, &msg_buffer->payload[79], 1);
		memcpy(&new_imu->second, &msg_buffer->payload[70], 1);
		break;
	}

	}

	return 0;
}


static void ParmCount(xi_an_imu_t *new_imu)
{
	new_imu->ParmCount=(new_imu->ParmCount1byte&0xfc)>>2;
	new_imu->imu_time_h_bit=new_imu->ParmCount1byte&0x1;

}


//void MUILT_beam_switch_gpio1(u8 channel)
//{
//	ps_gpio_config(0, PS_GPIO_MODE_OUTPUT);
//	ps_gpio_write(0, channel);  			//翻转
//}

static char pr_buf[512];

static void noe_m8_thread(void *arg)
{
    ssize_t i;
    ssize_t lenght;
    ubx_msg_t *msg_buffer;
    noe_m8_t *ublox;
    ubx_parse_state_t state;
    double tmp_float1;
    float tmp_float;
    xi_an_imu_t new_imu;
    uint8_t buffer[64];
    int j, len;
    char *ptr;
    double pitch;
    float roll=0;

//    int issue_point_folag=0;

    ublox = (noe_m8_t *)arg;
    if((!ublox) || (device_check(&ublox->device, NOE_M8_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        vTaskDelete(NULL);
    }

    msg_buffer = &ublox->msg_buffer;
//                	ublox->ms6110.roll=0;
//                	ublox->ms6110.pitch=0;
//                	ublox->ms6110.loc_flag = 4;
//                	//ublox->ms6110.loc_flag = loc_flag;
//                			ublox->ms6110.pos_type=2;
//
//                			ublox->ms6110.latitude=30.59;
//                	    	ublox->ms6110.longitude=104.08;
//                	    	ublox->ms6110.height=503;
//                	    	ublox->ms6110.loc_flag=1;
//
//                	    	ublox->ms6110.azimuth=0;
//                	    	ublox->ms6110.roll=0;
//                	    	ublox->ms6110.pitch=0;
//                	    	ublox->nav_timegps.week=2280;
//                	    	ublox->ms6110.pos_type=2;
//                	    	ublox->nav_timegps.iTOW=15064503;
    while(1)
    {
        //接收数据
        lenght = device_read(ublox->uart, buffer, sizeof(buffer));
        if(lenght <= 0) {
        	  vTaskDelay(2);
        	continue;
        }

#if (DEBUG_LEVEL >= DEBUG_OUTPUT)
        dump_hex(buffer, lenght);
#endif

        for(i=0; i<lenght; i++) {
            state = noe_m8_parse_byte(ublox, msg_buffer, buffer[i]);

            //接收未完成
            if(state ==  UBX_PARSE_INCOMPLETE) {
                continue;
            }
            //连续3次收到正确数据包后开启解析出错输出
            if((state ==  UBX_PARSE_OK)) {

#ifdef XIAN_IMU_PARES
       if (msg_buffer->payload_lenght == 78) {

            	taskENTER_CRITICAL();
#define xian_HEAD_LEN -1
//            	memcpy(&ublox->nav_timegps.week, &msg_buffer->payload[14+xian_HEAD_LEN], 2);
//            	memcpy(&ublox->nav_timegps.iTOW, &msg_buffer->payload[16+xian_HEAD_LEN], 4);

            	memcpy(&new_imu.latitude, &msg_buffer->payload[49+xian_HEAD_LEN], 4);
            	new_imu.latitude=EndianSwitchS32(new_imu.latitude);
            	memcpy(&new_imu.longitude, &msg_buffer->payload[45+xian_HEAD_LEN], 4);
            	new_imu.longitude=EndianSwitchS32(new_imu.longitude);
            	memcpy(new_imu.height, &msg_buffer->payload[53+xian_HEAD_LEN], 3);
            	EndianSwitchU24(new_imu.height);

            	memcpy(&new_imu.roll, &msg_buffer->payload[32+xian_HEAD_LEN], 2);
            	new_imu.roll=EndianSwitchU16(new_imu.roll);
            	memcpy(&new_imu.pitch, &msg_buffer->payload[30+xian_HEAD_LEN], 2);
            	new_imu.pitch=EndianSwitchU16(new_imu.pitch);
            	memcpy(&new_imu.azimuth, &msg_buffer->payload[34+xian_HEAD_LEN], 2);
            	new_imu.azimuth=EndianSwitchU16(new_imu.azimuth);

            	memcpy(&new_imu.ParmCount1byte, &msg_buffer->payload[63+xian_HEAD_LEN], 1);

            	memcpy(&new_imu.Pos_type, &msg_buffer->payload[57+xian_HEAD_LEN], 1);
            	memcpy(&new_imu.imu_time_4bety, &msg_buffer->payload[64+xian_HEAD_LEN], 4);
            	new_imu.imu_time_4bety=EndianSwitchS32(new_imu.imu_time_4bety);

            	ParmCount(&new_imu);
            	new_imu.week_time=(new_imu.imu_time_h_bit<<32)|new_imu.imu_time_4bety;
            	new_imu.byte4Height=convert_3byte_to_4byte(new_imu.height);

            	XA_Gnss_Parm(&new_imu,msg_buffer);

            	//new_imu.week=week_parm(&new_imu);
            	memcpy(&new_imu.week, &msg_buffer->payload[75+xian_HEAD_LEN], 2);
//            	new_imu.week = 0x90B;
            	iTime(&new_imu);

            	ublox->nav_timegps.week=(new_imu.week>>8|0)|(new_imu.week<<8|0);
            	ublox->nav_timegps.iTOW=new_imu.week_time*Le_1;
            	//ublox->nav_timegps.iTOW=374418000;
            	ublox->ms6110.latitude=(double)new_imu.latitude*Le_7;
            	ublox->ms6110.longitude=(double)new_imu.longitude*Le_7;
            	ublox->ms6110.height=(double)(new_imu.byte4Height*Le_2);

            	ublox->ms6110.roll=(double)(new_imu.roll*Le_2);
            	ublox->ms6110.pitch=(double)(new_imu.pitch*Le_2);
            	pitch=ublox->ms6110.pitch;
            	ublox->ms6110.pitch=1*ublox->ms6110.roll;
            	ublox->ms6110.roll=-pitch;
            	ublox->ms6110.azimuth=(double)(new_imu.azimuth*Le_2)-270;
            	if(ublox->ms6110.azimuth<0)
            	      ublox->ms6110.azimuth=360.0f+ublox->ms6110.azimuth;
            	taskEXIT_CRITICAL();
          }
#endif
			#if  ANTENNA_IMU_SCAN
            	if (msg_buffer->payload_lenght == 158) {
					#define MS6110_HEAD_LEN 28
					taskENTER_CRITICAL();

					memcpy(&ublox->nav_timegps.week, &msg_buffer->payload[14], 2);
					memcpy(&ublox->nav_timegps.iTOW, &msg_buffer->payload[16], 4);

					memcpy(&ublox->ms6110.pos_type, &msg_buffer->payload[MS6110_HEAD_LEN+4], 4);

					memcpy(&ublox->ms6110.latitude, &msg_buffer->payload[MS6110_HEAD_LEN+8], 8);
					if (!isnormal(ublox->ms6110.latitude))
						ublox->ms6110.latitude = 0;

					memcpy(&ublox->ms6110.longitude, &msg_buffer->payload[MS6110_HEAD_LEN+16], 8);
					if (!isnormal(ublox->ms6110.longitude))
						ublox->ms6110.longitude = 0;

					memcpy(&ublox->ms6110.height, &msg_buffer->payload[MS6110_HEAD_LEN+24], 8);
					if (!isnormal(ublox->ms6110.height))
						ublox->ms6110.height = 0;

					memcpy(&ublox->ms6110.undulation, &msg_buffer->payload[MS6110_HEAD_LEN+32], 4);
					if (!isnormal(ublox->ms6110.undulation))
						ublox->ms6110.undulation = 0;
					memcpy(&ublox->ms6110.north_velocity, &msg_buffer->payload[MS6110_HEAD_LEN+36], 8);
					if (!isnormal(ublox->ms6110.north_velocity))
						ublox->ms6110.north_velocity = 0;
					memcpy(&ublox->ms6110.east_velocity, &msg_buffer->payload[MS6110_HEAD_LEN+44], 8);
					if (!isnormal(ublox->ms6110.east_velocity))
						ublox->ms6110.east_velocity = 0;
					memcpy(&ublox->ms6110.up_velocity, &msg_buffer->payload[MS6110_HEAD_LEN+52], 8);
					if (!isnormal(ublox->ms6110.up_velocity))
						ublox->ms6110.up_velocity = 0;
					memcpy(&ublox->ms6110.roll, &msg_buffer->payload[MS6110_HEAD_LEN+60], 8);
					roll=ublox->ms6110.roll;
					if (!isnormal(ublox->ms6110.roll))
						ublox->ms6110.roll = 0;

					memcpy(&ublox->ms6110.pitch, &msg_buffer->payload[MS6110_HEAD_LEN+68], 8);
 				//ublox->ms6110.pitch=ublox->ms6110.pitch-;
				//	roll=ublox->ms6110.pitch;
					if (!isnormal(ublox->ms6110.pitch))
						ublox->ms6110.pitch = 0;

					memcpy(&ublox->ms6110.azimuth, &msg_buffer->payload[MS6110_HEAD_LEN+76], 8);
					ublox->ms6110.azimuth-=89.7;
//					tmp_float1=tmp_floa;
					if(ublox->ms6110.azimuth<=0)
					{
						ublox->ms6110.azimuth=360+ublox->ms6110.azimuth;
					}
					ublox->ms6110.roll=ublox->ms6110.pitch;
					ublox->ms6110.pitch=-1*roll;
//					ublox->ms6110.roll=-1*roll;
//					ublox->ms6110.pitch=ublox->ms6110.pitch;

					taskEXIT_CRITICAL();
            	}
            	else if (msg_buffer->payload_lenght == 124) {
					#define MS6110_HEAD_LEN1 12
					taskENTER_CRITICAL();

					uint8_t head_flag, loc_flag;

					loc_flag = msg_buffer->payload[MS6110_HEAD_LEN1+11];
					//loc_flag = ublox->ms6110.pos_type;
					memcpy(&tmp_float, &msg_buffer->payload[MS6110_HEAD_LEN1+88], 4);
					head_flag = msg_buffer->payload[MS6110_HEAD_LEN1+92];
					tmp_float+=90;
					if(tmp_float>=360)
					tmp_float=tmp_float-360;
					if(ublox->ms6110.pos_type==0)
					{
						loc_flag=0;
					}
					//ublox->ms6110.azimuth = tmp_float;
					if(loc_flag==1)
					ublox->ms6110.loc_flag = 2;
					else if(loc_flag==2)
					ublox->ms6110.loc_flag = 1;
					else if(loc_flag==0)
					ublox->ms6110.loc_flag = 3;


					taskEXIT_CRITICAL();
            	}

            	//ublox->ms6110.azimuth = tmp_float1;
            	//if(abs(tmp_float1-tmp_float)<=1)
//            	{
//            		ublox->ms6110.azimuth = tmp_float1;
//            	}
//            	else
//            	{
//            		ublox->ms6110.azimuth = tmp_float;
//            	}
#endif
            }
            //数据格式错误
            if((state ==  UBX_PARSE_BAD)) {
//                LOG_E("frame error");
            }

            //重置解析计数器
            msg_buffer->current_lenght = 0;
            msg_buffer->payload_lenght = 0;
        }
    }
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = NULL,
    .pwrite = NULL,
    .control = NULL,
};
int32_t noe_m8_init(void)
{
    uint32_t i;
    int32_t result;
    portBASE_TYPE xresult;
    TaskHandle_t task_handle;

    for(i=0; i<sizeof(noe_m8)/sizeof(noe_m8[0]); i++) {
        noe_m8[i].config = &noe_m8_config[i];
        memset(&noe_m8[i].nav_pvt, 0, sizeof(nav_pvt_t));
        noe_m8[i].manual_gps = 0;

        //创建数据更新标志信号量
        noe_m8[i].update = xSemaphoreCreateBinary();
        if(!noe_m8[i].update) {
            LOG_E("create updata failed");
            return ERR_NOMEM;
        }
        //配置同步信号量
        noe_m8[i].cfg_sync = xSemaphoreCreateBinary();
        if(!noe_m8[i].cfg_sync) {
            LOG_E("create cfg sync failed");
            return ERR_NOMEM;
        }
        //创建配置互斥锁
        noe_m8[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!noe_m8[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }

        //打开noe_m8所使用的串口
        noe_m8[i].uart = device_open(noe_m8[i].config->uart_name, DEVICE_RDWR);
        if(!noe_m8[i].uart) {
            LOG_E("open uart failed");
            return ERR_IO;
        }

        //注册IMU设备
        noe_m8[i].device.magic = NOE_M8_MAGIC;
        noe_m8[i].device.ops = &ops;
        result = device_register(&noe_m8[i].device, noe_m8[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }

        //创建解析线程
        xresult = xTaskCreate(noe_m8_thread,
                             "noe_m8_thread",
                             3072,
                             &noe_m8[i],
                             configMAX_PRIORITIES - 20,
                             &task_handle );
        if(xresult != pdPASS) {
            LOG_E("create task noe_m8_thread failed\r\n");
            return ERR_NOMEM;
        }

        //开启电源
        power_on(noe_m8[i].config->power);
    }

    return ERR_OK;
}

//设置波特率
int32_t noe_m8_set_cfg_prt_uart(device_t *device, uint8_t port_id, uint32_t baud_rate, uint16_t io_mask)
{
    int32_t result;
    cfg_prt_uart_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    //初始化帧头
    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_PRT;
    message.header.payload_length = 20;

    //初始化载荷
    message.port_id = port_id ;
    message.reserved = 0x00;
    message.tx_ready =0x0000;
    message.mode = 0x08d0;
    message.baud_rate = baud_rate;
    message.input_mask = io_mask;
    message.output_mask = io_mask;
    message.flags = 0x00;
    message.reserved2 = 0x00;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 24, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

//配置UBLOX NEO-6的更新速率
int32_t noe_m8_set_cfg_rate(device_t *device, uint16_t meas_rate)
{
    int32_t result;
    cfg_rate_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_RATE;
    message.header.payload_length = 6;

    message.meas_rate = meas_rate;
    message.nav_rate =  0x01;
    message.time_ref = 0x01;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 10, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

//保存配置函数
int32_t noe_m8_set_cfg_cfg_save(device_t *device)
{
    int32_t result;
    cfg_cfg_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_CFG;
    message.header.payload_length = 13;

    message.clear_mask = 0;
    message.save_mask =  0x0000ffff;
    message.load_mask =  0;
    message.device_mask = 0x03;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 17, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT*3) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

//消息的速率配置
int32_t noe_m8_set_cfg_msg(device_t *device, uint8_t class_id, uint8_t msg_id, uint8_t rate)
{
    int32_t result;
    cfg_msg_rate_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_MSG;
    message.header.payload_length = 3;

    message.message_class = class_id;
    message.message_id = msg_id;
    message.rate = rate;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 7, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

int32_t noe_m8_set_cfg_GNSS_GB(device_t *device)
{
    int32_t result;
    cfg_gnss_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_GNSS;
    message.header.payload_length = 60;

    message.msgVer = 0x00;
    message.numTrkChHw = 0x00 ;
    message.numTrkChUse = 0x20;
    message.numConfigBlocks = 0x07;

    message.repeated_block.gnssId = 0x00;  //GPS
    message.repeated_block.resTrkCh = 0x08;
    message.repeated_block.maxTrkCh = 0x10;
    message.repeated_block.reserved1 =0x00;
    message.repeated_block.flags = 0x01010001;

    message.repeated_block1.gnssId = 0x01;  //SBAS
    message.repeated_block1.resTrkCh = 0x01;
    message.repeated_block1.maxTrkCh = 0x03;
    message.repeated_block1.reserved1 =0x00;
    message.repeated_block1.flags = 0x01010000;

    message.repeated_block2.gnssId = 0x02;  //GALILEO
    message.repeated_block2.resTrkCh = 0x04;
    message.repeated_block2.maxTrkCh = 0x08;
    message.repeated_block2.reserved1 =0x00;
    message.repeated_block2.flags = 0x01010000;

    message.repeated_block3.gnssId = 0x03; //BEIDOU
    message.repeated_block3.resTrkCh = 0x08;
    message.repeated_block3.maxTrkCh = 0x10;
    message.repeated_block3.reserved1 =0x00;
    message.repeated_block3.flags = 0x01010001;

    message.repeated_block4.gnssId = 0x04;  //IMES
    message.repeated_block4.resTrkCh = 0x00;
    message.repeated_block4.maxTrkCh = 0x08;
    message.repeated_block4.reserved1 =0x00;
    message.repeated_block4.flags = 0x01010000;

    message.repeated_block5.gnssId = 0x05;  //QZSS
    message.repeated_block5.resTrkCh = 0x00;
    message.repeated_block5.maxTrkCh = 0x03;
    message.repeated_block5.reserved1 =0x00;
    message.repeated_block5.flags = 0x01010000;

    message.repeated_block6.gnssId = 0x06;  //GLONASS
    message.repeated_block6.resTrkCh = 0x08;
    message.repeated_block6.maxTrkCh = 0x0E;
    message.repeated_block6.reserved1 =0x00;
    message.repeated_block6.flags = 0x01010000;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 64, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

//复位函数
int32_t noe_m8_reset(device_t *device, uint16_t nav_bbr_mask, uint8_t reset_mode)
{
    int32_t result;
    cfg_rst_t message;
    uint8_t *msg_ptr = (uint8_t *)&message;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if(device_check(device, NOE_M8_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(ublox);

    message.header.sync1 = UBX_SYNC_BYTE_1;
    message.header.sync2 = UBX_SYNC_BYTE_2;
    message.header.message_class = MSG_CLASS_CFG;
    message.header.message_id = MSG_ID_CFG_RST;
    message.header.payload_length = 4;

    message.nav_bbr_mask = nav_bbr_mask;
    message.reset_mode = reset_mode;

    //计算校验和
    ubx_checksum(msg_ptr + 2, 8, message.checksum);

    if(device_write(ublox->uart, msg_ptr, sizeof(message)) != sizeof(message)) {
        UNLOCK(ublox);
        LOG_E("write tx_sync failed");
        return ERR_IO;
    }

    if(xSemaphoreTake(ublox->cfg_sync, NOE_M8_TIMEOUT) != pdPASS) {
        UNLOCK(ublox);
        LOG_E("take cfg_sync failed");
        return ERR_TIMEOUT;
    }

    if(ublox->cfg_result == MSG_CFG_ACK) {
        result = ERR_OK;
    }
    else {
        result = ERR_IO;
    }

    UNLOCK(ublox);

    return result;
}

int32_t noe_m8_read_ms6110(device_t *device, ms6110_t *ms)
{
	noe_m8_t *ublox = container_of(device, noe_m8_t, device);

	if((!ms) || (device_check(device, NOE_M8_MAGIC) != ERR_OK)) {
		LOG_E("parameter error");
		return ERR_INVAL;
	}

	taskENTER_CRITICAL();

	memcpy(ms, &ublox->ms6110, sizeof(ms6110_t));

	taskEXIT_CRITICAL();

	return ERR_OK;
}

int32_t noe_m8_read_gps(device_t *device, noe_m8_gps_t *gps)
{
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if((!gps) || (device_check(device, NOE_M8_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();

    if(ublox->manual_gps) {
        gps->valid_flag = 1;
        gps->longitude = ublox->ms6110.longitude;
        gps->latitude = ublox->ms6110.latitude;
        gps->height = ublox->ms6110.height;
        gps->speed = 0.0f;
    }
    else {
        if((ublox->nav_pvt.fixType == 2) || (ublox->nav_pvt.fixType == 3) || (ublox->nav_pvt.fixType == 4)) {
            gps->valid_flag = 1;
        }
        else {
            gps->valid_flag = 0;
        }

//        gps->longitude = ublox->nav_pvt.lon * GPS_SCALING;
//        gps->latitude = ublox->nav_pvt.lat * GPS_SCALING;
//        gps->height = ublox->nav_pvt.height * GPS_HIGH_SCALING;
//        gps->speed = ublox->nav_pvt.gSpeed * GPS_GSPEED_SCALING;
    	gps->longitude = (float)ublox->ms6110.longitude;
    		gps->latitude = (float)ublox->ms6110.latitude;
    		gps->height = (float)ublox->ms6110.height;
    		gps->speed = (float)ublox->ms6110.north_velocity;
    		gps->valid_flag = 1;
//        if((ublox->nav_pvt.fixType == 2) || (ublox->nav_pvt.fixType == 3) || (ublox->nav_pvt.fixType == 4)) {
//			gps->valid_flag = 1;
//		}
//		else {
//			gps->valid_flag = 0;
//		}
//        gps->valid_flag = 1;
//		//gps->longitude = 116.2;
//        gps->longitude = 104.06;
//		gps->latitude = 30.67;
//		gps->height = 10.0f;
//		gps->speed = 10.0f;
   }

    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t noe_m8_manual_gps(device_t *device, int32_t flag, float longitude, float latitude, float height)
{
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    ublox->manual_gps = flag;
    ublox->manual_longitude = longitude;
    ublox->manual_latitude = latitude;
    ublox->manual_height = height;

    return ERR_OK;
}

int32_t noe_m8_read_time(device_t *device, noe_m8_time_t *noe_m8_time)
{
    time_t time_utc;
    time_t time_gps;
    struct tm tm;
    noe_m8_t *ublox = container_of(device, noe_m8_t, device);

    if((!noe_m8_time) || (device_check(device, NOE_M8_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();

    noe_m8_time->week = ublox->nav_timegps.week;
    noe_m8_time->second = ublox->nav_timegps.iTOW;

    // if((ublox->nav_pvt.valid & 0x07) == 0x07) {
    //     noe_m8_time->valid_flag = 1;
    // }
    // else {
    //     noe_m8_time->valid_flag = 0;
    // }

    // tm.tm_year = ublox->nav_pvt.year-1900;
    // tm.tm_mon = ublox->nav_pvt.month - 1;
    // tm.tm_mday = ublox->nav_pvt.day;
    // tm.tm_hour = ublox->nav_pvt.hour;
    // tm.tm_min = ublox->nav_pvt.min;
    // tm.tm_sec = ublox->nav_pvt.sec;
    // time_utc = mktime(&tm);

    // time_gps = time_utc - 315964800;
    // noe_m8_time->week = (time_gps / 86400) / 7;
    // noe_m8_time->second = ublox->nav_pvt.iTOW;
    // noe_m8_time->time = time_utc;
    // noe_m8_time->leap = ((time_gps % (86400*7)) * 1000 - ublox->nav_pvt.iTOW) / 1000;

    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t noe_m8_self_check(device_t *device)
{
    //通过关闭GGA数据帧实现自检
    return noe_m8_set_cfg_msg(device, 0xF0, 0x00, 0);
}

int32_t noe_m8_produce_cfg(device_t *device)
{
    /* 关闭不需要的数据帧 */
    //GGA, 0xF0 0x00
    noe_m8_set_cfg_msg(device, 0xF0, 0x00, 0);
    vTaskDelay(10);
    //GLL, 0xF0 0x01
    noe_m8_set_cfg_msg(device, 0xF0, 0x01, 0);
    vTaskDelay(10);
    //GSA, 0xF0 0x02
    noe_m8_set_cfg_msg(device, 0xF0, 0x02, 0);
    vTaskDelay(10);
    //GSV, 0xF0 0x03
    noe_m8_set_cfg_msg(device, 0xF0, 0x03, 0);
    vTaskDelay(10);
    //RMC, 0xF0 0x04
    noe_m8_set_cfg_msg(device, 0xF0, 0x04, 0);
    vTaskDelay(10);
    //VTG, 0xF0 0x05
    noe_m8_set_cfg_msg(device, 0xF0, 0x05, 0);
    vTaskDelay(10);
    //TXT, 0xF0 0x41
    noe_m8_set_cfg_msg(device, 0xF0, 0x41, 0);
    vTaskDelay(10);

    //配置数据更新速率
    noe_m8_set_cfg_rate(device, 100);
    vTaskDelay(10);
    //设置卫星消息
    noe_m8_set_cfg_GNSS_GB(device);
    vTaskDelay(10);
    //使能需要的消息，输出周期200ms
    noe_m8_set_cfg_msg(device, MSG_CLASS_NAV, MSG_ID_NAV_PVT, 2);
    vTaskDelay(10);
    noe_m8_set_cfg_msg(device, MSG_CLASS_NAV, MSG_ID_NAV_TIMEGPS, 2);
    vTaskDelay(10);
    //保存配置
    noe_m8_set_cfg_cfg_save(device);
    vTaskDelay(10);

    return ERR_OK;
}

#endif /* USING_NOE_M8 */
