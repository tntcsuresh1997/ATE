/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "usart0_init.h"
#include "adc_init.h"
#include "Pwm_Read.h"
#include "I2C_Init.h"

#include <stdio.h>
#include <math.h>

#define Enable_5v  AVR32_PIN_PA20  //5v_En
/*
#define IN2_TP18   AVR32_PIN_PD23  //TP18
#define IN1_TP20   AVR32_PIN_PA12  //TP20
#define IN3_TP21   AVR32_PIN_PD24  //TP21
*/
//gpio_clr_gpio_pin(AVR32_PIN_PD23); //relay on for freq18
//gpio_clr_gpio_pin(AVR32_PIN_PA12); //relay on for freq20
//gpio_clr_gpio_pin(AVR32_PIN_PD24); //relay on for freq21


void pwm_measure(int);
void single_test(int);
void Relay5v_self(int);
void continues_ADC_Check(); 
void avrage(int , float []);
void avrage_c(int ,float []);
void avt1(int,int);
float digit_count(int);

int16_t adc_value[CH1];
//int16_t adc_value2[CH2];
float adc_arr[20],adc_arr_c[20];
float ADC,ADC0,ADC1;  //,,ADC3,ADC4,ADC5,ADC6,ADC7;
float adc1;
int16_t data=0; 

char buf[20];

//int16_t adc_values1[NO_ch2];

volatile avr32_tc_t *tc = EXAMPLE_TC;
//unsigned int input1_channel = 1,input0_channel = 2,ra=0;

int t = 0;
unsigned int input0_channel = 0,input1_channel = 1,input2_channel=2;
unsigned int sum_avr1=0,count1=0,count0=0,sum0=0,ra0=0,ra1=0,ra2=0,sum=0,l=0,sum1=0,sum2=0,sum_avr=0,count=0;
bool flag = false,flag1=false,flag0=false,xxx=false,yyy=false,zzz=false;

float freq = 0,s=0,s1=0,s2=0,freq0=0,freq1=0,freq2=0,total=0,total1=0,total2=0;




char  input_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'},write[4];
int byte10, byte9, byte8, byte7, byte6, byte5, byte4, byte3, byte2, byte1,i,j,k;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
     sysclk_init();
	 usart_init();
	 adc1_init();

	 /* init_tc_input_0(tc,input0_channel);
	  init_tc_input_1(tc,input1_channel);
	  init_tc_input_2(tc,input2_channel);
	  
	  tc_stop(tc,input0_channel);
	  tc_stop(tc,input1_channel);
	  tc_stop(tc,input2_channel);*/
	 
//	adcifa_start_sequencer(&AVR32_ADCIFA, ADCIFA_SEQ0);
	
while(1)
{
	  printf("\nEnter____for process\n");       
		 for(i=0;i<4;i++) { write[i] = USART1_Receive(); }
					  for (i = 0; i < 4; i++) 
					  {
						  for (j = 0; j <= 15; j++) 
						  {
							  if (write[i] == input_array[j]) 
							  {
								  switch (i) 
								  {
									  case 0 :
									  byte10 = j; //printf("%d ",j);
									  break;
									  case 1 :
									  byte9 = j;  //printf("%d ",j);
									  break;
									  case 2 :
									  byte8 = j;  //printf("%d ",j);
									  break;
									  case 3 :
									  byte7 = j; //printf("%d ",j);
									  break;
									  case 4 :
									  byte6 = j; //printf("%d ",j);
									  break;
									  case 5 :
									  byte5 = j; //printf("%d ",j);
									  break;
									  case 6 :
									  byte4 = j; //printf("%d ",j);
									  break;
									  case 7 :
									  byte3 = j; //printf("%d ",j);
									  break;
									  case 8 :
									  byte2 = j;  //printf("%d ",j);
									  break;
									  case 9 :
									  byte1 = j; // printf("%d ",j);
									  break;
									  default :
									  printf("Invalid Input\n");
									  break;
								  }
							  }
						  }
					  } 
				
				  if(byte10 == 3)
				  {
						     switch(byte9)
							 {
							   case 0:
							       printf("\n");  
								         gpio_set_gpio_pin(Enable_5v); 
								         delay_ms(15);
								  single_test(byte8); // 3020 3030 3040 3050 3060 3070
								   gpio_clr_gpio_pin(Enable_5v);
							     break;
							   case 1:
						          printf("\n");          //3110 -> 5v relay on, 3100 -> 5v Relay off
								  Relay5v_self(byte8);
						          break;
						       case 2:
						          printf("\n");          //3200
								  continues_ADC_Check(); //continues test
						          break;
							   case 3:
								  printf("\n"); 
								   gpio_set_gpio_pin(Enable_5v);
								    delay_ms(15);
								  pwm_measure(byte8); //3300 3310  3320
								   gpio_clr_gpio_pin(Enable_5v);
								// pwm_measure(byte8);
								  break;
							   case 4:
							          //i2c //3404 all top test in one mode
							      break;
						       default :
						          printf("Invalid Input__\n");
						          break;
							 }
					}
	}
}

