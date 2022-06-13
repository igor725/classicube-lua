#include "luaeng.h"
#include "luaworld.h"
#include "../../ClassiCube/src/World.h"

static int world_getlastsave(lua_State *L) {
	lua_pushnumber(L, (lua_Number)World.LastSave);
	return 1;
}

static int world_getname(lua_State *L) {
	lua_pushstringcc(L, &World.Name);
	return 1;
}

static int world_getdimensions(lua_State *L) {
	lua_pushinteger(L, (lua_Integer)World.Width);
	lua_pushinteger(L, (lua_Integer)World.Height);
	lua_pushinteger(L, (lua_Integer)World.Length);
	return 3;
}

static int world_getuuid(lua_State *L) {
	lua_pushlstring(L, World.Uuid, WORLD_UUID_LEN);
	return 1;
}

static const luaL_Reg worldlib[] = {
	{"getname", world_getname},
	{"getlastsave", world_getlastsave},
	{"getdimensions", world_getdimensions},
	{"getuuid", world_getuuid},

	{NULL, NULL}
};

int luaopen_world(lua_State *L) {
	luaL_newlib(L, worldlib);
	return 1;
}
