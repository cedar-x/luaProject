
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
	//���ջ�еĲ����Ƿ�Ϸ���1��ʾLua����ʱ�ĵ�һ������(������)���������ơ�
	//���Lua�����ڵ���ʱ���ݵĲ�����Ϊnumber���ú�����������ֹ�����ִ�С�
	double op1 = luaL_checknumber(lua, 1);
	double op2 = luaL_checknumber(lua, 2);
	//�������Ľ��ѹ��ջ�С�����ж������ֵ��������������ѹ��ջ�С�
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
	//��ָ���ĺ���ע��ΪLua��ȫ�ֺ������������е�һ���ַ�������ΪLua����
	//�ڵ���C����ʱʹ�õ�ȫ�ֺ��������ڶ�������Ϊʵ��C������ָ�롣
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
	//ע��c++����
	lua_register(L, "lua_clib", lua_clib);
	//luaL_register(L, "lengine", lengine);  lua5.2�Ѿ�ɾ����LuaL_register����
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
	// ����һ��Ԫ��ΪCTest��Table����Lua����
	*(CTest**)lua_newuserdata(L, sizeof(CTest*)) = new CTest();
	luaL_getmetatable(L, "CTest");
	lua_setmetatable(L, -2);
	return 1;
}

static int DestoryCTest(lua_State* L)
{
	// �ͷŶ���
	delete *(CTest**)lua_topointer(L, 1);
	return 0;
}

static int CallAdd(lua_State* L)
{
	// ����C++�෽�������庯����
	CTest* pT = *(CTest**)lua_topointer(L, 1);
	lua_pushnumber(L, pT->Add(lua_tonumber(L, 2), lua_tonumber(L, 3)));
	return 1;
}

int main(int argc, char * argv[])
{
	lua_State *L = luaL_newstate();
	luaopen_base(L);    

	// ��lua��ע����
	lua_pushcfunction(L, CreateCTest);    // ע�����ڴ������ȫ�ֺ���
	lua_setglobal(L,  "CTest");

	luaL_newmetatable(L, "CTest");           // ����һ��Ԫ��
	lua_pushstring(L, "__gc");                    // ��������
	lua_pushcfunction(L, DestoryCTest);
	lua_settable(L, -3);                               // �����������õ�ʵ�־��ڴ˰�
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);                           // ע����һ�䣬��ʵ�ǽ�__index���ó�Ԫ���Լ�
	lua_settable(L, -3);

	lua_pushstring(L, "Add");                     // ��Ԫ��������һ���������������л��ڸ�Ԫ���Table�Ͷ���Add������
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