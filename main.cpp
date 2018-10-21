#include "mbed.h"

I2C i2c(I2C_SDA, I2C_SCL); //pins for I2C communication (SDA, SCL)
Serial pc(USBTX, USBRX);
DigitalOut Led(PH_0);	// withe led contol: 1 -> enable 0-> disable

// Device address:
int sensor_addr = 41 << 1; // 41 left-shifted by 1: (82)

//Status led
DigitalOut green(LED1);
//RGB led control: 0-> ON 1-> OFF
DigitalOut verde(PA_12);
DigitalOut rojo(PB_12);
DigitalOut azul(PA_9);

int main() {
    pc.baud(9600);
    green = 1; // off
		Led.write(false);
    // Connect to the Color sensor and verify whether we connected to the correct sensor. 
    
    i2c.frequency(100000);
    
    pc.printf("\n\rDone\n\r");
    
    char id_regval[1] = {146};
    char data[1] = {0};

    i2c.write(sensor_addr,id_regval,1, true);
    i2c.read(sensor_addr,data,1,false);
    
    if (data[0]==68) {
        green = 0;
        wait (2); 
        green = 1;
        } else {
        green = 1; 
    }
    
    // Initialize color sensor
    // Initialize the integration time to 614ms (max vlue possible)
    char timing_register[2] = {129,0}; // ATIME (0x01) 
    i2c.write(sensor_addr,timing_register,2,false);
    
		// Set the ADC gain to x1 factor.
    char control_register[2] = {143,0};
    i2c.write(sensor_addr,control_register,2,false);
    
		// Used to power the TCS3472 device on and off, and enable functions and interrupts.
		// in this case, 3 --> 0000 0011 means disble interruptions, enable RGBC sensor and enable oscillator.
    char enable_register[2] = {128,3};
    i2c.write(sensor_addr,enable_register,2,false);
    
    // Read data from color sensor (Clear/Red/Green/Blue)
    
    while (true) { 
			
				Led = 1;
				wait(0.1);  										//At least 100ms to read high values.
				char all_reg[1] = {148};				// Direction of first register address.
        char all_data[8] = {0,0,0,0};		// Initialize array char 
        i2c.write(sensor_addr,all_reg,1, true);
        i2c.read(sensor_addr,all_data,8, false);
				
				Led = 0;
				
				int clear_value = ((int)all_data[1] << 8) | all_data[0];
				int red_value		= ((int)all_data[3] << 8) | all_data[2];
				int green_value = ((int)all_data[5] << 8) | all_data[4];
				int blue_value 	= ((int)all_data[7] << 8) | all_data[6];
				
	// Alternative way to read color by color. 
				/*
        char clear_reg[1] = {148};
        char clear_data[2] = {0,0};
        i2c.write(sensor_addr,clear_reg,1, true);
        i2c.read(sensor_addr,clear_data,2, false);
		
        
        int clear_value = ((int)clear_data[1] << 8) | clear_data[0];
        
        char red_reg[1] = {150};
        char red_data[2] = {0,0};
        i2c.write(sensor_addr,red_reg,1, true);
        i2c.read(sensor_addr,red_data,2, false);
        
        int red_value = ((int)red_data[1] << 8) | red_data[0];
        
        char green_reg[1] = {152};
        char green_data[2] = {0,0};
        i2c.write(sensor_addr,green_reg,1, true);
        i2c.read(sensor_addr,green_data,2, false);
        
        int green_value = ((int)green_data[1] << 8) | green_data[0];
        
        char blue_reg[1] = {154};
        char blue_data[2] = {0,0};
        i2c.write(sensor_addr,blue_reg,1, true);
        i2c.read(sensor_addr,blue_data,2, false);
				Led = 0;
        
        int blue_value = ((int)blue_data[1] << 8) | blue_data[0];
				*/
				
        
        // print sensor readings
        
        pc.printf("\rClear (%d), Red (%d), Green (%d), Blue (%d)\n", clear_value, red_value, green_value, blue_value);
				if (red_value >= green_value && red_value >= blue_value){rojo = 0;verde=1;azul=1;}
				else if (blue_value >= green_value && blue_value >= red_value){azul = 0;verde=1;rojo=1;}
				else{verde =0;azul=1;rojo=1;}
        wait(0.4);
    }
    
}

