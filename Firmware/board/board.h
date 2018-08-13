/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for LAC Rev. A board.
 */

/*
 * Board identifier.
 */
#define BOARD_LAC_REV_A
#define BOARD_NAME                  "LAC Rev. A"

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                MII_TLK111_ID
#define BOARD_PHY_RMII

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                12000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * IO pins assignments.
 */
#define GPIOA_CUR_X_B               0U
#define GPIOA_ETH_RMII_REF_CLK      1U
#define GPIOA_ETH_RMII_MDIO         2U
#define GPIOA_CUR_X_A               3U
#define GPIOA_PIN4                  4U
#define GPIOA_PWM_X_A_L             5U
#define GPIOA_CUR_Y_B               6U
#define GPIOA_ETH_RMII_CRS_DV       7U
#define GPIOA_RS422_SLEW            8U
#define GPIOA_STAT1                 9U
#define GPIOA_RS422_DE              10U
#define GPIOA_PG                    11U
#define GPIOA_RS422_RE              12U
#define GPIOA_JTAG_TMS              13U
#define GPIOA_JTAG_TCK              14U
#define GPIOA_JTAG_TDI              15U

#define GPIOB_CUR_Y_B               0U
#define GPIOB_CUR_Y_A               1U
#define GPIOB_LAY_SWH               2U
#define GPIOB_JTAG_TDO              3U
#define GPIOB_PIN4                  4U
#define GPIOB_PIN5                  5U
#define GPIOB_USART1_TX             6U
#define GPIOB_USART1_RX             7U
#define GPIOB_PIN8                  8U
#define GPIOB_AUX_A1                9U
#define GPIOB_WAKE_Y                10U
#define GPIOB_RMII_TX_EN            11U
#define GPIOB_RMII_TXD_0            12U
#define GPIOB_RMII_TXD_1            13U
#define GPIOB_PWM_X_B_L             14U
#define GPIOB_PWM_X_C_L             15U

#define GPIOC_AUX_ADC               0U
#define GPIOC_ETH_RMII_MDC          1U
#define GPIOC_LAX_FB                2U
#define GPIOC_CUR_X_C               3U
#define GPIOC_ETH_RMII_RXD0         4U
#define GPIOC_ETH_RMII_RXD1         5U
#define GPIOC_PWM_X_A_H             6U
#define GPIOC_PWM_X_B_H             7U
#define GPIOC_PWM_X_C_H             8U
#define GPIOC_STAT2                 9U
#define GPIOC_SPI3_SCK              10U
#define GPIOC_SPI3_MISO             11U
#define GPIOC_SPI3_MOSI             12U
#define GPIOC_LED1                  13U
#define GPIOC_PHY_PWDWN             14U
#define GPIOC_PHY_RESET             15U

