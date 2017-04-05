#include"read_guard.h"
#include<pthread.h>
#include<iostream>
#include<sstream>

using namespace std;

read_guard guard;
vector<string> test_v;
pthread_mutex_t cursor_mutex;
int cursor(0);

void create_vector(vector<string>& test_v)
{
	ostringstream oss;
	for(int i = 0; i<100000 ; i++)
	{
		oss.clear();
		oss<<rand();
		test_v.push_back(oss.str());
	}
}

void* query(void*)
{
	while(1)
	{
		rand()%cursor;
	}
}

void push_back_once()
{
	pthread_mutex_lock(&cursor_mutex);
	guard.push_back(test_v[cursor]);
	cursor++;
	pthread_mutex_unlock(&cursor_mutex);
}

void erase_once()
{
	pthread_mutex_lock(&cursor_mutex);
	if(cursor>10)
	{
		int a = rand()%cursor;
		test_v.erase(test_v.begin()+a);
	}
	pthread_mutex_unlock(&cursor_mutex);
}


int main(int argc, char* argv[])
{
	pthread_mutex_init(&cursor_mutex);
	create_vector(test_v);
	pthread_t querier, virefier, eraser, assigner, push_backer, finder;
	pthread_create(&querier, NULL, query, NULL);
	pthread_create(&virefier, NULL, virefy, NULL);
	pthread_create(&eraser, NULL, erase, NULL);
	pthread_create(&assigner, NULL, assign, NULL);
	pthread_create(&push_backer, NULL, push_back, NULL);
	pthread_create(&finder, NULL, find, NULL);

}




