DialogForm = class("DialogForm", Form)

local LEFT_MARGIN = 80
local RIGHT_MARGIN = LEFT_MARGIN
local TOP_MARGIN = 100
local BOTTOM_MARGIN = TOP_MARGIN

DialogForm.IsShow = false

function DialogForm.create(content, confirmCallback)
    local form = DialogForm.new()
    if form:init(content, confirmCallback) then
        return form
    end
end

function DialogForm:init(content, confirmCallback)
    self._confirmCallback = confirmCallback    

    local node = content
    if type(content) == "string" then
        node = cc.Label:createWithTTF(STR.SURE_TO_EXIT, TTF_FONT, 36)
        node:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)
        local width = math.min(wps.WinSize.width - 120, 540)
        if wps.w(node) + LEFT_MARGIN + RIGHT_MARGIN > width then
            node:setDimensions(cc.size(width - LEFT_MARGIN - RIGHT_MARGIN, 0))
        end
    end

    if not Form.init(self, cc.size(math.max(wps.w(node) + LEFT_MARGIN + RIGHT_MARGIN, 480), wps.h(node) + TOP_MARGIN + BOTTOM_MARGIN)) then
        return false
    end

    wps.addChild2Center(self._bg, node)

    local btnConfirm = JellyButton.create("common_btn_confirm")
    if confirmCallback then
        local btnCancel = JellyButton.create("common_btn_cancel")
        btnCancel:addClickEventListener(function() self:onCancel() end)
        wps.addChild2Pos(self._bg, btnConfirm, cc.p(wps.w(self._bg) / 2 + wps.w(btnConfirm) / 2 + 40, -wps.h(btnConfirm) / 2 - 10))
        wps.addChild2Pos(self._bg, btnCancel, cc.p(wps.w(self._bg) / 2 - wps.w(btnCancel) / 2 - 40, wps.y(btnConfirm)))
    else
        wps.addChild2Pos(self._bg, btnConfirm, cc.p(wps.w(self._bg) / 2, -wps.h(btnConfirm) / 2 - 10))
    end
    btnConfirm:addClickEventListener(function() self:onConfirm() end)

    return true
end

function DialogForm:onEnter()
    Form:onEnter()
    DialogForm.IsShow = true
end

function DialogForm:onExit()
    Form:onExit()
    DialogForm.IsShow = false
end

function DialogForm:show()
    if DialogForm.IsShow  then return end
    Form.show(self)
    self:setLocalZOrder(BaseScene.ZOrder.Dialog)
end

function DialogForm:onConfirm()
    if self._confirmCallback then
        self._confirmCallback()
    end
    self:hide()
end

function DialogForm:onCancel()
    self:hide()
end
