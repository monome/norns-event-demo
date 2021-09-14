#include "lua_event_demo.h"

static const luaL_Reg mod[] = {
    {NULL, NULL}
};

static luaL_Reg func[] = {
    {NULL, NULL}
};

LUA_EVENT_DEMO_API int luaopen_event_demo(lua_State *L) {
    lua_newtable(L);

    for (int i = 0; mod[i].name; i++) {
        mod[i].func(L);
    }

    luaL_setfuncs(L, func, 0);

    lua_pushstring(L, "VERSION");
    lua_pushstring(L, LUA_EVENT_DEMO_VERSION);
    lua_rawset(L, -3);

    return 1;
}
