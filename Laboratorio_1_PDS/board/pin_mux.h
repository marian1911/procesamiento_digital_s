/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTC6 (number 78), SW2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_Start_Stop_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_Start_Stop_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_Start_Stop_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_Start_Stop_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_Start_Stop_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA4 (number 38), SW3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_Sample_Time_GPIO GPIOA               /*!<@brief GPIO peripheral base pointer */
#define BOARD_Sample_Time_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_Sample_Time_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_Sample_Time_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_Sample_Time_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTC0 (number 70), J1[11]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ts_8k_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ts_8k_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ts_8k_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ts_8k_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_ts_8k_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                             /* @} */

/*! @name PORTC1 (number 71), J1[5]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ts_16k_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ts_16k_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ts_16k_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ts_16k_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_ts_16k_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name PORTC2 (number 72), J1[14]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ts_22k_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ts_22k_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ts_22k_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ts_22k_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_ts_22k_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name PORTC3 (number 73), J1[16]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ts_44k_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ts_44k_GPIO_PIN_MASK (1U << 3U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ts_44k_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ts_44k_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_ts_44k_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name PORTC4 (number 76), J2[4]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ts_48k_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ts_48k_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ts_48k_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ts_48k_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_ts_48k_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC6 (number 78), SW2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SW2_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_SW2_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SW2_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_SW2_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_SW2_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name PORTA4 (number 38), SW3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SW3_GPIO GPIOA               /*!<@brief GPIO peripheral base pointer */
#define BOARD_SW3_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SW3_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_SW3_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_SW3_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtonsPins(void);

/*! @name PORTB21 (number 67), D12[3]/LEDRGB_BLUE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_LED_BLUE_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_LED_BLUE_GPIO_PIN_MASK (1U << 21U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_LED_BLUE_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_LED_BLUE_PIN 21U                   /*!<@brief PORT pin number */
#define BOARD_LED_BLUE_PIN_MASK (1U << 21U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTB22 (number 68), D12[1]/LEDRGB_RED
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_LED_RED_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_LED_RED_GPIO_PIN_MASK (1U << 22U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_LED_RED_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_LED_RED_PIN 22U                   /*!<@brief PORT pin number */
#define BOARD_LED_RED_PIN_MASK (1U << 22U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE26 (number 33), J2[1]/D12[4]/LEDRGB_GREEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_LED_GREEN_GPIO GPIOE                /*!<@brief GPIO peripheral base pointer */
#define BOARD_LED_GREEN_GPIO_PIN_MASK (1U << 26U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_LED_GREEN_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_LED_GREEN_PIN 26U                   /*!<@brief PORT pin number */
#define BOARD_LED_GREEN_PIN_MASK (1U << 26U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB17 (number 63), U10[1]/UART0_TX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_DEBUG_UART_TX_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_DEBUG_UART_TX_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_DEBUG_UART_TX_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                      /* @} */

/*! @name PORTB16 (number 62), U7[4]/UART0_RX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_DEBUG_UART_RX_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_DEBUG_UART_RX_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_DEBUG_UART_RX_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name PORTA18 (number 50), U13[16]/RMII_RXCLK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_EXTAL0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_EXTAL0_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_EXTAL0_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*! @name EXTAL32 (number 29), Y3[2]/EXTAL32_RTC
  @{ */
/* @} */

/*! @name XTAL32 (number 28), Y3[1]/XTAL32_RTC
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

/*! @name PORTE25 (number 32), J2[18]/U8[6]/I2C0_SDA
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_SDA_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_SDA_PIN 25U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_SDA_PIN_MASK (1U << 25U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTE24 (number 31), J2[20]/U8[4]/I2C0_SCL
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_SCL_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_SCL_PIN 24U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_SCL_PIN_MASK (1U << 24U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTC6 (number 78), SW2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ACCEL_INT1_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ACCEL_INT1_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_INT1_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_INT1_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_INT1_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTC13 (number 85), U8[9]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ACCEL_INT2_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_ACCEL_INT2_GPIO_PIN_MASK (1U << 13U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_INT2_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_INT2_PIN 13U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_INT2_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

#define SOPT2_RMIISRC_EXTAL 0x00u /*!<@brief RMII clock source select: EXTAL clock */

/*! @name PORTB1 (number 54), U13[11]/RMII0_MDC
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_MDC_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_MDC_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_MDC_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTB0 (number 53), U13[10]/RMII0_MDIO
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_MDIO_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_MDIO_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_MDIO_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA13 (number 43), U13[13]/RMII0_RXD_0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXD0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXD0_PIN 13U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXD0_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA12 (number 42), U13[12]/RMII0_RXD_1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXD1_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXD1_PIN 12U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXD1_PIN_MASK (1U << 12U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA5 (number 39), U13[17]/RMII0_RXER
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXER_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXER_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXER_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA16 (number 46), U13[20]/RMII0_TXD0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXD0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXD0_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXD0_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA17 (number 47), U13[21]/RMII0_TXD1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXD1_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXD1_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXD1_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA15 (number 45), U13[19]/RMII0_TXEN
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXEN_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXEN_PIN 15U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXEN_PIN_MASK (1U << 15U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA14 (number 44), U13[15]/RMII0_CRS_DV
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_CRS_DV_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_CRS_DV_PIN 14U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_CRS_DV_PIN_MASK (1U << 14U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*! @name PORTA18 (number 50), U13[16]/RMII_RXCLK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII_RXCLK_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII_RXCLK_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_RMII_RXCLK_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitENETPins(void);

/*! @name PORTE0 (number 1), J15[P8]/SDHC0_D1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D1_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D1_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D1_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE1 (number 2), J15[P7]/SDHC0_D0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D0_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D0_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D0_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE2 (number 3), J15[P5]/SDHC0_DCLK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_DCLK_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_DCLK_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_DCLK_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTE3 (number 4), J15[P3]/SDHC0_CMD
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_CMD_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_CMD_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_CMD_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTE4 (number 5), J15[P2]/SDHC0_D3
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D3_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D3_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D3_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE5 (number 6), J15[P1]/SDHC0_D2
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D2_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D2_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D2_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE6 (number 7), J15[G1]/SD_CARD_DETECT
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SDHC_CD_GPIO GPIOE               /*!<@brief GPIO peripheral base pointer */
#define BOARD_SDHC_CD_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC_CD_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC_CD_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_SDHC_CD_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDHCPins(void);

/*! @name USB0_DP (number 10), J22[3]/K64_MICRO_USB_DP
  @{ */
/* @} */

/*! @name USB0_DM (number 11), J22[2]/K64_MICRO_USB_DN
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
