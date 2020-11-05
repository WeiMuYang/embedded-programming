/*******************************************************************************
Copyright (c) 1983-2016 Advantech Co., Ltd.
********************************************************************************
THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY INFORMATION
WHICH IS THE PROPERTY OF ADVANTECH CORP., ANY DISCLOSURE, USE, OR REPRODUCTION,
WITHOUT WRITTEN AUTHORIZATION FROM ADVANTECH CORP., IS STRICTLY PROHIBITED. 

================================================================================
REVISION HISTORY
--------------------------------------------------------------------------------
$Log:  $

--------------------------------------------------------------------------------
$NoKeywords:  $
*/
/******************************************************************************
*
* Windows Example:
*    StaticDI.cpp
*
* Example Category:
*    DIO
*
* Description:
*    This example demonstrates how to use Static DI function.
*
* Instructions for Running:
*    1. Set the 'deviceDescription' for opening the device. 
*	  2. Set the 'profilePath' to save the profile path of being initialized device. 
*    3. Set the 'startPort' as the first port for Di scanning.
*    4. Set the 'portCount' to decide how many sequential ports to operate Di scanning.
*
* I/O Connections Overview:
*    Please refer to your hardware reference manual.
*
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include "../../../inc/bdaqctrl.h"
#include "../inc/compatibility.h"
//-----------------------------------------------------------------------------------
// Configure the following parameters before running the demo
//-----------------------------------------------------------------------------------
typedef unsigned char byte;
#define  deviceDescription  L"DemoDevice,BID#0"
const wchar_t* profilePath = L"../../profile/DemoDevice.xml";
int32    startPort = 0;
int32    portCount = 1;

void waitAnyKey()
{
	do {SLEEP(1);} while (!kbhit());
}

int main(int argc, char*argv[])
{
	int i;
	byte  dataBuffer[64] = {0};//the first element of this array is used for start port
   //uint8 data = 0;//data is used to the 'InstantDiCtrl_ReadBit'.
   //int   bit = 0;//bit is used to the 'InstantDiCtrl_ReadBit'.
   IArray* dioPort = NULL;
	ErrorCode  ret = Success;
   wchar_t enumString[256];
   
	//Step 1: Create a 'InstantDiCtrl' for DI function.
	InstantDiCtrl * instantDiCtrl = InstantDiCtrl_Create();
	do 
	{
		// Step 2: select a device by device number or device description and specify the access mode.
		// in this example we use ModeWrite mode so that we can fully control the device, including configuring, sampling, etc.
		DeviceInformation devInfo;
		devInfo.DeviceNumber = -1;
		devInfo.DeviceMode = ModeWrite;
		devInfo.ModuleIndex = 0;
		wcscpy(devInfo.Description, deviceDescription);
		ret = InstantDiCtrl_setSelectedDevice(instantDiCtrl, &devInfo);
		CHK_RESULT(ret);
      ret = InstantDiCtrl_LoadProfile(instantDiCtrl, profilePath);//Loads a profile to initialize the device.
      CHK_RESULT(ret);

		// Step 3: Read DI ports' status and show.
      //set port dircetion
      //dioPort = InstantDiCtrl_getPorts(instantDiCtrl);
      //ret = DioPort_setDirectionMask((DioPort*)Array_getItem(dioPort, 0), Input);//Setting port0 direction  
      //CHK_RESULT(ret);

		printf(" Reading ports' status is in progress, any key to quit !\n\n");
		do 
		{
			ret = InstantDiCtrl_ReadAny(instantDiCtrl, startPort, portCount, dataBuffer);
         /************************************************************************/
         //ret = InstantDiCtrl_ReadBit(instantDiCtrl, startPort, bit, &data);
         //NOTE:
         //argument2:which port you want to contrl? For example, startPort is 0.
         //argument3:which bit you want to control? You can write 0--7, any number you want.
         //argument4:data is used to save the result.
         /************************************************************************/     
  			CHK_RESULT(ret);
			//Show ports' status
			for (i = startPort; i < startPort + portCount; ++i)
			{
				printf(" DI port %d status is: 0x%X\n\n", i, dataBuffer[i - startPort]);
            //printf(" DI port %d status is: 0x%X\n\n", i, data);//for 'InstantDiCtrl_ReadBit'
			}
			SLEEP(1);
		} while (!kbhit());
	} while (FALSE);

	// Step 4: Close device and release any allocated resource.
	InstantDiCtrl_Dispose(instantDiCtrl);

	// If something wrong in this execution, print the error code on screen for tracking.
	if (BioFailed(ret))
	{
      AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
      printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
		waitAnyKey();
	}
	return 0;
}