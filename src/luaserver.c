#include "luaeng.h"
#include "luaserver.h"
#include "../../ClassiCube/src/Server.h"

static int server_getname(lua_State *L) {
	lua_pushstringcc(L, &Server.Name);
	return 1;
}

static int server_getmotd(lua_State *L) {
	lua_pushstringcc(L, &Server.MOTD);
	return 1;
}

static int server_getaddr(lua_State *L) {
	lua_pushstringcc(L, &Server.Address);
	lua_pushinteger(L, (lua_Integer)Server.Port);
	return 2;
}

static int server_isclosed(lua_State *L) {
	lua_pushboolean(L, Server.Disconnected);
	return 1;
}

static int server_issp(lua_State *L) {
	lua_pushboolean(L, Server.IsSinglePlayer);
	return 1;
}

static const luaL_Reg serverlib[] = {
	{"getname", server_getname},
	{"getmotd", server_getmotd},
	{"getaddr", server_getaddr},
	{"isclosed", server_isclosed},
	{"issp", server_issp},

	{NULL, NULL}
};

int luaopen_server(lua_State *L) {
	luaL_register(L, luaL_checkstring(L, 1), serverlib);
	return 1;
}
