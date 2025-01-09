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
#include <stdio.h>
static const uint8_t EEPROM_ADD = 0x54<<1; 
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
 ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void print(int,char*);
void Voltage_test(int);
//void spi_test();

char  input_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'},write[4],buf[50];
int byte10, byte9, byte8, byte7, byte6, byte5, byte4, byte3, byte2, byte1,i,j,k,len;
char RX[4],Write[4];

/*
void controller_relay(){
	
HAL_GPIO_WritePin(GPIOA, SEN_ON_OUT_Pin, GPIO_PIN_RESET);	
HAL_GPIO_WritePin(GPIOA, LED_CHK_EN_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA, D1_A_EN_Pin,    GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA, D2_A_EN_Pin,    GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA, D3_A_EN_Pin,    GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA, FEDR_EN_Pin,    GPIO_PIN_RESET);
}
*/

void independent_rly(int x1,int x2,int x3, int x4,int x5,int x6){
HAL_GPIO_WritePin(GPIOA, SEN_ON_OUT_Pin, x1);	
HAL_GPIO_WritePin(GPIOA, LED_CHK_EN_Pin, x2);
HAL_GPIO_WritePin(GPIOA, D1_A_EN_Pin,    x3);
HAL_GPIO_WritePin(GPIOA, D2_A_EN_Pin,    x4);
HAL_GPIO_WritePin(GPIOA, D3_A_EN_Pin,    x5);
HAL_GPIO_WritePin(GPIOA, FEDR_EN_Pin,    x6);
}

void MUX1_select(int a3,int a2,int a1,int a0)
{
//	len = sprintf(buf,"\n"); print(len,buf);
  HAL_GPIO_WritePin(GPIOA,M1_A0_Pin,a0);
	HAL_GPIO_WritePin(GPIOB,M1_A1_Pin,a1);
	HAL_GPIO_WritePin(GPIOC,M1_A2_Pin,a2);
	HAL_GPIO_WritePin(GPIOB,M1_A3_Pin,a3);
}


void MUX2_select(int A3,int A2,int A1,int A0)
{
//  len = sprintf(buf,"\n"); print(len,buf);
  HAL_GPIO_WritePin(GPIOB,M2_A0_Pin,A0);
	HAL_GPIO_WritePin(GPIOB,M2_A1_Pin,A1);
	HAL_GPIO_WritePin(GPIOB,M2_A2_Pin,A2);
	HAL_GPIO_WritePin(GPIOB,M2_A3_Pin,A3);
}

void mux1(int select)
{
	  independent_rly(0,0,0,0,0,0); HAL_Delay(50);
	  MUX1_select(0,0,0,0); HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);

	switch(select)//3100
	{
		//case  0: MUX1_select(0,0,0,0); HAL_Delay(50); len = sprintf(buf,"VA_EN\n"); print(len,buf); break;            //6100
		//case  1: MUX1_select(0,0,0,1); HAL_Delay(50); len = sprintf(buf,"VB_EN\n"); print(len,buf); break;            //6101
		case  2: MUX1_select(0,0,1,0); HAL_Delay(50); len = sprintf(buf,"(RX_+2.6V)\n"); print(len,buf); break;  //    //6102
		case  3: MUX1_select(0,0,1,1); HAL_Delay(50); len = sprintf(buf,"15V(J1.1)_EN\n"); print(len,buf); break;   //6103
		case  4: MUX1_select(0,1,0,0); HAL_Delay(50); len = sprintf(buf,"3V3_UUT_EN\n"); print(len,buf); break; ////6104
		case  5: MUX1_select(0,1,0,1); HAL_Delay(50); len = sprintf(buf,"(TX_+15V)\n"); print(len,buf); break; // ////6105
		case  6: MUX1_select(0,1,1,0); HAL_Delay(50); len = sprintf(buf,"5V(J1.5)_EN\n"); print(len,buf); break; //6106
		case  7: MUX1_select(0,1,1,1); HAL_Delay(50); len = sprintf(buf,"5V(J3.3)_EN\n"); print(len,buf); break; //6107
	  //case  8: MUX_select(1,0,0,0); HAL_Delay(50);  len = sprintf(buf,"Emty\n"); print(len,buf); break; 
	  //case  9: MUX_select(1,0,0,1); HAL_Delay(50); len = sprintf(buf,"Emty\n"); print(len,buf); break;
	  //case 10: MUX1_select(1,0,1,0); HAL_Delay(50); len = sprintf(buf,"SEN-ON_EN\n"); print(len,buf); break;
	 	case 11: MUX1_select(1,0,1,1); HAL_Delay(50); len = sprintf(buf,"PLG_EN\n"); print(len,buf); break;  ////610B
		//case 12: MUX1_select(1,1,0,0); HAL_Delay(50); len = sprintf(buf,"NCS_EN\n"); print(len,buf); break;
		//case 13: MUX1_select(1,1,0,1); HAL_Delay(50); len = sprintf(buf,"MISO_EN\n"); print(len,buf); break;
		case 14: MUX1_select(1,1,1,0); HAL_Delay(50); len = sprintf(buf,"TS6B_Voltage\n"); print(len,buf); break; //610E
		case 15: MUX1_select(1,1,1,1); HAL_Delay(50); len = sprintf(buf,"TS6A_Voltage\n"); print(len,buf); break; //610F
		default: len = sprintf(buf,"Invalid\n"); print(len,buf); break;
	}
}



