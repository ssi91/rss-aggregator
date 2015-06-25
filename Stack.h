//
// Created by ssi on 03.04.15.
//

#ifndef _TEMPLATESTACK_STACK_H_
#define _TEMPLATESTACK_STACK_H_

#include <glob.h>

template< class T >
class Stack
{
private:
	T *a;
	size_t count;
public:
	Stack(const int &_a);
	Stack();
	Stack(const Stack< T > &stack);
	~Stack();
	void push(const T &_a);
	T pop();
	void incTop();
	void decTop();
	size_t getCount();
	T &getTop();

	Stack< T > &operator=(Stack< T > &);
};

#include "Stack.cpp"

#endif //_TEMPLATESTACK_STACK_H_
