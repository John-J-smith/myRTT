/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * usart device driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-05     John Shi     first version
 */

#include <board.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#ifdef BSP_USING_GPIO

static const struct pin_index pins[] =
{
#if defined(BSP_USING_GPIOA)
    __PIN(0 ,  A, 00),
    __PIN(1 ,  A, 01),
    __PIN(2 ,  A, 02),
    __PIN(3 ,  A, 03),
    __PIN(4 ,  A, 04),
    __PIN(5 ,  A, 05),
    __PIN(6 ,  A, 06),
    __PIN(7 ,  A, 07),
    __PIN(8 ,  A, 08),
    __PIN(9 ,  A, 09),
    __PIN(10,  A, 10),
    __PIN(11,  A, 11),
    __PIN(12,  A, 12),
    __PIN(13,  A, 13),
    __PIN(14,  A, 14),
    __PIN(15,  A, 15),
#if defined(BSP_USING_GPIOB)
    __PIN(16,  B, 00),
    __PIN(17,  B, 01),
    __PIN(18,  B, 02),
    __PIN(19,  B, 03),
    __PIN(20,  B, 04),
    __PIN(21,  B, 05),
    __PIN(22,  B, 06),
    __PIN(23,  B, 07),
    __PIN(24,  B, 08),
    __PIN(25,  B, 09),
    __PIN(26,  B, 10),
    __PIN(27,  B, 11),
    __PIN(28,  B, 12),
    __PIN(29,  B, 13),
    __PIN(30,  B, 14),
    __PIN(31,  B, 15),
#if defined(BSP_USING_GPIOC)
    __PIN(32,  C, 00),
    __PIN(33,  C, 01),
    __PIN(34,  C, 02),
    __PIN(35,  C, 03),
    __PIN(36,  C, 04),
    __PIN(37,  C, 05),
    __PIN(38,  C, 06),
    __PIN(39,  C, 07),
    __PIN(40,  C, 08),
    __PIN(41,  C, 09),
    __PIN(42,  C, 10),
    __PIN(43,  C, 11),
    __PIN(44,  C, 12),
    __PIN(45,  C, 13),
    __PIN(46,  C, 14),
    __PIN(47,  C, 15),
#if defined(BSP_USING_GPIOD)
    __PIN(48,  D, 00),
    __PIN(49,  D, 01),
    __PIN(50,  D, 02),
    __PIN(51,  D, 03),
    __PIN(52,  D, 04),
    __PIN(53,  D, 05),
    __PIN(54,  D, 06),
    __PIN(55,  D, 07),
    __PIN(56,  D, 08),
    __PIN(57,  D, 09),
    __PIN(58,  D, 10),
    __PIN(59,  D, 11),
    __PIN(60,  D, 12),
    __PIN(61,  D, 13),
    __PIN(62,  D, 14),
    __PIN(63,  D, 15),
#if defined(BSP_USING_GPIOE)
    __PIN(64,  E, 00),
    __PIN(65,  E, 01),
    __PIN(66,  E, 02),
    __PIN(67,  E, 03),
    __PIN(68,  E, 04),
    __PIN(69,  E, 05),
    __PIN(70,  E, 06),
    __PIN(71,  E, 07),
    __PIN(72,  E, 08),
    __PIN(73,  E, 09),
    __PIN(74,  E, 10),
    __PIN(75,  E, 11),
    __PIN(76,  E, 12),
    __PIN(77,  E, 13),
    __PIN(78,  E, 14),
    __PIN(79,  E, 15),
#if defined(BSP_USING_GPIOH)
    __PIN(80,  H, 00),
    __PIN(81,  H, 01),
    __PIN(82,  H, 02),
    __PIN(83,  H, 03),
    __PIN(84,  H, 04),
    __PIN(85,  H, 05),
    __PIN(86,  H, 06),
    __PIN(87,  H, 07),
    __PIN(88,  H, 08),
    __PIN(89,  H, 09),
    __PIN(90,  H, 10),
    __PIN(91,  H, 11),
    __PIN(92,  H, 12),
    __PIN(93,  H, 13),
    __PIN(94,  H, 14),
    __PIN(95,  H, 15),
#endif /* defined(BSP_USING_GPIOH) */
#endif /* defined(BSP_USING_GPIOE) */
#endif /* defined(BSP_USING_GPIOD) */
#endif /* defined(BSP_USING_GPIOC) */
#endif /* defined(BSP_USING_GPIOB) */
#endif /* defined(BSP_USING_GPIOA) */
};

static const struct pin_irq_map pin_irq_map[] =
{
    {0, 0},
};

static struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {0, 0, NULL, NULL},
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

    if(value > 0)
    {
        PORT_SetBits(index->gpio, index->pin);
    }
    else
    {
        PORT_ResetBits(index->gpio, index->pin);
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

    value = PORT_GetBit(index->gpio, index->pin);

    return value;
}

static void ht_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        stcPortInit.enPinMode = Pin_Mode_Out;
        stcPortInit.enPullUp = Enable;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        stcPortInit.enPinMode = Pin_Mode_In;
        stcPortInit.enPullUp = Disable;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        stcPortInit.enPinMode = Pin_Mode_In;
        stcPortInit.enPullUp = Enable;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        stcPortInit.enPinMode = Pin_Mode_In;
        stcPortInit.enPullUp = Enable;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        stcPortInit.enPinMode = Pin_Mode_Out;
        stcPortInit.enPullUp = Enable;
        stcPortInit.enPinOType = Pin_OType_Od;
    }
    PORT_SetFunc(index->gpio, index->pin, Func_Gpio, Disable);
    PORT_Init(index->gpio, index->pin, &stcPortInit);
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
    PORT_DeInit();

    return rt_device_pin_register("pin", &_ht_pin_ops, RT_NULL);
}
//INIT_BOARD_EXPORT(rt_hw_pin_init);

#endif /* RT_USING_PIN */
