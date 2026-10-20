/*
 * Empty C++ Application
 */


#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "xil_printf.h"

#include "platform.h"

#define DEBUG_TAG "main,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "./driver/core/debug.h"

#if (configTICK_RATE_HZ != 1000)
#error "system clock beat must be 1000 Hz"
#endif

#if (configUSE_TASK_FPU_SUPPORT != 2)
#error "must be enable fpu"
#endif

static void exception_handler(void *data)
{
    extern u32 DataAbortAddr;
    u32 FaultStatus;
    TaskHandle_t current_task;

    current_task = xTaskGetCurrentTaskHandle();
    xil_printf("Data abort task:%s\r\n", pcTaskGetName(current_task));
    xdbg_printf(XDBG_DEBUG_ERROR, "Data abort \n");
#ifdef __GNUC__
    FaultStatus = mfcp(XREG_CP15_DATA_FAULT_STATUS);
#elif defined (__ICCARM__)
    mfcp(XREG_CP15_DATA_FAULT_STATUS,FaultStatus);
#else
    { volatile register u32 Reg __asm(XREG_CP15_DATA_FAULT_STATUS);
    FaultStatus = Reg; }
#endif
    xil_printf("Data abort with Data Fault Status Register:%lx\n",FaultStatus);
    xil_printf("Address of Instruction causing Data abort:%lx\n",DataAbortAddr);
    for( ;; );
}
//static uint8_t buffer[1024];
//
//static void test_thread(void *arg)
//{
//    device_t *ocm_dev;
//    ssize_t tmp_size;
//
//    ocm_dev = device_open("ps_ocm", DEVICE_RDWR);
//    if(!ocm_dev) {
//        printf("open ps_ocm failed\r\n");
//        vTaskDelete(NULL);
//    }
//
//    while(1)
//    {
//        //从OCM读数据
//        tmp_size = device_read(ocm_dev, &buffer[0], sizeof(buffer));
//        //写回到OCM
//        if(tmp_size > 0) {
//            device_write(ocm_dev, buffer, tmp_size);
//        }
//    }
//}
//
//void test_ps1_ocm(void)
//{
//    TaskHandle_t xTask;
//
//    xTaskCreate(test_thread,
//                (const char *)"test_thread",
//                2048,
//                NULL,
//                tskIDLE_PRIORITY + 2,
//                &xTask);
//}


static void start_thread(void *arg)
{
#if DEBUG_LEVEL == DEBUG_OUTPUT
    TaskStatus_t  *task_info;
    UBaseType_t taskNbr;
    uint32_t TotalRunTime;
    TickType_t last_tick, cur_tick, run_tick;
    float run_time;
#endif
    uint32_t led_number;
    uint32_t last_led;
    device_state_t device_state;

    //输出编译日期和时间
    LOG_I("%s;%s\n", __DATE__, __TIME__);
    //平台初始化
    platform_init();

#if DEBUG_LEVEL == DEBUG_OUTPUT
    //申请内存
    task_info = (TaskStatus_t*)pvPortMalloc(50*sizeof(TaskStatus_t));
    if(task_info == NULL) {
        LOG_E("malloc memory failed");
        vTaskDelete(NULL);
    }
    last_tick = 0;
    run_time = 0.0;
#endif
    last_led = LED_GRE_POWER;
    power_off(LED_GRE_POWER);
    power_off(LED_RED_POWER);
    while(1)
    {
        get_device_state(&device_state);
        if(device_state.global_fault_code == 0x00) {
            led_number = LED_GRE_POWER;
        }
        else {
            led_number = LED_RED_POWER;
        }
        if(last_led != led_number) {
            power_off(last_led);
            last_led = led_number;
        }

        //LED闪烁

        power_on(led_number);
        vTaskDelay(800);
        power_off(led_number);
        vTaskDelay(800);

#if DEBUG_LEVEL == DEBUG_OUTPUT
        //打印系统中所有任务的信息
        taskNbr = uxTaskGetSystemState(task_info, 50, &TotalRunTime);
        xil_printf("task name\t\t priority\t task number\t stack high water\r\n");
        for(uint32_t i=0; i<taskNbr; i++) {
            if(strlen(task_info[i].pcTaskName) < 8)
                xil_printf("%s\t\t\t %ld\t\t %ld\t\t %d\r\n", task_info[i].pcTaskName, task_info[i].uxCurrentPriority, task_info[i].xTaskNumber, task_info[i].usStackHighWaterMark);
            else if(strlen(task_info[i].pcTaskName) < 16)
                xil_printf("%s\t\t %ld\t\t %ld\t\t %d\r\n", task_info[i].pcTaskName, task_info[i].uxCurrentPriority, task_info[i].xTaskNumber, task_info[i].usStackHighWaterMark);
            else
                xil_printf("%s\t %ld\t\t %ld\t\t %d\r\n", task_info[i].pcTaskName, task_info[i].uxCurrentPriority, task_info[i].xTaskNumber, task_info[i].usStackHighWaterMark);
        }

        //打印系统运行时间
        cur_tick = xTaskGetTickCount();
        if(cur_tick > last_tick) {
            run_tick = cur_tick - last_tick;
        }
        else {
            run_tick = portMAX_DELAY - last_tick + cur_tick;
        }
        last_tick = cur_tick;
        run_time += run_tick/1000.0f;
        xil_printf("run time: %ld sec\r\n", (uint32_t)(run_time + 0.5));
        xil_printf("free heap size: %ld sec\r\n",  xPortGetFreeHeapSize());
#endif
    }
}

int main()
{
    TaskHandle_t handle;
    portBASE_TYPE result;

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_DATA_ABORT_INT, exception_handler, NULL);
    //xil_printf("test uart ++++++++++++\r\n");

    //创建启动线程
    result = xTaskCreate(start_thread,
                         "start_thread",
                         2048,
                         NULL,
                         tskIDLE_PRIORITY + 2,
                         &handle );
    if(result != pdPASS) {
        LOG_E("create task start_thread failed\r\n");
        return -1;
    }

    /* 启动调取器 */
    vTaskStartScheduler();
    for(; ; );
}
