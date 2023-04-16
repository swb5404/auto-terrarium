/**
  Section: Included Files
*/

#define FCY 3685000UL

#include <p33EP64MC502.h>       // pic33ep64mc502 library
#include <libpic30.h>              // pic30 library
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>  
#include "EE200_LCD.h"
#include "mcc_generated_files/mcc.h"

#define ADR_TEMP 0x4C      // temperature sensor address

int round(float x)
{
    return (int)(x + 0.5);
}

int main(void)
{  
    // initialize the device
    SYSTEM_Initialize();
    Init_LCD_Module();
    
    LATAbits.LATA1 = 1;
    __delay_ms(1000);
    LATAbits.LATA1 = 0;
    
    
    uint8_t aht20_init_cmd[] = {0xBE, 0x08, 0x00, 0xBA}; // COMMAND TEMP INIT
    uint8_t aht20_meas_cmd[] = {0xAC, 0x33, 0x00}; // COMMAND TEMP MEASURE
   
    uint8_t data[7]; // buffer to hold the data
    char print_value[32];
    uint32_t raw_temp_value;
    int trans_temp_value;

    i2c_writeNBytes(0x38, aht20_init_cmd, sizeof(aht20_init_cmd)); // INIT TEMP

     // Wait for initialization to complete
    __delay_ms(200);
    
    
    while (1)
    {
        
        
        
        // Send the command to initiate measurement
        i2c_writeNBytes(0x38, aht20_meas_cmd, sizeof(aht20_meas_cmd));
        // Wait for measurement to complete
        __delay_ms(100);

        // Read the temperature and humidity data
        i2c_readDataBlock(0x38, 0x01, data, sizeof(data));
        
        
        uint8_t temp_raw3 = data[3];
        uint8_t temp_raw4 = data[4];
        uint8_t temp_raw5 = data[5];
        
        uint8_t first_four = temp_raw3 & 0x0F;
    
        raw_temp_value = (first_four << 16) | (temp_raw4 << 8) | (temp_raw5);
        trans_temp_value = (int)round(((float)raw_temp_value / (1 << 20)) * 200.0 - 50.0);
       
        //char format_spec[] = "%hhu";
        
        sprintf(print_value, "%i", trans_temp_value);
        
        
        
        Position_LCD_Cursor(0x00);
        Write_LCD_String(print_value);
                
        char print_blank[] = {"         "};
        Position_LCD_Cursor(0x00);
        Write_LCD_String(print_blank);
        //Position_LCD_Cursor(0x40);
        //Write_LCD_String(print_blank);

        ClrWdt();
        
    }
    return 0; 
}
/**
 End of File
*/
