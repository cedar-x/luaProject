
#include "mainHead.h"
#include "Foo.h"
#include "FooWrapper.h"

// FooWrapper::FooWrapper(void)
// {
// }
// FooWrapper::~FooWrapper(void)
// {
// }
// 
//RegType FooWrapper::Functions[3] = {};
RegType FooWrapper::Functions[3]={{"add",&(FooWrapper::add)}, {"setV",&(FooWrapper::setV)}, {"getV",&(FooWrapper::getV)}};
// FooWrapper::Functions[1].name = "setV";
// FooWrapper::Functions[1].mfunc = &FooWrapper.setV;
// FooWrapper::Functions[2].name = "getV";
// FooWrapper::Functions[2].mfunc = &FooWrapper.getV;
//FooPoint FooWrapper::aa = &(FooWrapper::add);

int FooWrapper::add(lua_State* lua)
{
	int a = luaL_checknumber(lua, -1);
	int b = luaL_checknumber(lua, -2);            
	int res = Foo::add(a, b);
	lua_pushnumber(lua, res);
	return 1;
}
int FooWrapper::setV(lua_State* lua)
{
	int v = luaL_checknumber(lua, -1);
	Foo::setV(v);
	return 0; 
}
int FooWrapper::getV(lua_State* lua)
{
	lua_pushnumber(lua, Foo::getV());
	return 1;
}


struct consumer
{
	int num;
	char name[18];
	char password[26];
	long yu_e;
};
class ATM
{ 
public:
	void in();
	void out();
	void changeP();
	void check();
	void give();
private:
	static consumer orange[2]; 
};
consumer ATM::orange[2]={{0, "zhangshang", "098988", 100},{0, "zhangshang", "098988", 100}};