#define GPIOD_FLASH_CS              0U
#define GPIOD_DRV_CS_X              1U
#define GPIOD_EN_GATE_X             2U
#define GPIOD_WAKE_X                3U
#define GPIOD_HALL_X_2              4U
#define GPIOD_HALL_X_3              5U
#define GPIOD_LAX_SWH               6U
#define GPIOD_LAX_SWL               7U
#define GPIOD_USART3_TX             8U
#define GPIOD_USART_RX              9U
#define GPIOD_HALL_Y_2              10U
#define GPIOD_HALL_Y_1              11U
#define GPIOD_HALL_Y_3              12U
#define GPIOD_LAY_SWL               13U
#define GPIOD_HALL_X_1              14U
#define GPIOD_FAULT_X               15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_AUX_A2                2U
#define GPIOE_PIN3                  3U
#define GPIOE_AUX_A0                4U
#define GPIOE_LED2                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_FAULT_Y               7U
#define GPIOE_PWM_Y_A_H             8U
#define GPIOE_PWM_Y_A_L             9U
#define GPIOE_PWM_Y_B_H             10U
#define GPIOE_PWM_Y_B_L             11U
#define GPIOE_PWM_Y_C_L             12U
#define GPIOE_PWM_Y_C_H             13U
#define GPIOE_PIN14                 14U
#define GPIOE_DRV_CS_Y              15U

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
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2U))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2U))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - CUR_X_B                   (analog).
 * PA1  - ETH_RMII_REF_CLK          (alternate 11).
 * PA2  - ETH_RMII_MDIO             (alternate 11).
 * PA3  - CUR_X_A                   (analog).
 * PA4  - PIN4                      (alternate 0).
 * PA5  - PWM_X_A_L                 (alternate 3).
 * PA6  - CUR_Y_B                   (analog).
 * PA7  - ETH_RMII_CRS_DV           (alternate 11).
 * PA8  - RS422_SLEW                (output pushpull low).
 * PA9  - STAT1                     (input floating).
 * PA10 - RS422_DE                  (output pushpull low).
 * PA11 - PG                        (input floating).
 * PA12 - RS422_RE                  (output pushpull low).
 * PA13 - JTAG_TMS                  (alternate 0).
 * PA14 - JTAG_TCK                  (alternate 0).
 * PA15 - JTAG_TDI                  (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_CUR_X_B) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_MODE_ANALOG(GPIOA_CUR_X_A) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN4) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_PWM_X_A_L) |  \
                                     PIN_MODE_ANALOG(GPIOA_CUR_Y_B) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_MODE_OUTPUT(GPIOA_RS422_SLEW) |    \
                                     PIN_MODE_INPUT(GPIOA_STAT1) |          \
                                     PIN_MODE_OUTPUT(GPIOA_RS422_DE) |      \
                                     PIN_MODE_INPUT(GPIOA_PG) |             \
                                     PIN_MODE_OUTPUT(GPIOA_RS422_RE) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TMS) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TCK) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_CUR_X_B) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_CUR_X_A) |    \
                                     PIN_OTYPE_OPENDRAIN(GPIOA_PIN4) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PWM_X_A_L) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CUR_Y_B) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RS422_SLEW) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_STAT1) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RS422_DE) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PG) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RS422_RE) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TMS) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_100M(GPIOA_CUR_X_B) |       \
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_MDIO) | \
                                     PIN_OSPEED_100M(GPIOA_CUR_X_A) |       \
                                     PIN_OSPEED_100M(GPIOA_PIN4) |          \
                                     PIN_OSPEED_50M(GPIOA_PWM_X_A_L) |      \
                                     PIN_OSPEED_100M(GPIOA_CUR_Y_B) |       \
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_OSPEED_25M(GPIOA_RS422_SLEW) |     \
                                     PIN_OSPEED_100M(GPIOA_STAT1) |         \
                                     PIN_OSPEED_25M(GPIOA_RS422_DE) |       \
                                     PIN_OSPEED_100M(GPIOA_PG) |            \
                                     PIN_OSPEED_25M(GPIOA_RS422_RE) |       \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TMS) |      \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TCK) |      \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_CUR_X_B) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_PUPDR_FLOATING(GPIOA_CUR_X_A) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOA_PWM_X_A_L) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_CUR_Y_B) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_RS422_SLEW) | \
                                     PIN_PUPDR_FLOATING(GPIOA_STAT1) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOA_RS422_DE) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PG) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_RS422_RE) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_JTAG_TMS) |   \
                                     PIN_PUPDR_PULLDOWN(GPIOA_JTAG_TCK) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_JTAG_TDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_CUR_X_B) |          \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_REF_CLK) | \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_MDIO) |    \
                                     PIN_ODR_HIGH(GPIOA_CUR_X_A) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |             \
                                     PIN_ODR_LOW(GPIOA_PWM_X_A_L) |         \
                                     PIN_ODR_HIGH(GPIOA_CUR_Y_B) |          \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_CRS_DV) |  \
                                     PIN_ODR_LOW(GPIOA_RS422_SLEW) |        \
                                     PIN_ODR_HIGH(GPIOA_STAT1) |            \
                                     PIN_ODR_LOW(GPIOA_RS422_DE) |          \
                                     PIN_ODR_HIGH(GPIOA_PG) |               \
                                     PIN_ODR_HIGH(GPIOA_RS422_RE) |         \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TMS) |         \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TCK) |         \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_CUR_X_B, 0) |        \
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_REF_CLK, 11) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_MDIO, 11) | \
                                     PIN_AFIO_AF(GPIOA_CUR_X_A, 0) |        \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PWM_X_A_L, 3) |      \
                                     PIN_AFIO_AF(GPIOA_CUR_Y_B, 0) |        \
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_CRS_DV, 11))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_RS422_SLEW, 0) |     \
                                     PIN_AFIO_AF(GPIOA_STAT1, 0) |          \
                                     PIN_AFIO_AF(GPIOA_RS422_DE, 0) |       \
                                     PIN_AFIO_AF(GPIOA_PG, 0) |             \
                                     PIN_AFIO_AF(GPIOA_RS422_RE, 0) |       \
                                     PIN_AFIO_AF(GPIOA_JTAG_TMS, 0) |       \
                                     PIN_AFIO_AF(GPIOA_JTAG_TCK, 0) |       \
                                     PIN_AFIO_AF(GPIOA_JTAG_TDI, 0))

