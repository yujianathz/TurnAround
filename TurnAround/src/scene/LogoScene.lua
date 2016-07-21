LogoScene = class("LogoScene", BaseScene)

function LogoScene.create()
    local scene = LogoScene.new()
    if scene:init() then
        return scene
    end
end

function LogoScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Logo) then
        return false
    end

    local bg = cc.LayerColor:create(cc.c4b(0xff, 0xff, 0xff, 0xfa), wps.WinSize.width, wps.WinSize.height)
    self:addChild(bg)

    self._logo = display.newSprite(wps.Update:getRealPath("logo.png"))
    wps.addChild2Pos(bg, self._logo, cc.p(wps.w(bg) / 2 + 10, wps.h(bg) / 2 + 80))

    local action = wps.seq(1.0, function() self:switchScene() end)
    self:runAction(action)

    return true;
end

function LogoScene:onCleanup()
    wps.Director:getTextureCache():removeTexture(self._logo:getTexture())

    BaseScene.onCleanup(self)
end

function LogoScene:switchScene()
    wps.loadSpriteFrames("common")
    BaseScene.replaceScene(MainScene.create())
end
