#ifndef XXS_FOO_H
#define XXS_FOO_H

class Foo
{
public:
	Foo(int value);
	~Foo();
public:
	int add(int, int);
	void setV(int);
	int getV();
private:
	int _value;
};
#endif
