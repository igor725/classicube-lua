#include "luaeng.h"
#include "luachat.h"
#include "..\..\ClassiCube\src\String.h"
#include "..\..\ClassiCube\src\Chat.h"

static int chat_add(lua_State *L) {
	cc_string text = lua_checkstringcc(L, 1);
	if(lua_isnumber(L, 2))
		Chat_AddOf(&text, (int)luaL_checkinteger(L, 2));
	else
		Chat_Add(&text);
	return 0;
}

static int chat_send(lua_State *L) {
	cc_string text = lua_checkstringcc(L, 1);
	Chat_Send(&text, lua_toboolean(L, 2));
	return 0;
}

static const luaL_Reg chatlib[] = {
	{"add", chat_add},
	{"send", chat_send},

	{NULL, NULL}
};

int luaopen_chat(lua_State *L) {
	lua_addintconst(L, MSG_TYPE_NORMAL);
	lua_addintconst(L, MSG_TYPE_STATUS_1);
	lua_addintconst(L, MSG_TYPE_STATUS_2);
	lua_addintconst(L, MSG_TYPE_STATUS_3);
	lua_addintconst(L, MSG_TYPE_BOTTOMRIGHT_1);
	lua_addintconst(L, MSG_TYPE_BOTTOMRIGHT_2);
	lua_addintconst(L, MSG_TYPE_BOTTOMRIGHT_3);
	lua_addintconst(L, MSG_TYPE_ANNOUNCEMENT);
	lua_addintconst(L, MSG_TYPE_BIGANNOUNCEMENT);
	lua_addintconst(L, MSG_TYPE_SMALLANNOUNCEMENT);
	lua_addintconst(L, MSG_TYPE_CLIENTSTATUS_1);
	lua_addintconst(L, MSG_TYPE_CLIENTSTATUS_2);

	luaL_register(L, luaL_checkstring(L, 1), chatlib);
	return 1;
}
