
#include <stdio.h>
#include "mainHead.h"
#include "consoleLog.h"

lua_State* consoleLog::m_lua = NULL;
consoleLog::consoleLog(lua_State* lua)
{
	m_lua = lua;
}
consoleLog::~consoleLog()
{

}
void consoleLog::warnInfo(const char* str)
{
	printf("consoleLog:warnInfo....%s\n", str);
	return;
}
void consoleLog::logInfo()
{
	printf("consoleLog:logInfo....\n");
	return;
}
void consoleLog::regFunc()
{
	lua_newtable(m_lua);

	lua_pushstring(m_lua, "warn");
	//lua_pushnumber(m_lua, (int)this);
	lua_pushlightuserdata(m_lua, this);
	lua_pushcclosure(m_lua, &consoleLog::lua_warnInfo, 1);
	lua_settable(m_lua, -3);

	lua_pushstring(m_lua, "log");
	lua_pushnumber(m_lua, (int)this);
	//lua_pushlightuserdata(m_lua, this);
	lua_pushcclosure(m_lua, &consoleLog::lua_warnInfo, 1);
	lua_settable(m_lua, -3);

	lua_setglobal(m_lua, "Console");
	lua_pop(m_lua, 1);
}
int consoleLog::lua_warnInfo(lua_State* lua)
{
	lua_State* mlua = consoleLog::getState();
	//两种都可行
	//consoleLog* obj = (consoleLog*)lua_touserdata(mlua, lua_upvalueindex(1));
	consoleLog* obj = (consoleLog*)lua_topointer(mlua, lua_upvalueindex(1));
	const char* str = lua_tostring(mlua, 2);
	//printf("lua_warnInfo:",obj->warnInfo());
	obj->warnInfo(str);
	return 0;
}