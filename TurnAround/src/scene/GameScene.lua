GameScene = class("GameScene", BaseScene)

local Status = 
{
    Prepare = 1,
    Gaming = 2,
    Pause = 3,
    End = 4,
}

local Block = 
{
    None = 0,
    Square = 1,
    Triangle = 2,
}

local BLOCK_NUMBER = 12

function GameScene.create()
    local scene = GameScene.new()
    if scene:init() then
        return scene
    end
end

function GameScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Game) then
        return false
    end

    wps.loadSpriteFrames("game")

    self._gameLayer = wps.createNode(cc.Layer)
    self._gameLayer:setContentSize(self:getContentSize())
    self:addChild(self._gameLayer)

    self._circle = display.newSprite("#common_circle")
    wps.addChild2Center( self._gameLayer, self._circle)

    self._count = cc.Label:createWithTTF("", TTF_FONT, 96)
    wps.addChild2Center(self._circle, self._count)

    self._round = display.newNode()
    wps.addChild2Center(self._circle, self._round, 1)

    self:prestartGame()

    return true;
end

function GameScene:onEnter()
    BaseScene.onEnter(self)

    local listener = wps.addCustomEventListener(Event.RestartGame, function(event) 
        self:prestartGame()        
    end)
    table.insert(self._listeners, listener)

    listener = wps.addCustomEventListener(Event.StartGame, function(event) 
        self:startGame()
    end)
    table.insert(self._listeners, listener)

    listener = wps.addCustomEventListener(Event.ExitGame, function(event)
        BaseScene.popScene()
    end)
    table.insert(self._listeners, listener)

    listener = wps.addCustomEventListener(Event.ShowPanel, function(event)
        self:pauseGame()
    end)
    table.insert(self._listeners, listener)

    listener = wps.addCustomEventListener(Event.HidePanel, function(event)
        self:resumeGame()
    end)
    table.insert(self._listeners, listener)

    listener = cc.EventListenerTouchOneByOne:create()
    listener:registerScriptHandler(function(touch, event) return self:onTouchBegin(touch, event) end, cc.Handler.EVENT_TOUCH_BEGAN)    
    listener:registerScriptHandler(function(touch, event) self:onTouchEnd(touch, event) end, cc.Handler.EVENT_TOUCH_ENDED)
    wps.Director:getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, self)
    table.insert(self._listeners, listener)
end

function GameScene:onCleanup()
    wps.unloadSpriteFrames("game")
    
    BaseScene.onCleanup(self)
end

function GameScene:onTouchBegin(touch, event)
    if self._status ~= Status.Gaming then return false end

    if self._role._type == Block.Square then
        self._role._type = Block.Triangle
    else
        self._role._type = Block.Square
    end
    self._role:setSpriteFrame(string.format("role%d_%d_1", Data.getSelectedRole(), self._role._type))

    return true
end

function GameScene:onTouchEnd(touch, event)
    if self._status ~= Status.Gaming then return end

    if self._role._type == Block.Square then
        self._role._type = Block.Triangle
    else
        self._role._type = Block.Square
    end
    self._role:setSpriteFrame(string.format("role%d_%d_1", Data.getSelectedRole(), self._role._type))
end

function GameScene:prestartGame()
    if self._status == Status.Prepare then return end
    self._status = Status.Prepare

    self._isRelive = false

    self._count._value = 0
    self:countdown(function() self:startGame() end)

    if self._blocks then
        for i = 1, #self._blocks do
            if self._blocks[i].node then
                self._blocks[i].node:removeFromParent()
                self._blocks[i].node = nil
            end
        end
    end
end

function GameScene:countdown(callback)
    self._count:setString("Ready")
    self._count:setScale(0)
    self._count:setOpacity(0)
    self._count:runAction(wps.seq(cc.Spawn:create({cc.FadeIn:create(0.5), cc.ScaleTo:create(0.5, 1.0)}), 0.2, function()
        self._count:setOpacity(0)
        self._count:setScale(0)    
        self._count:setString("GO")
    end, cc.Spawn:create({cc.FadeIn:create(0.5), cc.ScaleTo:create(0.5, 1.0)}), 0.2, function() 
        self._count:setString(self._count._value)
        callback()
    end))
end

function GameScene:startGame()
    if self._status == Status.Gaming then return end
    self._status = Status.Gaming

    self._gold = 0
    self._count._value = 0
    self._count:setString(self._count._value)  

    if self._role then
        self._role:removeFromParent()
    end

    local type = math.random() > 0.5 and Block.Square or Block.Triangle
    self._role = display.newSprite(string.format("#role%d_%d_1", Data.getSelectedRole(), type))
    self._role:setAnchorPoint(cc.p(0.5, 0))
    self._role._pos = 0
    self._role._type = type
    wps.addChild2Pos(self._round, self._role, cc.p(0, wps.h(self._circle) / 2 - 20))

    self._round:setRotation(0)
    self._round._lastRotation = 0
    self._round:runAction(cc.RepeatForever:create(cc.RotateBy:create(3.5, 360)))    

    self._gameLayer:onUpdate(function(dt) self:update(dt) end)
    self._count:setString(self._count._value)

    self:startRound()
end

function GameScene:endGame()
    if self._status == Status.End then return end
    self._status = Status.End

    self._gameLayer:unscheduleUpdate()
    self._round:stopAllActions()
    self._role:setSpriteFrame(string.format("role%d_%d_2", Data.getSelectedRole(), self._role._type))
    self._role:runAction(wps.seq(cc.JumpBy:create(1.0, cc.p(0, 0), 15, 4), function() FinishPanel.create(self._count._value, self._gold):show() end))  

    Data.playSound("dead")
