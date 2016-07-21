wps = wps or {}

wps.Director = cc.Director:getInstance()
wps.FileUtils = cc.FileUtils:getInstance()
wps.UserDefault = cc.UserDefault:getInstance()
wps.SpriteFrameCache = cc.SpriteFrameCache:getInstance()
wps.WinSize = wps.Director:getWinSize()
wps.Dispatcher = wps.Director:getEventDispatcher()
wps.Update = GBUpdate:getInstance()

function wps.size(node)
    return node:getContentSize()
end

function wps.w(node)
	return wps.size(node).width
end

function wps.sw(node)
    return wps.w(node) * node:getScaleX()
end

function wps.h(node)
    return wps.size(node).height
end

function wps.sh(node)
    return wps.h(node) * node:getScaleY()
end

function wps.x(node)
    return node:getPositionX()
end

function wps.y(node)
    return node:getPositionY()
end

function wps.ax(node)
    return node:getAnchorPoint().x
end

function wps.ay(node)
    return node:getAnchorPoint().y
end

function wps.left(node)
    return wps.x(node) - wps.ax(node) * wps.sw(node)
end

function wps.right(node)
    return wps.x(node) + (1 - wps.ax(node)) * wps.sw(node)
end

function wps.top(node)
    return wps.y(node) + (1 - wps.ay(node)) * wps.sh(node)
end

function wps.bottom(node)
    return wps.y(node) - wps.ay(node) * wps.sh(node)
end

function wps.addChild2Center(parent, child, zorder)
    local w = wps.w(parent)
    local h =  wps.h(parent)
    local ax = wps.ax(child)
    local ay = wps.ay(child)
    local sw = wps.sw(child)
    local sh = wps.sh(child)
    local pos = cc.p(w / 2 - (0.5 - ax) * sw, h / 2 - (0.5 - ay) * sh)
    wps.addChild2Pos(parent, child, pos, zorder)
end

function wps.addChild2Pos(parent, child, pos, zorder)
    child:setPosition(pos)
    parent:addChild(child, zorder or 0)
end

function wps.createNode(classname, ...)
    local node = classname:create(...)
    node:enableNodeEvents()

    return node
end

function wps.seq(...)
    local params = {...}
    for i, param in ipairs(params) do
        local type = type(param)
        if type == "function" then
            params[i] = cc.CallFunc:create(param)
        elseif type == "number" then
            params[i] = cc.DelayTime:create(param)
        end
    end

    return cc.Sequence:create(params)
end

function wps.dispatchCustomEvent(eventName, eventCustom)
    local event = cc.EventCustom:new(eventName)
    event._data = eventCustom
    wps.Director:getEventDispatcher():dispatchEvent(event)
end

function wps.addCustomEventListener(eventName, eventCallback)
    local listener = cc.EventListenerCustom:create(eventName, eventCallback)
    wps.Director:getEventDispatcher():addEventListenerWithFixedPriority(listener, 1)

    return listener
end

function wps.removeEventListener(listener)
    wps.Director:getEventDispatcher():removeEventListener(listener)
end

function wps.loadSpriteFrames(name)
    display.loadSpriteFrames(wps.Update:getRealPath(name..".plist"), wps.Update:getRealPath(name..".png"))
end

function wps.unloadSpriteFrames(name)
    display.removeSpriteFrames(wps.Update:getRealPath(name..".plist"), wps.Update:getRealPath(name..".png"))
end

--------------------------------- helper ------------------------------------------------------------------
function wps.createSpriteLabel(sprite, label, space)
    local node = wps.createNode(cc.Node)
    node:setAnchorPoint(cc.p(0.5, 0.5))
    node:setContentSize(cc.size(wps.sw(sprite) + wps.sw(label) + (space or 0), math.max(wps.sh(sprite), wps.sh(label))))
    wps.addChild2Pos(node, sprite, cc.p(wps.sw(sprite) / 2, wps.h(node) / 2))
    wps.addChild2Pos(node, label, cc.p(wps.w(node) - wps.sw(label) / 2, wps.h(node) / 2))

    return node
end
--------------------------------- helper ------------------------------------------------------------------