/*
 * GPIOB setup:
 *
 * PB0  - CUR_Y_B                   (analog).
 * PB1  - CUR_Y_A                   (analog).
 * PB2  - LAY_SWH                   (input floating).
 * PB3  - JTAG_TDO                  (alternate 0).
 * PB4  - PIN4                      (input pullup).
 * PB5  - PIN5                      (input pullup).
 * PB6  - USART1_TX                 (alternate 7).
 * PB7  - USART1_RX                 (alternate 7).
 * PB8  - PIN8                      (input pullup).
 * PB9  - AUX_A1                    (output pushpull low).
 * PB10 - WAKE_Y                    (output pushpull low).
 * PB11 - RMII_TX_EN                (alternate 11).
 * PB12 - RMII_TXD_0                (alternate 11).
 * PB13 - RMII_TXD_1                (alternate 11).
 * PB14 - PWM_X_B_L                 (alternate 3).
 * PB15 - PWM_X_C_L                 (alternate 3).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_CUR_Y_B) |       \
                                     PIN_MODE_ANALOG(GPIOB_CUR_Y_A) |       \
                                     PIN_MODE_INPUT(GPIOB_LAY_SWH) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_JTAG_TDO) |   \
                                     PIN_MODE_INPUT(GPIOB_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_USART1_TX) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_USART1_RX) |  \
                                     PIN_MODE_INPUT(GPIOB_PIN8) |           \
                                     PIN_MODE_OUTPUT(GPIOB_AUX_A1) |        \
                                     PIN_MODE_OUTPUT(GPIOB_WAKE_Y) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TX_EN) | \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TXD_0) | \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TXD_1) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PWM_X_B_L) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_PWM_X_C_L))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_CUR_Y_B) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_CUR_Y_A) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_LAY_SWH) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTAG_TDO) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART1_TX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART1_RX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_AUX_A1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_WAKE_Y) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TX_EN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TXD_0) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TXD_1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PWM_X_B_L) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PWM_X_C_L))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_100M(GPIOB_CUR_Y_B) |       \
                                     PIN_OSPEED_100M(GPIOB_CUR_Y_A) |       \
                                     PIN_OSPEED_100M(GPIOB_LAY_SWH) |       \
                                     PIN_OSPEED_100M(GPIOB_JTAG_TDO) |      \
                                     PIN_OSPEED_100M(GPIOB_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOB_PIN5) |          \
                                     PIN_OSPEED_50M(GPIOB_USART1_TX) |      \
                                     PIN_OSPEED_50M(GPIOB_USART1_RX) |      \
                                     PIN_OSPEED_100M(GPIOB_PIN8) |          \
                                     PIN_OSPEED_25M(GPIOB_AUX_A1) |         \
                                     PIN_OSPEED_25M(GPIOB_WAKE_Y) |         \
                                     PIN_OSPEED_100M(GPIOB_RMII_TX_EN) |    \
                                     PIN_OSPEED_100M(GPIOB_RMII_TXD_0) |    \
                                     PIN_OSPEED_100M(GPIOB_RMII_TXD_1) |    \
                                     PIN_OSPEED_50M(GPIOB_PWM_X_B_L) |      \
                                     PIN_OSPEED_50M(GPIOB_PWM_X_C_L))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_CUR_Y_B) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_CUR_Y_A) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_LAY_SWH) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_JTAG_TDO) |   \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_USART1_TX) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_USART1_RX) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN8) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_AUX_A1) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOB_WAKE_Y) |     \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TX_EN) | \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TXD_0) | \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TXD_1) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_PWM_X_B_L) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOB_PWM_X_C_L))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_CUR_Y_B) |           \
                                     PIN_ODR_HIGH(GPIOB_CUR_Y_A) |          \
                                     PIN_ODR_HIGH(GPIOB_LAY_SWH) |          \
                                     PIN_ODR_HIGH(GPIOB_JTAG_TDO) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOB_USART1_TX) |        \
                                     PIN_ODR_HIGH(GPIOB_USART1_RX) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN8) |             \
                                     PIN_ODR_LOW(GPIOB_AUX_A1) |            \
                                     PIN_ODR_HIGH(GPIOB_WAKE_Y) |           \
                                     PIN_ODR_HIGH(GPIOB_RMII_TX_EN) |       \
                                     PIN_ODR_HIGH(GPIOB_RMII_TXD_0) |       \
                                     PIN_ODR_HIGH(GPIOB_RMII_TXD_1) |       \
                                     PIN_ODR_LOW(GPIOB_PWM_X_B_L) |         \
                                     PIN_ODR_LOW(GPIOB_PWM_X_C_L))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_CUR_Y_B, 0) |        \
                                     PIN_AFIO_AF(GPIOB_CUR_Y_A, 0) |        \
                                     PIN_AFIO_AF(GPIOB_LAY_SWH, 0) |        \
                                     PIN_AFIO_AF(GPIOB_JTAG_TDO, 0) |       \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOB_USART1_TX, 7) |      \
                                     PIN_AFIO_AF(GPIOB_USART1_RX, 7))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOB_AUX_A1, 0) |         \
                                     PIN_AFIO_AF(GPIOB_WAKE_Y, 0) |         \
                                     PIN_AFIO_AF(GPIOB_RMII_TX_EN, 11) |    \
                                     PIN_AFIO_AF(GPIOB_RMII_TXD_0, 11) |    \
                                     PIN_AFIO_AF(GPIOB_RMII_TXD_1, 11) |    \
                                     PIN_AFIO_AF(GPIOB_PWM_X_B_L, 3) |      \
                                     PIN_AFIO_AF(GPIOB_PWM_X_C_L, 3))

/*
 * GPIOC setup:
 *
 * PC0  - AUX_ADC                   (analog).
 * PC1  - ETH_RMII_MDC              (alternate 11).
 * PC2  - LAX_FB                    (analog).
 * PC3  - CUR_X_C                   (analog).
 * PC4  - ETH_RMII_RXD0             (alternate 11).
 * PC5  - ETH_RMII_RXD1             (alternate 11).
 * PC6  - PWM_X_A_H                 (alternate 3).
 * PC7  - PWM_X_B_H                 (alternate 3).
 * PC8  - PWM_X_C_H                 (alternate 3).
 * PC9  - STAT2                     (input floating).
 * PC10 - SPI3_SCK                  (alternate 6).
 * PC11 - SPI3_MISO                 (alternate 6).
 * PC12 - SPI3_MOSI                 (alternate 6).
 * PC13 - LED1                      (output pushpull low).
 * PC14 - PHY_PWDWN                 (input pullup).
 * PC15 - PHY_RESET                 (output pushpull minimum).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_AUX_ADC) |       \
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_MDC) |\
                                     PIN_MODE_ANALOG(GPIOC_LAX_FB) |        \
                                     PIN_MODE_ANALOG(GPIOC_CUR_X_C) |       \
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_MODE_ALTERNATE(GPIOC_PWM_X_A_H) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_PWM_X_B_H) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_PWM_X_C_H) |  \
                                     PIN_MODE_INPUT(GPIOC_STAT2) |          \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_SCK) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_MISO) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_SPI3_MOSI) |  \
                                     PIN_MODE_OUTPUT(GPIOC_LED1) |          \
                                     PIN_MODE_INPUT(GPIOC_PHY_PWDWN) |      \
                                     PIN_MODE_OUTPUT(GPIOC_PHY_RESET))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_AUX_ADC) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_MDC) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LAX_FB) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_CUR_X_C) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PWM_X_A_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PWM_X_B_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PWM_X_C_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_STAT2) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_SCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PHY_PWDWN) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PHY_RESET))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_100M(GPIOC_AUX_ADC) |       \
                                     PIN_OSPEED_100M(GPIOC_ETH_RMII_MDC) |  \
                                     PIN_OSPEED_100M(GPIOC_LAX_FB) |        \
                                     PIN_OSPEED_100M(GPIOC_CUR_X_C) |       \
                                     PIN_OSPEED_100M(GPIOC_ETH_RMII_RXD0) | \
                                     PIN_OSPEED_100M(GPIOC_ETH_RMII_RXD1) | \
                                     PIN_OSPEED_50M(GPIOC_PWM_X_A_H) |      \
                                     PIN_OSPEED_50M(GPIOC_PWM_X_B_H) |      \
                                     PIN_OSPEED_50M(GPIOC_PWM_X_C_H) |      \
                                     PIN_OSPEED_100M(GPIOC_STAT2) |         \
                                     PIN_OSPEED_50M(GPIOC_SPI3_SCK) |       \
                                     PIN_OSPEED_50M(GPIOC_SPI3_MISO) |      \
                                     PIN_OSPEED_50M(GPIOC_SPI3_MOSI) |      \
                                     PIN_OSPEED_25M(GPIOC_LED1) |           \
                                     PIN_OSPEED_25M(GPIOC_PHY_PWDWN) |      \
                                     PIN_OSPEED_2M(GPIOC_PHY_RESET))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_AUX_ADC) |    \
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_MDC) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LAX_FB) |     \
                                     PIN_PUPDR_FLOATING(GPIOC_CUR_X_C) |    \
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_PUPDR_PULLDOWN(GPIOC_PWM_X_A_H) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOC_PWM_X_B_H) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOC_PWM_X_C_H) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_STAT2) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_SCK) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MISO) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MOSI) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_LED1) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_PHY_PWDWN) |    \
                                     PIN_PUPDR_PULLUP(GPIOC_PHY_RESET))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_AUX_ADC) |          \
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_MDC) |     \
                                     PIN_ODR_HIGH(GPIOC_LAX_FB) |           \
                                     PIN_ODR_HIGH(GPIOC_CUR_X_C) |          \
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_RXD0) |    \
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_RXD1) |    \
                                     PIN_ODR_LOW(GPIOC_PWM_X_A_H) |         \
                                     PIN_ODR_LOW(GPIOC_PWM_X_B_H) |         \
                                     PIN_ODR_LOW(GPIOC_PWM_X_C_H) |         \
                                     PIN_ODR_HIGH(GPIOC_STAT2) |            \
                                     PIN_ODR_HIGH(GPIOC_SPI3_SCK) |         \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MISO) |        \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOC_LED1) |             \
                                     PIN_ODR_HIGH(GPIOC_PHY_PWDWN) |        \
                                     PIN_ODR_HIGH(GPIOC_PHY_RESET))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_AUX_ADC, 0) |        \
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_MDC, 11) |  \
                                     PIN_AFIO_AF(GPIOC_LAX_FB, 0) |         \
                                     PIN_AFIO_AF(GPIOC_CUR_X_C, 9) |        \
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_RXD0, 11) | \
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_RXD1, 11) | \
                                     PIN_AFIO_AF(GPIOC_PWM_X_A_H, 3) |      \
                                     PIN_AFIO_AF(GPIOC_PWM_X_B_H, 3))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PWM_X_C_H, 3) |      \
                                     PIN_AFIO_AF(GPIOC_STAT2, 0) |          \
                                     PIN_AFIO_AF(GPIOC_SPI3_SCK, 6) |       \
                                     PIN_AFIO_AF(GPIOC_SPI3_MISO, 6) |      \
                                     PIN_AFIO_AF(GPIOC_SPI3_MOSI, 6) |      \
                                     PIN_AFIO_AF(GPIOC_LED1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PHY_PWDWN, 0) |      \
                                     PIN_AFIO_AF(GPIOC_PHY_RESET, 0))

/*
 * GPIOD setup:
 *
 * PD0  - FLASH_CS                  (output pushpull low).
 * PD1  - DRV_CS_X                  (output pushpull low).
 * PD2  - EN_GATE_X                 (output pushpull high).
 * PD3  - WAKE_X                    (output pushpull low).
 * PD4  - HALL_X_2                  (input floating).
 * PD5  - HALL_X_3                  (input floating).
 * PD6  - LAX_SWH                   (input floating).
 * PD7  - LAX_SWL                   (input floating).
 * PD8  - USART3_TX                 (alternate 7).
 * PD9  - USART_RX                  (alternate 7).
 * PD10 - HALL_Y_2                  (input floating).
 * PD11 - HALL_Y_1                  (input floating).
 * PD12 - HALL_Y_3                  (input floating).
 * PD13 - LAY_SWL                   (input floating).
 * PD14 - HALL_X_1                  (alternate 2).
 * PD15 - FAULT_X                   (input floating).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_OUTPUT(GPIOD_FLASH_CS) |      \
                                     PIN_MODE_OUTPUT(GPIOD_DRV_CS_X) |      \
                                     PIN_MODE_OUTPUT(GPIOD_EN_GATE_X) |     \
                                     PIN_MODE_OUTPUT(GPIOD_WAKE_X) |        \
                                     PIN_MODE_INPUT(GPIOD_HALL_X_2) |       \
                                     PIN_MODE_INPUT(GPIOD_HALL_X_3) |       \
                                     PIN_MODE_INPUT(GPIOD_LAX_SWH) |        \
                                     PIN_MODE_INPUT(GPIOD_LAX_SWL) |        \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_TX) |  \
                                     PIN_MODE_ALTERNATE(GPIOD_USART_RX) |   \
                                     PIN_MODE_INPUT(GPIOD_HALL_Y_2) |       \
                                     PIN_MODE_INPUT(GPIOD_HALL_Y_1) |       \
                                     PIN_MODE_INPUT(GPIOD_HALL_Y_3) |       \
                                     PIN_MODE_INPUT(GPIOD_LAY_SWL) |        \
                                     PIN_MODE_ALTERNATE(GPIOD_HALL_X_1) |   \
                                     PIN_MODE_INPUT(GPIOD_FAULT_X))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_FLASH_CS) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_DRV_CS_X) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_EN_GATE_X) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_WAKE_X) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_X_2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_X_3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LAX_SWH) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LAX_SWL) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_TX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART_RX) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_Y_2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_Y_1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_Y_3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LAY_SWL) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HALL_X_1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FAULT_X))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_25M(GPIOD_FLASH_CS) |       \
                                     PIN_OSPEED_25M(GPIOD_DRV_CS_X) |       \
                                     PIN_OSPEED_50M(GPIOD_EN_GATE_X) |      \
                                     PIN_OSPEED_25M(GPIOD_WAKE_X) |         \
                                     PIN_OSPEED_100M(GPIOD_HALL_X_2) |      \
                                     PIN_OSPEED_100M(GPIOD_HALL_X_3) |      \
                                     PIN_OSPEED_100M(GPIOD_LAX_SWH) |       \
                                     PIN_OSPEED_100M(GPIOD_LAX_SWL) |       \
                                     PIN_OSPEED_50M(GPIOD_USART3_TX) |      \
                                     PIN_OSPEED_50M(GPIOD_USART_RX) |       \
                                     PIN_OSPEED_100M(GPIOD_HALL_Y_2) |      \
                                     PIN_OSPEED_100M(GPIOD_HALL_Y_1) |      \
                                     PIN_OSPEED_100M(GPIOD_HALL_Y_3) |      \
                                     PIN_OSPEED_100M(GPIOD_LAY_SWL) |       \
                                     PIN_OSPEED_100M(GPIOD_HALL_X_1) |      \
                                     PIN_OSPEED_100M(GPIOD_FAULT_X))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_FLASH_CS) |     \
                                     PIN_PUPDR_PULLUP(GPIOD_DRV_CS_X) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOD_EN_GATE_X) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOD_WAKE_X) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_X_2) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_X_3) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_LAX_SWH) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_LAX_SWL) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_USART3_TX) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_USART_RX) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_Y_2) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_Y_1) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_Y_3) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_LAY_SWL) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_HALL_X_1) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_FAULT_X))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_FLASH_CS) |         \
                                     PIN_ODR_HIGH(GPIOD_DRV_CS_X) |         \
                                     PIN_ODR_LOW(GPIOD_EN_GATE_X) |         \
                                     PIN_ODR_HIGH(GPIOD_WAKE_X) |           \
                                     PIN_ODR_HIGH(GPIOD_HALL_X_2) |         \
                                     PIN_ODR_HIGH(GPIOD_HALL_X_3) |         \
                                     PIN_ODR_HIGH(GPIOD_LAX_SWH) |          \
                                     PIN_ODR_HIGH(GPIOD_LAX_SWL) |          \
                                     PIN_ODR_HIGH(GPIOD_USART3_TX) |        \
                                     PIN_ODR_HIGH(GPIOD_USART_RX) |         \
                                     PIN_ODR_HIGH(GPIOD_HALL_Y_2) |         \
                                     PIN_ODR_HIGH(GPIOD_HALL_Y_1) |         \
                                     PIN_ODR_HIGH(GPIOD_HALL_Y_3) |         \
                                     PIN_ODR_HIGH(GPIOD_LAY_SWL) |          \
                                     PIN_ODR_HIGH(GPIOD_HALL_X_1) |         \
                                     PIN_ODR_HIGH(GPIOD_FAULT_X))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_FLASH_CS, 0) |       \
                                     PIN_AFIO_AF(GPIOD_DRV_CS_X, 0) |       \
                                     PIN_AFIO_AF(GPIOD_EN_GATE_X, 0) |      \
                                     PIN_AFIO_AF(GPIOD_WAKE_X, 0) |         \
                                     PIN_AFIO_AF(GPIOD_HALL_X_2, 0) |       \
                                     PIN_AFIO_AF(GPIOD_HALL_X_3, 0) |       \
                                     PIN_AFIO_AF(GPIOD_LAX_SWH, 0) |        \
                                     PIN_AFIO_AF(GPIOD_LAX_SWL, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_USART3_TX, 7) |      \
                                     PIN_AFIO_AF(GPIOD_USART_RX, 7) |       \
                                     PIN_AFIO_AF(GPIOD_HALL_Y_2, 0) |       \
                                     PIN_AFIO_AF(GPIOD_HALL_Y_1, 0) |       \
                                     PIN_AFIO_AF(GPIOD_HALL_Y_3, 0) |       \
                                     PIN_AFIO_AF(GPIOD_LAY_SWL, 0) |        \
                                     PIN_AFIO_AF(GPIOD_HALL_X_1, 2) |       \
                                     PIN_AFIO_AF(GPIOD_FAULT_X, 0))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (input pullup).
 * PE1  - PIN1                      (input pullup).
 * PE2  - AUX_A2                    (output pushpull low).
 * PE3  - PIN3                      (input pullup).
 * PE4  - AUX_A0                    (output pushpull low).
 * PE5  - LED2                      (output pushpull low).
 * PE6  - PIN6                      (input pullup).
 * PE7  - FAULT_Y                   (input floating).
 * PE8  - PWM_Y_A_H                 (alternate 1).
 * PE9  - PWM_Y_A_L                 (alternate 1).
 * PE10 - PWM_Y_B_H                 (alternate 1).
 * PE11 - PWM_Y_B_L                 (alternate 1).
 * PE12 - PWM_Y_C_L                 (alternate 1).
 * PE13 - PWM_Y_C_H                 (alternate 1).
 * PE14 - PIN14                     (input pullup).
 * PE15 - DRV_CS_Y                  (output pushpull high).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                     PIN_MODE_OUTPUT(GPIOE_AUX_A2) |        \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                     PIN_MODE_OUTPUT(GPIOE_AUX_A0) |        \
                                     PIN_MODE_OUTPUT(GPIOE_LED2) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOE_FAULT_Y) |        \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_A_H) |  \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_A_L) |  \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_B_H) |  \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_B_L) |  \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_C_L) |  \
                                     PIN_MODE_ALTERNATE(GPIOE_PWM_Y_C_H) |  \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_OUTPUT(GPIOE_DRV_CS_Y))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_AUX_A2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_AUX_A0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_LED2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FAULT_Y) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_A_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_A_L) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_B_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_B_L) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_C_L) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PWM_Y_C_H) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DRV_CS_Y))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_100M(GPIOE_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOE_PIN1) |          \
                                     PIN_OSPEED_25M(GPIOE_AUX_A2) |         \
                                     PIN_OSPEED_100M(GPIOE_PIN3) |          \
                                     PIN_OSPEED_25M(GPIOE_AUX_A0) |         \
                                     PIN_OSPEED_25M(GPIOE_LED2) |           \
                                     PIN_OSPEED_100M(GPIOE_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOE_FAULT_Y) |       \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_A_H) |      \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_A_L) |      \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_B_H) |      \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_B_L) |      \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_C_L) |      \
                                     PIN_OSPEED_50M(GPIOE_PWM_Y_C_H) |      \
                                     PIN_OSPEED_100M(GPIOE_PIN14) |         \
                                     PIN_OSPEED_50M(GPIOE_DRV_CS_Y))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_AUX_A2) |     \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_AUX_A0) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_LED2) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN6) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_FAULT_Y) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_A_H) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_A_L) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_B_H) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_B_L) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_C_L) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PWM_Y_C_H) |  \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_DRV_CS_Y))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |             \
                                     PIN_ODR_LOW(GPIOE_AUX_A2) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |             \
                                     PIN_ODR_LOW(GPIOE_AUX_A0) |            \
                                     PIN_ODR_HIGH(GPIOE_LED2) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOE_FAULT_Y) |          \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_A_H) |         \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_A_L) |         \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_B_H) |         \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_B_L) |         \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_C_L) |         \
                                     PIN_ODR_LOW(GPIOE_PWM_Y_C_H) |         \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOE_DRV_CS_Y))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOE_AUX_A2, 0) |         \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOE_AUX_A0, 0) |         \
                                     PIN_AFIO_AF(GPIOE_LED2, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOE_FAULT_Y, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PWM_Y_A_H, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PWM_Y_A_L, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PWM_Y_B_H, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PWM_Y_B_L, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PWM_Y_C_L, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PWM_Y_C_H, 1) |      \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOE_DRV_CS_Y, 0))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input pullup).
 * PF1  - PIN1                      (input pullup).
 * PF2  - PIN2                      (input pullup).
 * PF3  - PIN3                      (input pullup).
 * PF4  - PIN4                      (input pullup).
 * PF5  - PIN5                      (input pullup).
 * PF6  - PIN6                      (input pullup).
 * PF7  - PIN7                      (input pullup).
 * PF8  - PIN8                      (input pullup).
 * PF9  - PIN9                      (input pullup).
 * PF10 - PIN10                     (input pullup).
 * PF11 - PIN11                     (input pullup).
 * PF12 - PIN12                     (input pullup).
 * PF13 - PIN13                     (input pullup).
 * PF14 - PIN14                     (input pullup).
 * PF15 - PIN15                     (input pullup).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_100M(GPIOF_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLUP(GPIOF_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input pullup).
 * PG1  - PIN1                      (input pullup).
 * PG2  - PIN2                      (input pullup).
 * PG3  - PIN3                      (input pullup).
 * PG4  - PIN4                      (input pullup).
 * PG5  - PIN5                      (input pullup).
 * PG6  - PIN6                      (input pullup).
 * PG7  - PIN7                      (input pullup).
 * PG8  - PIN8                      (input pullup).
 * PG9  - PIN9                      (input pullup).
 * PG10 - PIN10                     (input pullup).
 * PG11 - PIN11                     (input pullup).
 * PG12 - PIN12                     (input pullup).
 * PG13 - PIN13                     (input pullup).
 * PG14 - PIN14                     (input pullup).
 * PG15 - PIN15                     (input pullup).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_100M(GPIOG_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_PULLUP(GPIOG_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input pullup).
 * PH3  - PIN3                      (input pullup).
 * PH4  - PIN4                      (input pullup).
 * PH5  - PIN5                      (input pullup).
 * PH6  - PIN6                      (input pullup).
 * PH7  - PIN7                      (input pullup).
 * PH8  - PIN8                      (input pullup).
 * PH9  - PIN9                      (input pullup).
 * PH10 - PIN10                     (input pullup).
 * PH11 - PIN11                     (input pullup).
 * PH12 - PIN12                     (input pullup).
 * PH13 - PIN13                     (input pullup).
 * PH14 - PIN14                     (input pullup).
 * PH15 - PIN15                     (input pullup).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_100M(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_100M(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_100M(GPIOH_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input pullup).
 * PI1  - PIN1                      (input pullup).
 * PI2  - PIN2                      (input pullup).
 * PI3  - PIN3                      (input pullup).
 * PI4  - PIN4                      (input pullup).
 * PI5  - PIN5                      (input pullup).
 * PI6  - PIN6                      (input pullup).
 * PI7  - PIN7                      (input pullup).
 * PI8  - PIN8                      (input pullup).
 * PI9  - PIN9                      (input pullup).
 * PI10 - PIN10                     (input pullup).
 * PI11 - PIN11                     (input pullup).
 * PI12 - PIN12                     (input pullup).
 * PI13 - PIN13                     (input pullup).
 * PI14 - PIN14                     (input pullup).
 * PI15 - PIN15                     (input pullup).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_100M(GPIOI_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_PULLUP(GPIOI_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
