Form = class("Form", Panel)

function Form.setFrame(framename, capInsets)
    Form.Framename = framename
    Form.CapInsets = capInsets
end

function Form.setMargin(left, right, top, bottom)
    Form.LeftMargin = left
    Form.RightMargin = right
    Form.TopMargin = top
    Form.BottomMargin = bottom
end

function Form:init(size)
    if not Panel.init(self) then
        return false
    end

    assert(Form.Framename ~= "", "Please call setFrame first")

    self._bg = ccui.ImageView:create(Form.Framename, ccui.TextureResType.plistType)
    self._bg:setScale9Enabled(true)
    self._bg:setCapInsets(Form.CapInsets)
    self._bg:setContentSize(size)
    self:addChild(self._bg)

    return true
end

function Form:show()
    if self._bg:getNumberOfRunningActions() == 0 then
        Panel.show(self)
        self._bg:setPosition(cc.p(wps.w(self) / 2, -wps.h(self._bg) / 2))
        self._bg:runAction(cc.EaseBackOut:create(cc.MoveTo:create(0.3, cc.p(wps.x(self._bg), wps.h(self) / 2))))
    end
end

function Form:hide(isShowAction)
    if isShowAction then
        if self._bg:getNumberOfRunningActions() == 0 then
            self._bg:runAction(wps.seq(cc.EaseBackIn:create(cc.MoveTo:create(0.3, cc.p(wps.x(self._bg), -wps.h(self) / 2))), function() 
                Panel.hide(self)
            end))
        end
    else
        Panel.hide(self)
    end
end