void mux2(int select)
{
	independent_rly(0,0,0,0,0,0); HAL_Delay(50);
	  MUX1_select(0,0,0,0); HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);
	switch(select)//
	{
		//case  0: MUX2_select(0,0,0,0); HAL_Delay(50); len = sprintf(buf,"VPK2_EN\n"); print(len,buf); break; //6200  //RLY
		case  1: MUX2_select(0,0,0,1); HAL_Delay(50); len = sprintf(buf,"VPK1_EN\n"); print(len,buf); break; //6201  //RLY
		case  2: MUX2_select(0,0,1,0); HAL_Delay(50); len = sprintf(buf,"V1_EN\n"); print(len,buf); break; //6202 //RLY
		case  3: MUX2_select(0,0,1,1); HAL_Delay(50); len = sprintf(buf,"V2_EN\n"); print(len,buf); break; //6203//RLY
		case  4: MUX2_select(0,1,0,0); HAL_Delay(50); len = sprintf(buf,"V3_EN\n"); print(len,buf); break; //6204//RLY
		case  5: MUX2_select(0,1,0,1); HAL_Delay(50); len = sprintf(buf,"V4_EN\n"); print(len,buf); break; //6205 //RLY
		case  6: MUX2_select(0,1,1,0); HAL_Delay(50); len = sprintf(buf,"V5_EN\n"); print(len,buf); break; //6206 //RLY 
		case  7: MUX2_select(0,1,1,1); HAL_Delay(50); len = sprintf(buf,"PLS_EN\n"); print(len,buf); break; //6207 //RLY
		case  8: MUX2_select(1,0,0,0); HAL_Delay(50); len = sprintf(buf,"(RX_J1.16) RDY HI\n"); print(len,buf); break; //6208// RLY
		case  9: MUX2_select(1,0,0,1); HAL_Delay(50); len = sprintf(buf,"(RX_J1.14) TS6-THK HI\n"); print(len,buf); break; //6209 //RLY
		case 10: MUX2_select(1,0,1,0); HAL_Delay(50); len = sprintf(buf,"(RX_J1.12) TS6-BLK HI\n"); print(len,buf); break; //620A // RLY
		case 11: MUX2_select(1,0,1,1); HAL_Delay(50); len = sprintf(buf,"(RX_J1.10) SP-FST HI\n"); print(len,buf); break; //620B //RLY
		case 12: MUX2_select(1,1,0,0); HAL_Delay(50); len = sprintf(buf,"(RX_J1.8) SP-MV HI\n"); print(len,buf); break; //620C //RLY
		case 13: MUX2_select(1,1,0,1); HAL_Delay(50); len = sprintf(buf,"(RX_J1.6) SP-BLK HI\n"); print(len,buf); break; //620D //RLY
		case 14: MUX2_select(1,1,1,0); HAL_Delay(50); len = sprintf(buf,"(RX_J1.4) U-DFD HI\n"); print(len,buf); break; //620E //RLY
		case 15: MUX2_select(1,1,1,1); HAL_Delay(50); len = sprintf(buf,"(RX_J1.2) U-BLK HI\n"); print(len,buf); break; //620F //RLY
		default: len = sprintf(buf,"Invalid\n"); print(len,buf); break;
	}
}

