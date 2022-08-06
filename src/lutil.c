#include "lua.h"

#include <errno.h>
#include <string.h>

int push_error(lua_State *L, const char *info)
{
    lua_pushnil(L);
    if (info == NULL)
        lua_pushstring(L, strerror(errno));
    else
        lua_pushfstring(L, "%s: %s", info, strerror(errno));
    lua_pushinteger(L, errno);
    return 3;
}