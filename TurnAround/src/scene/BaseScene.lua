BaseScene = class("BaseScene", function() return wps.createNode(cc.Scene) end)

BaseScene.SceneId = 
{
    Logo                = 1,
    Main                = 2,
    Game               = 3,
    Share               = 4,
    Unlock             = 5,
    Rank                = 6,
}

BaseScene.ZOrder = 
{
    Bg                  = 0,
    Ui                  = 10,
    Panel               = 20,
    Indicator           = 30,
    Dialog              = 40,
    Guide               = 50,
    Toast               = 60,
    Touch               = 70,
}

BaseScene.SceneIds = {}
BaseScene.RunningScene = nil

function BaseScene.pushScene(scene)
    if scene then
        if #BaseScene.SceneIds > 0 and BaseScene.SceneIds[#BaseScene.SceneIds] ~= scene._sceneId then
            table.insert(BaseScene.SceneIds, scene._sceneId)
            wps.Director:pushScene(scene)
        end    
    end
end

function BaseScene.popScene()
    if #BaseScene.SceneIds > 0 then
        table.remove(BaseScene.SceneIds, #BaseScene.SceneIds)
        wps.Director:popScene()
    end
end

function BaseScene.replaceScene(scene)
    if scene then
        if #BaseScene.SceneIds > 0 and BaseScene.SceneIds[#BaseScene.SceneIds] ~= scene._sceneId then
            BaseScene.SceneIds[#BaseScene.SceneIds] = scene._sceneId
            wps.Director:replaceScene(scene)
        end
    end
end

function BaseScene.runWithScene(scene)
    if scene then
        if #BaseScene.SceneIds == 0 then
            table.insert(BaseScene.SceneIds, scene._sceneId)
            wps.Director:runWithScene(scene)
        end    
    end
end

function BaseScene:init(sceneId)
    self._sceneId = sceneId
    self:setContentSize(wps.WinSize)

    BaseScene.RunningScene = self

    if sceneId ~= BaseScene.SceneId.Logo then
        local bg = display.newSprite(wps.Update:getRealPath("main_bg.jpg"))
        wps.addChild2Center(self, bg)    
    end

    if sceneId ~= BaseScene.SceneId.Logo and sceneId ~= BaseScene.SceneId.Rank then
        local goldBg = display.newSprite("#common_gold_bg", nil, nil, {capInsets = cc.rect(40, 0, 2, 83)})
        goldBg:setContentSize(cc.size(242, 83))
        wps.addChild2Pos(self, goldBg, cc.p(wps.w(self) - wps.w(goldBg) / 2 - 10, wps.h(self) - wps.h(goldBg) / 2 - 10), BaseScene.ZOrder.Ui)

        self._goldValue = cc.Label:createWithTTF(Data.getRes(Data.ResType.Gold), TTF_FONT, 48)
        wps.addChild2Pos(goldBg, self._goldValue, cc.p(96, 34))
    end

    if sceneId == BaseScene.SceneId.Main or sceneId == BaseScene.SceneId.Game then
        local titleBestScore = display.newSprite("#common_best_score")
        wps.addChild2Pos(self, titleBestScore, cc.p(wps.w(titleBestScore) / 2 + 10, wps.h(self) - wps.h(titleBestScore) / 2 - 10), BaseScene.ZOrder.Ui)

        self._scoreValue = cc.Label:createWithTTF(Data.getBestScore(), TTF_FONT, 60)
        self._scoreValue:setAnchorPoint(cc.p(0.5, 1.0))
        self._scoreValue:enableOutline(cc.c4b(0, 0, 0, 0xff), 1)
        wps.addChild2Pos(titleBestScore, self._scoreValue, cc.p(wps.w(titleBestScore) / 2, 0))
    elseif sceneId == BaseScene.SceneId.Share or sceneId == BaseScene.SceneId.Unlock or sceneId == BaseScene.SceneId.Rank then
        local btnBack = JellyButton.create("common_btn_back")
        btnBack:addClickEventListener(function() BaseScene.popScene() end)
        wps.addChild2Pos(self, btnBack, cc.p(wps.w(btnBack) / 2 + 20, wps.h(self) - wps.h(btnBack) / 2 - 20), BaseScene.ZOrder.Ui)
    end

    return true
end

function BaseScene:onEnter()
    BaseScene.RunningScene = self

    self._listeners = {}

    local listener = wps.addCustomEventListener(Event.BestScore, function(event) 
        if self._scoreValue then
            self._scoreValue:setString(Data.getBestScore())
        end 
    end)
    table.insert(self._listeners, listener)

    listener = wps.addCustomEventListener(Event.Res, function(event)
        if event._data == Data.ResType.Gold then
            if self._goldValue then
                self._goldValue:setString(Data.getRes(Data.ResType.Gold))
            end
        end
    end)
    table.insert(self._listeners, listener)

    listener = cc.EventListenerKeyboard:create()
    listener:registerScriptHandler(function(keyCode, event) 
        if keyCode == cc.KeyCode.KEY_BACK then
            self:onKeyBack()
        end
    end, cc.Handler.EVENT_KEYBOARD_RELEASED)
    table.insert(self._listeners, listener)
    wps.Director:getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, self)

    if self._scoreValue then
        self._scoreValue:setString(Data.getBestScore())
    end
    if self._goldValue then
        self._goldValue:setString(Data.getRes(Data.ResType.Gold))
    end

    self:addChild(Toast.getInstance(), BaseScene.ZOrder.Toast)
end

function BaseScene:onExit()
    for i = 1, #self._listeners do
        wps.removeEventListener(self._listeners[i])
    end
    self._listeners = {}

    Toast.getInstance():removeFromParent(false)
end

function BaseScene:onCleanup()

end

function BaseScene:onKeyBack()
    
end