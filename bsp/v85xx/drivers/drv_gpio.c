/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-09-05     Shijj             the first version
 */

#include <board.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#ifdef BSP_USING_GPIO

#define GPIOA_PIN_NUM   16

static const struct pin_index pins[] =
{
#if defined(BSP_USING_GPIOA)
    __PIN(0 ,  A, 0 ),
    __PIN(1 ,  A, 1 ),
    __PIN(2 ,  A, 2 ),
    __PIN(3 ,  A, 3 ),
    __PIN(4 ,  A, 4 ),
    __PIN(5 ,  A, 5 ),
    __PIN(6 ,  A, 6 ),
    __PIN(7 ,  A, 7 ),
    __PIN(8 ,  A, 8 ),
    __PIN(9 ,  A, 9 ),
    __PIN(10,  A, 10),
    __PIN(11,  A, 11),
    __PIN(12,  A, 12),
    __PIN(13,  A, 13),
    __PIN(14,  A, 14),
    __PIN(15,  A, 15),
#if defined(BSP_USING_GPIOB)
    __PIN(16,  B, 0),
    __PIN(17,  B, 1),
    __PIN(18,  B, 2),
    __PIN(19,  B, 3),
    __PIN(20,  B, 4),
    __PIN(21,  B, 5),
    __PIN(22,  B, 6),
    __PIN(23,  B, 7),
    __PIN(24,  B, 8),
    __PIN(25,  B, 9),
    __PIN(26,  B, 10),
    __PIN(27,  B, 11),
    __PIN(28,  B, 12),
    __PIN(29,  B, 13),
    __PIN(30,  B, 14),
    __PIN(31,  B, 15),
#if defined(BSP_USING_GPIOC)
    __PIN(32,  C, 0),
    __PIN(33,  C, 1),
    __PIN(34,  C, 2),
    __PIN(35,  C, 3),
    __PIN(36,  C, 4),
    __PIN(37,  C, 5),
    __PIN(38,  C, 6),
    __PIN(39,  C, 7),
    __PIN(40,  C, 8),
    __PIN(41,  C, 9),
    __PIN(42,  C, 10),
    __PIN(43,  C, 11),
    __PIN(44,  C, 12),
    __PIN(45,  C, 13),
    __PIN(46,  C, 14),
    __PIN(47,  C, 15),
#if defined(BSP_USING_GPIOD)
    __PIN(48,  D, 0),
    __PIN(49,  D, 1),
    __PIN(50,  D, 2),
    __PIN(51,  D, 3),
    __PIN(52,  D, 4),
    __PIN(53,  D, 5),
    __PIN(54,  D, 6),
    __PIN(55,  D, 7),
    __PIN(56,  D, 8),
    __PIN(57,  D, 9),
    __PIN(58,  D, 10),
    __PIN(59,  D, 11),
    __PIN(60,  D, 12),
    __PIN(61,  D, 13),
    __PIN(62,  D, 14),
    __PIN(63,  D, 15),
#if defined(BSP_USING_GPIOE)
    __PIN(64,  E, 0),
    __PIN(65,  E, 1),
    __PIN(66,  E, 2),
    __PIN(67,  E, 3),
    __PIN(68,  E, 4),
    __PIN(69,  E, 5),
    __PIN(70,  E, 6),
    __PIN(71,  E, 7),
    __PIN(72,  E, 8),
    __PIN(73,  E, 9),
    __PIN(74,  E, 10),
    __PIN(75,  E, 11),
    __PIN(76,  E, 12),
    __PIN(77,  E, 13),
    __PIN(78,  E, 14),
    __PIN(79,  E, 15),
#if defined(BSP_USING_GPIOG)
    __PIN(80,  G, 0),
    __PIN(81,  G, 1),
    __PIN(82,  G, 2),
    __PIN(83,  G, 3),
    __PIN(84,  G, 4),
    __PIN(85,  G, 5),
    __PIN(86,  G, 6),
    __PIN(87,  G, 7),
    __PIN(88,  G, 8),
    __PIN(89,  G, 9),
    __PIN(90,  G, 10),
    __PIN(91,  G, 11),
    __PIN(92,  G, 12),
    __PIN(93,  G, 13),
    __PIN(94,  G, 14),
    __PIN(95,  G, 15),
#if defined(BSP_USING_GPIOH)
    __PIN(96,  H, 0),
    __PIN(97,  H, 1),
    __PIN(98,  H, 2),
    __PIN(99,  H, 3),
    __PIN(100, H, 4),
    __PIN(101, H, 5),
    __PIN(102, H, 6),
    __PIN(103, H, 7),
    __PIN(104, H, 8),
    __PIN(105, H, 9),
    __PIN(106, H, 10),
    __PIN(107, H, 11),
    __PIN(108, H, 12),
    __PIN(109, H, 13),
    __PIN(110, H, 14),
    __PIN(111, H, 15),
#if defined(BSP_USING_GPIOI)
    __PIN(112, I, 0),
    __PIN(113, I, 1),
    __PIN(114, I, 2),
    __PIN(115, I, 3),
    __PIN(116, I, 4),
    __PIN(117, I, 5),
    __PIN(118, I, 6),
    __PIN(119, I, 7),
    __PIN(120, I, 8),
    __PIN(121, I, 9),
    __PIN(122, I, 10),
    __PIN(123, I, 11),
    __PIN(124, I, 12),
    __PIN(125, I, 13),
    __PIN(126, I, 14),
    __PIN(127, I, 15),
#endif /* defined(BSP_USING_GPIOI) */
#endif /* defined(BSP_USING_GPIOH) */
#endif /* defined(BSP_USING_GPIOG) */
#endif /* defined(BSP_USING_GPIOE) */
#endif /* defined(BSP_USING_GPIOD) */
#endif /* defined(BSP_USING_GPIOC) */
#endif /* defined(BSP_USING_GPIOB) */
#endif /* defined(BSP_USING_GPIOA) */
};

