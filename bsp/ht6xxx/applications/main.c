/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     zylx         first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "fal.h"
#include "easyflash.h"
#include <arpa/inet.h>
#include <netdev.h>
#include <sys/socket.h>
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
//自动挂载表
const struct dfs_mount_tbl mount_table[] =
{
    {"flash0", "/", "elm", 0, 0},
    {0}
};

int fs_test(int argc, char **argv)
{
    int fd, size;
    char buffer[16];

    fd = open("/hello.txt", O_RDWR | O_CREAT);
    if (fd >= 0)
    {
        write(fd, "hello world\r\n", 13);
        close(fd);
        rt_kprintf("Write done.\n");
    }

    /* 以只读模式打开 /text.txt 文件 */
    fd = open("/hello.txt", O_RDONLY);
    if (fd >= 0)
    {
        size = read(fd, buffer, sizeof(buffer));
        close(fd);
        rt_kprintf("Read %d bytes from file hello.txt: %s \n", size, buffer);
    }

    return 0;
}
MSH_CMD_EXPORT(fs_test, file system test.);

void server_thread(void *parameter)
{
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    struct netdev *netdev = RT_NULL;
    int sockfd = -1;

    while(1)
    {

        rt_thread_mdelay(500);
    }
}

void led_thread_entry(void* parameter)
{
#define interval 250

    rt_pin_mode(IO_RUN_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_A_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_B_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_C_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_1_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_2_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_3_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_4_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_485_LED, PIN_MODE_OUTPUT);

    while (1)
    {
        HD_clrWDT();
        rt_pin_write(IO_RUN_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RUN_LED, PIN_HIGH);

        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_TX_LED, PIN_HIGH);

        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RX_LED, PIN_HIGH);

        HD_clrWDT();
        rt_pin_write(IO_485_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_485_LED, PIN_HIGH);

        rt_pin_write(IO_RELAY_A_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_A_LED, PIN_HIGH);

        rt_pin_write(IO_RELAY_B_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_B_LED, PIN_HIGH);

        HD_clrWDT();
        rt_pin_write(IO_RELAY_C_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_C_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_1_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_1_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_2_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_2_LED, PIN_HIGH);

        HD_clrWDT();
        rt_pin_write(IO_SIG_3_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_3_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_4_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_4_LED, PIN_HIGH);
    }
}

int main(void)
{
    rt_thread_t led_thread_ptr, server_thread_ptr;
    rt_uint32_t reboot_cnt = 0, len = 0;
    char reboot_cnt_buf[11];

    fal_init();
    easyflash_init();
    // init fal flash device driver for fs
    fs_partition_init();
    // mount file system
    dfs_mount("flashfs", "/", "elm", 0, 0);

    HD_clrWDT();

    ef_get_env_blob("reboot_cnt", reboot_cnt_buf, sizeof(reboot_cnt_buf), &len);
    reboot_cnt = atoi(reboot_cnt_buf);
    reboot_cnt++;
    rt_sprintf(reboot_cnt_buf, "%d", reboot_cnt);
    ef_set_env("reboot_cnt", reboot_cnt_buf);

    led_thread_ptr = rt_thread_create("led",
                                    led_thread_entry, RT_NULL,
                                    256, 5, 1);

    if (led_thread_ptr != RT_NULL) rt_thread_startup(led_thread_ptr);


    server_thread_ptr = rt_thread_create("server",
                                    server_thread, RT_NULL,
                                    256, 5, 1);

    if (server_thread_ptr != RT_NULL) rt_thread_startup(server_thread_ptr);

    return RT_EOK;
}
