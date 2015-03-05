
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
		//����һ����������C++����Ϊ����C++��������lua�з����
		lua_pushcfunction(lua, &LuaPort::constructor);
		lua_setglobal(lua, "Foo");

		//����userdataҪ�õ�Ԫ�أ���ΪFoo���� ����Ҫ����__gc������ �Ա�����ڴ�
		luaL_newmetatable(lua, "Foo");
		lua_pushstring(lua, "__gc");
		lua_pushcfunction(lua, &LuaPort::gc_obj);
		lua_settable(lua, -3);
	}
	static int constructor(lua_State* lua)
	{
		//1. ����c++����
		FooWrapper* obj = new FooWrapper(lua);
		//2. �½�һ���� tt={}
		lua_newtable(lua);
		//3. �½�һ��userdata��������c++����
		FooWrapper** a = (FooWrapper**)lua_newuserdata(lua, sizeof(FooWrapper*));
		*a = obj;
		//4. ����lua userdata��Ԫ��
		luaL_getmetatable(lua, "Foo");
		lua_setmetatable(lua, -2);
		//5. tt[0] = userdata
		lua_pushnumber(lua, 0);
		lua_insert(lua, -2);
		lua_settable(lua, -3);
		//6. ��table��ע��C++����
		for(int i =0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(lua, FooWrapper::Functions[i].name);
			lua_pushnumber(lua, i);
			lua_pushcclosure(lua, &LuaPort::porxy, 1);
			lua_settable(lua, -3);
		}
		// 7. ��������ظ�lua
		return 1;
	}
	static int porxy(lua_State* L)
	{
		// ȡ��Ҫ���õĺ������
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		// ȡtt[0] �� obj
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		FooWrapper** obj = (FooWrapper**)luaL_checkudata(L,-1,"Foo");
		lua_remove(L, -1);
		// ʵ�ʵĵ��ú���
		return ((*obj)->*(FooWrapper::Functions[i].mfunc))(L);
	}
	static int gc_obj(lua_State* L)
	{
		FooWrapper** obj = (FooWrapper**)luaL_checkudata(L,-1,"Foo");
		delete (*obj);
		return 0;
	}

	/*����2
	=================================================
	static void Register(lua_State* L)
	{
		lua_pushcfunction(L, LuaPort::constructor);
		lua_setglobal(L, ��Foo��);
		luaL_newmetatable(L, ��Foo��);
		lua_pushstring(L, ��__gc��);
		lua_pushcfunction(L, &LuaPort::gc_obj);
		lua_settable(L, -3);

		// ----------- ��һ���ĵط�
		// ����һ������Ԫ��
		lua_newtable(L);
		// ָ��__index����
		int meta = lua_gettop(L);
		lua_pushstring(L, ��__index��);
		lua_pushvalue(L, meta);
		lua_settable(L, �C3);
		// ע�����з���
		for (int i =  0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(L, FooWrapper::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &LuaPort::porxy, 1);
			lua_settable(L, -3);
		}
		// ����������Ԫ���Ա����, ����Ϊmethods
		lua_pushstring(L, ��methods��);
		lua_insert(L, -2);
		lua_settable(L, -3);
	}
	static int constructor(lua_State* L)
	{
		// 1. ����c++����
		FooWrapper* obj = new FooWrapper(L);
		// 2. �½�һ���� tt = {}
		lua_newtable(L);
		// 3. �½�һ��userdata��������c++����
		FooWrapper** a = (FooWrapper** )lua_newuserdata(L, sizeof(FooWrapper*));
		*a = obj;
		// 4. ����lua userdata��Ԫ��
		luaL_getmetatable(L, ��Foo��);
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -3);
		// ------------��һ���ĵط�
		// 5. tt[0] = userdata
		lua_insert(L, -2);
		lua_pushnumber(L, 0);
		lua_insert(L, -2);
		lua_settable(L, -4);
		// 6. �󶨷���Ԫ��
		lua_pushstring(L, ��methods��);
		lua_gettable(L, -2);
		lua_setmetatable(L, -3);
		lua_pop(L, 1);
		// ���ر�
		return 1;
	}
	*/

	/*����3
	//====================================
	static void Register(lua_State* L)

	{

		lua_pushcfunction(L, LuaPort::construct);
		lua_setglobal(L,  "Foo");
		luaL_newmetatable(L, "Foo");
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &LuaPort::gc_obj);
		lua_settable(L, -3);
		// ----- ��һ����
		// �ѷ���Ҳע���userdata��Ԫ����
		for (int i =  0; FooWrapper::Functions[i].name; ++i)
		{
			lua_pushstring(L, FooWrapper::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &LuaPort::porxy, 1);
			lua_settable(L, -3);
		}
		// ע��__index����
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);
	}
	static int constructor(lua_State* L)
	{
		FooWrapper* obj = new FooWrapper(L);
		FooWrapper** a = (FooWrapper**)lua_newuserdata(L, sizeof(FooWrapper*));
		*a = obj;
		luaL_getmetatable(L, ��Foo��);
		lua_setmetatable(L, -2);
		return 1;
	}
	static int porxy(lua_State* L)
	{
		int  i = (int)lua_tonumber(L, lua_upvalueindex(1));
		FooPort** obj = (FooPort**)luaL_checkudata(L, 1, ��Foo��);
		return ((*obj)->*(FooWrapper::FunctionS[i].mfunc))(L);
	}
	*/
};
#endif