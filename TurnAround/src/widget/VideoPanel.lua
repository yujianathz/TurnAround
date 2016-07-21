VideoPanel = class("VideoPanel", Panel)

function VideoPanel.create(showVideoCallback, cancelCallback)
    local panel = VideoPanel.new()
    if panel:init(showVideoCallback, cancelCallback) then
        return panel
    end
end

function VideoPanel:init(showVideoCallback, cancelCallback)
    if not Panel.init(self) then
        return false
    end
    self._showVideoCallback = showVideoCallback
    self._cancelCallback = cancelCallback    
    self:addClickEventListener(function(sender) 
        self._showVideoCallback()
        self:hide()
    end)

    local spriteVideo = display.newSprite("#game_btn_video")
    wps.addChild2Pos(self, spriteVideo, cc.p(wps.w(self) / 2, wps.h(self) * 0.25))
    self._spriteVideo = spriteVideo

    local label = cc.Label:createWithTTF(STR.CLICK_TO_RELIVE, TTF_FONT, 32)
    label:setColor(cc.c3b(241, 243, 196))
    label:enableOutline(cc.c4b(244, 77, 94, 0xff), 2)
    wps.addChild2Pos(self, label, cc.p(wps.x(spriteVideo), wps.top(spriteVideo) + wps.h(label) / 2 + 10))

    local loadingBg = display.newSprite("#game_progress_bg", nil, nil, {capInsets = cc.rect(20, 0, 2, 27)})
    loadingBg:setContentSize(cc.size(wps.w(spriteVideo) + 80, 27))
    wps.addChild2Pos(self, loadingBg, cc.p(wps.w(self) / 2, wps.bottom(spriteVideo) - wps.h(loadingBg) / 2 - 10))
    self._loadingBg = loadingBg

    local loadingFg = ccui.LoadingBar:create()
    loadingFg:setScale9Enabled(true)
    loadingFg:setCapInsets(cc.rect(20, 0, 2, 27))
    loadingFg:setContentSize(loadingBg:getContentSize())
    loadingFg:loadTexture("game_progress_fg", ccui.TextureResType.plistType)
    loadingFg:setPercent(100)
    wps.addChild2Center(loadingBg, loadingFg)
    self._loadingFg = loadingFg

    return true
end

function VideoPanel:onEnter()
    Panel.onEnter(self)

    self:scheduleUpdateWithPriorityLua(function(dt) 
        if self._loadingFg:getPercent() <= 0 then
            self:unscheduleUpdate()            
            self:runAction(cc.CallFunc:create(function() 
                self._cancelCallback()
                self:hide() 
            end))
            return
        end

        self._loadingFg:setPercent(self._loadingFg:getPercent() - dt * 100 / 3)
    end, 0)
end

function VideoPanel:onExit()
    Panel.onExit(self)

    self:unscheduleUpdate()
end