/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-21     John Shi     the first version
 */

#include <rtthread.h>
#include <ymodem.h>
#include <fal.h>
#include <easyflash.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 32
#define FAL_PARTITION_NAME  "download"

struct custom_ctx
{
    struct rym_ctx parent;
    const struct fal_partition *part;
    int flen;
    int off;
    char fpath[PATH_MAX];
};

static enum rym_code _rym_recv_begin(
    struct rym_ctx *ctx,
    rt_uint8_t *buf,
    rt_size_t len)
{
    struct custom_ctx *cctx = (struct custom_ctx *)ctx;

    cctx->fpath[0] = '/';
    rt_strncpy(&(cctx->fpath[1]), (const char *)buf, PATH_MAX - 1);
    cctx->part = fal_partition_find(FAL_PARTITION_NAME);
    if (cctx->part == RT_NULL)
    {
        rt_kprintf("partition not found\n");
        return RYM_CODE_CAN;
    }
    cctx->off = 0;
    cctx->flen = atoi(1 + (const char *)buf + rt_strnlen((const char *)buf, len - 1));
    if (cctx->flen == 0)
        cctx->flen = -1;

    return RYM_CODE_ACK;
}

static enum rym_code _rym_recv_data(
    struct rym_ctx *ctx,
    rt_uint8_t *buf,
    rt_size_t len)
{
    struct custom_ctx *cctx = (struct custom_ctx *)ctx;

    RT_ASSERT(cctx->part != RT_NULL);
    if (cctx->flen == -1)
    {
        fal_partition_write(cctx->part, cctx->off, buf, len);
    }
    else
    {
        int wlen = (len > cctx->flen - cctx->off) ? cctx->flen - cctx->off : len;
        fal_partition_write(cctx->part, cctx->off, buf, len);
        cctx->off += wlen;
    }

    return RYM_CODE_ACK;
}

static enum rym_code _rym_recv_end(
    struct rym_ctx *ctx,
    rt_uint8_t *buf,
    rt_size_t len)
{
    struct custom_ctx *cctx = (struct custom_ctx *)ctx;

    RT_ASSERT(cctx->part != RT_NULL);
    cctx->part = RT_NULL;

    return RYM_CODE_ACK;
}

static rt_err_t rym_download_file(rt_device_t idev)
{
    rt_err_t res;
    struct custom_ctx *ctx = (struct custom_ctx *)rt_malloc(sizeof(struct custom_ctx));
    char buf[32];

    if (!ctx)
    {
        rt_kprintf("rt_malloc failed\n");
        return RT_ENOMEM;
    }
    ctx->part = RT_NULL;
    RT_ASSERT(idev);
    res = rym_recv_on_device(&ctx->parent, idev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                             _rym_recv_begin, _rym_recv_data, _rym_recv_end, 1000);

    if(res == RT_EOK)
    {
        ef_set_env("iap_need_copy_app", "1");

        rt_sprintf(buf, "%d", ctx->flen);
        ef_set_env("iap_copy_app_size", buf);

        rt_enter_critical();
        while(1); /* wait for watch dog reset*/
        rt_exit_critical();
    }

    rt_free(ctx);

    return res;
}

#ifdef RT_USING_FINSH
#include <finsh.h>

static rt_err_t ry(uint8_t argc, char **argv)
{
    rt_err_t res;
    rt_device_t dev;

    if (argc > 1)
        dev = rt_device_find(argv[1]);
    else
        dev = rt_console_get_device();
    if (!dev)
    {
        rt_kprintf("could not find device.\n");
        return -RT_ERROR;
    }
    res = rym_download_file(dev);

    return res;
}
MSH_CMD_EXPORT(ry, YMODEM Receive e.g: ry [uart0] default by console.);

#endif /* RT_USING_FINSH */
