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
#include <dfs_posix.h> /* 使用文件系统时需要加入该头文件 */


#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "main"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

extern int fs_partition_init(void);

void led_thread_entry(void* parameter)
{
#define interval 250

    rt_pin_mode(IO_ALARM_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_ACT_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(IO_ALARM_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_ALARM_LED, PIN_HIGH);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_TX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_ACT_LED, PIN_HIGH);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_ACT_LED, PIN_LOW);
        rt_thread_mdelay(interval);
    }
}

/*
int set_console_baud(int argc, char **argv)
{
    rt_device_t console_dev;
    struct serial_configure cfg = RT_SERIAL_CONFIG_DEFAULT;

    if(argc == 2)
    {
        sscanf(argv[1], "%d", &cfg.baud_rate);
    }
    console_dev = rt_device_find("uart0");
    if(console_dev != RT_NULL)
    {
        rt_device_control(console_dev, RT_DEVICE_CTRL_CONFIG, &cfg);
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(set_console_baud, baud, set baudrate usage: baud 115200);
*/

int main(void)
{
    rt_thread_t led_thread_ptr;
    rt_uint32_t reboot_cnt = 0, len = 0;
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
    ef_set_env("reboot_cnt", reboot_cnt_buf);

    led_thread_ptr = rt_thread_create("led", led_thread_entry, RT_NULL, 256, 5, 1);
    if (led_thread_ptr != RT_NULL)
    {
        rt_thread_startup(led_thread_ptr);
    }

    return RT_EOK;
}
