#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "usb_device.h"


I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);


static void initTask(void *);
static void motorTask(void *);

TIM_HandleTypeDef    pwmTimHandle;
TIM_OC_InitTypeDef   PWMChannelConfig;
void PWMSetup(void);
void SET_PWM_DUTY_CYCLE(uint16_t DUTY_CYCLE);

void PWMSetupChannel2(void);
void PWMSetupChannel3(void);
void PWMSetupChannel4(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();


    xTaskCreate(initTask,  "Initial", 256, NULL, tskIDLE_PRIORITY+2, NULL) ;
    xTaskCreate(motorTask,  "Motor", 256, NULL, tskIDLE_PRIORITY+2, NULL) ;

  	vTaskStartScheduler();


  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

static void initTask(void *pvParameters)
{
	  /* init code for USB_DEVICE */
	  MX_USB_DEVICE_Init();

		PWMSetup();
		PWMSetupChannel2();
		PWMSetupChannel3();
		PWMSetupChannel4();

	  /* USER CODE BEGIN 5 */
	  /* Infinite loop */
	  for(;;)
	  {
		  uint8_t testDataToSend[13] = {'H', 'E', 'L','L','O',' ','W','O','R','L','D', '\r'};

		  CDC_Transmit_FS(testDataToSend, 12);
		  vTaskDelay(1000);
	  }
}

static void motorTask(void *pvParameters)
{
	for(;;)
	{
		SET_PWM_DUTY_CYCLE(12000); /*Reverse*/
		vTaskDelay(3000);

		SET_PWM_DUTY_CYCLE(15000); /*STOP*/
		vTaskDelay(3000);

		SET_PWM_DUTY_CYCLE(18000); /*FOREWARD*/
		vTaskDelay(3000);

		SET_PWM_DUTY_CYCLE(15000); /*STOP*/
		vTaskDelay(3000);
	}
}

void PWMSetup(void)
{
	/*BEGIN GPIO SETUP*/
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_6;

	//SETUP for alternate Function
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_HIGH;
    gpioInit.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &gpioInit);
	/*END GPIO SETUP*/

	/*BEGIN TIMER SETUP */
	  __TIM3_CLK_ENABLE();
	pwmTimHandle.Instance = TIM3;
	pwmTimHandle.Init.Prescaler         = 10-1; //uhPrescalerValue;
	pwmTimHandle.Init.Period            = 20000 - 1;	//SHOULD Give PWM Frequency of  500HZ
	pwmTimHandle.Init.ClockDivision     = 0;
	pwmTimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	pwmTimHandle.Init.RepetitionCounter = 0;
	pwmTimHandle.Channel = HAL_TIM_ACTIVE_CHANNEL_1 | HAL_TIM_ACTIVE_CHANNEL_2| HAL_TIM_ACTIVE_CHANNEL_3 | HAL_TIM_ACTIVE_CHANNEL_4;
	HAL_TIM_PWM_Init(&pwmTimHandle);

	/*END TIMER SETUP */


	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	PWMChannelConfig.OCMode       = TIM_OCMODE_PWM1;
	PWMChannelConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	PWMChannelConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	PWMChannelConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	PWMChannelConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	PWMChannelConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

	/* Set the pulse value for channel 1 */
	PWMChannelConfig.Pulse = 1500;    //START AT STOP

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_1);
}
/*10,000 / 2 = FULL SPEED*/
void SET_PWM_DUTY_CYCLE(uint16_t DUTY_CYCLE)
{
	PWMChannelConfig.Pulse = DUTY_CYCLE;

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_1);		//MOTOR 1

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_2);		//MOTOR 2

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_3);		//MOTOR 3

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_4);		//MOTOR 4
}

void PWMSetupChannel2(void)
{
	/*BEGIN GPIO SETUP*/
	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_7;

	//SETUP for alternate Function
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_HIGH;
    gpioInit.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &gpioInit);
	/*END GPIO SETUP*/

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_2);
}

void PWMSetupChannel3(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*BEGIN GPIO SETUP*/
	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_0;

	//SETUP for alternate Function
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_HIGH;
    gpioInit.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &gpioInit);
	/*END GPIO SETUP*/

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_3);
}

void PWMSetupChannel4(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*BEGIN GPIO SETUP*/
	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_1;

	//SETUP for alternate Function
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_HIGH;
    gpioInit.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &gpioInit);
	/*END GPIO SETUP*/

	HAL_TIM_PWM_ConfigChannel(&pwmTimHandle, &PWMChannelConfig, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&pwmTimHandle, TIM_CHANNEL_4);
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 7;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PA4   ------> I2S3_WS
     PA5   ------> SPI1_SCK
     PA6   ------> SPI1_MISO
     PA7   ------> SPI1_MOSI
     PB10   ------> I2S2_CK
     PB12   ------> I2S2_WS
     PC7   ------> I2S3_MCK
     PC10   ------> I2S3_CK
     PC12   ------> I2S3_SD
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE4 PE5 MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CLK_IN_Pin PB12 */
  GPIO_InitStruct.Pin = CLK_IN_Pin|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 I2S3_SCK_Pin PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|I2S3_SCK_Pin|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin, GPIO_PIN_RESET);

}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
