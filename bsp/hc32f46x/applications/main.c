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
#include <dlfcn.h> /* 动态库api */

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

void *dm_handle = RT_NULL;
int dl_open(int argc, char **argv)
{
    rt_device_t console_dev;
    struct serial_configure cfg = RT_SERIAL_CONFIG_DEFAULT;

    if(argc == 2)
    {
        // 加载动态库
        dm_handle = dlopen(argv[1], 0);
        if(dm_handle == RT_NULL)
        {
            LOG_E("load %s error", argv[1]);
            return 0;
        }
        else
        {
            LOG_I("load %s ok", argv[1]);
        }
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(dl_open, dlopen, load a dmodule);

int dl_close(int argc, char **argv)
{
    LOG_I("uninstall %x", dm_handle);
    dlclose(dm_handle);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(dl_close, dlclose, close a dmodule);

// 声明需要链接的动态函数类型
typedef int (*lib_func_t)(void);
typedef int (*add_func_t)(int, int);
typedef int (*multi_func_t)(int, int);
int dmodule_test(const char *dll_path)
{
    void *dmodule_handle = RT_NULL;
    // 声明动态函数指针
    lib_func_t lib_func;
    add_func_t add_func;
    multi_func_t multi_func;

    // 加载动态库
    dmodule_handle = dlopen(dll_path, 0);
    if(dmodule_handle == RT_NULL)
    {
        LOG_E("load %s error", dll_path);
        return 0;
    }
    else
    {
        LOG_I("load %s ok", dll_path);
    }

    // 加载函数 int lib_func(void);
    lib_func = dlsym(dmodule_handle, "lib_func");
    if(lib_func == RT_NULL)
    {
        LOG_E("lib_func not found");
    }
    else
    {
        LOG_I("execute lib_func:");
        lib_func();
    }

    // 加载函数 int add_func(int a, int b);
    add_func = dlsym(dmodule_handle, "add_func");
    if(add_func == RT_NULL)
    {
        LOG_E("add_func not found");
    }
    else
    {
        LOG_I("execute add_func(3, 4):");
        add_func(3, 4);
    }

    // 加载函数 int multi_func(int a, int b);
    multi_func = dlsym(dmodule_handle, "multi_func");
    if(multi_func == RT_NULL)
    {
        LOG_E("multi_func not found");
    }
    else
    {
        LOG_I("execute multi_func(5, 6):");
        multi_func(5, 6);
    }
    // 卸载动态库
    dlclose(dmodule_handle);
    LOG_I("uninstall %s", dll_path);

    return 0;
}

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

    dmodule_test("/lib.so");
    dmodule_test("/lib2.so");

    return RT_EOK;
}
