#ifndef __LUA_MODULE_REGISTER_H__
#define __LUA_MODULE_REGISTER_H__

#include "cocosdenshion/lua_cocos2dx_cocosdenshion_manual.h"
#include "ui/lua_cocos2dx_ui_manual.hpp"
#include "network/lua_xml_http_request.h"
#include "Network/Lua_web_socket.h"
#include "lua_gamebox.h"
#include "AppUtils.h"

int lua_module_register(lua_State* L)
{	
	register_ui_moudle(L);
	register_cocosdenshion_module(L);
	register_xml_http_request(L);
	tolua_web_socket_open(L);
	register_gamebox_module(L);
	register_apputils_module(L);	

    return 1;
}

#endif  // __LUA_MODULE_REGISTER_H__

