#include "set.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "set.c,%d", __LINE__
#include "../debug_manage/debug.h"

static int32_t load_file(set_t *set, const char *name)
{
	uint32_t i;
	int32_t result;
	int32_t lenght;
	char *temp1;
	char *temp2;
	FILE *file;
	char type[16];
	char key[MAX_KEY_LENGHT];
#if MAX_DATA_STR_LENGHT < 16
	char data[16];
#else
	char data[MAX_DATA_STR_LENGHT];
#endif
	char describe[MAX_DESCRIBE_LENGHT];
	char buffer[sizeof(key)+sizeof(data)+sizeof(type)];

	//打开文件
	file = fopen(name, "r+");
	if(!file) {
		//文件不存在，不用加载
		return 0;
	}

	result = 0;
	//加载文件内容
	for(i=0; i<set->max_entry; ) {
		//读取一行
		if(!fgets(buffer, sizeof(buffer), file)) {
			//文件结束，但没检测到结束标志
			LOG_E("read file failed");
			result = -1;
			break;
		}

		//检测文件结束标志
		if(strncmp(buffer, "file end!!", strlen("file end!!")) == 0) {
			set->max_level = i;
			break;
		}

		//提取key
		temp1 = strstr(buffer, "\"");
		if(!temp1) {
			continue;
		}
		temp2 = strstr(temp1+1, "\"");
		if(!temp2) {
			continue;
		}
		memset(key, 0, sizeof(key));
		if(sizeof(key) > (temp2-(temp1+1))) {
			lenght = temp2-(temp1+1);
		}
		else {
			lenght = sizeof(key);
		}
		strncpy(key, temp1+1, lenght);

		//提取类型
		temp1 = strstr(temp2+1, "\"");
		if(!temp1) {
			continue;
		}
		temp2 = strstr(temp1+1, "\"");
		if(!temp2) {
			continue;
		}
		memset(type, 0, sizeof(type));
		if(sizeof(type) > (temp2-(temp1+1))) {
			lenght = temp2-(temp1+1);
		}
		else {
			lenght = sizeof(type);
		}
		strncpy(type, temp1+1, lenght);

		//提取数据
		temp1 = strstr(temp2+1, "\"");
		if(!temp1) {
			continue;
		}
		temp2 = strstr(temp1+1, "\"");
		if(!temp2) {
			continue;
		}
		memset(data, 0, sizeof(data));
		if(sizeof(data) > (temp2-(temp1+1))) {
			lenght = temp2-(temp1+1);
		}
		else {
			lenght = sizeof(data);
		}
		strncpy(data, temp1+1, lenght);

		//提取描述
		temp1 = strstr(temp2+1, "\"");
		if(temp1) {
			temp2 = strstr(temp1+1, "\"");
		}
		if(temp1 && temp2) {
			memset(describe, 0, sizeof(describe));
			if(sizeof(describe) > (temp2-(temp1+1))) {
				lenght = temp2-(temp1+1);
			}
			else {
				lenght = sizeof(describe);
			}
			strncpy(describe, temp1+1, lenght);
		}
		else {
			memset(describe, 0, sizeof(describe));
			if(sizeof(describe) > strlen(" ")) {
				lenght = strlen(" ");
			}
			else {
				lenght = sizeof(describe);
			}
			strncpy(describe, " ", lenght);
		}

		//根据类型解析数据
		if(strcmp(type, "f64") == 0) {
			set->data[i].type = TYPE_F64;
			set->data[i].data.f64 = atof(data);
		}
		else if(strcmp(type, "f32") == 0) {
			set->data[i].type = TYPE_F32;
			set->data[i].data.f32 = (float)atof(data);
		}
		else if(strcmp(type, "i8") == 0) {
			set->data[i].type = TYPE_I8;
			set->data[i].data.i8 = (int8_t)strtol(data, NULL, 0);
		}
		else if(strcmp(type, "i16") == 0) {
			set->data[i].type = TYPE_I16;
			set->data[i].data.i16 = (int16_t)strtol(data, NULL, 0);
		}
		else if(strcmp(type, "i32") == 0) {
			set->data[i].type = TYPE_I32;
			set->data[i].data.i32 = (int32_t)strtol(data, NULL, 0);
		}
		else if(strcmp(type, "u8") == 0) {
			set->data[i].type = TYPE_U8;
			set->data[i].data.u8 = (uint8_t)strtoul(data, NULL, 0);
		}
		else if(strcmp(type, "u16") == 0) {
			set->data[i].type = TYPE_U16;
			set->data[i].data.u16 = (uint16_t)strtoul(data, NULL, 0);
		}
		else if(strcmp(type, "u32") == 0) {
			set->data[i].type = TYPE_U32;
			set->data[i].data.u32 = (uint32_t)strtoul(data, NULL, 0);
		}
		else if(strcmp(type, "str") == 0) {
			set->data[i].type = TYPE_STR;
			strncpy(set->data[i].data.str, data, MAX_DATA_STR_LENGHT);
		}
		else {
			continue;
		}
		strncpy(set->data[i].key, key, MAX_KEY_LENGHT);
		strncpy(set->data[i].desc, describe, MAX_DESCRIBE_LENGHT);
		set->data[i].state = 0x01;
		i++;
	}

	//未加载完
	if(i >= set->max_entry) {
		LOG_E("load failed");
		result = -1;
	}

	//关闭文件
	fclose(file);

	return result;
}

