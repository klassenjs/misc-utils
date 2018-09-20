/*
 * pa_devs.c
 *
 * List available PortAudio devices.
 *
 * Copyright (c) 2017 James Klassen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include<stdio.h>
#include<portaudio.h>

int main(int arvc, char** argv)
{
	int numDevices, i;
	const PaDeviceInfo *deviceInfo;

	if( Pa_Initialize() != paNoError)
		return(1);

	printf("Host APIs available: %d\n", Pa_GetHostApiCount());

	numDevices = Pa_GetDeviceCount();
	if(numDevices < 0) {	
		printf("Error: Pa_GetDeviceCount returned 0x%x\n", numDevices);
		return(1);
	}

	for( i = 0; i < numDevices; i++) {
		deviceInfo = Pa_GetDeviceInfo(i);

		if(deviceInfo != NULL) {
			printf("Found device %d:\n\tName: %s\n\thostApi: %d\n",
			       i, deviceInfo->name, deviceInfo->hostApi);
		}
	}

	Pa_Terminate();

	return 0;
}
