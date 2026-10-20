/*
 * offset_load.c
 *
 *  Created on: 2022年1月22日
 *      Author: Administrator
 */

#include "offset_load.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "offset_load.c,%d", __LINE__
#include "../debug_manage/debug.h"

#define USEING_CHECKSUM

typedef struct{
	uint32_t load_addr;
	uint32_t size;
	uint16_t sum;
	uint8_t  sum_flag;
}load_info_t;

#ifdef USEING_CHECKSUM
static uint16_t sum16(const uint8_t *data, uint32_t lenght, uint16_t init_val)
{
	uint32_t i;
	uint16_t sum = init_val;

	for(i=0; i<lenght; i++) {
		sum += data[i];
	}
	return sum;
}
#endif

int32_t load_offse_data(const char *file_name, uint32_t load_addr)
{
	int mem_fd;
	int file_fd;
	int result;
	uint32_t page_addr;
	uint32_t page_offset;
	uint32_t file_size;
	uint32_t map_size;
	uint8_t *mem_ptr;
	struct stat stat_buf;
	load_info_t load_info;
	uint32_t succeed_falg = 0;
	uint32_t page_size = sysconf(_SC_PAGESIZE);

	if(!file_name) {
		LOG_E("Invalid parameter");
	}

	load_info.load_addr = load_addr;
	LOG_I("load address %x\r\n", load_info.load_addr);
#ifdef USEING_CHECKSUM
	load_info.sum_flag = 1;
#else
	load_info.sum_flag = 0;
#endif

	//检查文件是否存在
	if(access(file_name, R_OK) != 0) {
		LOG_E("No such file or directory\r\n");
		return -1;
	}
	//获取文件大小
	if(lstat(file_name, &stat_buf) == -1) {
		LOG_E("get file info failed");;
		return -1;
	}
	file_size = stat_buf.st_size;
	LOG_I("file size %d\r\n", file_size);

	//打开mem设备
	mem_fd = open("/dev/mem", O_RDWR);
	if(mem_fd < 1) {
		LOG_E("open /dev/mem failed");
		return -1;
	}

	//打开数据文件
	file_fd = open(file_name, O_RDWR);
	if(file_fd < 1) {
		//关闭内存设备
		close(mem_fd);
		LOG_E("open %s failed", file_name);
		return -1;
	}

	//计算映射大小
	if((file_size%page_size) != 0)
		map_size = file_size + (page_size - file_size % page_size);
	else
		map_size = file_size;
	//计算写入地址的页起始地址和偏移
	page_addr = (load_info.load_addr & ~(page_size-1));
	page_offset = load_info.load_addr - page_addr;
	//内存映射
	mem_ptr = (uint8_t *)mmap(NULL, map_size, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, page_addr);
	if((int)mem_ptr != -1) {
		//加载文件到内存
		result = read(file_fd, mem_ptr+page_offset, file_size);
		if(result > 0) {
			//补偿数据大小
			load_info.size = result;
#ifdef USEING_CHECKSUM
			//和校验
			load_info.sum = sum16(mem_ptr+page_offset, result, 0xffff);
#endif
			//加载成功
			succeed_falg = 1;
		}
		else {
			//取消映射
			munmap(mem_ptr, map_size);
			//关闭内存设备
			close(mem_fd);
			//关闭数据文件
			close(file_fd);
			LOG_E("read %s failed", file_name);
			return -1;
		}
		//取消映射
		munmap(mem_ptr, map_size);
	}
	else {
		//关闭内存设备
		close(mem_fd);
		//关闭数据文件
		close(file_fd);
		LOG_E("mmap /dev/mem failed");
		return -1;
	}

	//写入加载信息
	if(succeed_falg) {
		page_addr = (0x30000000 & ~(page_size-1));
		page_offset = 0x30000000 - page_addr;
		mem_ptr = (uint8_t *)mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, page_addr);
		if((int)mem_ptr != -1) {
			*((load_info_t*)(mem_ptr + page_offset)) = load_info;
			munmap(mem_ptr, page_size);
		}
		else {
			//关闭内存设备
			close(mem_fd);
			//关闭数据文件
			close(file_fd);
			LOG_E("mmap /dev/mem failed");
			return -1;
		}
	}

	//关闭内存设备
	close(mem_fd);
	//关闭数据文件
	close(file_fd);

	LOG_I("load file succeed");
	LOG_I("load_addr = %x", load_info.load_addr);
	LOG_I("load size = %d", load_info.size);
#ifdef USEING_CHECKSUM
	LOG_I("SUM = %x", load_info.sum);
#endif

	return 0;
}

