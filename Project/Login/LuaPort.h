
#ifndef XXS_LUAPORT_H
#define XXS_LUAPORT_H
#include "mainHead.h"
#include "Foo.h"
#include "FooWrapper.h"

class LuaPort
{
public:

	static void RegisterClass(lua_State* lua)
	{
		//导出一个方法创建C++，因为创建C++对象是在lua中发起的
		lua_pushcfunction(lua, &LuaPort::constructor);
		lua_setglobal(lua, "Foo");

		//创建userdata要用的元素（名为Foo）， 起码要定义__gc方法， 以便回收内存
		luaL_newmetatable(lua, "Foo");
		lua_pushstring(lua, "__gc");
		lua_pushcfunction(lua, &LuaPort::gc_obj);
		lua_settable(lua, -3);
	}
	static int constructor(lua_State* lua)
	{
		//1. 构造c++对象
		FooWrapper* obj = new FooWrapper(lua);
		//2. 新建一个表 tt={}
		lua_newtable(lua);
		//3. 新建一个userdata用来持有c++对象
		FooWrapper** a = (FooWrapper**)lua_newuserdata(lua, sizeof(FooWrapper*));
		*a = obj;
		//4. 设置lua userdata的元素
		luaL_getmetatable(lua, "Foo");
		lua_setmetatable(lua, -2);
		//5. tt[0] = userdata
		lua_pushnumber(lua, 0);
		lua_insert(lua, -2);
		lua_settable(lua, -3);
		//6. 向table中注入C++函数
		for(int i =0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(lua, FooWrapper::Functions[i].name);
			lua_pushnumber(lua, i);
			lua_pushcclosure(lua, &LuaPort::porxy, 1);
			lua_settable(lua, -3);
		}
		// 7. 把这个表返回给lua
		return 1;
	}
	static int porxy(lua_State* L)
	{
		// 取出要调用的函数编号
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		// 取tt[0] 及 obj
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		FooWrapper** obj = (FooWrapper**)luaL_checkudata(L,-1,"Foo");
		lua_remove(L, -1);
		// 实际的调用函数
		return ((*obj)->*(FooWrapper::Functions[i].mfunc))(L);
	}
	static int gc_obj(lua_State* L)
	{
		FooWrapper** obj = (FooWrapper**)luaL_checkudata(L,-1,"Foo");
		delete (*obj);
		return 0;
	}

	/*方法2
	=================================================
	static void Register(lua_State* L)
	{
		lua_pushcfunction(L, LuaPort::constructor);
		lua_setglobal(L, “Foo”);
		luaL_newmetatable(L, “Foo”);
		lua_pushstring(L, “__gc”);
		lua_pushcfunction(L, &LuaPort::gc_obj);
		lua_settable(L, -3);

		// ----------- 不一样的地方
		// 创建一个方法元表
		lua_newtable(L);
		// 指定__index方法
		int meta = lua_gettop(L);
		lua_pushstring(L, “__index”);
		lua_pushvalue(L, meta);
		lua_settable(L, –3);
		// 注册所有方法
		for (int i =  0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(L, FooWrapper::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &LuaPort::porxy, 1);
			lua_settable(L, -3);
		}
		// 把这个表放入元表以便后用, 起名为methods
		lua_pushstring(L, “methods”);
		lua_insert(L, -2);
		lua_settable(L, -3);
	}
	static int constructor(lua_State* L)
	{
		// 1. 构造c++对象
		FooWrapper* obj = new FooWrapper(L);
		// 2. 新建一个表 tt = {}
		lua_newtable(L);
		// 3. 新建一个userdata用来持有c++对象
		FooWrapper** a = (FooWrapper** )lua_newuserdata(L, sizeof(FooWrapper*));
		*a = obj;
		// 4. 设置lua userdata的元表
		luaL_getmetatable(L, “Foo”);
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -3);
		// ------------不一样的地方
		// 5. tt[0] = userdata
		lua_insert(L, -2);
		lua_pushnumber(L, 0);
		lua_insert(L, -2);
		lua_settable(L, -4);
		// 6. 绑定方法元表
		lua_pushstring(L, “methods”);
		lua_gettable(L, -2);
		lua_setmetatable(L, -3);
		lua_pop(L, 1);
		// 返回表
		return 1;
	}
	*/

	/*方法3
	//====================================
	static void Register(lua_State* L)

	{

		lua_pushcfunction(L, LuaPort::construct);
		lua_setglobal(L,  "Foo");
		luaL_newmetatable(L, "Foo");
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &LuaPort::gc_obj);
		lua_settable(L, -3);
		// ----- 不一样的
		// 把方法也注册进userdata的元表里
		for (int i =  0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(L, FooWrapper::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &LuaPort::porxy, 1);
			lua_settable(L, -3);
		}
		// 注册__index方法
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);
	}
	static int constructor(lua_State* L)
	{
		FooWrapper* obj = new FooWrapper(L);
		FooWrapper** a = (FooWrapper**)lua_newuserdata(L, sizeof(FooWrapper*));
		*a = obj;
		luaL_getmetatable(L, “Foo”);
		lua_setmetatable(L, -2);
		return 1;
	}
	static int porxy(lua_State* L)
	{
		int  i = (int)lua_tonumber(L, lua_upvalueindex(1));
		FooPort** obj = (FooPort**)luaL_checkudata(L, 1, “Foo”);
		return ((*obj)->*(FooWrapper::FunctionS[i].mfunc))(L);
	}
	*/
};
#endif