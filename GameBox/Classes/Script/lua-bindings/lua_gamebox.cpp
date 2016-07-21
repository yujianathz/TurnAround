#include "lua_gamebox.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"

#include "GBButton.h"
#include "GBUpdate.h"
#include "GBShaderButton.h"
#include "GBNumberLabel.h"
#include "GBUtils.h"

static int tolua_gamebox_GBButton_create(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBButton", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string framename = ((const std::string)  tolua_tocppstring(tolua_S, 2, 0));
		GBButton* ret = GBButton::create(framename);
		object_to_luaval<GBButton>(tolua_S, "GBButton", (GBButton*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBButton:create", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBButton_create'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_register_gamebox_GBButton(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBButton");
	tolua_cclass(tolua_S, "GBButton", "GBButton", "ccui.Button", nullptr);

	tolua_beginmodule(tolua_S, "GBButton");
		tolua_function(tolua_S, "create", tolua_gamebox_GBButton_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBButton).name();
	g_luaType[typeName] = "GBButton";
	g_typeCast["GBButton"] = "GBButton";

	return 1;
}

static int tolua_gamebox_GBSqlite_getBool(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isboolean(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		bool defaultValue = (bool)tolua_toboolean(tolua_S, 2, 0);
		bool ret = GBSqlite::getBoolForKey(key.c_str(), defaultValue);
		tolua_pushboolean(tolua_S, ret);		
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.getBool", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_getBool'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBSqlite_getNumber(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		lua_Number defaultValue = tolua_tonumber(tolua_S, 2, 0);
		float ret = GBSqlite::getFloatForKey(key.c_str(), defaultValue);
		tolua_pushnumber(tolua_S, ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.getNumber", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_getNumber'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBSqlite_getString(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isstring(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		const std::string defaultValue = ((const std::string)  tolua_tocppstring(tolua_S, 2, 0));
		const std::string ret = GBSqlite::getStringForKey(key.c_str(), defaultValue);
		tolua_pushstring(tolua_S, ret.c_str());
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.getString", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_getString'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBSqlite_setBool(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isboolean(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		bool defaultValue = (bool)tolua_toboolean(tolua_S, 2, 0);
		GBSqlite::setBoolForKey(key.c_str(), defaultValue);

		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.setBool", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_setBool'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBSqlite_setNumber(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		lua_Number defaultValue = tolua_tonumber(tolua_S, 2, 0);
		GBSqlite::setFloatForKey(key.c_str(), defaultValue);

		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.setNumber", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_setNumber'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBSqlite_setString(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isstring(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string key = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		const std::string defaultValue = ((const std::string)  tolua_tocppstring(tolua_S, 2, 0));
		GBSqlite::setStringForKey(key.c_str(), defaultValue);

		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBSqlite.setString", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBSqlite_setString'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_register_gamebox_GBSqlite(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBSqlite");
	tolua_cclass(tolua_S, "GBSqlite", "GBSqlite", "", nullptr);

	tolua_beginmodule(tolua_S, "GBSqlite");
	tolua_function(tolua_S, "getBool", tolua_gamebox_GBSqlite_getBool);
	tolua_function(tolua_S, "getNumber", tolua_gamebox_GBSqlite_getNumber);
	tolua_function(tolua_S, "getString", tolua_gamebox_GBSqlite_getString);
	tolua_function(tolua_S, "setBool", tolua_gamebox_GBSqlite_setBool);
	tolua_function(tolua_S, "setNumber", tolua_gamebox_GBSqlite_setNumber);
	tolua_function(tolua_S, "setString", tolua_gamebox_GBSqlite_setString);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBSqlite).name();
	g_luaType[typeName] = "GBSqlite";
	g_typeCast["GBSqlite"] = "GBSqlite";

	return 1;
}

static int tolua_gamebox_GBUpdate_getInstance(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "GBUpdate", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'tolua_gamebox_GBUpdate_getInstance'", nullptr);
			return 0;
		}
		auto ret = GBUpdate::getInstance();
		object_to_luaval<GBUpdate>(tolua_S, "GBUpdate", (GBUpdate*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "GBUpdate:getInstance", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBUpdate_getInstance'.", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBUpdate_getRealPath(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;
	GBUpdate* self = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "GBUpdate", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<GBUpdate *>(tolua_tousertype(tolua_S, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_gamebox_GBUpdate_getRealPath'\n", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (1 == argc)
	{
		const char* arg0;
		std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "GBUpdate:getRealPath"); arg0 = arg0_tmp.c_str();
		if (ok)
		{
			string realPath = GBUpdate::getInstance()->getRealPath(arg0);
			tolua_pushstring(tolua_S, realPath.c_str());

			return 1;
		}
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBUpdate:getRealPath", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBUpdate_getRealPath'.", &tolua_err);
	return 0;
#endif
}

static int lua_register_gamebox_GBUpdate(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBUpdate");
	tolua_cclass(tolua_S, "GBUpdate", "GBUpdate", "", nullptr);

	tolua_beginmodule(tolua_S, "GBUpdate");
	tolua_function(tolua_S, "getInstance", tolua_gamebox_GBUpdate_getInstance);
	tolua_function(tolua_S, "getRealPath", tolua_gamebox_GBUpdate_getRealPath);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBUpdate).name();
	g_luaType[typeName] = "GBUpdate";
	g_typeCast["GBUpdate"] = "GBUpdate";

	return 1;
}

static int tolua_gamebox_GBShaderButton_create(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBShaderButton", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 2)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 2, 0, &tolua_err) || !tolua_isstring(tolua_S, 3, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string framename = ((const std::string)  tolua_tocppstring(tolua_S, 2, 0));
		const std::string pressShader = ((const std::string) tolua_tocppstring(tolua_S, 3, 0));
		GBShaderButton* ret = GBShaderButton::create(framename, pressShader);
		object_to_luaval<GBShaderButton>(tolua_S, "GBShaderButton", (GBShaderButton*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBShaderButton:create", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBShaderButton_create'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_register_gamebox_GBShaderButton(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBShaderButton");
	tolua_cclass(tolua_S, "GBShaderButton", "GBShaderButton", "GBButton", nullptr);

	tolua_beginmodule(tolua_S, "GBShaderButton");
	tolua_function(tolua_S, "create", tolua_gamebox_GBShaderButton_create);	
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBShaderButton).name();
	g_luaType[typeName] = "GBShaderButton";
	g_typeCast["GBShaderButton"] = "GBShaderButton";

	return 1;
}

static int tolua_gamebox_GBUtils_getOpenUDID(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	std::string ret = GBUtils::getOpenUDID();
	tolua_pushstring(tolua_S, ret.c_str());

	return 1;
}

static int tolua_gamebox_GBUtils_purchase(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	int argc = lua_gettop(tolua_S);
	if (argc == 1)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string str = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		GBUtils::purchase(str);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBUtils.purchase", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBUtils_purchase'.\n", &tolua_err);
#endif
	return 0;
}

static int lua_register_gamebox_GBUtils(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBUtils");
	tolua_cclass(tolua_S, "GBUtils", "GBUtils", "", nullptr);

	tolua_beginmodule(tolua_S, "GBUtils");
	tolua_function(tolua_S, "getOpenUDID", tolua_gamebox_GBUtils_getOpenUDID);
	tolua_function(tolua_S, "purchase", tolua_gamebox_GBUtils_purchase);	
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBUtils).name();
	g_luaType[typeName] = "GBUtils";
	g_typeCast["GBUtils"] = "GBUtils";

	return 1;
}

