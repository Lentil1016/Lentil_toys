#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include<iostream>

//! Base class animal
class animal
{
public:
//! Default constructor.
	animal();
//! Guess who?
	virtual int make_a_sound()=0;
};

#endif