void led_on_test(int led,int sel)
{
//	len = sprintf(buf,"LED_ON TEST\n"); print(len,buf);
	independent_rly(0,0,0,0,0,0); HAL_Delay(50);
	  MUX1_select(0,0,0,0); HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);
	
	if(led == 1)
	{
		switch(sel)
		{
		case 1:  independent_rly(0,1,1,0,0,0); HAL_Delay(50); len = sprintf(buf,"(D1) - ON\n"); print(len,buf); break;   //6511
		case 2:  independent_rly(0,1,0,1,0,0); HAL_Delay(50); len = sprintf(buf,"(D2) - ON\n"); print(len,buf); break;   //6512
		case 3:  independent_rly(0,1,0,0,1,0); HAL_Delay(50); len = sprintf(buf,"(D3) - ON\n"); print(len,buf); break;   //6513
		default: HAL_UART_Transmit(&huart1,(uint8_t*)"invalid input\n",16,100); break;
		}
	}
}


void CONTROLLER_rly(int select)
{	
	  MUX1_select(0,0,0,0); HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);
	switch(select)//
	{
	  case  0: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(1,0,0,0,0,0); len = sprintf(buf,"SEN_ON_OUT\n"); print(len,buf); break;  //6300
		case  1: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(0,1,0,0,0,0); len = sprintf(buf,"LED_CHK_EN\n"); print(len,buf); break;            //6301
		case  2: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(0,0,1,0,0,0); len = sprintf(buf,"(D1) - OFF\n"); print(len,buf); break;      //6302
		case  3: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(0,0,0,1,0,0); len = sprintf(buf,"(D2) - OFF\n"); print(len,buf); break;   //6303
		case  4: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(0,0,0,0,1,0); len = sprintf(buf,"(D3) - OFF\n"); print(len,buf); break; ////6304
		case  5: independent_rly(0,0,0,0,0,0); HAL_Delay(25); independent_rly(0,0,0,0,0,1); len = sprintf(buf,"FEDR_EN\n"); 	print(len,buf); break; ////6305
		default:   len = sprintf(buf,"invalied\n"); print(len,buf); break; //610
	}
}

void I2C_TEST()
{
	HAL_StatusTypeDef ret;
	
	  independent_rly(0,0,0,0,0,0); //HAL_Delay(50);
	  MUX1_select(0,0,0,0); //HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);
	//HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
  ret = HAL_I2C_Master_Transmit(&hi2c2,EEPROM_ADD,0x00,1,HAL_MAX_DELAY);
 // len = sprintf(buf,"STATUS  %d\n",ret); print(len,buf);
	if(ret == HAL_OK) { len = sprintf(buf,"EEPROM address check Pass = %d\n",ret); print(len,buf); }
	else{ len = sprintf(buf,"EEPROM address check FAIL = %d\n",ret); print(len,buf); }
}

void Ground_rdy_test(int b){
	
	int i;
	independent_rly(0,0,0,0,0,0); //HAL_Delay(50);
	MUX1_select(0,0,0,0); //HAL_Delay(50);
	MUX2_select(0,0,0,0); HAL_Delay(50);
	
	HAL_GPIO_WritePin(GPIOA, FEDR_EN_Pin,1);
	HAL_Delay(50);
  switch(b){
		case 0: MUX2_select(1,0,0,0); HAL_Delay(50); len = sprintf(buf,"(RX_J1.16) G_RDY lo\n"); print(len,buf);	break;  //6700
	  case 1: MUX2_select(1,0,0,1); HAL_Delay(50); len = sprintf(buf,"(RX_J1.14) TS6-THK HI\n"); print(len,buf); break;  //6701
	  default:   len = sprintf(buf,"invalied\n"); print(len,buf); break; 
	}
}




