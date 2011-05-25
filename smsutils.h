/*------------------------------------------------------------------------*/
/**
 * @file	smsutils.h
 * @brief   Sudden Motion Sensor
 *
 * @author  M.Nukui
 * @date	2006-05-03
 *
 * Copyright (C) 2006 M.Nukui All rights reserved.
 */

/**
 * 
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


#ifndef	SMSUTILS_H
#define	SMSUTILS_H

#include <IOKit/IOKitLib.h>
#include <stdint.h>

typedef struct {
	const char *	name;
	unsigned int	kernFunc;
	IOItemCount		inputSize;
	IOByteCount		outputSize;
	int				type;
	int				maxValue;
} sms_service_t;

typedef struct {
	io_connect_t	connect;
	sms_service_t *	service;
	float			unit;
} sms_t;


typedef struct {
	int		x;
	int		y;
	int		z;
} sms_data_t;

#ifdef __cplusplus
extern "C" {
#endif


kern_return_t smsOpen(sms_t * sms);
kern_return_t smsClose(sms_t * sms);
kern_return_t smsGetData(sms_t * sms, sms_data_t * data);


#ifdef __cplusplus
}
#endif


#endif /* SMSUTILS_H */