static int tolua_gamebox_GBNumberLabel_createWithTTF(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBNumberLabel", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc >= 4)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) || !tolua_isstring(tolua_S, 3, 0, &tolua_err)
			|| !tolua_isstring(tolua_S, 4, 0, &tolua_err) || !tolua_isnumber(tolua_S, 5, 0, &tolua_err))
		{
			goto tolua_lerror;
		}

		if (argc == 5 && !tolua_isboolean(tolua_S, 6, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		int targetNum = tolua_tonumber(tolua_S, 2, 0);
		const std::string format = ((const std::string)  tolua_tocppstring(tolua_S, 3, 0));
		const std::string font = ((const std::string) tolua_tocppstring(tolua_S, 4, 0));
		int size = tolua_tonumber(tolua_S, 5, 0);
		GBNumberLabel * ret;
		if (argc == 5)
		{
			bool isShort = (bool)tolua_toboolean(tolua_S, 6, 0);
			ret = GBNumberLabel::createWithTTF(targetNum, format, font, size, isShort);
		}
		else
		{
			ret = GBNumberLabel::createWithTTF(targetNum, format, font, size);
		}
		object_to_luaval<GBNumberLabel>(tolua_S, "GBNumberLabel", (GBNumberLabel*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBNumberLabel:createWithTTF", argc, 4);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBNumberLabel_createWithTTF'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBNumberLabel_createWithSystemFont(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBNumberLabel", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc >= 4)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) || !tolua_isstring(tolua_S, 3, 0, &tolua_err)
			|| !tolua_isstring(tolua_S, 4, 0, &tolua_err) || !tolua_isnumber(tolua_S, 5, 0, &tolua_err))
		{
			goto tolua_lerror;
		}

		if (argc == 5 && !tolua_isboolean(tolua_S, 6, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		int targetNum = tolua_tonumber(tolua_S, 2, 0);
		const std::string format = ((const std::string)  tolua_tocppstring(tolua_S, 3, 0));
		const std::string font = ((const std::string) tolua_tocppstring(tolua_S, 4, 0));
		int size = tolua_tonumber(tolua_S, 5, 0);
		GBNumberLabel * ret;
		if (argc == 5)
		{
			bool isShort = (bool)tolua_toboolean(tolua_S, 6, 0);
			ret = GBNumberLabel::createWithSystemFont(targetNum, format, font, size, isShort);
		}
		else
		{
			ret = GBNumberLabel::createWithSystemFont(targetNum, format, font, size);
		}
		object_to_luaval<GBNumberLabel>(tolua_S, "GBNumberLabel", (GBNumberLabel*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBNumberLabel:createWithSystemFont", argc, 4);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBNumberLabel_createWithSystemFont'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBNumberLabel_createWithBMFont(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBNumberLabel", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc >= 3)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) || !tolua_isstring(tolua_S, 3, 0, &tolua_err)
			|| !tolua_isstring(tolua_S, 4, 0, &tolua_err))
		{
			goto tolua_lerror;
		}

		if (argc == 4 && !tolua_isboolean(tolua_S, 5, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		int targetNum = tolua_tonumber(tolua_S, 2, 0);
		const std::string format = ((const std::string)  tolua_tocppstring(tolua_S, 3, 0));
		const std::string font = ((const std::string) tolua_tocppstring(tolua_S, 4, 0));		
		GBNumberLabel * ret;
		if (argc == 4)
		{
			bool isShort = (bool)tolua_toboolean(tolua_S, 5, 0);
			ret = GBNumberLabel::createWithBMFont(targetNum, format, font, isShort);
		}
		else
		{
			ret = GBNumberLabel::createWithBMFont(targetNum, format, font);
		}
		object_to_luaval<GBNumberLabel>(tolua_S, "GBNumberLabel", (GBNumberLabel*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBNumberLabel:createWithBMFont", argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBNumberLabel_createWithBMFont'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBNumberLabel_enableOutline(lua_State* tolua_S)
{
	int argc = 0;
	GBNumberLabel* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "GBNumberLabel", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (GBNumberLabel*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'tolua_gamebox_GBNumberLabel_enableOutline'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		cocos2d::Color4B arg0;

		ok &= luaval_to_color4b(tolua_S, 2, &arg0, "GBNumberLabel:enableOutline");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'tolua_gamebox_GBNumberLabel_enableOutline'", nullptr);
			return 0;
		}
		cobj->enableOutline(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	if (argc == 2)
	{
		cocos2d::Color4B arg0;
		int arg1;

		ok &= luaval_to_color4b(tolua_S, 2, &arg0, "cc.Label:enableOutline");

		ok &= luaval_to_int32(tolua_S, 3, (int *)&arg1, "cc.Label:enableOutline");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'tolua_gamebox_GBNumberLabel_enableOutline'", nullptr);
			return 0;
		}
		cobj->enableOutline(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "GBNumberLabel:enableOutline", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBNumberLabel_enableOutline'.", &tolua_err);
#endif

	return 0;
}

static int tolua_gamebox_GBNumberLabel_setTargetNum(lua_State* tolua_S)
{
	int argc = 0;
	GBNumberLabel* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "GBNumberLabel", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (GBNumberLabel*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'tolua_gamebox_GBNumberLabel_setTargetNum'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif
		int targetNum = tolua_tonumber(tolua_S, 2, 0);
		cobj->setTargetNum(targetNum);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "GBNumberLabel:setTargetNum", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBNumberLabel_setTargetNum'.", &tolua_err);
#endif

	return 0;
}

static int lua_register_gamebox_GBNumberLabel(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBNumberLabel");
	tolua_cclass(tolua_S, "GBNumberLabel", "GBNumberLabel", "cc.Node", nullptr);

	tolua_beginmodule(tolua_S, "GBNumberLabel");
	tolua_function(tolua_S, "createWithTTF", tolua_gamebox_GBNumberLabel_createWithTTF);
	tolua_function(tolua_S, "createWithSystemFont", tolua_gamebox_GBNumberLabel_createWithSystemFont);
	tolua_function(tolua_S, "createWithBMFont", tolua_gamebox_GBNumberLabel_createWithBMFont);
	tolua_function(tolua_S, "enableOutline", tolua_gamebox_GBNumberLabel_enableOutline);
	tolua_function(tolua_S, "setTargetNum", tolua_gamebox_GBNumberLabel_setTargetNum);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBNumberLabel).name();
	g_luaType[typeName] = "GBNumberLabel";
	g_typeCast["GBNumberLabel"] = "GBNumberLabel";

	return 1;
}

static int tolua_gamebox_GBShaderEffect_create(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "GBShaderEffect", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 2, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif
		const std::string str = ((const std::string)  tolua_tocppstring(tolua_S, 2, 0));
		GBShaderEffect* ret = GBShaderEffect::create(str);
		object_to_luaval<GBShaderEffect>(tolua_S, "GBShaderEffect", (GBShaderEffect*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "GBShaderEffect:create", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBShaderEffect_create'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_gamebox_GBShaderEffect_getGLProgramState(lua_State* tolua_S)
{
	int argc = 0;
	GBShaderEffect* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    //if (!tolua_isusertype(tolua_S, 1, "GBShaderEffect", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (GBShaderEffect*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'tolua_gamebox_GBShaderEffect_getGLProgramState'", nullptr);
		return 0;
	}
#endif

	GLProgramState * ret = cobj->getGLProgramState();
	object_to_luaval<GLProgramState>(tolua_S, "cc.GLProgramState", (GLProgramState*)ret);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_gamebox_GBShaderEffect_getGLProgramState'.", &tolua_err);
#endif

	return 0;
}

static int lua_register_gamebox_GBShaderEffect(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "GBShaderEffect");
	tolua_cclass(tolua_S, "GBShaderEffect", "GBShaderEffect", "cc.Ref", nullptr);

	tolua_beginmodule(tolua_S, "GBShaderEffect");
	tolua_function(tolua_S, "create", tolua_gamebox_GBShaderEffect_create);
	tolua_function(tolua_S, "getGLProgramState", tolua_gamebox_GBShaderEffect_getGLProgramState);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(GBShaderEffect).name();
	g_luaType[typeName] = "GBShaderEffect";
	g_typeCast["GBShaderEffect"] = "GBShaderEffect";

	return 1;
}

int register_gamebox_module(lua_State* L)
{
	lua_getglobal(L, "_G");
	if (lua_istable(L, -1))//stack:...,_G,
	{
		tolua_open(L);
		lua_register_gamebox_GBButton(L);
		lua_register_gamebox_GBShaderButton(L);
		lua_register_gamebox_GBSqlite(L);
		lua_register_gamebox_GBUpdate(L);
		lua_register_gamebox_GBUtils(L);
		lua_register_gamebox_GBShaderEffect(L);
		lua_register_gamebox_GBNumberLabel(L);
		tolua_endmodule(L);
	}
	lua_pop(L, 1);

	return 1;
}