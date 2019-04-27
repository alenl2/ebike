#include "drive.h"
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_tim_ex.h>
#include <stm32f4xx_hal_tim.h>

DriveData driveData;
int pwmPin = PA1;
TIM_HandleTypeDef htim2;

//HardwareSerial Drive(USART6); // uart 1



static void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
}

/* TIM1 init function */
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
  htim2.Init.Prescaler = 19;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;

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
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
    

  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */
  


  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }



void drive_init() {


  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();


  MX_TIM2_Init();
  HAL_TIM_Base_Start(&htim2); 
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);





    //Starts the TIM Base generation

    //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse_width);

    //Drive.begin(115200, SERIAL_8N1);
    //pwmWrite(pwmPin, 0);

    /*
    uint16_t command = 1010;
    uint8_t arcData[13];

    float dirveVar = 1.1f;

    //header
    arcData[0] = 0x51;
    arcData[1] = 0x58;

    //length
    arcData[2] = 9;

    //command
    arcData[3] = command & 0xff;
    arcData[4] = (command >> 8);

    //hardcoded
    arcData[5]=0x42;
    arcData[6]=0x0;
    arcData[7]=0x0;

    //command
    arcData[8]=0x0; //motor id

    union {
        float fval;
        byte bval[4];
    } floatAsBytes;

    floatAsBytes.fval = dirveVar;

    //command data
    arcData[9] = floatAsBytes.bval[0];
    arcData[10]= floatAsBytes.bval[1];
    arcData[11]= floatAsBytes.bval[2];
    arcData[12]= floatAsBytes.bval[3];

    uint8_t sum = 0;
    for(int i=3;i<12;i++){
        sum+=arcData[i];
    }
    arcData[13] = 255 - (sum % 256);*/


}

void drive_update() {
  /*
    if(driveData.throttleProcentual > 0){
        driveData.pwmValue = (driveData.throttleProcentual);
    }else{
        driveData.pwmValue = 0;
    }*/


    
    //__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, driveData.pwmValue);
    //analogWrite(pwmPin, driveData.pwmValue);    
   // Drive.println(driveData.throttleProcentual+100.0f);
}