void pwm_measure(int ch) 
{
	unsigned int ra0=0,ra1=0,ra2=0; //6_6_23 added
	const float fmin = 13.333333;
	const float fmax = 13.333333;
	const float tp18_fmin = 12.813;
	const float tp18_fmax = 13.689;
	
	float showf0=0,showf1=0,showf2=0;
	int c4=0,c2=0,c3=0;
	//bool check_18_flag=false;
	//tc_software_trigger(tc,input1_channel);//
	if(ch == 0){
	//	tc_start(tc,input1_channel);
	while(t<50)
	{
		tc_start(tc,input1_channel);
		 delay_us(300);
		while(l<20)
		{
			ra0 = tc_read_ra(tc,input1_channel);
		//	printf("RA0 = %d\n",ra0);     //
			//	delay_us(30);
			if(ra0 == 0) { yyy = true; break;}//
			if((ra0 != 0) && (ra0<=4))
			{
				sum=sum+ra0;
				flag0=true;
				count0++;
				l++;
			}
			else
			{
				sum0=sum0+ra0; l++;
			}
				//tc_stop(tc,input1_channel);
		}
		if(yyy){ yyy = false; break;}//
		tc_stop(tc,input1_channel);
		delay_us(50); //100
		t++;
	}
		if(flag0)
		{
			//printf("COUNT = %d\n",count0);
			total = (float)sum/count0;
			freq0 = (float)(40000000/total);
			showf0 = digit_count(freq0);
			//check_18_flag = true;
			printf("value: %0.6f\n",showf0);
				  printf("TP18 max = 13.689  Mhz min = 12.813 Mhz \n");
				  if(showf0 >= tp18_fmin && showf0 <= tp18_fmax){ printf("TP18 FREQUENCY *PASS*\n"); } else { printf("TP18 FREQUENCY *FAIL*\n"); }
			//	flag = false;
		}
		else{
		//	printf("1\n");
	//	printf("sum0 = %d",sum0);
			s = (float)sum0/20;
			freq = (float)(40000000/s);
			showf0 = digit_count(freq);
			//check_18_flag=true;
			printf("value: %0.6f\n",showf0);
			 printf("TP18 max = 13.689  Mhz min = 12.813 Mhz \n");
			 if(showf0 >= tp18_fmin && showf0 <= tp18_fmax){ printf("TP18 FREQUENCY *PASS*\n"); } else { printf("TP18 FREQUENCY *FAIL*\n"); }
		}
}
	t=0;
	l=0;
	sum0=0;
	sum=0;
	s=0.0;
	total=0.0;
	count0 =0;
	ra0=0;
	freq0=0;
	freq =0;
	showf0=0;
	
	//tc_software_trigger(tc,input1_channel);//
	 tc_sync_trigger(tc);
	//if(check_18_flag){
	//	check_18_flag = false;
	if(ch == 1){
	while(t<50){
		tc_start(tc,input0_channel);
		delay_us(30);
		while(l<250)
		{
			ra1 = tc_read_ra(tc,input0_channel);
		//	printf("RA1 = %d\n",ra1); //
			delay_us(100);
			if(ra1 == 0){ sum_avr1=0;zzz = false; break;} //
			if(ra1 != 0){
				if(ra1 == 3)
				{
					flag1 = true;
				//	sum_avr1=3;
					c3++;
					//count1++;
					l++;
					//printf("l:%d",l);
					//printf("inside if \n");
					//	printf("RA1 = %d\n",ra1);
				}
				else
				{
					sum1=sum1+ra1; l++;
				/*	printf("else l:%d",l);
					printf("inside else \n");
					//printf("else RA1 = %d\n",ra1);*/
				}
			}
		}
		if(zzz){ zzz = false; break;} //
		tc_stop(tc,input0_channel);
		delay_us(100);
		t++;
	}
	//printf("TP20 no of 3 = %d\n",c3);
		if((flag1 == true)&&(c3>=55))
		{
		//	printf("TP20_____ = %d\n",c3);
		//	printf("sum_avr = %d\n",sum_avr1);
			//total1 = (float)sum_avr1/count1;
			freq1 = (float)(40000000/3);
			showf1 = digit_count(freq1);
			printf("value: %0.6f\n",showf1);
			//printf("TP20 value: %f Hz\n",freq1);
			printf("TP20 max = 13.330 Mhz min = 13.330 Mhz \n");
			if(showf1 == fmin && showf1 == fmax){ printf("TP20 FREQUENCY *PASS*\n"); } else { printf("TP20 FREQUENCY *FAIL*\n"); }
                                 
			//	flag = false;
		}
		else{
		//	printf("2\n");
			s1 = (float)sum1/250;
			freq1 = (float)(40000000/s1);
			showf1 = digit_count(freq1);
			printf("value: %0.6f\n",showf1);
			//printf("TP20 value: %f Hz\n",freq1);
			printf("TP20 max = 13.330 Mhz min = 13.330 Mhz \n");
			if(showf1 == fmin && showf1 == fmax){ printf("TP20 FREQUENCY *PASS*\n"); } else { printf("TP20 FREQUENCY *FAIL*\n"); }

		}
}
//}
/*else{
	showf1 = 0.00;
	printf("value: %0.6f\n",showf1);
	//printf("TP20 value: %f Hz\n",freq1);
	printf("TP20 max = 13.330 Mhz min = 13.330 Mhz \n");
	if(showf1 == fmin && showf1 == fmax){ printf("TP20 FREQUENCY *PASS*\n"); } else { printf("TP20 FREQUENCY *FAIL*\n"); }
}*/
	t=0;
	l=0;
	sum1=0;
	sum_avr1=0;
	s1=0;
	total1=0;
//	count1 =0;
   ra1=0;
   freq1=0;
   c2=0;  //
   c3=0;  //
   c4=0;  //
	//tc_software_trigger(tc,input0_channel);//
	 tc_sync_trigger(tc);
	
	if(ch == 2){
	while(t<50){
		tc_start(tc,input2_channel);
		delay_us(30);
		
		while(l<250)
		{
			ra2 = tc_read_ra(tc,input2_channel);
	//		printf("RA2 = %d\n",ra2);        //
		//	delay_us(15);
		if(ra2 == 0){ xxx=true; break;}//
			if(ra2 != 0)
			{
				if(ra2 == 3) //if((ra2 >= 2)&&(ra2 <= 4))
				{
			//		printf("inside 3\n");
					flag = true;
				//	sum_avr=3;
						c4++;
					//count++;
					l++;
				}
				else
				{
					sum2=sum2+ra2; l++;
				}
			}
		}
		if(xxx){ xxx = false; break;} //
		tc_stop(tc,input2_channel);
		delay_us(100);
		t++;
	}
//	printf("TP21 no of 3 = %d\n",c4); //
		if((flag == true)&&(c4>=55))
		{
		//	printf("TP21____ 3 = %d\n",c4);
		//	printf("sum_avr = %d\n",sum_avr);
		//	total2 = (float)sum_avr/count;
			freq2 = (float)(40000000/3);
			showf2 = digit_count(freq2);
			printf("value: %0.6f\n",showf2);
			//printf("TP21 value: %f Hz\n",freq2);
			printf("TP21 max = 13.330 Mhz min = 13.330 Mhz \n");
			if(showf2 == fmin && showf2 == fmax){ printf("TP21 FREQUENCY *PASS*\n"); } else { printf("TP21 FREQUENCY *FAIL*\n"); }
			//	flag = false;
		}
		else{
			//	printf("total TP21 = %0.3f\n",total);
		//	printf("3\n");
	//	printf("SUM____ = %d\n",sum2);
			s2 = (float)sum2/250;
			freq2 = (float)(40000000/s2);
			showf2 = digit_count(freq2);
			printf("value: %0.6f\n",showf2);
			//printf("TP21 value: %f Hz\n",freq2);
			printf("TP21 max = 13.330 Mhz min = 13.330 Mhz \n");
			if(showf2 == fmin && showf2 == fmax){ printf("TP21 FREQUENCY *PASS*\n"); } else { printf("TP21 FREQUENCY *FAIL*\n"); }
		}
		
	}
	t=0;
	l=0;
	sum_avr=0;
	sum2=0;
	s2=0;
	total2=0;
	count = 0;
	ra2=0;
	freq2=0;
	 c2=0;  //
	 c3=0;  //
	 c4=0;  //
	 
	// tc_software_trigger(tc,input2_channel);//
	 tc_sync_trigger(tc);
}


