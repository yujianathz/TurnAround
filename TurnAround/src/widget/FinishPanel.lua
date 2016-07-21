FinishPanel = class("FinishPanel", Panel)

local PLAY_AD_BONUS = 30

function FinishPanel.create(score, gold)
    local panel = FinishPanel.new()
    if panel:init(score, gold) then
        return panel
    end
end

function FinishPanel:init(score, gold)
    if not Panel.init(self) then
        return false
    end

    Data.setBestScore(score)
    Data.changeRes(Data.ResType.Gold, gold)    

    wps.loadSpriteFrames("gameover")

    local bg = display.newSprite("#gameover_bg")
    wps.addChild2Pos(self, bg, cc.p(wps.w(self) / 2, wps.h(self) / 2 + 180))
    self._bg = bg

    local labelGet = cc.Label:createWithTTF(STR.GET..":", TTF_FONT, 60)
    labelGet:setAnchorPoint(cc.p(1.0, 0.5))
    labelGet:setColor(cc.YELLOW)
    labelGet:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, labelGet, cc.p(300, 380))

    local labelScore = cc.Label:createWithTTF(STR.SCORE..":", TTF_FONT, 60)
    labelScore:setAnchorPoint(cc.p(1.0, 0.5))
    labelScore:setColor(cc.c3b(255, 45, 162))
    labelScore:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, labelScore, cc.p(wps.right(labelGet), wps.bottom(labelGet) - wps.h(labelScore) / 2 - 48))

    local labelHighest = cc.Label:createWithTTF(STR.HIGHEST..":", TTF_FONT, 60)
    labelHighest:setAnchorPoint(cc.p(1.0, 0.5))
    labelHighest:setColor(cc.RED)
    labelHighest:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, labelHighest, cc.p(wps.right(labelScore), wps.bottom(labelScore) - wps.h(labelHighest) / 2 - 48))

    local numberGet = cc.Label:createWithTTF(gold, TTF_FONT, 72)
    numberGet:setAnchorPoint(cc.p(0, 0.5))
    numberGet:setColor(labelGet:getColor())
    numberGet:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, numberGet, cc.p(wps.right(labelGet) + 10, wps.y(labelGet)))

    local goldIco = display.newSprite("#common_ico_gold")
    wps.addChild2Pos(bg, goldIco, cc.p(wps.right(numberGet) + wps.w(goldIco) / 2 + 5, wps.y(numberGet)))

    local numberScore = cc.Label:createWithTTF(score, TTF_FONT, 72)
    numberScore:setAnchorPoint(cc.p(0, 0.5))
    numberScore:setColor(labelScore:getColor())
    numberScore:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, numberScore, cc.p(wps.right(labelScore) + 10, wps.y(labelScore)))
    
    local numberHighest = cc.Label:createWithTTF(Data.getBestScore(), TTF_FONT, 72)
    numberHighest:setAnchorPoint(cc.p(0, 0.5))
    numberHighest:setColor(labelHighest:getColor())
    numberHighest:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
    wps.addChild2Pos(bg, numberHighest, cc.p(wps.right(labelHighest) + 10, wps.y(labelHighest)))

    local btnRestart = JellyButton.create("gameover_btn_restart")
    btnRestart:addClickEventListener(function()  
        wps.dispatchCustomEvent(Event.RestartGame)
        self:hide()
    end)
    wps.addChild2Pos(self, btnRestart, cc.p(wps.w(self) / 2, wps.bottom(bg) - wps.h(btnRestart) / 2 - 20))

    if AppUtils.isVideoPlayable() then
        local colorLayer = ccui.Layout:create()
        colorLayer:setContentSize(cc.size(wps.w(self), 132))
        colorLayer:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
        colorLayer:setBackGroundColor(cc.BLACK)
        colorLayer:setBackGroundColorOpacity(0x80)
        colorLayer:setTouchEnabled(true)
        colorLayer:addClickEventListener(function(sender) 
            AppUtils.playVideo()
            Data.changeRes(Data.ResType.Gold, PLAY_AD_BONUS)

            local label = cc.Label:createWithTTF(PLAY_AD_BONUS, TTF_FONT, 48)
            label:setColor(cc.YELLOW)
            label:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
            local node = wps.createSpriteLabel(display.newSprite("#common_ico_gold"), label, 10)
            Toast.getInstance():show(node, 1.0)

            colorLayer:removeFromParent()
        end)
        wps.addChild2Pos(self, colorLayer, cc.p(0, wps.top(btnRestart)), 2)

        local label = cc.Label:createWithTTF(STR.CLICK_TO_GET_GOLD..":", TTF_FONT, 32)
        label:setColor(cc.c3b(241, 243, 196))
        label:enableOutline(cc.c4b(244, 77, 94, 0xff), 2)
        wps.addChild2Center(colorLayer, label)

        local sprite = display.newSprite("#game_btn_video")
        wps.addChild2Pos(colorLayer, sprite, cc.p(wps.left(label) - wps.w(sprite) / 2 - 25, wps.y(label)))

        local value = cc.Label:createWithTTF(PLAY_AD_BONUS, TTF_FONT, 32)
        value:setColor(label:getColor())
        value:enableOutline(cc.c4b(244, 77, 94, 0xff), 2)
        wps.addChild2Pos(colorLayer, value, cc.p(wps.right(label) + wps.w(value) / 2 + 20, wps.y(label)))

        local ico = display.newSprite("#common_ico_gold")
        ico:setScale(0.5)
        wps.addChild2Pos(colorLayer, ico, cc.p(wps.right(value) + wps.sw(ico) / 2 + 10, wps.y(label)))
    end

    local btnBack = JellyButton.create("gameover_btn_back")
    btnBack:addClickEventListener(function()
        wps.dispatchCustomEvent(Event.ExitGame)
        self:hide()
    end)
    wps.addChild2Pos(self, btnBack, cc.p(wps.w(self) / 2 - wps.w(btnBack) / 2 - 10, wps.h(btnBack) / 2 + 46))

    local btnRank = JellyButton.create("main_btn_rank")
	btnRank.setVisible(device.platform ~= "ios")
    btnRank:addClickEventListener(function() 
        self:onRank()
    end)
    wps.addChild2Pos(self, btnRank, cc.p(wps.w(self) / 2 + wps.w(btnRank) / 2 + 10, wps.y(btnBack)))

    local btnShare = JellyButton.create("gameover_btn_share")
    btnShare:addClickEventListener(function() 
        self:onShare()
    end)
    wps.addChild2Pos(self, btnShare, cc.p(wps.left(btnBack) - wps.w(btnShare) / 2 - 20, wps.y(btnBack)))

    local btnUnlock = JellyButton.create("main_btn_unlock")
    btnUnlock:ignoreContentAdaptWithSize(false)
    btnUnlock:setContentSize(cc.size(118, 118))
    btnUnlock:addClickEventListener(function() 
        self:onUnlock()
    end)
    wps.addChild2Pos(self, btnUnlock, cc.p(wps.right(btnRank) + wps.w(btnUnlock) / 2 + 20, wps.y(btnRank)))

    if Data.getBestScore() == score then
        local crown = display.newSprite("#gameover_best_score")
        crown:setScale(3.0)
        crown:setOpacity(0)
        crown:setPosition(cc.p(wps.w(self) / 2, wps.h(self) / 2))
        wps.addChild2Center(self, crown)

        local params = 
        {
            cc.ScaleTo:create(0.5, 1.0),
            cc.FadeIn:create(0.5),
            cc.MoveTo:create(0.5, self:convertToNodeSpace(labelScore:convertToWorldSpace(cc.p(0, wps.h(labelScore))))),
        }
        crown:runAction(cc.Spawn:create(params))
    end

    Data.playSound("fail")

    self:runAction(wps.seq(1.0, function() AppUtils.showAd2() end))    
    return true
