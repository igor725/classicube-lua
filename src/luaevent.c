#include "luaeng.h"
#include "luaevent.h"
#include "../../ClassiCube/src/Game.h"
#include "../../ClassiCube/src/Inventory.h"
#include "../../ClassiCube/src/Event.h"

static int event_register(lua_State *L) {}

static cc_bool lookupeventfunc(const char *evsect, const char *evname) {
	if(!MainState) return false;
	lua_getglobal(MainState, "ccevents");
	lua_getfield(MainState, -1, evsect);
	if(!lua_istable(MainState, -1)) {
		lua_pop(MainState, 2);
		return false;
	}
	lua_getfield(MainState, -1, evname);
	if(!lua_isfunction(MainState, -1)) {
		lua_pop(MainState, 3);
		return false;
	}

	lua_remove(MainState, -2);
	lua_remove(MainState, -3);
	lua_remove(MainState, -4);
	return true;
}

static void callvoidevent(const char *evsect, const char *evname) {
	if(lookupeventfunc("chat", "colcode"))
		lua_pcall(MainState, 0, 0, 0);
}

/**
 * 
 * Entity
 * 
 */

static void evtentadded(void *obj, EntityID id) {
	(void)obj;
	if(lookupeventfunc("entity", "added")) {
		lua_pushinteger(MainState, (lua_Integer)id);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtentremoved(void *obj, EntityID id) {
	(void)obj;
	if(lookupeventfunc("entity", "removed")) {
		lua_pushinteger(MainState, (lua_Integer)id);
		lua_pcall(MainState, 1, 0, 0);
	}
}

/**
 * 
 * Tab events
 * 
 */

static void evttabadded(void *obj, EntityID id) {
	if(lookupeventfunc("tablist", "added")) {
		lua_pushinteger(MainState, (lua_Integer)id);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evttabchanged(void *obj, EntityID id) {
	if(lookupeventfunc("tablist", "changed")) {
		lua_pushinteger(MainState, (lua_Integer)id);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evttabremoved(void *obj, EntityID id) {
	if(lookupeventfunc("tablist", "removed")) {
		lua_pushinteger(MainState, (lua_Integer)id);
		lua_pcall(MainState, 1, 0, 0);
	}
}

/**
 * 
 * Texture events
 * 
 */
static void evttextureatlas(void *obj) {
	(void)obj;
	callvoidevent("texture", "atlas");
}

static void evttexturepack(void *obj) {
	(void)obj;
	callvoidevent("texture", "pack");
}

static void evttexturefile(void *obj) {
	(void)obj;
	callvoidevent("texture", "file");
}

/**
 * 
 * User events
 * 
 */

static void evtuserblock(void *obj, IVec3 pos, BlockID old, BlockID new) {
	if(lookupeventfunc("user", "setblock")) {
		lua_pushstring(MainState, "*IVec3 placeholder*");
		lua_pushinteger(MainState, (lua_Integer)old);
		lua_pushinteger(MainState, (lua_Integer)new);
		lua_pcall(MainState, 3, 0, 0);
	}
}

static void evtuserhacks(void *obj) {
	if(lookupeventfunc("user", "sethacks")) {
		lua_pushstring(MainState, "*Hacks placeholder*");
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtuserheld(void *obj) {
	if(lookupeventfunc("user", "setheld")) {
		lua_pushinteger(MainState, (BlockID)Inventory_Get(Inventory.SelectedIndex));
		lua_pcall(MainState, 1, 0, 0);
	}
}

/**
 * 
 * Block events
 * 
 */

static void evtblockperm(void *obj) {
	(void)obj;
	callvoidevent("block", "updateperms");
}

static void evtblockdefine(void *obj) {
	(void)obj;
	callvoidevent("block", "updatedefs");
}

/**
 * 
 * World events
 * 
 */

static void evtworldnew(void *obj) {
	(void)obj;
	callvoidevent("world", "newmap");
}

static void evtworldloading(void *obj, float progress) {
	(void)obj;
	if(lookupeventfunc("world", "loading")) {
		lua_pushnumber(MainState, (lua_Number)progress);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtworldloaded(void *obj) {
	(void)obj;
	callvoidevent("world", "loaded");
}

static void evtworldenv(void *obj, int var) {
	(void)obj;
	if(lookupeventfunc("world", "updateenv")) {
		lua_pushinteger(MainState, (lua_Number)var);
		lua_pcall(MainState, 1, 0, 0);
	}
}

/**
 * 
 * Chat events
 * 
 */

static void entchatrecv(void *obj, const cc_string *text, int msgType) {
	(void)obj;
	if(lookupeventfunc("chat", "receive")) {
		lua_pushstringcc(MainState, text);
		lua_pushinteger(MainState, (lua_Integer)msgType);
		lua_pcall(MainState, 2, 0, 0);
	}
}

static void entchatsend(void *obj, const cc_string *text, int msgType) {
	(void)obj;
	if(lookupeventfunc("chat", "send")) {
		lua_pushstringcc(MainState, text);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void entchatcolcode(void *obj, cc_uint8 code) {
	(void)obj;
	if(lookupeventfunc("chat", "setcolcode")) {
		lua_pushinteger(MainState, (lua_Integer)code);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void entchatfont(void *obj) {
	(void)obj;
	callvoidevent("chat", "setfont");
}

/**
 * 
 * Window events
 * 
 */

static void evtwndresize(void *obj) {
	(void)obj;
	callvoidevent("window", "resize");
}

static void evtwndclose(void *obj) {
	(void)obj;
	callvoidevent("window", "close");
}

static void evtwndfocus(void *obj) {
	(void)obj;
	callvoidevent("window", "focus");
}

static void evtwndstate(void *obj) {
	(void)obj;
	callvoidevent("window", "state");
}

static void evtwndcreate(void *obj) {
	(void)obj;
	callvoidevent("window", "create");
}

static void evtwndinactive(void *obj) {
	(void)obj;
	callvoidevent("window", "inactive");
}

/**
 * 
 * Input events
 * 
 */

static void evtinputpress(void *obj, char key) {
	if(lookupeventfunc("input", "press")) {
		lua_pushinteger(MainState, (lua_Integer)key);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtinputdown(void *obj, int key, cc_bool waspressed) {
	if(lookupeventfunc("input", "down")) {
		lua_pushinteger(MainState, (lua_Integer)key);
		lua_pushboolean(MainState, waspressed);
		lua_pcall(MainState, 2, 0, 0);
	}
}

static void evtinputup(void *obj, int key) {
	if(lookupeventfunc("input", "up")) {
		lua_pushinteger(MainState, (lua_Integer)key);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtinputwheel(void *obj, float delta) {
	if(lookupeventfunc("input", "wheel")) {
		lua_pushnumber(MainState, (lua_Number)delta);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtinputtext(void *obj, cc_string *text) {
	if(lookupeventfunc("input", "text")) {
		lua_pushstringcc(MainState, text);
		lua_pcall(MainState, 1, 0, 0);
	}
}

/**
 * 
 * Pointer events
 * 
 */

static void evtpointermoved(void *obj, int idx) {
	if(lookupeventfunc("pointer", "moved")) {
		lua_pushinteger(MainState, (lua_Integer)idx);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtpointerdown(void *obj, int idx) {
	if(lookupeventfunc("pointer", "down")) {
		lua_pushinteger(MainState, (lua_Integer)idx);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtpointerup(void *obj, int idx) {
	if(lookupeventfunc("pointer", "up")) {
		lua_pushinteger(MainState, (lua_Integer)idx);
		lua_pcall(MainState, 1, 0, 0);
	}
}

static void evtpointerrawmove(void *obj, float dx, float dy) {
	if(lookupeventfunc("pointer", "rawmove")) {
		lua_pushnumber(MainState, (lua_Number)dx);
		lua_pushnumber(MainState, (lua_Number)dy);
		lua_pcall(MainState, 2, 0, 0);
	}
}

/**
 * 
 * Network events
 * 
 */

static void evtnetconnect(void *obj) {
	(void)obj;
	callvoidevent("net", "connect");
}

static void evtnetdisconnect(void *obj) {
	(void)obj;
	callvoidevent("net", "disconnect");
}

static void evtnetplugmsg(void *obj, cc_uint8 channel, cc_uint8 *data) {
	(void)obj;
	if(lookupeventfunc("net", "pluginmsg")) {
		lua_pushinteger(MainState, (lua_Integer)channel);
		lua_pushlstring(MainState, data, 64);
		lua_pcall(MainState, 2, 0, 0);
	}
}

static int newind(lua_State *L) {
	luaL_error(L, "This table is readonly");
	return 0;
}

static void push_evtmetatable(lua_State *L) {
	lua_newtable(L);
	lua_pushcfunction(L, newind);
	lua_setfield(L, -2, "__newindex");
	lua_pushstring(L, "tf you doing here???");
	lua_setfield(L, -2, "__metatable");
}

int luaopen_event(lua_State *L) {
	lua_newtable(L);
	(lua_newtable(L), lua_setfield(L, -2, "entity"));
	(lua_newtable(L), lua_setfield(L, -2, "tablist"));
	(lua_newtable(L), lua_setfield(L, -2, "texture"));
	(lua_newtable(L), lua_setfield(L, -2, "gfx"));
	(lua_newtable(L), lua_setfield(L, -2, "user"));
	(lua_newtable(L), lua_setfield(L, -2, "block"));
	(lua_newtable(L), lua_setfield(L, -2, "world"));
	(lua_newtable(L), lua_setfield(L, -2, "chat"));
	(lua_newtable(L), lua_setfield(L, -2, "window"));
	(lua_newtable(L), lua_setfield(L, -2, "input"));
	(lua_newtable(L), lua_setfield(L, -2, "pointer"));
	(lua_newtable(L), lua_setfield(L, -2, "net"));
	(push_evtmetatable(L), lua_setmetatable(L, -2));
	lua_setglobal(L, "ccevents");
	
	// Entity events
	Event_Register_(&EntityEvents.Added, NULL, evtentadded);
	Event_Register_(&EntityEvents.Removed, NULL, evtentremoved);

	// Tab list
	Event_Register_(&TabListEvents.Added, NULL, evttabadded);
	Event_Register_(&TabListEvents.Changed, NULL, evttabchanged);
	Event_Register_(&TabListEvents.Removed, NULL, evttabremoved);

	// Texture events
	Event_Register_(&TextureEvents.AtlasChanged, NULL, evttextureatlas);
	Event_Register_(&TextureEvents.PackChanged, NULL, evttexturepack);
	Event_Register_(&TextureEvents.FileChanged, NULL, evttexturefile);

	// User events
	Event_Register_(&UserEvents.BlockChanged, NULL, evtuserblock);
	Event_Register_(&UserEvents.HacksStateChanged, NULL, evtuserhacks);
	Event_Register_(&UserEvents.HeldBlockChanged, NULL, evtuserheld);

	// Block events
	Event_Register_(&BlockEvents.PermissionsChanged, NULL, evtblockperm);
	Event_Register_(&BlockEvents.BlockDefChanged, NULL, evtblockdefine);

	// World events
	Event_Register_(&WorldEvents.NewMap, NULL, evtworldnew);
	Event_Register_(&WorldEvents.Loading, NULL, evtworldloading);
	Event_Register_(&WorldEvents.MapLoaded, NULL, evtworldloaded);
	Event_Register_(&WorldEvents.EnvVarChanged, NULL, evtworldenv);

	// Chat events
	Event_Register_(&ChatEvents.ChatReceived, NULL, entchatrecv);
	Event_Register_(&ChatEvents.ChatSending, NULL, entchatsend);
	Event_Register_(&ChatEvents.ColCodeChanged, NULL, entchatcolcode);
	Event_Register_(&ChatEvents.FontChanged, NULL, entchatfont);

	// Window events
	Event_Register_(&WindowEvents.Resized, NULL, evtwndresize);
	Event_Register_(&WindowEvents.Closing, NULL, evtwndclose);
	Event_Register_(&WindowEvents.FocusChanged, NULL, evtwndfocus);
	Event_Register_(&WindowEvents.StateChanged, NULL, evtwndstate);
	Event_Register_(&WindowEvents.Created, NULL, evtwndcreate);
	Event_Register_(&WindowEvents.InactiveChanged, NULL, evtwndinactive);

	// Input events
	Event_Register_(&InputEvents.Press, NULL, evtinputpress);
	Event_Register_(&InputEvents.Down, NULL, evtinputdown);
	Event_Register_(&InputEvents.Up, NULL, evtinputup);
	Event_Register_(&InputEvents.Wheel, NULL, evtinputwheel);
	Event_Register_(&InputEvents.TextChanged, NULL, evtinputtext);

	// Pointer events
	Event_Register_(&PointerEvents.Moved, NULL, evtpointermoved);
	Event_Register_(&PointerEvents.Down, NULL, evtpointerdown);
	Event_Register_(&PointerEvents.Up, NULL, evtpointerup);
	Event_Register_(&PointerEvents.RawMoved, NULL, evtpointerrawmove);

	// Net events
	Event_Register_(&NetEvents.Connected, NULL, evtnetconnect);
	Event_Register_(&NetEvents.Disconnected, NULL, evtnetdisconnect);
	Event_Register_(&NetEvents.PluginMessageReceived, NULL, evtnetplugmsg);

	return 0;
}
