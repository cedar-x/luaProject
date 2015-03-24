#ifndef XXS_CONSOLE_H
#define XXS_CONSOLE_H

class consoleLog{
public:
	consoleLog(lua_State* lua);
	~consoleLog();
public:
	void warnInfo(const char*);
	void logInfo();
public:
	void regFunc();
	static int lua_warnInfo(lua_State* lua);
	static lua_State* getState()
	{
		return m_lua;
	}
private:
	static lua_State* m_lua;
};
#endif