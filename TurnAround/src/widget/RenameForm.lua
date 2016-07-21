RenameForm = class("RenameForm", Form)

function RenameForm.create(closeCallback)
    local form = RenameForm.new()
    if form:init(closeCallback) then
        return form
    end
end

function RenameForm:init(closeCallback)
    if not Form.init(self, cc.size(590, 300)) then
        return false
    end

    self._closeCallback = closeCallback    

    wps.loadSpriteFrames("rename")

    local labelTitle = cc.Label:createWithTTF(STR.INPUT_NICKNAME..":", TTF_FONT, 36)
    labelTitle:setColor(cc.BLACK)
    wps.addChild2Pos(self._bg, labelTitle, cc.p(wps.w(labelTitle) / 2 + 80, wps.h(self._bg) / 2 + wps.h(labelTitle) / 2 + 20))

    local editor = ccui.EditBox:create(cc.size(378, 70), "common_text_bg", ccui.TextureResType.plistType)    
    editor:setFontSize(36)
    editor:setFontColor(cc.BLACK)
    editor:setMaxLength(20)    
    editor:setText(self:randomName())
    wps.addChild2Pos(self._bg, editor, cc.p(wps.w(editor) / 2 + 70, wps.bottom(labelTitle) - wps.h(editor) / 2 - 10))
    self._editor = editor

    local btnRandom = JellyButton.create("rename_btn_random")
    btnRandom:addClickEventListener(function(sender) editor:setText(self:randomName()) end)
    wps.addChild2Pos(self._bg, btnRandom, cc.p(wps.right(editor) + wps.w(btnRandom) / 2, wps.y(editor)))

    local btnCancel = JellyButton.create("common_btn_cancel")
    btnCancel:addClickEventListener(function(sender) self:hide() end)
    wps.addChild2Pos(self._bg, btnCancel, cc.p(wps.w(self._bg) / 2 - wps.w(btnCancel) / 2 - 70, -wps.h(btnCancel) / 2 - 10))

    local btnConfirm = JellyButton.create("common_btn_confirm")
    btnConfirm:addClickEventListener(function(sender)  
        self:onConfirm()
        self:hide()
    end)
    wps.addChild2Pos(self._bg, btnConfirm, cc.p(wps.w(self._bg) / 2 + wps.w(btnConfirm) / 2 + 70, wps.y(btnCancel)))

    return true
end

function RenameForm:onCleanup()
    wps.unloadSpriteFrames("rename")

    if self._closeCallback then self._closeCallback() end

    Form.onCleanup(self)
end

function RenameForm:onConfirm()
    local text = self._editor:getText()
    if string.utf8len(text) > 20 then
        Toast:getInstance():show(STR.NICKNAME_TOO_LONG)
    elseif string.utf8len(text) == 0 then
        Toast:getInstance():show(STR.NICKNAME_EMPTY)
    else
        Data.setNickname(text)        
    end
end

function RenameForm:randomName()
    local index0 = self._firstNameIndex
    local index1 = self._secondNameIndex
    while self._firstNameIndex == index0 and self._secondNameIndex == index1 do
        index0 = math.random(1, #NameInfo.FirstNames)
        index1 = math.random(1, #NameInfo.SecondNames)        
    end
    self._firstNameIndex = index0
    self._secondNameIndex = index1

    return NameInfo.FirstNames[index0]..NameInfo.SecondNames[index1]
end
