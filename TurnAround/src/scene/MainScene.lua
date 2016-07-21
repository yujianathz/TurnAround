MainScene = class("MainScene", BaseScene)

function MainScene.create()
    local scene = MainScene.new()
    if scene:init() then
        return scene
    end
end

function MainScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Main) then
        return false
    end

    wps.loadSpriteFrames("main")

    local btnSound = JellyButton.create("main_btn_sound")
    btnSound:addClickEventListener(function()        
        Data.setIsMute(not Data.IsMute)
        btnSound._mute:setVisible(Data.IsMute)
    end)
    wps.addChild2Pos(self, btnSound, cc.p(wps.w(btnSound) / 2 + 15, wps.h(btnSound) / 2 + 15))

    btnSound._mute = display.newSprite("#main_img_mute")
    btnSound._mute:setVisible(Data.IsMute)
    wps.addChild2Center(btnSound, btnSound._mute)

    local btnRank = JellyButton.create("main_btn_rank")
	btnRank.setVisible(device.platform ~= "ios")
    btnRank:addClickEventListener(function()
        self:onRank()
    end)
    wps.addChild2Pos(self, btnRank, cc.p(wps.w(self) - wps.w(btnRank) / 2 - 15, wps.y(btnSound)))

    local spriteCircle = display.newSprite("#common_circle")
    wps.addChild2Pos(self, spriteCircle, cc.p(wps.w(self) / 2, wps.h(self) / 2 - 60))

    local spriteRole = display.newSprite(string.format("#role%d_%d_1", Data.getSelectedRole(), math.random() > 0.5 and 1 or 2))
    spriteRole:setScale(0)
    spriteRole:setAnchorPoint(cc.p(0.5, 0))
    wps.addChild2Pos(spriteCircle, spriteRole, cc.p(wps.w(spriteCircle) / 2, wps.h(spriteCircle) - 30))
    self._spriteRole = spriteRole

    spriteRole:runAction(wps.seq(cc.EaseBackOut:create(cc.ScaleTo:create(0.2, 1.0)), function()
        local actions = 
        {
            cc.EaseBackOut:create(cc.ScaleTo:create(0.2, 1.05, 0.95)),
            cc.EaseBackIn:create(cc.ScaleTo:create(1.5, 0.95, 1.05)),
        } 
        spriteRole:runAction(cc.RepeatForever:create(cc.Sequence:create(actions)))
        spriteRole:runAction(cc.RepeatForever:create(cc.JumpBy:create(0.5, cc.p(0, 0), 5, 1)))
    end))

    local btnStart = JellyButton.create("main_btn_start")
    btnStart._isDisableSound = true
    btnStart:addClickEventListener(function()
        self:onGame()
    end)
    wps.addChild2Center(spriteCircle, btnStart)

    local spriteTitle = display.newSprite("#main_title")
    wps.addChild2Pos(self, spriteTitle, cc.p(wps.w(self) / 2, wps.top(spriteCircle) + wps.h(spriteTitle) / 2 + 60))

    spriteTitle:runAction(cc.RepeatForever:create(
        wps.seq(cc.EaseBackOut:create(cc.MoveTo:create(5.0, cc.p(wps.w(self) / 2 - 10, wps.y(spriteTitle)))), cc.EaseBackOut:create(cc.MoveTo:create(5.0, cc.p(wps.w(self) / 2 + 10, wps.y(spriteTitle)))))))

    local btnGift = JellyButton.create("main_btn_gift")
    btnGift:addClickEventListener(function() self:onGift() end)
    btnGift:setEnabled(false)
    wps.addChild2Pos(self, btnGift, cc.p(wps.w(self) / 2 - wps.w(btnGift) / 2 - 50, wps.bottom(spriteCircle) - wps.h(btnGift) / 2 - 36))
    self._btnGift = btnGift

    local timeBg = display.newSprite("#common_message_bg", nil, nil, {capInsets = cc.rect(26, 0, 2, 69)})
    timeBg:setContentSize(cc.size(wps.w(btnGift) + 20, 69))
    timeBg:setAnchorPoint(cc.p(0.5, 0))
    wps.addChild2Pos(btnGift, timeBg, cc.p(wps.w(btnGift) / 2, wps.h(btnGift)))
    self._timeBg = timeBg

    local timeLabel = cc.Label:createWithSystemFont("", SYS_FONT, 32)
    wps.addChild2Pos(timeBg, timeLabel, cc.p(wps.w(timeBg) / 2, 48))
    self._timeLabel = timeLabel

    local btnUnlock = JellyButton.create("main_btn_unlock")
    btnUnlock:addClickEventListener(function() self:onUnlock() end)
    wps.addChild2Pos(self, btnUnlock, cc.p(wps.w(self) / 2 + wps.w(btnUnlock) / 2 + 50, wps.y(btnGift)))

    Data.playMusic("bg")

    return true;
end

function MainScene:onEnter()
    BaseScene.onEnter(self)

    AppUtils.showAd1()

    self._spriteRole:setSpriteFrame(string.format("role%d_%d_1", Data.getSelectedRole(), math.random() > 0.5 and 1 or 2))
    
    self:scheduleUpdateWithPriorityLua(function(dt) self:onUpdate(dt) end, 0)
end

function MainScene:onExit()
    BaseScene.onExit(self)
    
    self:unscheduleUpdate()
end

function MainScene:onCleanup()
    wps.unloadSpriteFrames("main")

    BaseScene.onCleanup(self)
end

function MainScene:onKeyBack()
    DialogForm.create(STR.SURE_TO_EXIT, function() wps.Director:endToLua() end):show()
end

function MainScene:onRank()
	BaseScene.pushScene(RankScene.create())  
end

function MainScene:onGame()
    Data.playSound("start")
    BaseScene.pushScene(GameScene.create())
end

function MainScene:onGift()
    for i = 1, #GiftInfo do
        local info = GiftInfo[i]
        if info._isClaimed == nil then
            if Data.RunningDuration >= info.Duration then
                local label = cc.Label:createWithTTF(info.Gold, TTF_FONT, 48)
                label:setColor(cc.YELLOW)
                label:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
                local node = wps.createSpriteLabel(display.newSprite("#common_ico_gold"), label, 10)
                Toast.getInstance():show(node)

                Data.changeRes(Data.ResType.Gold, info.Gold)

                info._isClaimed = true
                Data.RunningDuration = info.Duration
                break    
            end
        end
    end    
end

function MainScene:onUnlock()
    BaseScene.pushScene(UnlockScene.create())
end

function MainScene:onUpdate(dt)    
    for i = 1, #GiftInfo do
        local info = GiftInfo[i]
        if info._isClaimed == nil then
            if Data.RunningDuration >= info.Duration then
                self._btnGift:setEnabled(true)
                self._timeBg:setVisible(false)               
            else
                self._btnGift:setEnabled(false)
                self._timeBg:setVisible(true)
                self._timeLabel:setString(Data.formatTime(info.Duration - Data.RunningDuration))
            end
            return
        end
    end
    self._btnGift:setEnabled(false)
    self._timeBg:setVisible(false)
end


