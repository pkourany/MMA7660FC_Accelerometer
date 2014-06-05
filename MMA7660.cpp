/*****************************************************************************/
//	Function:	 Cpp file for class MMC7660 
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 10,2013
//	Version: v0.9b
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/
//#include <Wire.h>
#include "MMA7660.h"
/*Function: Write a byte to the register of the MMA7660*/
void MMA7660::write(uint8_t _register, uint8_t _data)
{
	//Wire.begin();
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.write(_register);   
	Wire.write(_data);
	Wire.endTransmission(true);
}
/*Function: Read a byte from the regitster of the MMA7660*/
uint8_t MMA7660::read(uint8_t _register)
{
	uint8_t data_read;
	//Wire.begin();
	Wire.beginTransmission(MMA7660_ADDR);
	Wire.write(_register); 
	Wire.endTransmission(false);
	//Wire.beginTransmission(MMA7660_ADDR);
	Wire.requestFrom(MMA7660_ADDR, 1, false);
	while(Wire.available())
	{
		data_read = Wire.read();
	}
	Wire.endTransmission(true);
	return data_read;
}


uint8_t MMA7660::writeAddrWriteData(uint8_t deviceAddress, uint8_t addr, const unsigned char* buf, int length)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(addr);
    for (int i=0;i<length;i++)
    	Wire.write(buf[i]);
    Wire.endTransmission();
    return length;
}
  
uint8_t MMA7660::writeAddrReadData(uint8_t deviceAddress, uint8_t addr, unsigned char* buf, int length)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(addr);
    Wire.endTransmission(false);  // Don't issue a stop because we need to read the value.
    Wire.requestFrom((uint8_t) deviceAddress, (uint8_t) length, (uint8_t) false);   
    for (int i=0;i<length;i++,buf++) *buf = Wire.read();
    Wire.endTransmission(true);  // Now issue the stop
    return length;
}


void MMA7660::init()
{
	Wire.begin();
	setMode(MMA7660_STAND_BY);
	setSampleRate(AUTO_SLEEP_32);
	setMode(MMA7660_ACTIVE);
}
void MMA7660::setMode(uint8_t mode)
{
	write(MMA7660_MODE,mode);
}
void MMA7660::setSampleRate(uint8_t rate)
{
	write(MMA7660_SR,rate);
}
/*Function: Get the contents of the registers in the MMA7660*/
/*          so as to calculate the acceleration.            */
void MMA7660::getXYZ(int8_t *x,int8_t *y,int8_t *z)
{
	unsigned char val[3];
	int count = 0;
  	val[0] = val[1] = val[2] = 64;
	while(Wire.available() > 0)
		Wire.read();
	Wire.requestFrom(MMA7660_ADDR,3);
	while(Wire.available())  
  	{
    	if(count < 3)
    	{
	      	while ( val[count] > 63 )  // reload the damn thing it is bad
	        {
	          val[count] = Wire.read();
	        }
    	}
        count++;
  	}
	*x = ((int8_t)(val[0]<<2))/4;
  	*y = ((int8_t)(val[1]<<2))/4;
  	*z = ((int8_t)(val[2]<<2))/4;
}

void MMA7660::getAcceleration(float *ax,float *ay,float *az)
{
	int8_t x,y,z;
	getXYZ(&x,&y,&z);
	*ax = (float)(x/21.00);
	*ay = (float)(y/21.00);
	*az = (float)(z/21.00);
}

