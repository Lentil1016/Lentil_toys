#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include<iostream>

//! Base class animal
/*!
 * # Markdown?
 * ## Test Markdown
 *
 * |Test|Markdown|
 * |-|-|
 * |Key|value|
 * |Key|Value|
 *
 * ```
 * #!/bin/bash
 * echo RTFM
 * ```
 *
 *
 *
 * */
class animal
{
public:
//! Default constructor.
	animal();
//! Guess who?
	virtual int make_a_sound()=0;
};

#endif
