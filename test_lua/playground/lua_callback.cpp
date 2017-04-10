#include"lua.h"
#include<math.h>
#include<stdarg.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>


static int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, sin(d));

	//C++ can return multi result to lua
	//by push multi-time and return the
	//number of result
	return 1;
}

static int l_dir(lua_State *L)
{
	DIR *dir;
	struct dirent *entry;
	int i = 0;
	const char* path=luaL_checkstring(L,1);

	dir = opendir(path);
	if(dir == NULL){
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		return 2;
	}

	lua_newtable(L);
	i=1;
	while((entry=readdir(dir))!=NULL)
	{
		lua_pushnumber(L, i++);
		lua_pushstring(L, entry-> d_name);
		lua_settable(L, -3);
	}

	closedir(dir);

	return 1;
}


int main(void)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//this is a macro
	//#define lua_register(L, n, f) (lua_pushcfunction(L, f), lua_setflobal(L, n))
	lua_register(L, "mysin", l_sin);

	const char* testfunc1 = "print(mysin(3.14/2))";

	if(luaL_dostring(L, testfunc1))
		printf("Failed to invoke\n");

	lua_register(L, "mydir", l_dir);

	const char* testfunc2 = "for i, v in pairs (mydir('/home/lentild'))do print (i, v) end";

	if(luaL_dostring(L, testfunc2))
		printf("Failed to incoke \n");

	lua_close(L);


	return 0;

}
