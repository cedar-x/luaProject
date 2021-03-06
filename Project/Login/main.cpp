
#include "mainHead.h"
#include "consoleLog.h"
#include "LuaPort.h"

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
void luaSystemAdd()
{
	lua_getglobal(L,"console");
	if(!lua_istable(L, -1)){
		printf("console must be table..\n");
		return;
	}
	lua_pushstring(L, "Add");
	lua_gettable(L, -2);
	lua_pushvalue(L, -2);
	lua_pushinteger(L, 11);
	lua_pushinteger(L, 3);
	if (lua_pcall(L, 3, 1,0)!=0)
	{
		printf("call console:Add failed..");
		return;
	}
	int sum = (int)lua_tonumber(L, -1);
	printf("console:add(11,3):%d\n", sum);
	lua_pop(L, 1);
	return ;
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

/////////////////////////
static int lua_print(lua_State* lua)
{
	const char* buff = luaL_checkstring(lua, -1);
	printf(buff);
	return 0;
}
static int lua_clib(lua_State* lua)
{
	const char* buff = luaL_checkstring(lua, -1);
	lua_pop(lua, 1);
	 
	int ref = luaL_ref(lua, LUA_REGISTRYINDEX);
	lua_rawgeti(lua, LUA_REGISTRYINDEX, ref);
	if(!lua_isfunction(lua, -1))
	{
		printf("call back function is no avlid:%d..\n", ref);
		return 0;
	}
	lua_pushstring(lua, buff);
	lua_pushcfunction(lua, (lua_CFunction)lua_print);
	if (lua_pcall(lua, 2, 0,0)){
		//const char* err = luaL_checkstring(lua, 1);
		//printf("%d, [err]:%s\n", lua_gettop(lua), err);
		printf("lua_clib  error:%s\n", lua_tostring(L, -1));
		return 0;
	}
	luaL_unref(lua, LUA_REGISTRYINDEX, ref);
	return 0;
}
static const luaL_Reg lengine[] = {
	{"clib", lua_clib},
	{NULL, NULL},
};

/////////////////////////

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
	//==
	LuaPort::RegisterClass(L);
	lua_pushnumber(L,0);
	LuaPort::constructor(L);
	//==
	consoleLog* console = new consoleLog(L);
	console->regFunc();
	////////////////////////
	//注册c++函数
	lua_register(L, "lua_clib", lua_clib);
	//luaL_register(L, "lengine", lengine);  lua5.2已经删除了LuaL_register函数
	////////////////////////
	lua_getglobal(L, "main");
	if(lua_pcall(L,0,0,0)){
		printf("CallMain Error:%s\n", lua_tostring(L, -1));
		getchar();
		return 0;
	}
	luaSystemAdd();
	lua_close(L);
	getchar();
	return 0;
}


/*


class CTest
{
public:
	CTest(){};
	virtual ~CTest(){};
	int Add(int x, int y)
	{
		printf("%p Add: x=%d, y=%d\n", this, x, y);
		return x + y;
	};
};

static int CreateCTest(lua_State* L)
{
	// 创建一个元表为CTest的Table——Lua对象
	*(CTest**)lua_newuserdata(L, sizeof(CTest*)) = new CTest();
	luaL_getmetatable(L, "CTest");
	lua_setmetatable(L, -2);
	return 1;
}

static int DestoryCTest(lua_State* L)
{
	// 释放对象
	delete *(CTest**)lua_topointer(L, 1);
	return 0;
}

static int CallAdd(lua_State* L)
{
	// 调用C++类方法的跳板函数。
	CTest* pT = *(CTest**)lua_topointer(L, 1);
	lua_pushnumber(L, pT->Add(lua_tonumber(L, 2), lua_tonumber(L, 3)));
	return 1;
}

int main(int argc, char * argv[])
{
	lua_State *L = luaL_newstate();
	luaopen_base(L);    

	// 往lua中注册类
	lua_pushcfunction(L, CreateCTest);    // 注册用于创建类的全局函数
	lua_setglobal(L,  "CTest");

	luaL_newmetatable(L, "CTest");           // 创建一个元表
	lua_pushstring(L, "__gc");                    // 垃圾回收
	lua_pushcfunction(L, DestoryCTest);
	lua_settable(L, -3);                               // 公共函数调用的实现就在此啊
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);                           // 注意这一句，其实是将__index设置成元表自己
	lua_settable(L, -3);

	lua_pushstring(L, "Add");                     // 放元表中增加一个函数。这样所有基于该元表的Table就都有Add方法了
	lua_pushcfunction(L, CallAdd);
	lua_settable(L, -3);
	lua_pop(L,1);

	if(luaL_loadfile(L, "./script/main.lua")||lua_pcall(L,0,0,0)){
		printf("Loadfile Error:%s\n", lua_tostring(L, -1));
		getchar();
		return 0;
	}
	lua_getglobal(L, "main");
	if(lua_pcall(L,0,0,0)){
		printf("CallMain Error:%s\n", lua_tostring(L, -1));
		getchar();
		return 0;
	}
	getchar();
	lua_close(L);
}

*/