static int32_t diff_file(const char *name1, const char *name2)
{
	int32_t result;
	size_t lenght1;
	size_t lenght2;
	FILE *file1;
	FILE *file2;
	char buffer1[128];
	char buffer2[128];

	//打开文件
	file1 = fopen(name1, "r+");
	if(!file1) {
		LOG_W("open %s failed", name1);
		return -1;
	}
	file2 = fopen(name2, "r+");
	if(!file2) {
		fclose(file1);
		LOG_W("open %s failed", name2);
		return -1;
	}

	result = 0;
	for(; ; ) {
		lenght1 = fread(buffer1, 1, sizeof(buffer1), file1);
		lenght2 = fread(buffer2, 1, sizeof(buffer2), file2);
		//读取结束
		if((lenght1 == 0) && (lenght2 == 0) && feof(file1) && feof(file2)) {
			 break;
		}
		//文件不相等
		if(lenght1 != lenght2) {
			result = -1;
			LOG_W("File inequality");
			break;
		}
		if(memcmp(buffer1, buffer2, lenght1) != 0) {
			result = -1;
			LOG_W("File inequality");
			break;
		}
	}
	fclose(file1);
	fclose(file2);

	return result;
}

static int32_t check_file(const char *name)
{
	int32_t result;
	FILE *file;
	char buffer[128];

	//打开文件
	file = fopen(name, "r+");
	if(!file) {
		LOG_W("open %s failed", name);
		return -1;
	}

	result = 0;
	for(; ; ) {
		//读取一行
		if(!fgets(buffer, sizeof(buffer), file)) {
			//文件结束，但没检测到结束标志
			result = -1;
			LOG_W("File format error");
			break;
		}

		//检测文件结束标志
		if(strncmp(buffer, "file end!!", strlen("file end!!")) == 0) {
			break;
		}
	}

	return result;
}

static int32_t copy_file(const char *src, const char *dest)
{
	char system_command[MAX_FILE_PATH * 2 + 32];

	//进行文件拷贝
	snprintf(system_command, sizeof(system_command), "cp %s %s", src, dest);
	system(system_command);

	//清空磁盘IO
	system("sync");

	return 0;
}

//初始化集合
int32_t set_init(set_t *set)
{
	char *file_name;
	DIR *dir;
	char set_name[MAX_FILE_PATH+16];
	char temp_name[MAX_FILE_PATH+16];
	char sys_cmd[256];

	if((!set) || (!set->work_path)) {
		LOG_E("parameter error");
		return -1;
	}

	//分配内存
	set->data = malloc(sizeof(item_t)*set->max_entry);
	if(!set->data) {
		LOG_E("malloc failed");
		return -1;
	}
	memset(set->data, 0, sizeof(item_t)*set->max_entry);

	//检测目录是否存在，不存在则创建
	dir = opendir(set->work_path);
	if(!dir) {
		//创建目录
		snprintf(sys_cmd, sizeof(sys_cmd), "mkdir -p %s", set->work_path);
		system(sys_cmd);
		snprintf(sys_cmd, sizeof(sys_cmd), "chmod 0777 %s", set->work_path);
		system(sys_cmd);
		//检查目录创建是否成功
		dir = opendir(set->work_path);
		if(!dir) {
			LOG_E("create directory failed");
			return -1;
		}
	}
	//关闭目录
	closedir(dir);

	//组合文件名
	if(set->work_path[strlen(set->work_path)-1] != '/') {
		snprintf(set_name, sizeof(set_name), "%s/set.txt", set->work_path);
		snprintf(temp_name, sizeof(temp_name), "%s/temp.txt", set->work_path);
	}
	else {
		snprintf(set_name, sizeof(set_name), "%sset.txt", set->work_path);
		snprintf(temp_name, sizeof(temp_name), "%stemp.txt", set->work_path);
	}

	//比较两个文件
	if(diff_file(set_name, temp_name) < 0) {
		//检查文件是否完整
		if(check_file(temp_name) < 0) {
			file_name = set_name;
		}
		else {
			file_name = temp_name;
		}
	}
	else {
		file_name = set_name;
	}

	//加载文件
	if(load_file(set, file_name) < 0) {
		free(set->data);
		LOG_E("load file failed");
		return -1;
	}

	//初始化成功
	return 0;
}

//释放集合，释放时会将数据同步到文件
int32_t set_deinit(set_t *set)
{
	if((!set) || (!set->work_path)) {
		LOG_E("parameter error");
		return -1;
	}

	//同步数据到磁盘
	if(set_sync(set) != 0) {
		LOG_E("Failed to write to disk");
		return -1;
	}
	//释放内存
	free(set->data);

	return 0;
}

