/*------------------------------------------------------------------------*/
/**
 * @file	smsutils.c
 * @brief   Sudden Motion Sensor Utilties
 *
 * @author  M.Nukui
 * @date	2006-05-03
 *
 * Copyright (C) 2006 M.Nukui All rights reserved.
 */

/*
 * ===== LICENSE =====
 * 
 * Copyright 2006 Makoto Nukui. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *   and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *   conditions and the following disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <mach/mach_port.h>

#include "smsutils.h"


typedef struct {
	int8_t		x;
	int8_t		y;
	int8_t		z;
} sms_data1_t;

typedef struct {
	int16_t		x;
	int16_t		y;
	int16_t		z;
} sms_data2_t;

static sms_service_t	sms_services[] = {
	{	"SMCMotionSensor",		5,	40,	40,		2,	255 },	// MacBook, MacBook Pro
	{	"IOI2CMotionSensor",	21,	60,	60,		1,	127 },	// PowerBook G4, iBook G4
	{	"PMUMotionSensor",		21,	60,	60,		1,	127	},	// Hi Res Powerbook

	{	NULL,					0,	0,	0,		0,	0	}
};

kern_return_t smsOpen(sms_t * sms)
{
	sms_service_t *	service;
	kern_return_t	result;
	mach_port_t		masterPort;
	io_object_t		device;

	memset(sms, 0, sizeof(sms_t));

	result = IOMasterPort(MACH_PORT_NULL, &masterPort);
	if (result != kIOReturnSuccess) return result;

	for (service = sms_services; service->name; service++)
	{
		CFMutableDictionaryRef	matchingDictionary = IOServiceMatching(service->name);
		io_iterator_t			iterator;

		if (! matchingDictionary) continue;

		result = IOServiceGetMatchingServices(masterPort, matchingDictionary, &iterator);
		if (result != kIOReturnSuccess) continue;

		device = IOIteratorNext(iterator);
		IOObjectRelease(iterator);

		if (device)
		{
			result = IOServiceOpen(device, mach_task_self(), 0, &sms->connect);
			IOObjectRelease(device);

			if (result == kIOReturnSuccess)
			{
				sms->service = service;
				sms->unit = 90.0 / service->maxValue;
				break;
			}
		}
		else
		{
			result = -1;
		}
	}

	if (masterPort) 
		mach_port_deallocate(mach_task_self(), masterPort);

	return result;
}


kern_return_t smsClose(sms_t * sms)
{
	if (sms->connect)
		return IOServiceClose(sms->connect);
	else
		return kIOReturnSuccess;
}

kern_return_t smsGetData(sms_t * sms, sms_data_t * data)
{
	size_t		structureOutputSize;
	sms_service_t *	service;
	void *			inputStructure;
	void *			outputStructure;
	kern_return_t	result;

	service = sms->service;

	inputStructure = alloca(service->inputSize);
	outputStructure = alloca(service->outputSize);

	memset(inputStructure, 0, service->inputSize);
	memset(outputStructure, 0, service->outputSize);

	structureOutputSize = service->outputSize;
	
	result = IOConnectCallMethod(
				sms->connect,
				service->kernFunc,
				NULL, 0, // scalar input size/pointer
				inputStructure, service->inputSize, 
				NULL, NULL, // scalar output size pointer/pointer
				outputStructure, &structureOutputSize);
						
	if (result == kIOReturnSuccess)
	{
		if (service->type == 2)
		{
			sms_data2_t *	p = (sms_data2_t *)outputStructure;
			
			data->x = p->x;
			data->y = p->y;
			data->z = p->z;
		}
		else
		{
			sms_data1_t *	p = (sms_data1_t *)outputStructure;

			data->x = p->x;
			data->y = p->y;
			data->z = p->z;
		}
	}

	return result;
}
