#ifndef __SET_H_
#define __SET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/stat.h>

//文件路径最大长度
#define MAX_FILE_PATH		128

#define MAX_KEY_LENGHT		64
#define MAX_DATA_STR_LENGHT	64
#define MAX_DESCRIBE_LENGHT	128

#define TYPE_F64	0x00  //对应字符串"f64"
#define TYPE_F32	0x01  //对应字符串"f32"
#define TYPE_I8		0x02  //对应字符串"i8"
#define TYPE_I16	0x03  //对应字符串"i16"
#define TYPE_I32	0x04  //对应字符串"i32"
#define TYPE_U8		0x06  //对应字符串"u8"
#define TYPE_U16	0x07  //对应字符串"u16"
#define TYPE_U32	0x08  //对应字符串"u32"
#define TYPE_STR	0x0a  //对应字符串"str"

#define ENTER	   "\n"

typedef union {
	double f64;
	float f32;
	int8_t i8;
	int16_t i16;
	int32_t i32;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	char str[MAX_DATA_STR_LENGHT];
}data_type_t;

/*
 * 存储格式
 * "key=\"键值\",type=\"数据类型\",data=\"数据\"\n"
 * */
typedef struct{
	uint8_t state;					//状态,0x00无效, 0x01有效，被删除的暂时标记未无效状态
	char key[MAX_KEY_LENGHT];		//键值
	uint32_t type;					//数据类型
	data_type_t data;				//数据
	char desc[MAX_DESCRIBE_LENGHT];	//描述信息
}item_t;

typedef struct{
	const char *work_path;
	uint32_t max_entry;

	uint32_t max_level;
	item_t *data;
}set_t;

//初始化集合
int32_t set_init(set_t *set);
//释放集合，释放时会将数据同步到文件
int32_t set_deinit(set_t *set);
//添加或修改一个元素
int32_t set_add_revise(set_t *set, const char *key, uint32_t type, const data_type_t *data);
//获取集合中的一个元素
int32_t set_get(set_t *set, const char *key, uint32_t *type, data_type_t *data);
//删除一个元素
int32_t set_del(set_t *set, char *key);
//同步到文件
int32_t set_sync(set_t *set);

#ifdef __cplusplus
}
#endif

#endif // __SET_H_
