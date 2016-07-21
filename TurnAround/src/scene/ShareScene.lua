ShareScene = class("ShareScene", BaseScene)

function ShareScene.create()
    local scene = ShareScene.new()
    if scene:init() then
        return scene
    end
end

function ShareScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Share) then
        return false
    end

    local screenshot = display.newSprite(wps.FileUtils:getWritablePath().."screenshot.jpg")

    local bg = cc.LayerColor:create(cc.c4b(0xff, 0xff, 0xff, 0xfa), wps.w(screenshot) + 20, wps.h(screenshot) + 20)
    bg:ignoreAnchorPointForPosition(false)
    bg:setAnchorPoint(cc.p(0.5, 0.5))
    bg:runAction(wps.seq(cc.Spawn:create({cc.ScaleTo:create(1.0, 0.5), cc.Repeat:create(cc.RotateBy:create(0.3, 360), 3)}), function() self:onShare() end))
    wps.addChild2Center(self, bg)
    
    wps.addChild2Center(bg, screenshot)
    self._screenshot = screenshot

    return true
end

function ShareScene:onCleanup()
    wps.Director:getTextureCache():removeTexture(self._screenshot:getTexture())
    BaseScene.onCleanup(self)
end

function ShareScene:onShare()
    AppUtils.openShare(STR.SHARE_CONTENT, wps.FileUtils:getWritablePath().."screenshot.jpg", STR.GAME_NAME, STR.SHARE_URL)
end