void single_test(int D1)
{
	
	for(i=0;i<20;i++)
 {
	//adcifa_start_sequencer(&AVR32_ADCIFA, ADCIFA_SEQ0);
	if (adcifa_get_values_from_sequencer(&AVR32_ADCIFA,ADCIFA_SEQ0,&adcifa_sequence_opt,adc_value) == ADCIFA_STATUS_COMPLETED)
	{
		delay_ms(60);
		if(D1 == 2||3||4||5||6){  ADC = (5*(float)adc_value[D1]/2038); }
		    if(ADC < 0){ ADC = ADC*(-1); adc_arr[i] = ADC;}
		   else
		   { adc_arr[i] = ADC; }
    }
  }
	 avrage(D1,adc_arr);  
	 ADCIFA_clear_eos_sequencer_0();
}

void Relay5v_self(int RV)
{
	if(RV == 1)
	{
		gpio_set_gpio_pin(Enable_5v);
		delay_ms(1000);
		printf("Power ON\n");
		if (adcifa_get_values_from_sequencer(&AVR32_ADCIFA,ADCIFA_SEQ0,&adcifa_sequence_opt,adc_value) == ADCIFA_STATUS_COMPLETED)
		{	
			       printf("Input Resolution +5V : %d\n",adc_value[0]);
			       ADC0= (5*(float)adc_value[0]/2038);
			       printf("Input test +5V : %0.3f\n",ADC0);
			 
			      printf("Input Resolution -5V : %d\n",adc_value[1]);
			      ADC1= (5*(float)adc_value[1]/2038);  
				
			 //  if(adc_value[1] <= -200){  adc1 = (ADC1)*6.62;  printf("Input test -5V : %0.3f\n",adc1); }
			  // else { printf("Input test -5V : %0.3f\n",adc1); }
			  
			   ADCIFA_clear_eos_sequencer_0();
			   adcifa_start_sequencer(&AVR32_ADCIFA, ADCIFA_SEQ0);
		}	   
	}
	else if(RV == 0)
	{	gpio_clr_gpio_pin(Enable_5v); printf("Power OFF\n"); }
}

