#ifndef _CAT_H_
#define _CAT_H_
#include"animal.h"

//! A cat class
/*!
 * A cat will say meow
 */
class cat:public animal
{
public:
//! I'm a cat
	cat();
//! I will say meow
	virtual int make_a_sound();
};

#endif
