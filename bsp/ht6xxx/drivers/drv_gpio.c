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

static const struct pin_index pins[] =
{
#if defined(GPIOA)
    __HT_PIN(0 ,  A, 0 ),
    __HT_PIN(1 ,  A, 1 ),
    __HT_PIN(2 ,  A, 2 ),
    __HT_PIN(3 ,  A, 3 ),
    __HT_PIN(4 ,  A, 4 ),
    __HT_PIN(5 ,  A, 5 ),
    __HT_PIN(6 ,  A, 6 ),
    __HT_PIN(7 ,  A, 7 ),
    __HT_PIN(8 ,  A, 8 ),
    __HT_PIN(9 ,  A, 9 ),
    __HT_PIN(10,  A, 10),
    __HT_PIN(11,  A, 11),
    __HT_PIN(12,  A, 12),
    __HT_PIN(13,  A, 13),
    __HT_PIN(14,  A, 14),
    __HT_PIN(15,  A, 15),
#if defined(GPIOB)
    __HT_PIN(16,  B, 0),
    __HT_PIN(17,  B, 1),
    __HT_PIN(18,  B, 2),
    __HT_PIN(19,  B, 3),
    __HT_PIN(20,  B, 4),
    __HT_PIN(21,  B, 5),
    __HT_PIN(22,  B, 6),
    __HT_PIN(23,  B, 7),
    __HT_PIN(24,  B, 8),
    __HT_PIN(25,  B, 9),
    __HT_PIN(26,  B, 10),
    __HT_PIN(27,  B, 11),
    __HT_PIN(28,  B, 12),
    __HT_PIN(29,  B, 13),
    __HT_PIN(30,  B, 14),
    __HT_PIN(31,  B, 15),
#if defined(GPIOC)
    __HT_PIN(32,  C, 0),
    __HT_PIN(33,  C, 1),
    __HT_PIN(34,  C, 2),
    __HT_PIN(35,  C, 3),
    __HT_PIN(36,  C, 4),
    __HT_PIN(37,  C, 5),
    __HT_PIN(38,  C, 6),
    __HT_PIN(39,  C, 7),
    __HT_PIN(40,  C, 8),
    __HT_PIN(41,  C, 9),
    __HT_PIN(42,  C, 10),
    __HT_PIN(43,  C, 11),
    __HT_PIN(44,  C, 12),
    __HT_PIN(45,  C, 13),
    __HT_PIN(46,  C, 14),
    __HT_PIN(47,  C, 15),
#if defined(GPIOD)
    __HT_PIN(48,  D, 0),
    __HT_PIN(49,  D, 1),
    __HT_PIN(50,  D, 2),
    __HT_PIN(51,  D, 3),
    __HT_PIN(52,  D, 4),
    __HT_PIN(53,  D, 5),
    __HT_PIN(54,  D, 6),
    __HT_PIN(55,  D, 7),
    __HT_PIN(56,  D, 8),
    __HT_PIN(57,  D, 9),
    __HT_PIN(58,  D, 10),
    __HT_PIN(59,  D, 11),
    __HT_PIN(60,  D, 12),
    __HT_PIN(61,  D, 13),
    __HT_PIN(62,  D, 14),
    __HT_PIN(63,  D, 15),
#if defined(GPIOE)
    __HT_PIN(64,  E, 0),
    __HT_PIN(65,  E, 1),
    __HT_PIN(66,  E, 2),
    __HT_PIN(67,  E, 3),
    __HT_PIN(68,  E, 4),
    __HT_PIN(69,  E, 5),
    __HT_PIN(70,  E, 6),
    __HT_PIN(71,  E, 7),
    __HT_PIN(72,  E, 8),
    __HT_PIN(73,  E, 9),
    __HT_PIN(74,  E, 10),
    __HT_PIN(75,  E, 11),
    __HT_PIN(76,  E, 12),
    __HT_PIN(77,  E, 13),
    __HT_PIN(78,  E, 14),
    __HT_PIN(79,  E, 15),
#if defined(GPIOG)
    __HT_PIN(80,  G, 0),
    __HT_PIN(81,  G, 1),
    __HT_PIN(82,  G, 2),
    __HT_PIN(83,  G, 3),
    __HT_PIN(84,  G, 4),
    __HT_PIN(85,  G, 5),
    __HT_PIN(86,  G, 6),
    __HT_PIN(87,  G, 7),
    __HT_PIN(88,  G, 8),
    __HT_PIN(89,  G, 9),
    __HT_PIN(90,  G, 10),
    __HT_PIN(91,  G, 11),
    __HT_PIN(92,  G, 12),
    __HT_PIN(93,  G, 13),
    __HT_PIN(94,  G, 14),
    __HT_PIN(95,  G, 15),
#if defined(GPIOH)
    __HT_PIN(96,  H, 0),
    __HT_PIN(97,  H, 1),
    __HT_PIN(98,  H, 2),
    __HT_PIN(99,  H, 3),
    __HT_PIN(100, H, 4),
    __HT_PIN(101, H, 5),
    __HT_PIN(102, H, 6),
    __HT_PIN(103, H, 7),
    __HT_PIN(104, H, 8),
    __HT_PIN(105, H, 9),
    __HT_PIN(106, H, 10),
    __HT_PIN(107, H, 11),
    __HT_PIN(108, H, 12),
    __HT_PIN(109, H, 13),
    __HT_PIN(110, H, 14),
    __HT_PIN(111, H, 15),
#if defined(GPIOI)
    __HT_PIN(112, I, 0),
    __HT_PIN(113, I, 1),
    __HT_PIN(114, I, 2),
    __HT_PIN(115, I, 3),
    __HT_PIN(116, I, 4),
    __HT_PIN(117, I, 5),
    __HT_PIN(118, I, 6),
    __HT_PIN(119, I, 7),
    __HT_PIN(120, I, 8),
    __HT_PIN(121, I, 9),
    __HT_PIN(122, I, 10),
    __HT_PIN(123, I, 11),
    __HT_PIN(124, I, 12),
    __HT_PIN(125, I, 13),
    __HT_PIN(126, I, 14),
    __HT_PIN(127, I, 15),
#endif /* defined(GPIOI) */
#endif /* defined(GPIOH) */
#endif /* defined(GPIOG) */
#endif /* defined(GPIOE) */
#endif /* defined(GPIOD) */
#endif /* defined(GPIOC) */
#endif /* defined(GPIOB) */
#endif /* defined(GPIOA) */
};