void spi_test()
{
	float val = 2;
  uint8_t var,SPI_cmd_word[]={0x00,0x00,0x00};
	var = (uint8_t)(val*255)/2.5;
	
 // SPI_cmd_word[0] = 0x00;
	SPI_cmd_word[0] = 0x30 | 0x00; // internal reference selection
  SPI_cmd_word[1] = 0xff;
	SPI_cmd_word[2] = 0x00;
	
  len = sprintf(buf, "DAC SPI Test\n");print(len,buf);
	
	 HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
  HAL_SPI_Transmit(&hspi2,SPI_cmd_word,3,HAL_MAX_DELAY);
	//HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);
	
		while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
	for(i=0;i<3;i++){
		len = sprintf(buf, "DAC Status ok: %d hex: %x\n",SPI_cmd_word[i],SPI_cmd_word[i]);print(len,buf);
	 }
	
	/*
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
	SPI_cmd_word[0] = 0x20 | 0x0F; // SELECT CHANNEL reference selection
  SPI_cmd_word[1] = var;
	SPI_cmd_word[2] = 0x00;
	 HAL_SPI_Transmit(&hspi2,SPI_cmd_word,sizeof(SPI_cmd_word),HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);
	 while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
	len = sprintf(buf, "DAC Status ok2\n");print(len,buf);*/
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
//{ HAL_UART_Receive_IT(&huart1,(uint8_t*)RX,4); }

/*void SPI_Interface(){
	
	uint8_t val1 = 0x30;
	uint8_t val2[2];
	int t=0;
	
	
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET); // chip select high
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_SET); // chip select low
	HAL_Delay(2000);
 
	for(t=0;t<2;t++){
		val2[t]=(val1>>(4*t))&0x0f;
		len = sprintf(buf, "-->1 %x\n",val2[t]);print(len,buf);
		HAL_SPI_Transmit(&hspi2,val2,1,1000); // send each 4 bit command and address
	}
	HAL_SPI_Transmit(&hspi2,&val1,1,1000); // send each 4 bit command and address
	uint8_t val3=0xff;
	HAL_SPI_Transmit(&hspi2,&val3,1,1000);// send dummy data
	uint8_t val4=0x00;
	HAL_SPI_Transmit(&hspi2,&val4,1,1000);// send dummy data
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);// chip select high
	HAL_Delay(500);
	

	val1=0x00;
	val2[0]=0;
	val2[1]=0;
	
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_SET);
	
  for(t=0;t<2;t++){
		val2[t]=(val1>>(4*t))&0x0f;
		len = sprintf(buf, "-->2 %x\n",val2[t]);print(len,buf);
		HAL_SPI_Transmit(&hspi2,val2,1,1000); // send each 4 bit command and address
	}
	
	HAL_SPI_Transmit(&hspi2,&val1,1,1000);
	uint8_t val5=0xff;
	HAL_SPI_Transmit(&hspi2,&val5,1,1000);
	uint8_t val6=0x00;
	HAL_SPI_Transmit(&hspi2,&val6,1,1000);
	HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_RESET);
}*/

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
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	
	//  HAL_GPIO_WritePin(GPIOB,DAC_CS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(Rly_15v_GPIO_Port,Rly_15v_Pin,GPIO_PIN_RESET); //HAL_Delay(50); // relay OFF
    HAL_GPIO_WritePin(GPIOA,FEDR_EN_Pin,GPIO_PIN_RESET); 
    independent_rly(0,0,0,0,0,0); //HAL_Delay(50);
	  MUX1_select(0,0,0,0); //HAL_Delay(50);
		MUX2_select(0,0,0,0); 
		HAL_Delay(50);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//  HAL_UART_Receive_IT(&huart1,(uint8_t*)RX,4); 
	
  while (1)
  {
		
	//		HAL_UART_Transmit(&huart1,(uint8_t*)"Enter The Number\n",16,100);
		 HAL_UART_Receive(&huart1,(uint8_t*)RX,4,HAL_MAX_DELAY);
// HAL_UART_Receive_IT(&huart1,(uint8_t*)RX,4); 
		for(i=0;i<4;i++){  write[i] = RX[i]; 
 //  len = sprintf(buf,"RX[%d] = %c write[%d] = %c\n",i,RX[i],i,write[i]); print(len,buf);
		}
		//  HAL_UART_Transmit(&huart1,(uint8_t*)RX,sizeof(RX),HAL_MAX_DELAY);
		 
					  for (i = 0; i < 4; i++) 
					  {
						  for (j = 0; j <= 15; j++) 
						  {
							  if (write[i] == input_array[j]) 
							  {
								  switch (i) 
								  {
									  case 0 :
									  byte10 = j; //len = ssprintf(buf,"\n%d ",j); HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,100);
									  break;
									  case 1 :
									  byte9 = j;  //len = ssprintf(buf,"%d ",j); HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,100);
									  break;
									  case 2 :
									  byte8 = j;  //len = ssprintf(buf,"%d ",j); HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,100);
									  break;
									  case 3 :
									  byte7 = j;  //len = ssprintf(buf,"%d\n",j); HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,100);
									  break;
									  case 4 :
									  byte6 = j; //sprintf("%d ",j);
									  break;
									  case 5 :
									  byte5 = j; //sprintf("%d ",j);
									  break;
									  case 6 :
									  byte4 = j; //sprintf("%d ",j);
									  break;
									  case 7 :
									  byte3 = j; //sprintf("%d ",j);
									  break;
									  case 8 :
									  byte2 = j;  //sprintf("%d ",j);
									  break;
									  case 9 :
									  byte1 = j; // sprintf("%d ",j);
									  break;
									  default :
									// HAL_UART_Transmit(&huart1,(uint8_t*)"invalid input\n",13,100);
									  break;
								  }
							  }
						  }
					  } 
 
							 if(byte10 == 6)
				       {
						    switch(byte9)
								{
							   case 0: Voltage_test(byte8); break;//6010 -> on 6000 // 0ff
				         case 1: mux1(byte7);  break; //6100 to 610F
				         case 2: mux2(byte7);  break; //6200 to 620F 
								 case 3: CONTROLLER_rly(byte7);  break; //6300 to 6305 
								 case 4: I2C_TEST(); break;//6400
								 case 5: led_on_test(byte8,byte7); break;//6511 TO 6513
								 case 6: spi_test(); break; // 6610
								 case 7: Ground_rdy_test(byte7); break; //6700 to 
						     default: HAL_UART_Transmit(&huart1,(uint8_t*)"s2_invalid input\n",16,100); break;
							  }
							 }
								 else
								 {
									//HAL_UART_Transmit(&huart1,(uint8_t*)"in else invalid input\n",24,100); 
								 }
								 //	for(i=0;i<4;i++){  write[i] = RX[i] = 0x00;  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_1LINE;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Rly_15v_Pin|M1_A2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SEN_ON_OUT_Pin|LED_CHK_EN_Pin|D1_A_EN_Pin|D2_A_EN_Pin
                          |D3_A_EN_Pin|FEDR_EN_Pin|M1_A0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, M2_A3_Pin|M2_A2_Pin|DAC_CS_Pin|M2_A1_Pin
                          |M1_A1_Pin|M2_A0_Pin|M1_A3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : Rly_15v_Pin M1_A2_Pin */
  GPIO_InitStruct.Pin = Rly_15v_Pin|M1_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SEN_ON_OUT_Pin LED_CHK_EN_Pin D1_A_EN_Pin D2_A_EN_Pin
                           D3_A_EN_Pin FEDR_EN_Pin M1_A0_Pin */
  GPIO_InitStruct.Pin = SEN_ON_OUT_Pin|LED_CHK_EN_Pin|D1_A_EN_Pin|D2_A_EN_Pin
                          |D3_A_EN_Pin|FEDR_EN_Pin|M1_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : M2_A3_Pin M2_A2_Pin M2_A1_Pin M1_A1_Pin
                           M2_A0_Pin M1_A3_Pin */
  GPIO_InitStruct.Pin = M2_A3_Pin|M2_A2_Pin|M2_A1_Pin|M1_A1_Pin
                          |M2_A0_Pin|M1_A3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DAC_CS_Pin */
  GPIO_InitStruct.Pin = DAC_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DAC_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void Voltage_test(int A)
{
	 float Current=0,vin=0; //output = 0.0; const double ACSOffset = 2.5;
	  if(A == 1)
		{
			len = sprintf(buf,"Power ON\n"); print(len,buf);
			HAL_GPIO_WritePin(Rly_15v_GPIO_Port,Rly_15v_Pin,GPIO_PIN_SET); // relay ON
		HAL_Delay(50);
		/*HAL_ADC_Start(&hadc1); 
    HAL_ADC_PollForConversion(&hadc1, 100); 
    uint16_t adc_val = HAL_ADC_GetValue(&hadc1);
			//len = sprintf(buf,"pixcel = %d\n",adc_val); print(len,buf); //
    float vin = (float)(adc_val*(5/4095)); // voltage
	  Current = (vin/20)/0.025;
    HAL_ADC_Stop(&hadc1); 
    HAL_Delay(20); 
	
	     len = sprintf(buf,"*5V voltage*%0.2f*V*\n",vin); print(len,buf);
       len = sprintf(buf,"*5V current*%0.2f*A*\n",Current); print(len,buf);*/
		}
		else if(A == 0)
		{
				HAL_GPIO_WritePin(Rly_15v_GPIO_Port,Rly_15v_Pin,GPIO_PIN_RESET); // relay OFF
		independent_rly(0,0,0,0,0,0); //HAL_Delay(50);
	  MUX1_select(0,0,0,0); //HAL_Delay(50);
		MUX2_select(0,0,0,0); HAL_Delay(50);
			len = sprintf(buf,"Power OFF\n"); print(len,buf);
		}
}
void print(int l,char *b)
{
	 HAL_UART_Transmit(&huart1,(uint8_t*)b,l,250);
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
