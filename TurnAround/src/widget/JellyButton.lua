JellyButton = class("JellyButton", function(framename) return GBShaderButton:create(framename, wps.Update:getRealPath("shader/highlight.fsh")) end)

function JellyButton.create(framename, soundName)
    local button = JellyButton.new(framename)
    if button:init(soundName) then
        return button
    end
end

function JellyButton:init(soundName)
    self._soundName = soundName

    self:registerScriptHandler(function(state)
        if state == "releaseUpEvent" then
            self:releaseUpEvent()
        end
    end)

    self:playAction()

    return true
end

function JellyButton:releaseUpEvent()
    Data.playSound(self._soundName or "click")
end

function JellyButton:playAction()
    if not self:isEnabled() then return end

    self:runAction(wps.seq(cc.EaseSineInOut:create(cc.ScaleTo:create(0.2, 1.05, 0.95)),
        cc.EaseSineInOut:create(cc.ScaleTo:create(0.2, 1.0)), function() self:startAction() end))
end

function JellyButton:startAction()
    self:runAction(wps.seq(5.0 + math.random() * 5.0, function() 
        if math.random() > 0.5 then
            self:startAction()
        else
            self:playAction()
        end
    end))
end