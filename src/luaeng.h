#ifndef CCLUAENG_H
#define CCLUAENG_H
#ifdef _WIN32
#	define CC_API __declspec(dllimport)
#	define CC_VAR __declspec(dllimport)
#else
#	define CC_API
#	define CC_VAR
#endif

#include "luathing.h"
#include "..\..\ClassiCube\src\String.h"

#define lua_pushstringcc(L, s) lua_pushlstring(L, (s)->buffer, (s)->length)
#define lua_checkstringcc(L, n) String_FromReadonly(luaL_checkstring(L, n))
#define lua_addintconst(L, n) (lua_pushinteger(L, n), lua_setglobal(L, #n))

// Compatibility with most of Lua versions
#if LUA_VERSION_NUM < 501 // Below 5.1
// Dunno who will try to do it :/
#	error "This version of Lua is not supported"
#elif LUA_VERSION_NUM > 501 // Above 5.1
#define luaL_register(L, n, lib) luaL_newlib(L, lib)
#define lua_objlen(L, idx) lua_rawlen(L, idx)
#	if LUA_VERSION_NUM == 502 || defined(LUA_COMPAT_BITLIB)
#		define luaopen_bit luaopen_bit32
#		define CCLUA_HAS_BIT
#	endif
#elif !defined(LUA_JITLIBNAME) // Lua 5.1
#	define CCLUA_NONJIT_51
void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup);
void *luaL_testudata(lua_State *L, int ud, const char *tname);
void luaL_setmetatable(lua_State *L, const char *tname);
#else // Looks like LuaJIT
#	define CCLUA_HAS_JIT
#	define CCLUA_HAS_BIT
#endif

lua_State *LuaEng_SpawnState(void);
cc_bool    LuaEng_ExecuteFile(lua_State *L, const cc_string *ccfilepath);
cc_bool    LuaEng_ExecuteString(lua_State *L, const cc_string *code);

extern lua_State *MainState;
#endif
