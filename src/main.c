#include "luaeng.h"
#include "..\..\ClassiCube\src\Game.h"
#include "..\..\ClassiCube\src\Chat.h"
#include "..\..\ClassiCube\src\String.h"
#include "..\..\ClassiCube\src\Server.h"

#ifdef _WIN32
#	define CC_EXP __declspec(dllexport)
#else
#	define CC_EXP
#endif

static const cc_string execscript_sym = String_FromConst("@");

static const cc_string strings[] = {
	String_FromConst("&cFailed to init lua_State"),
	String_FromConst("&cNo arguments passed, see /client help lua"),
};

static void LuaCmd_Execute(const cc_string *args, int argsCount) {
	if(!argsCount) {
		Chat_Add(&strings[1]);
		return;
	}

	char outbuf[512];
	cc_string out = String_FromArray(outbuf);
	String_AppendConst(&out, "&1[ClientLua]&f ");
	cc_bool failed = false;

	if(String_CaselessStarts(args, &execscript_sym)) {
		const cc_string path = String_UNSAFE_SubstringAt(args, 1);
		failed = LuaEng_ExecuteFile(MainState, &path);
	} else failed = LuaEng_ExecuteString(MainState, args);

	if(failed) {
		cc_string luastring = String_FromReadonly(lua_tostring(MainState, -1));
		String_Format1(&out, "&cFailed to execute code: %s", &luastring);
		lua_pop(MainState, 1);
	} else {
		int top = lua_gettop(MainState);
		String_AppendConst(&out, "&aCode executed");
		if(top > 0) {
			String_AppendConst(&out, ": ");
			for(int i = top; i > 0; i--) {
				switch(lua_type(MainState, -i)) {
					case LUA_TSTRING:
						String_Append(&out, '"');
						String_AppendConst(&out, lua_tostring(MainState, -i));
						String_Append(&out, '"');
						break;

					case LUA_TNUMBER:
						String_AppendFloat(&out, lua_tonumber(MainState, -i), 4);
						break;

					case LUA_TBOOLEAN:
						String_AppendConst(&out, lua_toboolean(MainState, -i) ? "true" : "false");
						break;

					default:
						String_Append(&out, '[');
						String_AppendConst(&out, luaL_typename(MainState, -i));
						String_Append(&out, ']');
						break;
				}
				String_Append(&out, ' ');
			}
			lua_pop(MainState, top);
		}
	}

	Chat_Add(&out);
}

static struct ChatCommand LuaCmd = {
	"Lua", LuaCmd_Execute, COMMAND_FLAG_UNSPLIT_ARGS,
	{
		"&a/client lua @[filename]",
		"&eRuns specified [filename] script",
		"&a/client lua [script]",
		"&eRuns specified lua string"
	}
};

static void LuaPlugin_Init(void) {
	MainState = LuaEng_SpawnState();
	if(!MainState) {
		Chat_Add(&strings[0]);
		return;
	}
	Commands_Register(&LuaCmd);
	String_AppendConst(&Server.AppName, " + Lua 1.0.0");
}

CC_EXP int Plugin_ApiVersion = 1;
CC_EXP struct IGameComponent Plugin_Component = {
	LuaPlugin_Init
};
