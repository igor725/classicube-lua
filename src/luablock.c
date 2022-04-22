#include "luaeng.h"
#include "luablock.h"
#include "../../ClassiCube/src/Block.h"

static int block_getname(lua_State *L) {
	cc_string bname = Block_UNSAFE_GetName(
		(BlockID)luaL_checkinteger(L, 1)
	);
	lua_pushstringcc(L, &bname);
	return 1;
}

static int block_findid(lua_State *L) {
	cc_string str = lua_checkstringcc(L, 1);
	lua_pushinteger(L, (lua_Integer)Block_FindID(&str));
	return 1;
}

static int block_parse(lua_State *L) {
	cc_string str = lua_checkstringcc(L, 1);
	lua_pushinteger(L, (lua_Integer)Block_Parse(&str));
	return 1;
}

static const luaL_Reg blocklib[] = {
	{"getname", block_getname},
	{"findid", block_findid},
	{"parse", block_parse},

	{NULL, NULL}
};

int luaopen_block(lua_State *L) {
	luaL_register(L, luaL_checkstring(L, 1), blocklib);
	return 1;
}
