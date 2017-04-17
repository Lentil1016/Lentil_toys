#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<event.h>
#include<boost/bind.hpp>
#include<boost/function.hpp>
#include<iostream>

using namespace boost;
using namespace std;

void onTime(int sock, short event, void *arg)
{
	printf("hello world \n");
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	event_add((struct event*)arg, &tv);
}

int main()
{
	event_init();

	struct event ev_time;
	evtimer_set(&ev_time, onTime, &ev_time);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	event_add(&ev_time, &tv);

	event_dispatch();
	return 0;
}

