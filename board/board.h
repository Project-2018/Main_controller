/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32F769I-Discovery board.
 */

/* 
 * 1 = STM32F4 Discovery board
 * 0 = Vanlift hardware 
 */
#if 0

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F4_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics STM32F4-Discovery"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

#else

/*
 * Board identifier.
 */
#define BOARD_MAIN_CONTROLLER
#define BOARD_NAME                  "Main controller"


/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                25000000U
#endif

#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0U
#define GPIOA_BTN_UP                1U
#define GPIOA_DISP_TX               2U
#define GPIOA_DISP_RX               3U
#define GPIOA_BTN_DOWN              4U
#define GPIOA_SPI1_SCK              5U
#define GPIOA_SPI1_MISO             6U
#define GPIOA_SPI1_MOSI             7U
#define GPIOA_I2C_C                 8U
#define GPIOA_ESC_TX                9U
#define GPIOA_ESC_RX                10U
#define GPIOA_USB_DM                11U
#define GPIOA_USB_DP                12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWDCLK                14U
#define GPIOA_PIN15                 15U

#define GPIOB_LIFTING_SPEED_UP      0U
#define GPIOB_LIFTING_SPEED_DOWN    1U
#define GPIOB_PIN2                  2U
#define GPIOB_PIN3                  3U
#define GPIOB_PIN4                  4U
#define GPIOB_PIN5                  5U
#define GPIOB_DISP_LED4             6U
#define GPIOB_DISP_LED3             7U
#define GPIOB_DISP_LED2             8U
#define GPIOB_DISP_LED1             9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_PIN14                 14U
#define GPIOB_PIN15                 15U

#define GPIOC_CHARGE_VOLT           0U
#define GPIOC_CHARGE_CURR           1U
#define GPIOC_BATT_NTC              2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_SOLENOID              5U
#define GPIOC_SPEAKER               6U
#define GPIOC_PIN7                  7U
#define GPIOC_PIN8                  8U
#define GPIOC_I2C_D                 9U
#define GPIOC_PIN10                 10U
#define GPIOC_PIN11                 11U
#define GPIOC_PIN12                 12U
#define GPIOC_PIN13                 13U
#define GPIOC_OSC32IN               14U
#define GPIOC_OSC32OUT              15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_PIN2                  2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_STAT_LED              15U

#define GPIOE_MEMS_INT1             0U
#define GPIOE_MEMS_INT2             1U
#define GPIOE_PIN2                  2U
#define GPIOE_SPI1_CS               3U
#define GPIOE_PIN4                  4U
#define GPIOE_UP_LED                5U
#define GPIOE_DOWN_LED              6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_DISP_BUTT4            9U
#define GPIOE_DISP_BUTT3            10U
#define GPIOE_DISP_BUTT2            11U
#define GPIOE_DISP_BUTT1            12U
#define GPIOE_SW3                   13U
#define GPIOE_SW2                   14U
#define GPIOE_SW1                   15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_BTN_UP                 PAL_LINE(GPIOA, 1U)
#define LINE_DISP_TX                PAL_LINE(GPIOA, 2U)
#define LINE_DISP_RX                PAL_LINE(GPIOA, 3U)
#define LINE_BTN_DOWN               PAL_LINE(GPIOA, 4U)
#define LINE_SPI1_SCK               PAL_LINE(GPIOA, 5U)
#define LINE_SPI1_MISO              PAL_LINE(GPIOA, 6U)
#define LINE_SPI1_MOSI              PAL_LINE(GPIOA, 7U)
#define LINE_I2C_C                  PAL_LINE(GPIOA, 8U)
#define LINE_ESC_TX                 PAL_LINE(GPIOA, 9U)
#define LINE_ESC_RX                 PAL_LINE(GPIOA, 10U)
#define LINE_USB_DM                 PAL_LINE(GPIOA, 11U)
#define LINE_USB_DP                 PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWDCLK                 PAL_LINE(GPIOA, 14U)

#define LINE_LIFTING_SPEED_UP       PAL_LINE(GPIOB, 0U)
#define LINE_LIFTING_SPEED_DOWN     PAL_LINE(GPIOB, 1U)
#define LINE_DISP_LED4              PAL_LINE(GPIOB, 6U)
#define LINE_DISP_LED3              PAL_LINE(GPIOB, 7U)
#define LINE_DISP_LED2              PAL_LINE(GPIOB, 8U)
#define LINE_DISP_LED1              PAL_LINE(GPIOB, 9U)

