Toast = class("Toast", function() return wps.createNode(cc.Node) end)

function Toast.getInstance()
    if Toast._instance == nil then
        Toast._instance = Toast.new()
        Toast._instance:retain()
    end

    return Toast._instance
end

function Toast:ctor()
    self:setContentSize(wps.WinSize)
end

function Toast:show(displayNode, delay)
    local node = nil
    if type(displayNode) == "string" then
        local label = cc.Label:createWithTTF(displayNode, TTF_FONT, 70)
        label:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
        node = label
    else
        node = displayNode
    end        

    node:setCascadeOpacityEnabled(true)
    node:setScale(0)
    node:runAction(wps.seq((delay or 0) + 1.2, cc.FadeOut:create(0.2), cc.RemoveSelf:create()))
    node:runAction(wps.seq(delay or 0, cc.ScaleTo:create(0.1, 1.0), cc.MoveBy:create(1.4, cc.p(0, 160))))
    wps.addChild2Center(self, node)
end