#ifndef _CAT_H_
#define _CAT_H_
#include"animal.h"

class cat:public animal
{
public:
	cat();
	virtual int make_a_sound();
};

#endif
