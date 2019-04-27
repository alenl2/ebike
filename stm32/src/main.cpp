#include "display.h"
#include "brake.h"
#include "key.h"
#include "throttle.h"
#include "pas.h"
#include "drive.h"
#include "drive.h"


HardwareSerial  Pc(USART2); // uart 1

void debugLcd(){
  if(displayVariables.newData == false){
    return;
  }
  displayVariables.newData=false;
  Pc.print("P1:");
  Pc.print(displayVariables.ui8_p1);
  Pc.print(" ");

  Pc.print("P2:");
  Pc.print(displayVariables.ui8_p2);
  Pc.print(" ");

  Pc.print("P3:");
  Pc.print(displayVariables.ui8_p3);
  Pc.print(" ");
  
  Pc.print("P4:");
  Pc.print(displayVariables.ui8_p4);
  Pc.print(" ");

  Pc.print("P5:");
  Pc.print(displayVariables.ui8_p5);
  Pc.print(" ");

  Pc.print("C1:");
  Pc.print(displayVariables.ui8_c1);
  Pc.print(" ");

  Pc.print("C2:");
  Pc.print(displayVariables.ui8_c2);
  Pc.print(" ");

  Pc.print("C4:");
  Pc.print(displayVariables.ui8_c4);
  Pc.print(" ");

  Pc.print("C5:");
  Pc.print(displayVariables.ui8_c5);
  Pc.print(" ");

  Pc.print("C12:");
  Pc.print(displayVariables.ui8_c12);
  Pc.print(" ");

  Pc.print("C13:");
  Pc.print(displayVariables.ui8_c13);
  Pc.print(" ");

  Pc.print("C14:");
  Pc.print(displayVariables.ui8_c14);
  Pc.print(" ");

  Pc.print("Max spd:");
  Pc.print(displayVariables.ui8_max_speed);
  Pc.print(" ");

  Pc.print("Whell size:");
  Pc.print(displayVariables.ui8_wheel_size);
  Pc.print(" ");

  Pc.print("Assist:");
  Pc.print(displayVariables.ui8_assist_level);
  Pc.print(" ");

  Pc.print("Light:");
  Pc.print(displayVariables.lightOn);
  Pc.print(" ");

  Pc.print("Push assist:");
  Pc.print(displayVariables.walkAssistOn);
  Pc.print(" ");

  Pc.print("Cruise:");
  Pc.print(displayVariables.enterCruise);
  Pc.print(" ");

  Pc.print("RAW:");
  for(int i=0;i<13;i++){
    Pc.print(displayVariables.displaySerialBuffer[i], BIN);
    Pc.print(" ");
  }
  Pc.print("Calc crc:");
  Pc.print(displayVariables.calcCrc);
  Pc.print(" ");

  Pc.print("Report crc: ");
  Pc.print(displayVariables.reportedCrc);
  Pc.print(" ");
  Pc.flush();
}

void debugThrottle(){
  Pc.print(throttleData.throttleOn);
  Pc.print(" ");
  Pc.print(throttleData.procentual);
  Pc.print(" ");
  Pc.print(throttleData.throttleValue);
}

void debugBrake(){
  Pc.print("Main brake:");
  Pc.print(brakeData.brakeOn);
  Pc.print(" ");
  Pc.print("Aux brake:");
  Pc.print(brakeData.auxBrakeOn);
}

void debugKey(){
  Pc.print("Key:");
  Pc.print(keyData.keyOn);
}

void debugPas(){
  Pc.print("PasSpeed:");
  Pc.print(pasData.pasSpeed);
  Pc.print(" ");
  Pc.print("PasOn:");
  Pc.print(pasData.pasOn);
}

void debugDrive(){
  Pc.print("PWM:");
  Pc.print(driveData.pwmValue);
}

void setup() {
  HAL_Init();
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    Pc.begin(115200);
    display_init();
    brake_init();
    key_init();
    throttle_init();
    pas_init();
    drive_init();

    displayData.error = None;
    displayData.batteryVoltage = 49;
    displayData.speed = 2000;
}

bool runInCruise = false;
float throttleValue = 0;

void loop() {
  key_update();
  display_parse();
  display_update();
  drive_update();
  throttle_update();
  pas_update();
  brake_update();

  debugKey();
  debugThrottle();
  debugBrake() ;
  debugDrive();
  Pc.println();

  if(keyData.keyOn == false /*|| displayVariables.didInit == false*/){
    //driveData.throttleProcentual = 0;
    displayData.error = Info5;
    return;
  }

  if(displayVariables.enterCruise){
    runInCruise = true;
  }

  if(brakeData.brakeOn){
    displayData.brake = true;
    throttleValue = 0;
  }else{
    displayData.brake = false;
    throttleValue = 0;
    if(runInCruise){
      displayData.cruise = true;
      throttleValue = throttleValue;
    }else{
      displayData.cruise = false;
      if(throttleData.throttleOn){
        displayData.throttle = true; 
        throttleValue = throttleData.procentual+1;
      }else{
        displayData.throttle = false;
        if(pasData.pasOn){
          displayData.pas = true;
          throttleValue = displayVariables.ui8_assist_level/5.0f*100;
        }else{
          displayData.pas = false;
          throttleValue = 1;
        }
      }
    }
  }
  driveData.throttleProcentual = throttleValue;

}


/*
TODO:
  LCD:
    READ:
        Fix P and C values
    
    Write:
        Calibrate speedo

RESET DISPLAY DID INIT AFTER 1sec of no data

*/