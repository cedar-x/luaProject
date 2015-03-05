
// code from: http://www.cnblogs.com/ringofthec/archive/2010/10/26/luabindobj.html
#include <stdio.h>
#include "Foo.h"

Foo::Foo(int value)
{
	_value = value;
	printf("Foo Constructor!\n");
}
Foo::~Foo()
{
	printf("Foo Destructor!\n");
}
int Foo::add(int op1, int op2)
{
	return op1+op2;
}
void Foo::setV(int val)
{
	_value = val;
}
int Foo::getV()
{
	return _value;
}