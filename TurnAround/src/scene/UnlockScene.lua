UnlockScene = class("UnlockScene", BaseScene)

function UnlockScene.create()
    local scene = UnlockScene.new()
    if scene:init() then
        return scene
    end
end

function UnlockScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Unlock) then
        return false
    end

    wps.loadSpriteFrames("unlock")
    
    local height = 0

    self._titleBg = display.newSprite("#common_title_bg")
    self._titleBg:setPositionX(wps.w(self) / 2)
    self:addChild(self._titleBg)
    wps.addChild2Pos(self._titleBg, display.newSprite("#unlock_title"), cc.p(wps.w(self._titleBg) / 2, 160))
    
    height = height + wps.h(self._titleBg)

    local marginTitle = 22
    self._items = {}
    for i = 1, Data.RoleType.Number do
        local item = self:createItem(i)
        item._marginTitle = marginTitle
        item:setPositionX((wps.w(self) - 40) / 3 * ((i - 1) % 3 + 0.5) + 20)
        self:addChild(item)
        table.insert(self._items, item)

        if i % 3 == 0 then
            marginTitle = marginTitle + wps.h(item) + 30
        end
    end

    height = height + marginTitle

    local marginTop = (wps.h(self) - height) / 2
    self._titleBg:setPositionY(wps.h(self) - marginTop - wps.h(self._titleBg) / 2)

    for i = 1, #self._items do
        local item = self._items[i]
        item:setPositionY(wps.bottom(self._titleBg) - item._marginTitle - wps.h(item) / 2)
    end

    return true
end

function UnlockScene:onEnter()
    BaseScene.onEnter(self)

    local listener = wps.addCustomEventListener(Event.SelectedRole, function(event)        
        for i = 1, #self._items do
            local item = self._items[i]
            local isUnlock = Data.isRoleUnlock(item._tag)
            if item._tag == Data.getSelectedRole() then
                item._rect:setSpriteFrame("unlock_item_running")
                item._btn:loadTextureNormal("unlock_btn_running", ccui.TextureResType.plistType)
            else
                if isUnlock then
                    item._rect:setSpriteFrame("unlock_item_unlock")
                    item._btn:loadTextureNormal("unlock_btn_select", ccui.TextureResType.plistType)
                end
            end
        end
    end)
    table.insert(self._listeners, listener)
    
    listener = wps.addCustomEventListener(Event.UnlockRole, function(event)
        local index = event._data
        local item = self._items[index]
        item._rect:setSpriteFrame("unlock_item_unlock")
        item._btn:loadTextureNormal("unlock_btn_select", ccui.TextureResType.plistType)
        item._btn:removeAllChildren()
    end)
    table.insert(self._listeners, listener) 
end

function UnlockScene:onCleanup()
    wps.unloadSpriteFrames("unlock")
    BaseScene.onCleanup(self)
end

function UnlockScene:createItem(index)
    local isSelected = Data.getSelectedRole() == index
    local isUnlock = Data.isRoleUnlock(index)
    local rect = nil
    local btn = nil
    if isUnlock then
        if isSelected then
            rect = display.newSprite("#unlock_item_running")
            btn = JellyButton.create("unlock_btn_running")
            btn:setTouchEnabled(false)
        else
            rect = display.newSprite("#unlock_item_unlock")
            btn = JellyButton.create("unlock_btn_select")
        end
    else
        rect = display.newSprite("#unlock_item_lock")
        btn = JellyButton.create("unlock_btn_unlock")
        
        local ico = display.newSprite("#common_ico_gold")
        ico:setScale(0.5)
        local label = cc.Label:createWithTTF(RoleInfo[index].Price, TTF_FONT, 32)
        label:enableOutline(cc.c4b(0, 0, 0, 0xff), 1)        
        wps.addChild2Center(btn, wps.createSpriteLabel(ico, label))
    end

    local role = display.newSprite(string.format("#role%d_1_1", index))
    role:setAnchorPoint(cc.p(0.5, 0))
    wps.addChild2Pos(rect, role, cc.p(wps.w(rect) / 2, 48))

    local actions = 
    {
        cc.EaseBackOut:create(cc.ScaleTo:create(5.0, 1.05, 0.95)),
        cc.EaseBackIn:create(cc.ScaleTo:create(5.0, 0.95, 1.05)),
    } 
    role:runAction(cc.RepeatForever:create(cc.Sequence:create(actions)))

    local w = math.max(wps.w(rect), wps.w(btn))
    local h = wps.h(rect) + wps.h(btn) - 10
    local node = wps.createNode(cc.Node)
    node:setContentSize(cc.size(w, h))
    node:setAnchorPoint(cc.p(0.5, 0.5))
    wps.addChild2Pos(node, rect, cc.p(wps.w(node) / 2, wps.h(node) - wps.h(rect) / 2))
    wps.addChild2Pos(node, btn, cc.p(wps.w(node) / 2, wps.h(btn) / 2))

    node._rect = rect
    node._btn = btn
    node._tag = index

    btn:addClickEventListener(function(sender) 
        self:onClick(index)
    end)

    return node
end

function UnlockScene:onClick(index)
    local isSelected = Data.getSelectedRole() == index
    local isUnlock = Data.isRoleUnlock(index)
    local rect = nil
    local btn = nil
    if isUnlock then
        if not isSelected then
            Data.setSelectedRole(index)
        end
    else
        if not Data.hasRes(Data.ResType.Gold, RoleInfo[index].Price) then
            return
        end
        Data.changeRes(Data.ResType.Gold, -RoleInfo[index].Price)
        Data.unlockRole(index)
    end    
end