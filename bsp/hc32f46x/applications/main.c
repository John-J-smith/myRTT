/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * main.c
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-23     John Shi     first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <fal.h>
#include <easyflash.h>
#include <dfs_posix.h> /* 当需要使用文件操作时，需要包含这个头文件 */

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "main"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

// 手动fs分区设备初始化
extern int fs_partition_init(void);

void led_thread_entry(void* parameter)
{
#define interval 250

    rt_pin_mode(IO_ACT_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);

    while (1)
    {
        /*rt_pin_write(IO_RX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_TX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_thread_mdelay(interval);*/

        rt_pin_write(IO_ACT_LED, PIN_HIGH);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_ACT_LED, PIN_LOW);
        rt_thread_mdelay(interval);
    }
}

void YX_test_thread_entry(void* parameter)
{
    rt_pin_mode(IO_YX_DT_1, PIN_MODE_INPUT);
    rt_pin_mode(IO_YX_DT_2, PIN_MODE_INPUT);
    rt_pin_mode(IO_YX_DT_3, PIN_MODE_INPUT);
    rt_pin_mode(IO_YX_DT_4, PIN_MODE_INPUT);

    rt_pin_write(IO_RX_LED, PIN_LOW);
    rt_pin_write(IO_TX_LED, PIN_LOW);
    rt_pin_write(IO_ACT_LED, PIN_LOW);
    rt_pin_mode(IO_ACT_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);
    rt_pin_write(IO_RX_LED, PIN_LOW);
    rt_pin_write(IO_TX_LED, PIN_LOW);
    rt_pin_write(IO_ACT_LED, PIN_LOW);

    while (1)
    {
        if(rt_pin_read(IO_YX_DT_1) && rt_pin_read(IO_YX_DT_2) && rt_pin_read(IO_YX_DT_3) && rt_pin_read(IO_YX_DT_4))
        {
            rt_pin_write(IO_RX_LED, PIN_LOW);
            rt_pin_write(IO_TX_LED, PIN_LOW);
        }
        else
        {
            rt_pin_write(IO_RX_LED, PIN_HIGH);
            rt_pin_write(IO_TX_LED, PIN_HIGH);
        }

        rt_thread_mdelay(1);
    }
}

