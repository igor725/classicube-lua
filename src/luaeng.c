#include "luaeng.h"
#include "luachat.h"
#include "luaevent.h"

lua_State *MainState = NULL;

// Compatibility with Lua 5.1
#ifdef CSLUA_NONJIT_51
void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
	for(; l->name != NULL; l++) {
		int i;
		for(i = 0; i < nup; i++)
			lua_pushvalue(L, -nup);
		lua_pushcclosure(L, l->func, nup);
		lua_setfield(L, -(nup + 2), l->name);
	}
	lua_pop(L, nup);
}

void luaL_setmetatable(lua_State *L, const char *tname) {
	luaL_getmetatable(L, tname);
	lua_setmetatable(L, -2);
}

void *luaL_testudata (lua_State *L, int ud, const char *tname) {
	void *p = lua_touserdata(L, ud);
	if(p != NULL) {
		if(lua_getmetatable(L, ud)) {
			luaL_getmetatable(L, tname);
			if(!lua_rawequal(L, -1, -2))
				p = NULL;
			lua_pop(L, 2);
			return p;
		}
	}
	return NULL;
}
#endif

static const luaL_Reg lualibs[] = {
	// Lua stuff
	{"", luaopen_base},
	{LUA_MATHLIBNAME, luaopen_math},
	{LUA_STRLIBNAME, luaopen_string},
	{LUA_TABLIBNAME, luaopen_table},
	{LUA_IOLIBNAME, luaopen_io},
	{LUA_OSLIBNAME, luaopen_os},
	{LUA_LOADLIBNAME, luaopen_package},
	{LUA_DBLIBNAME, luaopen_debug},
#ifdef CCLUA_HAS_BIT
	{LUA_BITLIBNAME, luaopen_bit},
#endif
#ifdef CCLUA_HAS_JIT
	{LUA_JITLIBNAME, luaopen_jit},
	{LUA_FFILIBNAME, luaopen_ffi},
#endif

	// ClassiCube stuff
	{"chat", luaopen_chat},
	{"event", luaopen_event},
	{NULL, NULL}
};

lua_State *LuaEng_SpawnState(void) {
	lua_State *L = luaL_newstate();
	if(!L) return NULL;

	for(const luaL_Reg *lib = lualibs; lib->func; lib++) {
#		if LUA_VERSION_NUM < 502
			lua_pushcfunction(L, lib->func);
			lua_pushstring(L, lib->name);
			lua_call(L, 1, 0);
#		else
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);
#		endif
	}

	return L;
}

cc_bool LuaEng_ExecuteFile(lua_State *L, const cc_string *ccfilepath) {
	char filepath[256];
	String_CopyToRawArray(filepath, ccfilepath);
	return luaL_dofile(L, filepath);
}

cc_bool LuaEng_ExecuteString(lua_State *L, const cc_string *code) {
	char codebuf[256];
	String_CopyToRawArray(codebuf, code);
	return luaL_dostring(L, codebuf);
}
