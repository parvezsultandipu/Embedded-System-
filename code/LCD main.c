#include <18F4431.h>                                                          
#device  ADC = 10                                                          
#fuses   HS, NOWDT                                                      
#use     delay(clock=4MHZ) 
#include "piclcd.c"

// Define pin connections
#define RED_LED    PIN_C0
#define PINK_LED   PIN_B0
#define GREEN_LED  PIN_B1
#define BUZZER     PIN_B2   // Buzzer connected to B2

float adc_value, voltage;

void beep_buzzer(int times)
{
   int i;
   for(i = 0; i < times; i++)
   {
      output_high(BUZZER);
      delay_us(500);          // 1kHz tone ? High for 500µs
      output_low(BUZZER);
      delay_us(500);          // Low for 500µs
   }
}

void main()
{
   lcd_init();                          
   setup_adc(ADC_CLOCK_INTERNAL);       
   setup_adc_ports(ALL_ANALOG);         

   // Turn off all outputs initially
   output_low(RED_LED);                 
   output_low(PINK_LED);
   output_low(GREEN_LED);
   output_low(BUZZER);                 

   while(TRUE)                          
   {     
     set_adc_channel(0);                
     delay_us(10);                      
     adc_value = read_adc();            
     voltage = (adc_value * 5.0) / 1023.0;  

     lcd_gotoxy(1, 1);
     printf(LCD_PUTC, "Voltage = %.2fV", voltage);  

     // Case 1: Low Voltage
     if(voltage < 2.5)
     {
         lcd_gotoxy(1, 2);
         printf(LCD_PUTC, "Low Voltage    ");

         output_toggle(RED_LED);        
         output_low(PINK_LED);          
         output_low(GREEN_LED);         
         beep_buzzer(100);              // Beep buzzer 100 cycles
     }
     // Case 2: High Voltage
     else if(voltage > 4.0)
     {
         lcd_gotoxy(1, 2);
         printf(LCD_PUTC, "High Voltage   ");

         output_toggle(PINK_LED);       
         output_low(RED_LED);           
         output_low(GREEN_LED);         
         beep_buzzer(100);              // Beep buzzer 100 cycles
     }
     // Case 3: Normal Voltage
     else
     {
         lcd_gotoxy(1, 2);
         printf(LCD_PUTC, "Normal Voltage ");

         output_low(RED_LED);           
         output_low(PINK_LED);          
         output_toggle(GREEN_LED);      
         output_low(BUZZER);            // Buzzer off
         delay_ms(500);                 // Delay only in this case
     }

     lcd_clear();                       
   }  
}

