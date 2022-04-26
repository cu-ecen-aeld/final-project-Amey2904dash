//Name   : i2ctemp.c
//Author : Varun Mehta, Amey Dashaputre, Tanmay Kothale
//Date   : 04/26/2022
//brief  : Reads the temperature value measured by the I2C based TMP102 sensor itnerfaced to RPi via I2C-1 
//References : https://www.kernel.org/doc/Documentation/i2c/dev-interface
//Additional reference : 1. https://github.com/ControlEverythingCommunity/TMP112/blob/master/C/TMP112.c
//			  2. https://github.com/cu-ecen-aeld/final-project-VenkatTata/blob/master/tempsensor/i2ctemp.c

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#define TEMP_FILE_PATH "/etc/face-rec-sample/temp.txt" 

void main() 
{
	// Create I2C bus
	int file;
	
	//Always adapter 1 on RPi
	char *i2c_dev_filename = "/dev/i2c-1";
	file = open(i2c_dev_filename, O_RDWR);
	if(file < 0) 
	{
		syslog(LOG_ERR,"Failed to open the i2c-1 bus");
		exit(1);
	}

	//TMP102 I2C address is 0x48(72)
	int addr= 0x48; 
	
	//set the address of the device to address
	if(ioctl(file, I2C_SLAVE, 0x48) < 0)
	{
		syslog(LOG_ERR,"Failed to set the address of the device to address");
		exit(1);
	}
	
	int temp_fd; //for a file to open and write temperature value in it
	
	// Select configuration register(0x01)
	// Continous Conversion mode, 12-Bit Resolution
	char buf[3] = {0};
	buf[0] = 0x01;
	buf[1] = 0x60;
	buf[2] = 0xA0;
	write(file, buf, 3);

	//Wait for the transaction to complete and sensor to initialise and perform measurement
	sleep(1);
	
	while(1)
	{
		//open the file to store temperature
		temp_fd = open(TEMP_FILE_PATH, O_WRONLY);
		
		//On completing the measurement, the values can be read
		char reg[1] = {0x00};
		write(file, reg, 1);

		//read the measured temperature value
		char measured_temp[2] = {0};
		int rc = read(file, measured_temp, 2);
		if( rc != 2)
		{
			syslog(LOG_ERR,"i2c read transaction failed");
			printf("i2c read transaction failed %d",rc);
			exit(1);
		}
		
		//Convert register values to temperature measurements
		int temp = (measured_temp[0] * 256 + measured_temp[1]) / 16;
		if(temp > 2047)
		{
			temp -= 4096;
		}
		
		//Convert recorded value from sensor to a readable value
		int final_temp = (int) (temp * 0.0625);
		
		char temp2[2]; //to convert the temp in string and store it to file
		sprintf(temp2, "%d", final_temp);	//converting from int to string
		
		//Log the result on temperature (for Debug)
		//printf("Temperature in Celsius : %d degree C\n\r", final_temp);
		//printf("Temperature in Celsius Temp2 : %s degree C\n\r", temp2);
		
		//sleep for 1 msec
		//usleep(1000);
		
		//write the data to file
		write(temp_fd, temp2, sizeof(temp2));
		
		//sleep for 1 second
		usleep(1000000);
		
		//close the file
		close(temp_fd);
	}

}
