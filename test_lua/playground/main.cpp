#include<iostream>
#include<lua.hpp>
#include<string>
using namespace std;



int main()
{
	//create lua state
	lua_State *L = luaL_newstate();
	if(L==NULL)
	{
		return 0;
	}

	//load lua file
	int bRet = luaL_loadfile(L,"hello.lua");
	if(bRet)
	{
		cout<<"cannot load file"<<endl;
		return 0;
	}

	//run lua file
	bRet = lua_pcall(L,0,0,0);
	if(bRet)
	{
		cout<<"pcall error"<<endl;
		return 0;
	}

	lua_getglobal(L,"tb1");
	lua_getfield(L,-1,"name");
	string str=lua_tostring(L,-1);
	cout<<"tb1:name = "<<str.c_str()<<endl;

	lua_getglobal(L,"add");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 20);
	int iRet= lua_pcall(L,2,1,0);

	if(iRet)
	{
		const char *pErrorMsg = lua_tostring(L, -1);
		cout<<pErrorMsg<<endl;
		lua_close(L);
		return 0;
	}
	if(lua_isnumber(L, -1))
	{
		double fValue = lua_tonumber(L, -1);
		cout<<"Result is "<<fValue<<endl;
	}

	lua_close(L);

	return 0;
}
