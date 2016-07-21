CC_USE_DEPRECATED_API = true
CC_USE_FRAMEWORK = true

require "cocos.init"

require "init"

local function main()
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    math.randomseed(os.time())

    Panel.setPanelZOrder(BaseScene.ZOrder.Panel)
    Panel.setShowEvent(Event.ShowPanel)
    Panel.setHideEvent(Event.HidePanel)
    Form.setFrame("common_form_bg", cc.rect(90, 100, 4, 7))

    local scene = LogoScene.create()
    table.insert(BaseScene.SceneIds, scene._sceneId)
    wps.Director:replaceScene(scene)
end

-- cclog
cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
end

xpcall(main, __G__TRACKBACK__)
