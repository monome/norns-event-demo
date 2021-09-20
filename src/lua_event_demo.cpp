#include "lua_event_demo.h"

#include <chrono>
#include <thread>
#include <iostream>

// lua
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

extern "C" {
// matron
#include "event_types.h"
#include "event_custom.h"
#include "events.h"
}

//
// state
//

static std::thread generator;
static bool quitting = false;
static bool running = false;
static uint32_t counter = 0;

//
// custom event and behavior
//

static void event_demo_weave_op(lua_State *lvm, void *value, void *context) {
    uint32_t *cp = static_cast<uint32_t*>(value);
    std::cout << "weave_op: called, counter = " << *cp << std::endl;

    // call a global lua function with current counter value
    lua_getglobal(lvm, "event_demo_handler");
    lua_pushinteger(lvm, *cp);
    lua_pcall(lvm, 1, 0, 0); // one argument, zero results, default error message
}

static void event_demo_free_op(void *value, void *context) {
    // nothing to do here since value is just a pointer to the global counter
    uint32_t *cp = static_cast<uint32_t*>(value);
    std::cout << "free_op: called" << std::endl;
}

static struct event_custom_ops event_demo_ops = {
    .type_name = "event_demo",
    .weave = &event_demo_weave_op,
    .free = &event_demo_free_op,
};

//
// module functions
//

static void event_generator_thread() {
    while (!quitting) {
        // create a new event and post to matron's event queue
        union event_data *ev = event_custom_new(&event_demo_ops, &counter, NULL);
        event_post(ev);

        // increment global counter for laughs
        counter++;

        // wait for next thing to happen
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

static int demo_start(lua_State *L) {
    if (!running) {
        quitting = false;
        generator = std::thread(&event_generator_thread);
        std::cout << "event_demo: generator started" << std::endl;
        running = true;
    }
    return 0;
}

static int demo_stop(lua_State *L) {
    if (running) {
        std::cout << "event_demo: stopping generator" << std::endl;
        quitting = true;
        generator.join();
        std::cout << "event_demo: generator stopped";
        running = false;
    }
    return 0;
}

//
// module definition
//

static const luaL_Reg mod[] = {
    {NULL, NULL}
};

static luaL_Reg func[] = {
    {"start", demo_start},
    {"stop", demo_stop},
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
