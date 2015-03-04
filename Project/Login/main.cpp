#include <stdio.h>

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};


lua_State *L;
int luaAdd(int x, int y)
{

	int sum=0;
	lua_getglobal(L, "add");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	if(lua_pcall(L, 2, 1, 0)){
		printf("call error is:%s.\n", lua_tostring(L, -1));
		return 0;
	}
	sum = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return sum;
}
void luaWidth_Height()
{
	int width=0,height=0;
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if(!lua_isnumber(L, -2)){
		printf("width must be a number..\n");
		return ;
	}
	if(!lua_isnumber(L, -1)){
		printf("height must be a number..\n");
		return ;
	}
	width = lua_tonumber(L, -2);
	height = lua_tonumber(L, -1);
	printf("width:%d, height:%d\n", width, height);
	lua_pop(L,1);
	lua_pop(L,1);
}
void luaBkground()
{
	int r=0,g=0,b=0,a=0;
	lua_getglobal(L,"bkground");
	if(!lua_istable(L, -1)){
		printf("bkground must be table..\n");
		return;
	}
	lua_getfield(L, -1, "r");
	if (lua_isnumber(L, -1)){
		r = (int)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "g");
	if (lua_isnumber(L, -1)){
		g = lua_tonumber(L, -1);
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "b");
	if (lua_isnumber(L, -1)){
		b = lua_tonumber(L, -1);
	}
	lua_pop(L, 1);

	lua_pushnumber(L,212);
	lua_setfield(L, -2, "a");

	lua_getfield(L, -1, "a");
	if (lua_isnumber(L, -1)){
		a = (int)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);

	printf("r:%d  g:%d  b:%d  a:%d\n",r,g,b,a);
	lua_pop(L, 1);
}
void luaNewbk()
{
	lua_newtable(L);
	lua_pushnumber(L,1);
	lua_setfield(L,-2,"r");

	lua_pushnumber(L,2);
	lua_setfield(L,-2,"g");

	lua_pushnumber(L,3);
	lua_setfield(L,-2,"b");
	lua_setglobal(L,"background");
}
static int lua_add(lua_State* lua)
{
	//检查栈中的参数是否合法，1表示Lua调用时的第一个参数(从左到右)，依此类推。
	//如果Lua代码在调用时传递的参数不为number，该函数将报错并终止程序的执行。
	double op1 = luaL_checknumber(lua, 1);
	double op2 = luaL_checknumber(lua, 2);
	//将函数的结果压入栈中。如果有多个返回值，可以在这里多次压入栈中。
	lua_pushnumber(lua, op1+op2);
	return 1;
}
static int lua_sub(lua_State* lua)
{
	double op1 = luaL_checknumber(L,1);
	double op2 = luaL_checknumber(L,2);
	lua_pushnumber(L,op1 - op2);
	return 1;
}
int main(int argc, char *argv[])
{
	int sum = 0;
	L = luaL_newstate();
	luaopen_base(L);
	luaL_openlibs(L);
	//将指定的函数注册为Lua的全局函数变量，其中第一个字符串参数为Lua代码
	//在调用C函数时使用的全局函数名，第二个参数为实际C函数的指针。
	lua_register(L, "add2", lua_add);
	lua_register(L, "sub2", lua_sub);
	if(luaL_loadfile(L, "./script/main.lua")||lua_pcall(L,0,0,0)){
		printf("Loadfile Error:%s\n", lua_tostring(L, -1));
		getchar();
		return 0;
	}
	sum = luaAdd(11,22);
	printf("c++->lua:sum:%d\n", sum);
	luaWidth_Height();
	luaBkground();
	luaNewbk();

	lua_getglobal(L, "main");
	if(lua_pcall(L,0,0,0)){
		printf("CallMain Error:%s\n", lua_tostring(L, -1));
		getchar();
		return 0;
	}
	lua_close(L);
	getchar();
	return 0;
}