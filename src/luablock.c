#include "luaeng.h"
#include "luablock.h"
#include "../../ClassiCube/src/Block.h"
#include "../../ClassiCube/src/Game.h"
#include "../../ClassiCube/src/World.h"

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

static int block_set(lua_State *L) {
	int x = (int)luaL_checkinteger(L, 1),
	y = (int)luaL_checkinteger(L, 2),
	z = (int)luaL_checkinteger(L, 3);
	BlockID id = (BlockID)luaL_checkinteger(L, 4);

	if(lua_toboolean(L, 5))
		Game_ChangeBlock(x, y, z, id);
	else
		Game_UpdateBlock(x, y, z, id);

	return 0;
}

static int block_get(lua_State *L) {
	int x = (int)luaL_checkinteger(L, 1),
	y = (int)luaL_checkinteger(L, 2),
	z = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(MainState, World_GetBlock(x, y, z));
	return 1;
}

static const luaL_Reg blocklib[] = {
	{"getname", block_getname},
	{"findid", block_findid},
	{"parse", block_parse},

	{"set", block_set},
	{"get", block_get},

	{NULL, NULL}
};

int luaopen_block(lua_State *L) {
	luaL_newlib(L, blocklib);
	return 1;
}