#define LINE_CHARGE_VOLT            PAL_LINE(GPIOC, 0U)
#define LINE_CHARGE_CURR            PAL_LINE(GPIOC, 1U)
#define LINE_BATT_NTC               PAL_LINE(GPIOC, 2U)
#define LINE_SOLENOID               PAL_LINE(GPIOC, 5U)
#define LINE_SPEAKER                PAL_LINE(GPIOC, 6U)
#define LINE_I2C_D                  PAL_LINE(GPIOC, 9U)
#define LINE_OSC32IN                PAL_LINE(GPIOC, 14U)
#define LINE_OSC32OUT               PAL_LINE(GPIOC, 15U)

#define LINE_STAT_LED               PAL_LINE(GPIOD, 15U)

#define LINE_MEMS_INT1              PAL_LINE(GPIOE, 0U)
#define LINE_MEMS_INT2              PAL_LINE(GPIOE, 1U)
#define LINE_SPI1_CS                PAL_LINE(GPIOE, 3U)
#define LINE_UP_LED                 PAL_LINE(GPIOE, 5U)
#define LINE_DOWN_LED               PAL_LINE(GPIOE, 6U)
#define LINE_DISP_BUTT4             PAL_LINE(GPIOE, 9U)
#define LINE_DISP_BUTT3             PAL_LINE(GPIOE, 10U)
#define LINE_DISP_BUTT2             PAL_LINE(GPIOE, 11U)
#define LINE_DISP_BUTT1             PAL_LINE(GPIOE, 12U)
#define LINE_SW3                    PAL_LINE(GPIOE, 13U)
#define LINE_SW2                    PAL_LINE(GPIOE, 14U)
#define LINE_SW1                    PAL_LINE(GPIOE, 15U)

#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 * All input with pull-up except:
 * PA1  - BTN_UP                    (analog) - (input)
 * PA2  - DISP_TX                   (alternate 7)
 * PA3  - DISP_RX                   (alternate 7)
 * PA4  - BTN_DOWN                  (analog) - (input)
 * PA5  - SPI1_SCK                  (alternate 5)
 * PA6  - SPI1_MISO                 (alternate 5)
 * PA7  - SPI1_MOSI                 (alternate 5)
 * PA8  - I2C_C                     (alternate 4)
 * PA9  - ESC_TX                    (alternate 7)
 * PA10 - ESC_RX                    (alternate 7)
 * PA11 - USB_DM                    (alternate 10)
 * PA12 - USB_DP                    (alternate 10)
 * PA13 - SWDIO                     (alternate 0)
 * PA14 - SWDCLK                    (alternate 0)
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BTN_UP)           | \
                                     PIN_MODE_ALTERNATE(GPIOA_DISP_TX)      | \
                                     PIN_MODE_ALTERNATE(GPIOA_DISP_RX)      | \
                                     PIN_MODE_INPUT(GPIOA_BTN_DOWN)         | \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_SCK)     | \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_MISO)    | \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_MOSI)    | \
                                     PIN_MODE_ALTERNATE(GPIOA_I2C_C)        | \
                                     PIN_MODE_ALTERNATE(GPIOA_ESC_TX)       | \
                                     PIN_MODE_ALTERNATE(GPIOA_ESC_RX)       | \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DM)       | \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DP)       | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO)        | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDCLK))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_SWDIO)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDCLK)       | \
                                     PIN_OTYPE_OPENDRAIN(GPIOA_I2C_C))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_SWDIO)           | \
                                     PIN_OSPEED_HIGH(GPIOA_SWDCLK))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(GPIOA_BTN_UP)         | \
                                     PIN_PUPDR_PULLUP(GPIOA_BTN_DOWN)       | \
                                     PIN_PUPDR_PULLUP(GPIOA_ESC_TX)         | \
                                     PIN_PUPDR_PULLUP(GPIOA_ESC_RX)       | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO)        | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDCLK))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_SWDIO)              | \
                                     PIN_ODR_HIGH(GPIOA_SWDCLK))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_DISP_TX,   7)        | \
                                     PIN_AFIO_AF(GPIOA_DISP_RX,   7)        | \
                                     PIN_AFIO_AF(GPIOA_SPI1_SCK,  5)        | \
                                     PIN_AFIO_AF(GPIOA_SPI1_MISO, 5)        | \
                                     PIN_AFIO_AF(GPIOA_SPI1_MOSI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_I2C_C,     4)        | \
                                     PIN_AFIO_AF(GPIOA_ESC_TX,    7)        | \
                                     PIN_AFIO_AF(GPIOA_ESC_RX,    7)        | \
                                     PIN_AFIO_AF(GPIOA_USB_DM,    10)       | \
                                     PIN_AFIO_AF(GPIOA_USB_DP,    10)       | \
                                     PIN_AFIO_AF(GPIOA_SWDIO,     0U)       | \
                                     PIN_AFIO_AF(GPIOA_SWDCLK,    0U))

