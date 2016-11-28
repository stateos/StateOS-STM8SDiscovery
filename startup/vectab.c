void _stext(); // startup routine

void TRAP_IRQHandler();
void TLI_IRQHandler();
void AWU_IRQHandler();
void CLK_IRQHandler();
void EXTI_PORTA_IRQHandler();
void EXTI_PORTB_IRQHandler();
void EXTI_PORTC_IRQHandler();
void EXTI_PORTD_IRQHandler();
void EXTI_PORTE_IRQHandler();
void EXTI_PORTF_IRQHandler();
void CAN_RX_IRQHandler();
void CAN_TX_IRQHandler();
void SPI_IRQHandler();
void TIM1_UPD_OVF_TRG_BRK_IRQHandler();
void TIM1_CAP_COM_IRQHandler();
void TIM5_UPD_OVF_BRK_TRG_IRQHandler();
void TIM5_CAP_COM_IRQHandler();
void TIM2_UPD_OVF_BRK_IRQHandler();
void TIM2_CAP_COM_IRQHandler();
void TIM3_UPD_OVF_BRK_IRQHandler();
void TIM3_CAP_COM_IRQHandler();
void UART1_RX_IRQHandler();
void UART1_TX_IRQHandler();
void UART4_RX_IRQHandler();
void UART4_TX_IRQHandler();
void I2C_IRQHandler();
void UART2_RX_IRQHandler();
void UART2_TX_IRQHandler();
void UART3_RX_IRQHandler();
void UART3_TX_IRQHandler();
void ADC1_IRQHandler();
void ADC2_IRQHandler();
void TIM4_UPD_OVF_IRQHandler();
void TIM6_UPD_OVF_TRG_IRQHandler();
void EEPROM_EEC_IRQHandler();

#pragma section const { vector }

void (* const @vector _vectab[32])() =
{
/* -2 */  _stext,
/* -1 */  TRAP_IRQHandler,
/*  0 */  TLI_IRQHandler,
/*  1 */  AWU_IRQHandler,
/*  2 */  CLK_IRQHandler,
/*  3 */  EXTI_PORTA_IRQHandler,
/*  4 */  EXTI_PORTB_IRQHandler,
/*  5 */  EXTI_PORTC_IRQHandler,
/*  6 */  EXTI_PORTD_IRQHandler,
/*  7 */  EXTI_PORTE_IRQHandler,
#if   defined(STM8S208) || defined(STM8AF52Ax)
/*  8 */  CAN_RX_IRQHandler,
/*  9 */  CAN_TX_IRQHandler,
#elif defined(STM8S903) || defined(STM8AF622x)
/*  8 */  EXTI_PORTF_IRQHandler,
/*  9 */  0,
#else
/*  8 */  0,
/*  9 */  0,
#endif
/* 10 */  SPI_IRQHandler,
/* 11 */  TIM1_UPD_OVF_TRG_BRK_IRQHandler,
/* 12 */  TIM1_CAP_COM_IRQHandler,
#if   defined(STM8S903) || defined(STM8AF622x)
/* 13 */  TIM5_UPD_OVF_BRK_TRG_IRQHandler,
/* 14 */  TIM5_CAP_COM_IRQHandler,
#else
/* 13 */  TIM2_UPD_OVF_BRK_IRQHandler,
/* 14 */  TIM2_CAP_COM_IRQHandler,
#endif
#if   defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || defined(STM8S005) || defined(STM8AF52Ax) || defined(STM8AF62Ax) || defined(STM8AF626x)
/* 15 */  TIM3_UPD_OVF_BRK_IRQHandler,
/* 16 */  TIM3_CAP_COM_IRQHandler,
#else
/* 15 */  0,
/* 16 */  0,
#endif
#if   defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || defined(STM8S003) || defined(STM8AF52Ax) || defined(STM8AF62Ax) || defined(STM8S903)
/* 17 */  UART1_TX_IRQHandler,
/* 18 */  UART1_RX_IRQHandler,
#elif defined(STM8AF622x)
/* 17 */  UART4_TX_IRQHandler,
/* 18 */  UART4_RX_IRQHandler,
#else
/* 17 */  0,
/* 18 */  0,
#endif
/* 19 */  I2C_IRQHandler,
#if   defined(STM8S105) || defined(STM8S005) || defined(STM8AF626x)
/* 20 */  UART2_RX_IRQHandler,
/* 21 */  UART2_TX_IRQHandler,
#elif defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined(STM8AF52Ax) || defined(STM8AF62Ax)
/* 20 */  UART3_RX_IRQHandler,
/* 21 */  UART3_TX_IRQHandler,
#else
/* 20 */  0,
/* 21 */  0,
#endif
#if   defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined(STM8AF52Ax) || defined(STM8AF62Ax)
/* 22 */  ADC2_IRQHandler,
#else
/* 22 */  ADC1_IRQHandler,
#endif
#if   defined(STM8S903) || defined(STM8AF622x)
/* 23 */  TIM6_UPD_OVF_TRG_IRQHandler,
#else
/* 23 */  TIM4_UPD_OVF_IRQHandler,
#endif
/* 24 */  EEPROM_EEC_IRQHandler,
/* 25 */  0,
/* 26 */  0,
/* 27 */  0,
/* 28 */  0,
/* 29 */  0,
};
