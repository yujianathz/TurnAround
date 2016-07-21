#ifndef  _LUA_GAMEBOX_H_
#define  _LUA_GAMEBOX_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_gamebox_module(lua_State* L);

#endif // _LUA_GAMEBOX_H_