static const struct pin_irq_map pin_irq_map[] =
{
    {0, NULL},
};

static struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {0, NULL},
};

static uint32_t pin_irq_enable_mask = 0;

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])

static const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = RT_NULL;
    }
    else
    {
        index = RT_NULL;
    }

    return index;
};

static void ht_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    if(pin < GPIOA_PIN_NUM)
    {
        GPIOA_WriteBit(index->gpio, index->pin, value);
    }
    else
    {
        GPIOBToF_WriteBit(index->gpio, index->pin, value);
    }
}

static int ht_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return value;
    }

    if(pin < GPIOA_PIN_NUM)
    {
        value = GPIOA_ReadInputDataBit(index->gpio, index->pin);
    }
    else
    {
        value = GPIOBToF_ReadInputDataBit(index->gpio, index->pin);
    }

    return value;
}

static void ht_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        if(pin < GPIOA_PIN_NUM)
        {
            GPIOA_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_OUTPUT_CMOS});
        }
        else
        {
            GPIOBToF_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_OUTPUT_CMOS});
        }
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        if(pin < GPIOA_PIN_NUM)
        {
            GPIOA_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
        else
        {
            GPIOBToF_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        if(pin < GPIOA_PIN_NUM)
        {
            GPIOA_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
        else
        {
            GPIOBToF_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        if(pin < GPIOA_PIN_NUM)
        {
            GPIOA_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
        else
        {
            GPIOBToF_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_INPUT});
        }
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        if(pin < GPIOA_PIN_NUM)
        {
            GPIOA_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_OUTPUT_OD});
        }
        else
        {
            GPIOBToF_Init(index->gpio, &(GPIO_InitType){index->pin, GPIO_Mode_OUTPUT_OD});
        }
    }
}

rt_inline rt_int32_t bit2bitno(rt_uint32_t bit)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if ((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}

rt_inline const struct pin_irq_map *get_pin_irq_map(uint32_t pinbit)
{
    rt_int32_t mapindex = bit2bitno(pinbit);
    if (mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_NULL;
    }
    return &pin_irq_map[mapindex];
};

static rt_err_t ht_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                                     rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    return RT_EOK;
}

static rt_err_t ht_pin_dettach_irq(struct rt_device *device, rt_int32_t pin)
{
    return RT_EOK;
}

static rt_err_t ht_pin_irq_enable(struct rt_device *device, rt_base_t pin,
                                     rt_uint32_t enabled)
{
    return RT_EOK;
}
const static struct rt_pin_ops _ht_pin_ops =
{
    ht_pin_mode,
    ht_pin_write,
    ht_pin_read,
    ht_pin_attach_irq,
    ht_pin_dettach_irq,
    ht_pin_irq_enable,
};

rt_inline void pin_irq_hdr(int irqno)
{
    if (pin_irq_hdr_tab[irqno].hdr)
    {
        pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    pin_irq_hdr(bit2bitno(GPIO_Pin));
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    pin_irq_hdr(bit2bitno(GPIO_Pin));
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    pin_irq_hdr(bit2bitno(GPIO_Pin));
}

void EXTI0_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI1_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI2_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI3_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI4_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI5_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI6_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI7_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI8_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

void EXTI9_IRQHandler(void)
{
    rt_interrupt_enter();

    rt_interrupt_leave();
}

int rt_hw_pin_init(void)
{
    /* 无关引脚开漏输出低 */
    GPIOA_ResetBits(GPIOA, GPIO_Pin_All);
	GPIOBToF_ResetBits(GPIOB, GPIO_Pin_All);
	GPIOBToF_ResetBits(GPIOC,  GPIO_Pin_All);
	GPIOBToF_ResetBits(GPIOD, GPIO_Pin_All);
	GPIOBToF_ResetBits(GPIOE, GPIO_Pin_All);
	GPIOBToF_ResetBits(GPIOF, GPIO_Pin_All);
	GPIOA_Init(GPIOA, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});
	GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});
	GPIOBToF_Init(GPIOC, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});
	GPIOBToF_Init(GPIOD, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});
	GPIOBToF_Init(GPIOE, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});
	GPIOBToF_Init(GPIOF, &(GPIO_InitType){GPIO_Pin_All, GPIO_Mode_OUTPUT_OD});

    return rt_device_pin_register("pin", &_ht_pin_ops, RT_NULL);
}
//INIT_BOARD_EXPORT(rt_hw_pin_init);

#endif /* RT_USING_PIN */