//添加或修改一个元素
int32_t set_add_revise(set_t *set, const char *key, uint32_t type, const data_type_t *data)
{
	uint32_t i;

	if((!set) || (!key) || (!data)) {
		LOG_E("parameter error");
		return -1;
	}

	//尝试修改
	for(i=0; i<set->max_level; i++) {
		if(set->data[i].state != 0) {
			if(strcmp(set->data[i].key, key) == 0) {
				set->data[i].type = type;
				set->data[i].data = *data;
				return 0;
			}
		}
	}

	//尝试增加
	for(i=0; i<set->max_entry; i++) {
		if(set->data[i].state == 0) {
			strncpy(set->data[i].key, key, MAX_KEY_LENGHT);
			set->data[i].type = type;
			set->data[i].data = *data;
			set->data[i].state = 0x01;
			if((i+1) > set->max_level) {
				set->max_level = i+1;
			}
			return 0;
		}
	}

	LOG_E("add or revise failed");

	return -1;
}

//获取集合中的一个元素
int32_t set_get(set_t *set, const char *key, uint32_t *type, data_type_t *data)
{
	uint32_t i;

	if((!set) || (!key) || (!type) || (!data)) {
		LOG_E("parameter error");
		return -1;
	}


	for(i=0; i<set->max_level; i++) {
		if(set->data[i].state != 0) {
			if(strcmp(set->data[i].key, key) == 0) {
				*type = set->data[i].type;
				*data = set->data[i].data;
				return 0;
			}
		}
	}

	LOG_E("get failed");

	return -1;
}

//删除一个元素
int32_t set_del(set_t *set, char *key)
{
	uint32_t i;

	if((!set) || (!key)) {
		LOG_E("parameter error");
		return -1;
	}

	for(i=0; i<set->max_level; i++) {
		if(strcmp(set->data[i].key, key) == 0) {
			set->data[i].state = 0;
			return 0;
		}
	}

	LOG_E("delete failed");

	return -1;
}

//同步到文件
int32_t set_sync(set_t *set)
{
	uint32_t i;
	FILE *file;
#if MAX_DATA_STR_LENGHT < 16
	char buffer[MAX_KEY_LENGHT + 16 + 32]
#else
	char buffer[MAX_KEY_LENGHT + MAX_DATA_STR_LENGHT + 32 + MAX_DESCRIBE_LENGHT];
#endif
	char set_name[MAX_FILE_PATH+16];
	char temp_name[MAX_FILE_PATH+16];

	if((!set) || (!set->work_path)) {
		LOG_E("parameter error");
		return -1;
	}

	//组合文件名
	if(set->work_path[strlen(set->work_path)-1] != '/') {
		snprintf(set_name, sizeof(set_name), "%s/set.txt", set->work_path);
		snprintf(temp_name, sizeof(temp_name), "%s/temp.txt", set->work_path);
	}
	else {
		snprintf(set_name, sizeof(set_name), "%sset.txt", set->work_path);
		snprintf(temp_name, sizeof(temp_name), "%stemp.txt", set->work_path);
	}

	//打开或创建文件
	file = fopen(temp_name, "w+");
	if(!file) {
		LOG_E("open file error");
		return -1;
	}

	//将数据写入文件
	for(i=0; i<set->max_level; i++) {
		//忽略无效数据
		if(set->data[i].state == 0) {
			continue;
		}

		//将数据转换为字符串
		if(set->data[i].type == TYPE_F64) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%0.12f\",desc=\"%s\""ENTER,
					set->data[i].key, "f64", set->data[i].data.f64, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_F32) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%0.12f\",desc=\"%s\""ENTER,
					set->data[i].key, "f32", set->data[i].data.f32, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_I8) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%d\",desc=\"%s\""ENTER,
					set->data[i].key, "i8", set->data[i].data.i8, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_I16) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%d\",desc=\"%s\""ENTER,
					set->data[i].key, "i16", set->data[i].data.i16, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_I32) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%d\",desc=\"%s\""ENTER,
					set->data[i].key, "i32", set->data[i].data.i32, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_U8) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%u\",desc=\"%s\""ENTER,
					set->data[i].key, "u8", set->data[i].data.u8, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_U16) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%u\",desc=\"%s\""ENTER,
					set->data[i].key, "u16", set->data[i].data.u16, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_U32) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%u\",desc=\"%s\""ENTER,
					set->data[i].key, "u32", set->data[i].data.u32, set->data[i].desc);
		}
		else if(set->data[i].type == TYPE_STR) {
			snprintf(buffer, sizeof(buffer), "key=\"%s\",type=\"%s\",data=\"%s\",desc=\"%s\""ENTER,
					set->data[i].key, "str", set->data[i].data.str, set->data[i].desc);
		}

		//将字符串写入文件
		fputs(buffer, file);
	}

	//写入文件结束标志
	fputs("file end!!"ENTER, file);

	//关闭文件
	fclose(file);

	//拷贝到set.txt文件
	copy_file(temp_name, set_name);

	return 0;
}
