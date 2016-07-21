Panel = class("Panel", function() return wps.createNode(ccui.Layout) end)

local Panels = {}

function Panel.hasPanels()
    return #Panels > 0
end

function Panel.setPanelZOrder(zorder)
    Panel.ZOrder = zorder
end

function Panel.setShowEvent(event)
    Panel.ShowEvent = event
end

function Panel.setHideEvent(event)
    Panel.HideEvent = event
end

function Panel:init()
    self:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
    self:setBackGroundColor(cc.BLACK)
    self:setBackGroundColorOpacity(0xa0)
    self:setContentSize(wps.WinSize)
    self:setTouchEnabled(true)

    return true
end

function Panel:onEnter()    
    table.insert(Panels, self)
    self._pos = #Panels

    if Panel.ShowEvent then
        wps.dispatchCustomEvent(Panel.ShowEvent, self)
    end
end

function Panel:onExit()
    table.remove(Panels, self._pos)
    self._pos = nil

    if Panel.HideEvent then
        wps.dispatchCustomEvent(Panel.HideEvent, self)        
    end
end

function Panel:onCleanup()

end

function Panel:show()
    BaseScene.RunningScene:addChild(self, Panel.ZOrder)
end

function Panel:hide()
    self:removeFromParent()
end