class A
{
public:
	A()
	{
		pthread_mutex_init(&_mutex);
	}

	~A()
	{
		pthread_mutex_lock(&_mutex);
		//do something
		pthread_mutex_destory(&_mutex);
	}

	do_something()
	{
		pthread_mutex_lock(&_mutex);
		//do something
		pthread_mutex_unlock(&_mutex);
	}
private:
	pthread_mutex_t _mutex;
}
