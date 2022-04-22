#include "luaeng.h"
#include "luamisc.h"

static int misc_pluginmesg(lua_State *L) {
	cc_uintptr size = 0;
	cc_uint8 channel = (cc_uint8)luaL_checkinteger(L, 1);
	const char *data = luaL_checklstring(L, 2, &size);
	luaL_argcheck(L, size == 64, 2, "data must contain 64 bytes");
	CPE_SendPluginMessage(channel, (cc_uint8 *)data);
	return 0;
}

static int misc_clipboard(lua_State *L) {
	if(lua_gettop(L) > 0) {
		cc_string str = lua_checkstringcc(L, 1);
		Clipboard_SetText(&str);
		return 0;
	}

	return 0;
}

static const luaL_Reg misclib[] = {
	{"pluginmesg", misc_pluginmesg},
	{"clipboard", misc_clipboard},

	{NULL, NULL}
};

int luaopen_misc(lua_State *L) {
	luaL_register(L, luaL_checkstring(L, 1), misclib);
	return 1;
}