void RS485_test_thread_entry(void* parameter)
{
#define BUF_SIZE 512

    rt_device_t rs485_1, rs485_2, rs485_3, rs485_4;
    rt_uint8_t *buf = RT_NULL;
    rt_uint32_t len = 0;

    buf = (rt_uint8_t *)rt_malloc(BUF_SIZE);

    rt_pin_mode(IO_TR_RS485_1, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TR_RS485_2, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TR_RS485_3, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TR_RS485_4, PIN_MODE_OUTPUT);

    // enable 485 input
    rt_pin_write(IO_TR_RS485_1, PIN_LOW);
    rt_pin_write(IO_TR_RS485_2, PIN_LOW);
    rt_pin_write(IO_TR_RS485_3, PIN_LOW);
    rt_pin_write(IO_TR_RS485_4, PIN_LOW);

    if(RT_NULL == (rs485_1 = rt_device_find("uart1")))
    {
        LOG_E("uart 1 not found");
    }
    else
    {
        rt_device_open(rs485_1, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    }

    if(RT_NULL == (rs485_2 = rt_device_find("uart2")))
    {
        LOG_E("uart 2 not found");
    }
    else
    {
        rt_device_open(rs485_2, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    }

    if(RT_NULL == (rs485_3 = rt_device_find("uart3")))
    {
        LOG_E("uart 3 not found");
    }
    else
    {
        rt_device_open(rs485_3, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    }

    if(RT_NULL == (rs485_4 = rt_device_find("uart4")))
    {
        LOG_E("uart 4 not found");
    }
    else
    {
        rt_device_open(rs485_4, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    }

    rt_pin_write(IO_TR_RS485_1, PIN_HIGH);
    rt_device_write(rs485_1, 0, "hello world 1\r\n", 17);
    rt_pin_write(IO_TR_RS485_1, PIN_LOW);

    rt_pin_write(IO_TR_RS485_2, PIN_HIGH);
    rt_device_write(rs485_2, 0, "hello world 2\r\n", 17);
    rt_pin_write(IO_TR_RS485_2, PIN_LOW);

    rt_pin_write(IO_TR_RS485_3, PIN_HIGH);
    rt_device_write(rs485_3, 0, "hello world 3\r\n", 17);
    rt_pin_write(IO_TR_RS485_3, PIN_LOW);

    rt_pin_write(IO_TR_RS485_4, PIN_HIGH);
    rt_device_write(rs485_4, 0, "hello world 4\r\n", 17);
    rt_pin_write(IO_TR_RS485_4, PIN_LOW);

    while (1)
    {
        len = 0;
        if((len = rt_device_read(rs485_1, 0, buf, BUF_SIZE)) > 0)
        {
            rt_pin_write(IO_TR_RS485_1, PIN_HIGH);
            rt_device_write(rs485_1, 0, buf, len);
            rt_thread_mdelay(1);
            rt_pin_write(IO_TR_RS485_1, PIN_LOW);
        }

        len = 0;
        if((len = rt_device_read(rs485_2, 0, buf, BUF_SIZE)) > 0)
        {
            rt_pin_write(IO_TR_RS485_2, PIN_HIGH);
            rt_device_write(rs485_2, 0, buf, len);
            rt_thread_mdelay(1);
            rt_pin_write(IO_TR_RS485_2, PIN_LOW);
        }

        len = 0;
        if((len = rt_device_read(rs485_3, 0, buf, BUF_SIZE)) > 0)
        {
            rt_pin_write(IO_TR_RS485_3, PIN_HIGH);
            rt_device_write(rs485_3, 0, buf, len);
            rt_thread_mdelay(1);
            rt_pin_write(IO_TR_RS485_3, PIN_LOW);
        }

        len = 0;
        if((len = rt_device_read(rs485_4, 0, buf, BUF_SIZE)) > 0)
        {
            rt_pin_write(IO_TR_RS485_4, PIN_HIGH);
            rt_device_write(rs485_4, 0, buf, len);
            rt_pin_write(IO_TR_RS485_4, PIN_LOW);
        }

        rt_thread_mdelay(1);
    }
}


int main(void)
{
    rt_thread_t led_thread_ptr, yx_thread_ptr, RS485_thread_ptr;
    /*rt_uint32_t reboot_cnt = 0, len = 0;
    char reboot_cnt_buf[11];

    fal_init();
    easyflash_init();
    // init fal flash device driver for fs
    fs_partition_init();
    // mount file system
    dfs_mount("flashfs", "/", "elm", 0, 0);

    ef_get_env_blob("reboot_cnt", reboot_cnt_buf, sizeof(reboot_cnt_buf), &len);
    reboot_cnt = atoi(reboot_cnt_buf);
    reboot_cnt++;
    rt_sprintf(reboot_cnt_buf, "%d", reboot_cnt);
    ef_set_env("reboot_cnt", reboot_cnt_buf);*/

    led_thread_ptr = rt_thread_create("led", led_thread_entry, RT_NULL, 256, 5, 1);
    if (led_thread_ptr != RT_NULL)
    {
        rt_thread_startup(led_thread_ptr);
    }

    yx_thread_ptr = rt_thread_create("yx", YX_test_thread_entry, RT_NULL, 512, 2, 1);
    if (yx_thread_ptr != RT_NULL)
    {
        rt_thread_startup(yx_thread_ptr);
    }

    RS485_thread_ptr = rt_thread_create("485", RS485_test_thread_entry, RT_NULL, 512, 4, 1);
    if (RS485_thread_ptr != RT_NULL)
    {
        rt_thread_startup(RS485_thread_ptr);
    }

    return RT_EOK;
}