end

function FinishPanel:onCleanup()
    wps.unloadSpriteFrames("gameover")
    Panel.onCleanup(self)
end

function FinishPanel:onShare()    
    local renderTexture = cc.RenderTexture:create(wps.w(self), wps.h(self), cc.TEXTURE2_D_PIXEL_FORMAT_RGB_A8888)
    renderTexture:beginWithClear(0, 0, 0, 0)
    BaseScene.RunningScene:visit()
    renderTexture:endToLua()
    if renderTexture:saveToFile("screenshot.jpg", cc.IMAGE_FORMAT_JPEG) then
        Data.playSound("screenshot")

        local layer = cc.LayerColor:create(cc.c4b(0xff, 0xff, 0xff, 0), wps.w(self), wps.h(self))
        layer:runAction(wps.seq(cc.FadeTo:create(0.2, 0xa0), cc.FadeOut:create(1.0), function() 
            BaseScene.pushScene(ShareScene.create())
        end, cc.RemoveSelf:create()))
        BaseScene.RunningScene:addChild(layer, BaseScene.ZOrder.Indicator)
    end
end

function FinishPanel:onRank()
    BaseScene.pushScene(RankScene.create())
end

function FinishPanel:onUnlock()
    BaseScene.pushScene(UnlockScene.create())
end