/*
 * GPIOB setup:
 * All input with pull-up except:
 * PB0  - DOWN_1                    (analog)
 * PB1  - DOWN_2                    (analog)
 * PB6  - DISP_LED4                 (alternate 2)
 * PB7  - DISP_LED3                 (alternate 2)
 * PB8  - DISP_LED2                 (alternate 2)
 * PB9  - DISP_LED1                 (alternate 2)
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_LIFTING_SPEED_UP)   | \
                                     PIN_MODE_ANALOG(GPIOB_LIFTING_SPEED_DOWN) | \
                                     PIN_MODE_ALTERNATE(GPIOB_DISP_LED4)       | \
                                     PIN_MODE_ALTERNATE(GPIOB_DISP_LED3)       | \
                                     PIN_MODE_ALTERNATE(GPIOB_DISP_LED2)       | \
                                     PIN_MODE_ALTERNATE(GPIOB_DISP_LED1))
#define VAL_GPIOB_OTYPER            0x00000000
#define VAL_GPIOB_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOB_PUPDR             0xFFFFFFFF
#define VAL_GPIOB_ODR               0xFFFFFFFF
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_DISP_LED4, 2)        | \
                                     PIN_AFIO_AF(GPIOB_DISP_LED3, 2))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_DISP_LED2, 2)        | \
                                     PIN_AFIO_AF(GPIOB_DISP_LED1, 2))

/*
 * GPIOC setup:
 * All input with pull-up except:
 * PC0  - CHARGE_VOLT               (analog)
 * PC1  - CHARGE_CURR               (analog)
 * PC2  - BATT_NTC                  (analog)
 * PC6  - SPEAKER                   (alternate 3)
 * PC9  - I2C_D                     (alternate 4)
 * PC14 - OSC32IN                   (input floating)
 * PC15 - OSC32OUT                  (input floating)
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_CHARGE_VOLT)     | \
                                     PIN_MODE_ANALOG(GPIOC_CHARGE_CURR)     | \
                                     PIN_MODE_ANALOG(GPIOC_BATT_NTC)        | \
                                     PIN_MODE_ALTERNATE(GPIOC_SPEAKER)      | \
                                     PIN_MODE_ALTERNATE(GPIOC_I2C_D)        | \
                                     PIN_MODE_INPUT(GPIOC_OSC32IN)          | \
                                     PIN_MODE_OUTPUT(GPIOC_SOLENOID)        | \
                                     PIN_MODE_INPUT(GPIOC_OSC32OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_OPENDRAIN(GPIOC_I2C_D)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SOLENOID))
#define VAL_GPIOC_OSPEEDR           PIN_OSPEED_HIGH(GPIOC_SOLENOID)
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_OSC32IN)      | \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32OUT))
#define VAL_GPIOC_ODR               0xFFFFFFFF
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_SPEAKER, 3))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_I2C_D,   4))

/*
 * GPIOD setup:
 * All input with pull-up except:
 * PD15 - STAT_LED                  (output)
 */
#define VAL_GPIOD_MODER             (PIN_MODE_OUTPUT(GPIOD_STAT_LED))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_STAT_LED))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_STAT_LED))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_STAT_LED))
#define VAL_GPIOD_ODR               (PIN_ODR_LOW(GPIOD_STAT_LED))
#define VAL_GPIOD_AFRL              0x00000000
#define VAL_GPIOD_AFRH              0x00000000

