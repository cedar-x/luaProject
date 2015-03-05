
#ifndef XXS_FOOWRAPPER_H
#define XXS_FOOWRAPPER_H

class FooWrapper;
typedef int (FooWrapper::*FooPoint)(lua_State*);

struct RegType
{
	const char* name;
	FooPoint mfunc;
};


class FooWrapper :public Foo
{
public:
	FooWrapper(lua_State* lua):Foo(luaL_checknumber(lua, -1)){}
	//FooWrapper(lua_State* lua):Foo(3){}
	//~FooWrapper(void);
	int add(lua_State* lua);
	int setV(lua_State* lua);
	int getV(lua_State* lua);
	static RegType Functions[3];
	//static FooPoint aa;
};


#endif
