#include "lua.h"
#include "lauxlib.h"
#include "lutil.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include "environ.h"
#endif
/*
---#DES 'env.get_env'
---
---Gets value of env variable.
---Returns value on success or nil, error description and errno on failure.
---@param name string
---@return string?, string?, integer?

name -- value/nil
*/
static int eli_getenv(lua_State *L)
{
    const char *nam = luaL_checkstring(L, 1);
    char *val;
#ifdef _WIN32
    char sval[256];
    size_t len = GetEnvironmentVariable(nam, val = sval, sizeof sval);
    if (sizeof sval < len)
        len = GetEnvironmentVariable(nam, val = lua_newuserdata(L, len), len);
    if (len == 0)
        return push_error(L);
    lua_pushlstring(L, val, len);
#else
    val = getenv(nam);
    if (!val)
        return push_error(L, NULL);
    lua_pushstring(L, val);
#endif
    return 1;
}

/*
---#DES 'env.set_env'
---
---Gets value of env variable.
---Returns true on success or nil, error description and errno on failure.
---@param name string
---@param value string
---@return boolean|nil, nil|string, nil|integer

name value -- true/nil error
name nil -- true/nil error*/
static int eli_setenv(lua_State *L)
{
    const char *nam = luaL_checkstring(L, 1);
    const char *val = lua_tostring(L, 2);
#ifdef _WIN32
    if (!SetEnvironmentVariable(nam, val))
        return push_error(L);
#else
    int err = val ? setenv(nam, val, 1) : unsetenv(nam);
    if (err == -1)
        return push_error(L, NULL);
#endif
    lua_pushboolean(L, 1);
    return 1;
}


/* 
---#DES 'env.environment'
---
---Gets environment table (all env variables).
---Returns table on success or nil, error description and errno on failure.
---@return table<string,string>?, string?, integer?

-- environment-table */
static int eli_environ(lua_State *L)
{
    const char *nam, *val, *end;
    lua_newtable(L);
#ifdef _WIN32
    const char *envs = GetEnvironmentStrings();
    if (!envs)
        return push_error(L);
    for (nam = envs; *nam; nam = end + 1)
    {
        end = strchr(val = strchr(nam, '=') + 1, '\0');
        lua_pushlstring(L, nam, val - nam - 1);
        lua_pushlstring(L, val, end - val);
        lua_settable(L, -3);
    }
#else
    const char **env;
    for (env = (const char **)environ; (nam = *env); env++)
    {
        end = strchr(val = strchr(nam, '=') + 1, '\0');
        lua_pushlstring(L, nam, val - nam - 1);
        lua_pushlstring(L, val, end - val);
        lua_settable(L, -3);
    }
#endif
    return 1;
}

static const struct luaL_Reg eliEnvExtra[] = {
    {"get_env", eli_getenv},
    {"set_env", eli_setenv},
    {"environment", eli_environ},
    {NULL, NULL},
};

int luaopen_eli_env_extra(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, eliEnvExtra, 0);
    return 1;
}