/*
 * GPIOE setup:
 * All input with pull-up except:
 * PE0  - MEMS_INT1                 (input)
 * PE1  - MEMS_INT2                 (input)
 * PE3  - SPI1_CS                   (output)
 * PE5  - UP_LED                    (alternate 3)
 * PE6  - DOWN_LED                  (alternate 3)
 * PE9  - DISP_BUTT4                (input)
 * PE10 - DISP_BUTT3                (input)
 * PE11 - DISP_BUTT2                (input)
 * PE12 - DISP_BUTT1                (input)
 * PE13 - SW3                       (input)
 * PE14 - SW2                       (input)
 * PE15 - SW1                       (input)
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_MEMS_INT1)        | \
                                     PIN_MODE_INPUT(GPIOE_MEMS_INT2)        | \
                                     PIN_MODE_OUTPUT(GPIOE_SPI1_CS)         | \
                                     PIN_MODE_ALTERNATE(GPIOE_UP_LED)       | \
                                     PIN_MODE_ALTERNATE(GPIOE_DOWN_LED)     | \
                                     PIN_MODE_INPUT(GPIOE_DISP_BUTT4)       | \
                                     PIN_MODE_INPUT(GPIOE_DISP_BUTT3)       | \
                                     PIN_MODE_INPUT(GPIOE_DISP_BUTT2)       | \
                                     PIN_MODE_INPUT(GPIOE_DISP_BUTT1)       | \
                                     PIN_MODE_INPUT(GPIOE_SW3)              | \
                                     PIN_MODE_INPUT(GPIOE_SW2)              | \
                                     PIN_MODE_INPUT(GPIOE_SW1))
#define VAL_GPIOE_OTYPER            0x00000000
#define VAL_GPIOE_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_MEMS_INT1)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_MEMS_INT2)      | \
                                     PIN_PUPDR_PULLDOWN(GPIOE_UP_LED)       | \
                                     PIN_PUPDR_PULLDOWN(GPIOE_DOWN_LED)     | \
                                     PIN_PUPDR_PULLUP(GPIOE_DISP_BUTT4)     | \
                                     PIN_PUPDR_PULLUP(GPIOE_DISP_BUTT3)     | \
                                     PIN_PUPDR_PULLUP(GPIOE_DISP_BUTT2)     | \
                                     PIN_PUPDR_PULLUP(GPIOE_DISP_BUTT1)     | \
                                     PIN_PUPDR_PULLUP(GPIOE_SW3)            | \
                                     PIN_PUPDR_PULLUP(GPIOE_SW2)            | \
                                     PIN_PUPDR_PULLUP(GPIOE_SW1))
#define VAL_GPIOE_ODR               0xFFFFFFFF
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_UP_LED,   3)         | \
                                     PIN_AFIO_AF(GPIOE_DOWN_LED, 3))
#define VAL_GPIOE_AFRH              0x00000000

/*
 * GPIOF setup:
 * All input with pull-up except:
 */
#define VAL_GPIOF_MODER             0x00000000
#define VAL_GPIOF_OTYPER            0x00000000
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             0xFFFFFFFF
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL              0x00000000
#define VAL_GPIOF_AFRH              0x00000000

/*
 * GPIOG setup:
 * All input with pull-up except:
 */
#define VAL_GPIOG_MODER             0x00000000
#define VAL_GPIOG_OTYPER            0x00000000
#define VAL_GPIOG_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOG_PUPDR             0xFFFFFFFF
#define VAL_GPIOG_ODR               0xFFFFFFFF
#define VAL_GPIOG_AFRL              0x00000000
#define VAL_GPIOG_AFRH              0x00000000

/*
 * Port H setup.
 * All input with pull-up except:
 * PH0 - OSC_IN                     (input floating).
 * PH1 - OSC_OUT                    (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN)           | \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN)          | \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN)       | \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN)             | \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT))
#define VAL_GPIOH_AFRL              0x00000000
#define VAL_GPIOH_AFRH              0x00000000

/*
 * GPIOI setup:
 * All input with pull-up except:
 */
#define VAL_GPIOI_MODER             0x00000000
#define VAL_GPIOI_OTYPER            0x00000000
#define VAL_GPIOI_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOI_PUPDR             0xFFFFFFFF
#define VAL_GPIOI_ODR               0xFFFFFFFF
#define VAL_GPIOI_AFRL              0x00000000
#define VAL_GPIOI_AFRH              0x00000000

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */