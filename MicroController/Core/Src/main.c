/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"
#include "keypad.h"
#include "melody.h"
#include "game.h"
#include "lcd.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "LiquidCrystal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_VOLUME ADC2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef mytime;
RTC_DateTypeDef mydate;

unsigned char uart_buffer[20];
unsigned char uart_data;
int uart_buffer_pos = 0;

int timer_count = 0;
volatile uint32_t last_gpio_exti;

char si_player_name[] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
char si_game_level= 'E';
int si_is_player_entered_name=0;

int lcd_page = 0;
int lcd_page_menu_current_option = 1;
int lcd_page_game_menu_current_option;

int ph_index=0;

int msg_player_health=0;
int msg_remained_enemies=0;

int is_game_page_cleared_for_boss=0;
int is_boss_game_started = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_ADC2_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

void Reset_App();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_ADC2_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  Init_Keypad();
  HAL_UART_Receive_IT(&huart3, &uart_data, sizeof(uart_data));

  mytime.Hours = 18;
  mytime.Minutes=12;
  mytime.Seconds=41;
  HAL_RTC_SetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
  mydate.Year = 23;
  mydate.Month=2;
  mydate.WeekDay=2;
  HAL_RTC_SetDate(&hrtc, &mydate, RTC_FORMAT_BIN);

  LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);
  begin(20,4);
  LCD_Init();
  LCD_Display_Page_Menu();
  LCD_Update_Selected_Option_In_Page_Menu(1, 1);
  HAL_TIM_Base_Start_IT(&htim1);
  Init_Buzzer_PWM(&htim2,TIM_CHANNEL_2);
  PWM_Play();
  Set_Menu_Melody();
  HAL_ADC_Start_IT(&hadc2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_TIM1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 39;
  hrtc.Init.SynchPrediv = 999;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 3599;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_1
                          |GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT4_Pin */
  GPIO_InitStruct.Pin = MEMS_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD1
                           PD3 PD5 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_1
                          |GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD2 PD4 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC2){
		int raw_volume = HAL_ADC_GetValue(&hadc2);
		int volume = (raw_volume - 859) * 25 / (4095-770);
		PWM_Set_Volume(volume);
		 HAL_ADC_Start_IT(&hadc2);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){ 		//every 50 ms

	if(htim->Instance == TIM1){
		timer_count++;
		int cur_page = lcd_page;
		if((timer_count * 50 ) % 1000 == 0 && cur_page == 5){
			char time_str[9];
			char date_str[9];
			HAL_RTC_GetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &mydate, RTC_FORMAT_BIN);
			sprintf(time_str,"%2d:%2d:%2d",mytime.Hours,mytime.Minutes,mytime.Seconds);
			sprintf(date_str,"%2d:%2d:%2d",mydate.Year ,mydate.Month  ,mydate.WeekDay);
			LCD_Update_Time(time_str,date_str);
		}

		if(cur_page == 1){
			int is_non_boss_game_ended = SI_Is_Non_Boss_Game_Ended();
			if(is_non_boss_game_ended == 1 && is_game_page_cleared_for_boss ==0){
				is_game_page_cleared_for_boss=1;
				for(int i=0;i<4;i++){
					for(int j=1;j<20;j++){
						setCursor(j, i);
						print(" ");
					}
				}
			}
			int is_game_ended = SI_Is_Game_Ended();
			if(is_game_ended==1){
				lcd_page = 7;
				Set_Rest_Melody();
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
				int winner = SI_Get_Winner();
				LCD_Clear_Page_Game();
				LCD_Display_Page_End();
				int score = SI_Get_Dead_Enemies();
				char score_str[5];
				sprintf(score_str,"%d",score);
				LCD_Update_Games_Info_In_End(si_player_name,score_str);
				if(winner == 1){
					char msg[] = "You Win\n";
					HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
				}else if(winner == -1){
					char msg[] = "Enemy Win\n";
					HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
				}

			}else{
				if(is_non_boss_game_ended==1){
					int winner = SI_Get_Winner();
					if(winner == -1){
						lcd_page = 7;
						Set_Rest_Melody();
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
						LCD_Clear_Page_Game();
						LCD_Display_Page_End();
						int score = SI_Get_Dead_Enemies();
						char score_str[5];
						sprintf(score_str,"%d",score);
						LCD_Update_Games_Info_In_End(si_player_name,score_str);
						if(winner == 1){
							char msg[] = "You Win\n";
							HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
						}else if(winner == -1){
							char msg[] = "Enemy Win\n";
							HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
						}
					}else{
						if(is_boss_game_started == 0){
							is_boss_game_started = 1;
							Set_Boss_Melody();
						}
						SI_Handle_Boss_Game();
						int remained_enemies = SI_Get_Remained_Enemies_Killings_For_Winnig();
						if(remained_enemies==3 && msg_remained_enemies==0){
							msg_remained_enemies = 1;
							char msg[] = "Only 3 Enemies Remained For Win\n";
							HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
						}
						int player_health = SI_Get_Player_Health();
						if((player_health == 2||player_health == 1) && msg_player_health==0){
							msg_player_health=1;
							char msg[] = "Your Health Is less than 2.\n";
							HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
						}
						UpdatedEntity* ues =  SI_Get_Updated_Entities();
						LCD_Update_Game_Boss(ues);
					}
				}else{
					SI_Handle_Game();
					int is_refresh_needed_for_enemies_moves = SI_Is_Enemies_Move_Down();
					int remained_enemies = SI_Get_Remained_Enemies_Killings_For_Winnig();
					if(remained_enemies==3 && msg_remained_enemies==0){
						msg_remained_enemies = 1;
						char msg[] = "Only 3 Enemies Remained For Win\n";
						HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
					}
					int player_health = SI_Get_Player_Health();
					if(player_health == 1 && msg_player_health==0){
						msg_player_health=1;
						char msg[] = "Your Health Is 1.\n";
						HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
					}
					int ipke = SI_Is_Player_Kill_Enemy();
					if(ipke == 1){
						Set_Kill_Effect();
					}
					UpdatedEntity* ues =  SI_Get_Updated_Entities();
					if(is_refresh_needed_for_enemies_moves == 1){
						LCD_Update_Game_With_Enemy_Move(ues);
					}else{
						LCD_Update_Game_Without_Enemy_Move(ues);
					}

				}
			}
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART3){
		if(uart_data != 0x0A){
			uart_buffer[uart_buffer_pos] = uart_data;
			uart_buffer_pos += 1;
		}else{
			uart_buffer[uart_buffer_pos] = '\0';
			int str_len = uart_buffer_pos + 2; //if user enter {aa} str_len is 4 because {a + a + 0x0A + \0}
			char input[str_len];
			for (int i = 0; i < str_len; i++) {
				input[i] = toupper(uart_buffer[i]); //for example if user enter RezA it changes to REZA for next processes
			}
			uart_buffer_pos = 0 ;
			if(lcd_page == 2){
//				char msg[] = "Please Enter A Valid Name With String Length < 8\n";
//				int size =strlen(input);
//				if(size > 1){
//					HAL_UART_Transmit(&huart3, msg, sizeof(msg), 1000);
//				}else{
//					if(ph_index< 8){
//						si_player_name[ph_index] = input[0];
//						si_is_player_entered_name = 1;
//						ph_index++;
//						LCD_Update_Char_Name_Pointer(ph_index+2);
//						if(ph_index>7){
//							ph_index=7;
//						}
//						Reset_PK();
//					}
//				}
			}
		}
	}
	HAL_UART_Receive_IT(&huart3, &uart_data, sizeof(uart_data));

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (last_gpio_exti + 200 > HAL_GetTick()) // Simple button debouncing
  {
	return;
  }
  last_gpio_exti = HAL_GetTick();

	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
	uint8_t button_number = Handle_Keypad(GPIO_Pin);
	  //   C0   C1   C2   C3
	  // +----+----+----+----+
	  // | 1  | 2  | 3  | 4  |  R0
	  // +----+----+----+----+
	  // | 5  | 6  | 7  | 8  |  R1
	  // +----+----+----+----+
	  // | 9  | 10 | 11 | 12 |  R2
	  // +----+----+----+----+
	  // | 13 | 14 | 15 | 16 |  R3
	  // +----+----+----+----+
	/*
	 * page 0 : menu
	 * page 1 : game
	 * page 2 : enter name
	 * page 3 : enter game level
	 * page 5 : about
	 * page 7 : end
	 *
	 */
	int player_col;
	int new_player_col;

	int page = lcd_page;
	switch (page){
		case 0 :
			switch (button_number)
			  {
				case 2:
					if(lcd_page_menu_current_option != 1){
						lcd_page_menu_current_option--;
						LCD_Update_Selected_Option_In_Page_Menu(lcd_page_menu_current_option,lcd_page_menu_current_option+1);
					}
					break;
				case 4:
					LCD_Clear_Page_Menu();
					if(lcd_page_menu_current_option == 1){
						lcd_page = 2;
						Set_Rest_Melody();
						LCD_Display_Page_Entering_Name();
						ph_index = 0;
					}else if(lcd_page_menu_current_option == 2){
						lcd_page = 5;
						Set_Rest_Melody();
						LCD_Display_Page_About();
					}
					break;
				case 10:
					if(lcd_page_menu_current_option != 2){
						lcd_page_menu_current_option++;
						LCD_Update_Selected_Option_In_Page_Menu(lcd_page_menu_current_option,lcd_page_menu_current_option-1);
					}
					break;
				default :
					break;
			  }
			break;
		case 1 :
			switch (button_number)
			  {
				case 5:
					player_col = SI_Get_Player_Col();
					SI_Move_Player(LEFT);
					new_player_col = SI_Get_Player_Col();
					LCD_Update_Game_Player(new_player_col,player_col);
					break;
				case 10:
					SI_Shoot_Player();
					Set_Fire_Effect();
					if(is_boss_game_started == 1){
						Set_Boss_Melody();
					}
					break;
				case 7:
					player_col = SI_Get_Player_Col();
					SI_Move_Player(RIGHT);
					new_player_col = SI_Get_Player_Col();
					LCD_Update_Game_Player(new_player_col,player_col);
					break;
				default :
					break;
			  }

			break;
		case 2 ://Entering Name
			if(button_number == 4){
				LCD_Clear_Player_Name();
				ph_index = 0;
				Reset_PK();
				si_is_player_entered_name = 0;
			}else if(button_number == 12){
				if(si_is_player_entered_name==1){
					lcd_page = 3;
					LCD_Clear_Page_Entering_Name();
					LCD_Display_Page_Setting_Level();
				}else{
					char msg[] = "Please Enter Or Complete Your Name.\n";
					HAL_UART_Transmit(&huart3, msg, sizeof(msg), 1000);
				}
			}else{
				if(ph_index < 8){
					si_is_player_entered_name = 0;
					PKResult pk_result = Handle_Phone_Keypad(button_number);
					LCD_Update_Char_Name(pk_result.character, ph_index+2);
					if(pk_result.is_accepted==1 ){
						si_player_name[ph_index] = pk_result.character[0];
						si_is_player_entered_name = 1;
						ph_index++;
						LCD_Update_Char_Name_Pointer(ph_index+2);
						if(ph_index>7){
							ph_index=7;
						}
						Reset_PK();
					}
					if(pk_result.is_deleted==1){
						si_player_name[ph_index] = '\0';
						Reset_PK();
					}
				}
			}
			break;

		case 3 :
			switch (button_number)
			  {
				case 5:
					si_game_level = 'E';
					break;
				case 6:
					si_game_level = 'N';
					break;
				case 7:
					si_game_level = 'H';
					break;
				case 4:
					LCD_Clear_Page_Setting_Level();
					lcd_page = 1;
					SI_Init_Game(si_game_level, 50);
					LCD_Display_Page_Game();
					char msg[100];
					int target = 0;
					if(si_game_level == 'H'){
						target = 35;
					}else if(si_game_level == 'N'){
						target = 25;
					}else if(si_game_level == 'E'){
						target = 15;
					}
					sprintf(msg,"game started!!level is %c and enemy target for winning is %d.\n",si_game_level,target);
					HAL_UART_Transmit(&huart3, msg, strlen(msg), 1000);
					break;
				default :
					break;
			  }
			break;
		case 5 :
			if(button_number == 16){
				lcd_page = 0;
				LCD_Clear_Page_About();
				LCD_Display_Page_Menu();
				Set_Menu_Melody();
			}
			break;
		case 7 :
			if(button_number == 4){
				lcd_page = 0;
				LCD_Clear_Page_End();
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
				LCD_Display_Page_Menu();
				Set_Menu_Melody();
				Reset_App();
			}
			break;
		default :
			break;
	}
}
void Reset_App(){
	timer_count =0;
	lcd_page = 0;
	lcd_page_menu_current_option = 1;
	ph_index=0;
	msg_player_health=0;
	msg_remained_enemies=0;
	is_game_page_cleared_for_boss=0;
	is_boss_game_started = 0;
	uart_buffer_pos = 0;
	timer_count = 0;
	for(int i=0;i<10;i++){
		si_player_name[i] = '\0';
	}
	si_game_level= 'E';
	si_is_player_entered_name=0;
	SI_Reset_Game();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
