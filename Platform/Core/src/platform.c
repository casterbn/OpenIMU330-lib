/** ***************************************************************************
 * @file platform.c 
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *****************************************************************************/
/*******************************************************************************
Copyright 2018 ACEINNA, INC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <string.h>
#include <stdint.h>

//*****************************
#include "BitStatus.h"
#include "platform_version.h"
#include "board.h"
#include "timer.h"

int     userSerialChan;
int     gpsSerialChan;
int     debugSerialChan;

BOOL fSPI  = FALSE;
BOOL fUART = FALSE;

BITStatusStruct     gBitStatus;


void    platformEnableExternalSync()
{
    board_configure_sync_pin();
    ConfigureTimerForSyncCapture();
}

BOOL platformHasMag()
{
    return FALSE;
}


BOOL _useExtSync = FALSE;

BOOL platformIsExtSyncUsed(void)
{
    return _useExtSync;
}

void platformEnableExtSync(BOOL enable)
{
    _useExtSync = enable;
}

char *platformBuildInfo()
{
    return SOFTWARE_PART;
}

uint32_t _commType = UART_COMM;

uint32_t platformGetUnitCommunicationType()
{
    return _commType;
}

void platformSetUserCommunicationType(uint32_t type)
{
    if(type != UART_COMM && type != SPI_COMM && type != CAN_BUS){
        return;
    }
    _commType = type;
}

int platformGetSysRange()
{
    return _400_DPS_RANGE;
}

void   platformUpdateInterfaceTestStatus(BOOL fGood)
{
    if(!fGood){
        gBitStatus.hwBIT.bit.test2Error         = 1;
        gBitStatus.hwBIT.bit.testError          = 1;
        gBitStatus.BITStatus.bit.hardwareError  = 1;
        gBitStatus.BITStatus.bit.hardwareStatus = 1;
    }

    gBitStatus.swStatus.bit.testMode        = 1;
    gBitStatus.BITStatus.bit.softwareStatus = 1;
    gBitStatus.BITStatus.bit.masterStatus   = (gBitStatus.BITStatus.all & 0x1E00) ? 1 : 0;
}

void   platformUpdateSensorCheckStatus(BOOL fAccelError, BOOL fRateError)
{
    if(!gBitStatus.hwBIT.bit.AccelerometerError){
        gBitStatus.hwBIT.bit.AccelerometerError = fAccelError;  // sticky
    }

    if(!gBitStatus.hwBIT.bit.GyroError){
        gBitStatus.hwBIT.bit.GyroError = fRateError;            // sticky
    }

    if(gBitStatus.hwBIT.bit.AccelerometerError || gBitStatus.hwBIT.bit.GyroError){
        gBitStatus.BITStatus.bit.hardwareStatus = 1;            // sticky
        gBitStatus.BITStatus.bit.sensorStatus   = 1;            // sticky
    }
}

void   platformSetSensorFaultStatus()
{
    gBitStatus.hwBIT.bit.AccelerometerError = 1;            // sticky
    gBitStatus.hwBIT.bit.GyroError          = 1;            // sticky
    gBitStatus.BITStatus.bit.hardwareStatus = 1;            // sticky
    gBitStatus.BITStatus.bit.masterFail     = 1;            // sticky
    gBitStatus.BITStatus.bit.sensorStatus   = 1;            // sticky
}

void platformSetInvalidCalStatus()
{
    gBitStatus.BITStatus.bit.softwareStatus      = 1;            // sticky
    gBitStatus.swDataBIT.bit.calibrationCRCError = 1;            // sticky
    gBitStatus.BITStatus.bit.masterStatus        = 1;            // sticky
}

void    platformInitBITStatus()
{
    memset(&gBitStatus, 0, sizeof(gBitStatus));
}    




BOOL bootMode = FALSE;

BOOL platformIsInBootMode()
{
    return bootMode;
}

void platformSetMode(BOOL isBoot)
{
    bootMode = isBoot;
}

char *platformGetModelInfo()
{
    return SOFTWARE_PART;
}


/*end void initConfigureUnit(void) */
