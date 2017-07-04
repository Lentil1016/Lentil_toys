#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include<iostream>

//! Base class animal

/** @file man_animal.h
 *  \verbinclude man_animal.h
 */

/*!

# markdown test
## markdown test

|markdown|test|
|-|-|
|key|value|

```
#!/bin/bash
echo "test markdown"
```

*/

class animal
{
public:
//! Default constructor.
	animal();
//! Guess who?
	virtual int make_a_sound()=0;
};

#endif
