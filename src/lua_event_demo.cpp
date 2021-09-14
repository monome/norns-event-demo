#include "lua_event_demo.h"

#include <chrono>
#include <thread>
#include <iostream>

extern "C" {
// matron
#include "event_types.h"
#include "events.h"
}

//
// state
//

static std::thread generator;
static bool quitting = false;
static uint32_t counter = 0;

//
// custom event and behavior
//

static void event_demo_weave_op(void *value) {
    uint32_t *cp = static_cast<uint32_t*>(value);
    // TODO: call a global lua function with current counter value
    std::cout << "weave_op: called, counter = " << *cp << std::endl;
}

static void event_demo_free_op(void *value) {
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
        union event_data *ev = event_custom_new(&event_demo_ops, &counter);
        event_post(ev);

        // increment global counter for laughs
        counter++;

        // wait for next thing to happen
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

static int demo_start(lua_State *L) {
    quitting = false;
    generator = std::thread(&event_generator_thread);
    std::cout << "event_demo: generator started" << std::endl;
}

static int demo_stop(lua_State *L) {
    std::cout << "event_demo: stopping generator" << std::endl;
    quitting = true;
    generator.join();
    std::cout << "event_demo: generator stopped";
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
