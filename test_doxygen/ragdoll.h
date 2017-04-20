#ifndef _RAGDOLL_H_
#define _RAGDOLL_H_
#include"cat.h"


//! A ragdoll class
/*!
 * A ragdoll class in detail
 */
class ragdoll:public cat
{
public:
//! I'm a ragdoll
	ragdoll();
//! I will say meow~~~~~~~~
	virtual int make_a_sound();
};

#endif