void continues_ADC_Check() //  continues test
{	
		 Relay5v_self(1);  //5v & -5v Relay on
		  printf("\n");  
		 
	for(k=2;k<=7;k++){
		adcifa_start_sequencer(&AVR32_ADCIFA, ADCIFA_SEQ0);
		for(j=0;j<20;j++){
		if (adcifa_get_values_from_sequencer(&AVR32_ADCIFA,ADCIFA_SEQ0,&adcifa_sequence_opt,adc_value) == ADCIFA_STATUS_COMPLETED)
		{
			     ADC = (5*(float)adc_value[k]/2038);
				 
				 if(ADC < 0){
					 ADC = ADC*(-1);   
					 adc_arr_c[j] = ADC; }
				 else
				 {  
					 adc_arr_c[j] = ADC;
				 }
		}
	   }  
	   avrage(k,adc_arr_c);
	}
	 ADCIFA_clear_eos_sequencer_0();
	  delay_us(30);
	  pwm_measure(0);
	  delay_us(30);
	  pwm_measure(1);
	   delay_us(30);
	  pwm_measure(2);
	 delay_us(30); //5
		
	   printf("\n");  
	   Relay5v_self(0);  //5v & -5v Relay off
}

void avrage(int channel,float *arr)
{
	const float tp3_max = 0.207, tp3_min = 0.027,tp5_max = 0.244, tp5_min = 0.070,tp6_max = 0.740, tp6_min = 0.725;
	const float tp9_max = 0.840, tp9_min = 0.515,tp10_max = 2.701, tp10_min = 2.451,tp20_max = 0.400, tp20_min = 0.100;
	int i;
	float sum=0,avr = 0;

	for(i=0;i<20;i++) { sum += arr[i]; }
	avr = (float)(sum/20);
	
	if(channel == 2){  
		printf("value: %0.3f \n",avr);
		printf("TP3 max = %0.3f V min = %0.3f V\n",tp3_max,tp3_min);
		if(avr >= tp3_min && avr <= tp3_max){  printf("TP3 adc *PASS*\n"); } else { printf("TP3 ADC *FAIL*\n"); } 
		 }   
		if(channel == 3){  printf("value: %0.3f \n",avr); 
			printf("TP5 max = %0.3f  V min = %0.3f V\n",tp5_max,tp5_min); 
		if(avr >= tp5_min && avr <= tp5_max){  printf("TP5 adc *PASS*\n"); } else { printf("TP5 ADC *FAIL*\n"); } 
		}    
			if(channel == 4){  printf("value: %0.3f \n",avr); 
				printf("TP6 max = %0.3f  V min = %0.3f V\n",tp6_max,tp6_min); 
			  if(avr >= tp6_min && avr <= tp6_max){  printf("TP6 adc *PASS*\n"); } else { printf("TP6 ADC *FAIL*\n"); } 
			   }    
				if(channel == 5){  printf("value: %0.3f \n",avr); 
					printf("TP9 max = %0.3f  V min = %0.3f V\n",tp9_max,tp9_min);  
				    if(avr >= tp9_min && avr <= tp9_max){  printf("TP9 adc *PASS*\n"); } else { printf("TP9 ADC *FAIL*\n"); } 
					}    
					if(channel == 6){  printf("value: %0.3f \n",avr);
						printf("TP10 max = %0.3f  V min = %0.3f V\n",tp10_max,tp10_min); 
					if(avr >= tp10_min && avr <= tp10_max){  printf("TP10 adc *PASS*\n"); } else { printf("TP10 ADC *FAIL*\n"); } 
					  }    
					/*	if(channel == 7){ printf("value: %0.3f \n",avr); 
							printf("TP20 max = %0.3f  V min = %0.3f V\n",tp20_max,tp20_min); 
						if(avr >= tp20_min && avr <= tp20_max){  printf("TP20 adc *PASS*\n"); } else { printf("TP20 ADC *FAIL*\n"); }  } */    
}
/*
void avt1(int ch,int a)
{
	float  s = (float)a/20;
	float freq = (float)(40000000/s);
	if(ch == 1){ printf("TP18_frequency test : %0.3f Hz\n",freq); }
	if(ch == 2){ printf("TP20_frequency test : %0.3f Hz\n",freq); }
}*/


float digit_count(int arg)
{
	int D_count=0;
	float T=0;
	int temp = arg; 
	while(arg != 0)
	{
		arg/=10;
		D_count++;
	}
	if(D_count <= 8)
	{
		
		if((D_count == 7)||(D_count==8)){

			T = (float)temp/1000000;
			//printf("frequency1 : %0.2f x 10^6 Hz",T);
			return T;

		}
		else if((D_count<7) &&(D_count>3)){

			T=(float)temp/1000;
			//printf("frequency1 :%0.2f x 10^3 Hz",T);
			return T;
		}
	}
}

	
