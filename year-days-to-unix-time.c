/*
 * year-days-to-unix-time.c
 *
 * Simple example of converting a year and day-of-year combo into a time_t.
 * Probably horribly broken outside the tested cases.
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
#include<time.h>

time_t calc_sec(int year, int day)
{
	int days = 365 * (year - 1970) + day;
	
	// Calculate the number of leap years between 1970 and year.
	// 1972 is the first leap year, so 1973 is the first year after a leap day.
	int leap_days = ((year - 1973) / 4);

	time_t seconds_since_1970 = 86400 * ( days + leap_days );
	
	//printf(" %d ", leap_days);
	
	return seconds_since_1970;
}

char* show_time(int year, int day, int seconds)
{
	time_t sec = calc_sec(year, day) + (seconds % 86400);
	return asctime(gmtime(&sec));
}

int main()
{
	int year;
	int sec = 3600; // 1am

	for (year = 2000; year <= 2017; year++) {
		int leap_year = (year % 4 == 0);
		printf("%d %s", year, show_time(year, 1, sec));      // expect jan 1
		printf("%d %s", year, show_time(year, 31+29, sec));  // expect feb 29 or mar  1
		printf("%d %s", year, show_time(year, 100, sec));	   // expect apr  9 or apr 10
		printf("%d %s", year, show_time(year, leap_year ? 366:365, sec)); // expect dec 31
		printf("\n");
	}


	return 0;
}
