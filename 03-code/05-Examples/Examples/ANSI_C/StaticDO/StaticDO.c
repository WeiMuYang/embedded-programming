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
*    StaticDO.c
*
* Example Category:
*    DIO
*
* Description:
*    This example demonstrates how to use Static DO function.
*
* Instructions for Running:
*    1. Set the 'deviceDescription' for opening the device.
*	  2. Set the 'profilePath' to save the profile path of being initialized device. 
*    3. Set the 'startPort'as the first port for Do .
*    4. Set the 'portCount'to decide how many sequential ports to operate Do.
*
* I/O Connections Overview:
*    Please refer to your hardware reference manual.
*
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
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

int main(int argc, char* argv[])
{
   int i;
   uint32 inputVal = 0;
   byte dataBuffer[64] = {0};//the first element is used for start port
   //uint32 data = 0;//the data is used to the 'InstantDoCtrl_WriteBit';
   //int bit = 1;//the bit is used to the 'InstantDoCtrl_WriteBit';
   IArray* dioPort = NULL;
   ErrorCode ret = Success;
   wchar_t enumString[256];

   // Step 1: Create a instantDoCtrl for DO function.
   InstantDoCtrl * instantDoCtrl = InstantDoCtrl_Create();
   do 
   {
      // Step 2: Select a device by device number or device description and specify the access mode.
      // in this example we use ModeWrite mode so that we can fully control the device, including configuring, sampling, etc.
      DeviceInformation devInfo;
      devInfo.DeviceNumber = -1;
      devInfo.DeviceMode = ModeWrite;
      devInfo.ModuleIndex = 0;
      wcscpy(devInfo.Description, deviceDescription);
      ret = InstantDoCtrl_setSelectedDevice(instantDoCtrl, &devInfo);
      CHK_RESULT(ret);
      ret = InstantDoCtrl_LoadProfile(instantDoCtrl, profilePath);//Loads a profile to initialize the device.
      CHK_RESULT(ret);

      // Step 3: Write DO ports
      //set port dircetion
      //dioPort = InstantDoCtrl_getPorts(instantDoCtrl);
      //ret = DioPort_setDirectionMask((DioPort*)Array_getItem(dioPort, 0), Output);//Setting port0 direction    
      //CHK_RESULT(ret);
      for (i = startPort; i < portCount + startPort; ++i)
      {
         printf(" Input a 16 hex number for DO port %d to output(for example, 0x00): ", i);
         scanf("%x", &inputVal);
         dataBuffer[i - startPort] = (byte)inputVal;
      }
      ret = InstantDoCtrl_WriteAny(instantDoCtrl, startPort, portCount, dataBuffer);
      //***************************************************************************//
      /*printf(" Input value: ");
      scanf("%d", &data);//for 'WriteBit'.
      ret = InstantDoCtrl_WriteBit(instantDoCtrl, startPort, bit, data);*/
      //NOTE:
      //This function is used to write digital data to the specified DO channel immediately.
      //argument2:which port you want to contrl? For example, startPort is 0.
      //argument3:which bit you want to control? You can write 0--7, any number you want.
      //argument4:What status you want, open or close? 1 menas open, 0 means close.
      //**************************************************************************//
      CHK_RESULT(ret);
      printf("\n DO output completed !");
   } while (FALSE);

   // Step 4: Close device and release any allocated resource.
   InstantDoCtrl_Dispose(instantDoCtrl);

   // If something wrong in this execution, print the error code on screen for tracking.
   if (BioFailed(ret))
   {
      AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
      printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
   }
   waitAnyKey();
   return 0;
}