static const struct pin_irq_map pin_irq_map[] =
{
    {Pin_0, EXTI0_IRQn},
    {Pin_1, EXTI1_IRQn},
    {Pin_2, EXTI2_IRQn},
    {Pin_3, EXTI3_IRQn},
    {Pin_4, EXTI4_IRQn},
    {Pin_5, EXTI5_IRQn},
    {Pin_6, EXTI6_IRQn},
    {Pin_7, EXTI7_IRQn},
    {Pin_8, EXTI8_IRQn},
    {Pin_9, EXTI9_IRQn},
};

static struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
};
static uint32_t pin_irq_enable_mask=0;

uint8_t HD_System_GPIO_Read(HT_GPIO_TypeDef* GPIOx,uint32_t GPIO_PIN)
{
    if(GPIOx->PTDAT & (1 << GPIO_PIN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t HD_System_GPIO_DIR_Read(HT_GPIO_TypeDef *GPIOx, uint32_t GPIO_PIN)
{
    if (GPIOx->PTDIR & (1 << GPIO_PIN))
    {
        return OUTPUT;
    }
    else
    {
        return INPUT;
    }
}

void HD_System_GPIO_DIR( HT_GPIO_TypeDef* GPIOx,uint32_t GPIO_PIN,uint8_t PuPd,uint8_t DIR)
{
    if(DIR == OUTPUT)
    {
        GPIOx->PTDIR |= (0x0001<<GPIO_PIN);
    }
    else
    {
        GPIOx->PTDIR &= ~(0x0001<<GPIO_PIN);
    }

    if(PuPd == PUPDR_UP)
    {
        GPIOx->PTUP &= ~(0x0001<<GPIO_PIN);
    }
    else
    {
        GPIOx->PTUP |= (0x0001<<GPIO_PIN);
    }

    // if(PuPd == PUPDR_OD)            /*!< 判断是否使能开漏输出功能  */
    // {
    //     GPIOx->PTOD &= ~(0x0001<<GPIO_PIN);         /*!< 配置开漏输出              */
    // }
    // else
    // {
    //     GPIOx->PTOD |= (0x0001<<GPIO_PIN);            /*!< 配置推挽输出              */
    // }
}

void HD_System_GPIO_AF(HT_GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t ucGPIOAF, uint8_t PuPd)
{
    if(PuPd == PUPDR_UP)
    {
        GPIOx->PTUP &= ~(0x0001<<GPIO_PIN);
    }
    else
    {
        GPIOx->PTUP |= (0x0001<<GPIO_PIN);
    }

    GPIOx->IOCFG |= (0x0001<<GPIO_PIN);

    if(ucGPIOAF == 0x02)
    {
        GPIOx->AFCFG &= ~(0x0001<<GPIO_PIN);
    }
    else
    {
        GPIOx->AFCFG |= (0x0001<<GPIO_PIN);
    }
}

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

    HD_System_GPIO_DATA(index->gpio, index->pin, value);
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

    value = HD_System_GPIO_Read(index->gpio, index->pin);

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
        HD_System_GPIO_DIR(index->gpio, index->pin, PUPDR_UP, OUTPUT);
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        HD_System_GPIO_DIR(index->gpio, index->pin, PUPDR_UP, INPUT);
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        HD_System_GPIO_DIR(index->gpio, index->pin, PUPDR_UP, INPUT);
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        HD_System_GPIO_DIR(index->gpio, index->pin, PUPDR_UP, INPUT);
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        HD_System_GPIO_DIR(index->gpio, index->pin, PUPDR_OD, OUTPUT);
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
    return rt_device_pin_register("pin", &_ht_pin_ops, RT_NULL);
}
INIT_BOARD_EXPORT(rt_hw_pin_init);

#endif /* RT_USING_PIN */
