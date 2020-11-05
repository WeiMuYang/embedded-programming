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
*    StaticDO.cpp
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
#include "../inc/compatibility.h"
#include "../../../inc/bdaqctrl.h"
using namespace Automation::BDaq;
//-----------------------------------------------------------------------------------
// Configure the following parameters before running the demo
//-----------------------------------------------------------------------------------
typedef unsigned char byte;

#define  deviceDescription  L"USB-5830,BID#0"
const wchar_t* profilePath = L"";
int32    startPort = 0;
int32    portCount = 1;

inline void waitAnyKey()
{
   do{SLEEP(1);} while(!kbhit());
} 

int main(int argc, char* argv[])
{
   ErrorCode        ret = Success;
   // Step 1: Create a instantDoCtrl for DO function.
   InstantDoCtrl * instantDoCtrl = InstantDoCtrl::Create();
   do
   {
      // Step 2: Select a device by device number or device description and specify the access mode.
      // in this example we use ModeWrite mode so that we can fully control the device, including configuring, sampling, etc.
      DeviceInformation devInfo(deviceDescription);
      ret = instantDoCtrl->setSelectedDevice(devInfo);
      CHK_RESULT(ret);
      ret = instantDoCtrl->LoadProfile(profilePath);//Loads a profile to initialize the device.
      CHK_RESULT(ret);
      
      // Step 3: Write DO ports
      //Set port direction
      //Array<DioPort>* dioPort = instantDoCtrl->getPorts();
      //ret = dioPort->getItem(0).setDirectionMask(Output); //Setting port0 direction
      //CHK_RESULT(ret);
      byte  bufferForWriting[64] = {0};//the first element is used for start port
      //uint32 data = 0;//the data is used to the 'WriteBit';
      //int bit = 1;//the bit is used to the 'WriteBit';
      for (int32 i = startPort; i < portCount + startPort; ++i)
      {
         uint32 inputVal = 0;
         printf(" Input a 16 hex number for DO port %d to output(for example, 0x00): ", i);
         scanf("%x", &inputVal);
         bufferForWriting[i-startPort] = (byte)inputVal; 
      }
      for (int i = 0; i < 16; ++i)
          printf("%d ", bufferForWriting[i]);
      printf("\n");


      ret = instantDoCtrl->Write(startPort, portCount, bufferForWriting );
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 1);
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 0);
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 1);
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 0);
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 1);
      ret = instantDoCtrl->WriteBit(startPort, 0x02, 0);

      /************************************************************************/
      /*printf(" Input value: ");
      //scanf("%d", &data);//for 'WriteBit'
      //ret = instantDoCtrl->WriteBit(startPort, bit, data);
      //NOTE:
		//This function is used to write digital data to the specified DO channel immediately
      //argument1:which port you want to contrl? For example, startPort is 0.
      //argument2:which bit you want to control? You can write 0--7, any number you want.
      //argument3:What status you want, open or close? 1 menas open, 0 means close.*/
      /************************************************************************/
      CHK_RESULT(ret);
      printf("\n DO output completed !");

      // Read back the DO status. 
      // Note: 
      // For relay output, the read back must be deferred until the relay is stable.
      // The delay time is decided by the HW SPEC.
      // BYTE bufferForReading[64] = {0};
      // ret = instantDoCtrl->Read( startPort,portCount,bufferForReading );
      // if(BioFailed(ret))
      // {
      //    break;
      // }
      // Show DO ports' status
      // for ( LONG i = startPort;i < portCount + startPort; ++i)
      //{
      //    printf("Now, DO port %d status is:  0x%X\n\n", i, bufferForReading[i-startPort]);
      //}
   }while(false);

	// Step 4: Close device and release any allocated resource.
	instantDoCtrl->Dispose();

	// If something wrong in this execution, print the error code on screen for tracking.
   if(BioFailed(ret))
   {
      wchar_t enumString[256];
      AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
      printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
   }
   waitAnyKey();
   return 0;
}