end

function GameScene:pauseGame()
    if self._status == Status.Gaming then
        self._status = Status.Pause
        self._round:pause()
    end    
end

function GameScene:resumeGame()
    if self._status == Status.Pause then
        self._status = Status.Gaming
        self._round:resume()
    end    
end

function GameScene:update(dt)
    if self._status ~= Status.Gaming then return end

    self._round:setRotation(self._round:getRotation() % 360)

    local nextPos = (self._role._pos + 1) % BLOCK_NUMBER
    local nextRotation = nextPos * 30
    if (self._round._lastRotation <= nextRotation and self._round:getRotation() >= nextRotation) or self._round._lastRotation > self._round:getRotation() then
        self._role._pos = nextPos

        local block = nil
        if nextPos == 0 then
            block = self._blocks[#self._blocks]
        else
            block = self._blocks[nextPos]
        end
        
        if block.type ~= Block.None then
            if block.type == self._role._type then
                if block.node._hasGold then
                    local ico = display.newSprite("#common_ico_gold")
                    ico:setPosition(self:convertToNodeSpace(block.node._block:convertToWorldSpace(cc.p(wps.w(block.node._block) / 2, wps.h(block.node._block) / 2))))
                     self._gameLayer:addChild(ico, BaseScene.ZOrder.Ui + 1)

                    ico:runAction(wps.seq(cc.MoveTo:create(0.5, self:convertToNodeSpace(self._goldValue:convertToWorldSpace(cc.p(wps.w(self._goldValue) / 2, wps.h(self._goldValue) / 2)))),
                        function() 
                            self._gold = self._gold + 1
                            Data.changeRes(Data.ResType.Gold, 1)                            
                        end, cc.RemoveSelf:create()))
                end

                block.node:runAction(wps.seq(cc.ScaleTo:create(0.3, 0), cc.RemoveSelf:create()))                
                block.node = nil

                self._count._value = self._count._value + 1
                self._count:setString(self._count._value)

                self._role:stopAllActions()
                self._role:runAction(wps.seq(cc.EaseBounceOut:create(cc.ScaleTo:create(0.1, 0.9, 1.1)), cc.EaseBounceIn:create(cc.ScaleTo:create(0.1, 1.0))))

                Data.playSound("eat")
            else
                block.node:removeFromParent()
                block.node = nil

                if AppUtils.isVideoPlayable() then
                    VideoPanel.create(function() self:showVideo() end, function() self:endGame() end):show()
                else
                    self:endGame()
                end
                return
            end
        end
    end
    self._round._lastRotation = self._round:getRotation()

    if self._role._pos == 0 then
        self:startRound()
    end
end

function GameScene:startRound()
    if self._role._pos ~= 0 then return end

    if self._blocks then
        for i = 1, #self._blocks do
            if self._blocks[i].node then
                self._blocks[i].node:removeFromParent()
                self._blocks[i].node = nil
            end
        end
    end

    self._blocks = {}
    for i = 1, BLOCK_NUMBER do
        if i < 4 then
            table.insert(self._blocks, {type = Block.None})
        elseif i == 4 then
            table.insert(self._blocks, {type = math.random() > 0.5 and Block.Square or Block.Triangle})
        else
            local rand = math.random()
            if rand < 0.3 then
                table.insert(self._blocks, {type = Block.None})
            elseif rand < 0.65 then
                table.insert(self._blocks, {type = Block.Square})
            else
                table.insert(self._blocks, {type = Block.Triangle})
            end
        end
    end
    
    local actions = {}
    local delay = 0.2
    for i = 4, #self._blocks do
        table.insert(actions, cc.DelayTime:create(delay))
        table.insert(actions, cc.CallFunc:create(function() self:createBlock(i) end))
    end
    self._circle:runAction(cc.Sequence:create(actions))
end

function GameScene:createBlock(pos)
    if self._blocks[pos] == nil or self._blocks[pos].type == Block.None then
        return
    end

    if self._blocks[pos].node == nil then
        local node = display.newNode()
        node:setRotation(pos * 30)
        wps.addChild2Center(self._circle, node)
        self._blocks[pos].node = node

        local block = nil
        if math.random() < 0.05 then
            block = display.newSprite(self._blocks[pos].type == Block.Square and string.format("#block%d_square_ex", Data.getSelectedRole()) or "#block_triangle_ex")
            node._hasGold = true
        else
            block = display.newSprite(self._blocks[pos].type == Block.Square and string.format("#block%d_square", Data.getSelectedRole()) or "#block_triangle")
        end
        block:setAnchorPoint(cc.p(0.5, 0))
        block:setScale(1.2, 0)
        wps.addChild2Pos(node, block, cc.p(0, wps.h(self._circle) / 2 - 20))
        node._block = block

        block:runAction(wps.seq(cc.EaseBackOut:create(cc.ScaleTo:create(0.2, 1.0)), function()
            local actions = 
            {
                cc.EaseBackOut:create(cc.ScaleTo:create(0.2, 1.1, 0.9)),
                cc.EaseBackIn:create(cc.ScaleTo:create(1.5, 0.9, 1.1)),
            } 
            block:runAction(cc.RepeatForever:create(cc.Sequence:create(actions)))
            block:runAction(cc.RepeatForever:create(cc.JumpBy:create(0.5, cc.p(0, 0), 5, 1)))
        end))
    end
end

function GameScene:showVideo()
    if self._isRelive then return end
    self._isRelive = true
    
    self:runAction(cc.CallFunc:create(function() 
        AppUtils.playVideo()
        self:pauseGame()
        self:runAction(wps.seq(1.0, function() self:countdown(function() self:resumeGame() end) end))        
    end))     
end
