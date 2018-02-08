/*
 * thread-test.c
 *
 * A POSIX threads scheduler experiment
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
#include<sched.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mtx;

void* th1(void *arg)
{
	while(1) {
		pthread_mutex_lock(&mtx);
		puts("th1 locked");
		sleep(1);
		puts("th1 unlocked");
		pthread_mutex_unlock(&mtx);
		sched_yield(); // needed or th2 never gets time.
	}
}


void* th2(void *arg)
{
	while(1) {
		puts("th2 waiting");
		pthread_mutex_lock(&mtx);
		puts("th2 got it");
		pthread_mutex_unlock(&mtx);
		sleep(1);
	}
}

int main()
{
	pthread_t pth1;
	pthread_t pth2;

	pthread_mutex_init(&mtx, NULL);
	pthread_create(&pth1, NULL, th1, NULL);
	pthread_create(&pth2, NULL, th2, NULL);
	pthread_join(pth1, NULL);
	return 0;
}
