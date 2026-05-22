/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char cumle[100] = "";
char mors_haznesi[10] = "";
uint32_t basma_an_ms = 0;
uint32_t birakma_an_ms = 0;
uint32_t basili_tutma_sure = 0;
int harf_eklendi = 0;
int kelime_eklendi = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char mors_coz(char* semboller) {

    if (strcmp(semboller, ".-") == 0) return 'A';
    if (strcmp(semboller, "-...") == 0) return 'B';
    if (strcmp(semboller, "-.-.") == 0) return 'C';
    if (strcmp(semboller, "-..") == 0) return 'D';
    if (strcmp(semboller, ".") == 0) return 'E';
    if (strcmp(semboller, "..-.") == 0) return 'F';
    if (strcmp(semboller, "--.") == 0) return 'G';
    if (strcmp(semboller, "....") == 0) return 'H';
    if (strcmp(semboller, "..") == 0) return 'I';
    if (strcmp(semboller, ".---") == 0) return 'J';
    if (strcmp(semboller, "-.-") == 0) return 'K';
    if (strcmp(semboller, ".-..") == 0) return 'L';
    if (strcmp(semboller, "--") == 0) return 'M';
    if (strcmp(semboller, "-.") == 0) return 'N';
    if (strcmp(semboller, "---") == 0) return 'O';
    if (strcmp(semboller, ".--.") == 0) return 'P';
    if (strcmp(semboller, ".-.") == 0) return 'R';
    if (strcmp(semboller, "...") == 0) return 'S';
    if (strcmp(semboller, "-") == 0) return 'T';
    if (strcmp(semboller, "..-") == 0) return 'U';
    if (strcmp(semboller, "...-") == 0) return 'V';
    if (strcmp(semboller, "-.--") == 0) return 'Y';
    if (strcmp(semboller, "--..") == 0) return 'Z';

    return '?';
}

void uart_mesaj_gonder(char* format, ...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf), format, args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, 100);
    va_end(args);
}

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
{


	      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	      {
	          basma_an_ms = HAL_GetTick();
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	          while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	          birakma_an_ms = HAL_GetTick();
	          basili_tutma_sure = birakma_an_ms - basma_an_ms;

	          if (basili_tutma_sure < 500) {
	              strcat(mors_haznesi, ".");
	              uart_mesaj_gonder("[Sinyal: . | Sure: %lu ms]\r\n", basili_tutma_sure);
	          } else {
	              strcat(mors_haznesi, "-");
	              uart_mesaj_gonder("[Sinyal: - | Sure: %lu ms]\r\n", basili_tutma_sure);
	          }

	          harf_eklendi = 0; //yeni signal geldiginde, harf bekleme suresinin sifirlanmasi
	          kelime_eklendi = 0;
	          HAL_Delay(50);
	      }

	      uint32_t bosta_gecen = HAL_GetTick() - birakma_an_ms;


	      if (bosta_gecen > 3000 && birakma_an_ms != 0 && strlen(mors_haznesi) > 0 && !harf_eklendi)
	      {
	          char yeni_harf = mors_coz(mors_haznesi);
	          int len = strlen(cumle);
	          cumle[len] = yeni_harf;
	          cumle[len + 1] = '\0';

	          uart_mesaj_gonder("\r\n>> Harf Tamamlandi: %c (Mors: %s)\r\n", yeni_harf, mors_haznesi);
	          memset(mors_haznesi, 0, sizeof(mors_haznesi));
	          harf_eklendi = 1;
	      }

	      // 3. KELİME SONU KONTROLÜ (5 Saniye)
	      if (bosta_gecen > 1000 && birakma_an_ms != 0 && !kelime_eklendi && harf_eklendi)
	      {
	          strcat(cumle, " ");
	          uart_mesaj_gonder(">> Kelime Arasi: (Bosluk eklendi)\r\n");
	          kelime_eklendi = 1;
	      }

	      // 4. CÜMLE SONU VE YAZDIRMA (7 Saniye)
	      if (bosta_gecen > 7000 && birakma_an_ms != 0)
	      {
	          uart_mesaj_gonder("\r\n================================\r\n");
	          uart_mesaj_gonder("FINAL CUMLE: %s", cumle);
	          uart_mesaj_gonder("\r\n================================\r\n\r\n");

	          memset(cumle, 0, sizeof(cumle));
	          birakma_an_ms = 0; // Sistemi bekleme moduna al
	      }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Green_Led_GPIO_Port, Green_Led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : Green_Led_Pin */
  GPIO_InitStruct.Pin = Green_Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Green_Led_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
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
