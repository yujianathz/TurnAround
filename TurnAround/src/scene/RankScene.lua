RankScene = class("RankScene", BaseScene)

function RankScene.create()
    local scene = RankScene.new()
    if scene:init() then
        return scene
    end
end

function RankScene:init()
    if not BaseScene.init(self, BaseScene.SceneId.Rank) then
        return false
    end

    wps.loadSpriteFrames("rank")
    
    self._titleBg = display.newSprite("#common_title_bg")
    wps.addChild2Pos(self, self._titleBg, cc.p(wps.w(self) / 2, wps.h(self) - wps.h(self._titleBg) / 2 - 50))
    wps.addChild2Pos(self._titleBg, display.newSprite("#rank_title"), cc.p(wps.w(self._titleBg) / 2, 160))

    local titleRank = display.newSprite("#rank_title_rank")
    local titleName = display.newSprite("#rank_title_name")
    local titleScore = display.newSprite("#rank_title_score")
    wps.addChild2Pos(self, titleRank, cc.p(wps.w(titleRank) / 2 + 70, wps.bottom(self._titleBg) - wps.h(titleRank) / 2))
    wps.addChild2Pos(self, titleName, cc.p(wps.w(self) / 2, wps.y(titleRank)))
    wps.addChild2Pos(self, titleScore, cc.p(wps.w(self) - wps.w(titleRank) / 2 - 70, wps.y(titleRank)))

    local list = ccui.ListView:create()
    list:setContentSize(cc.size(wps.w(self), wps.bottom(titleRank) - 10))
    list:setDirection(ccui.ScrollViewDir.vertical)
    list:setGravity(ccui.ListViewGravity.centerHorizontal)
    self:addChild(list)
    self._list = list

    self._xhr = cc.XMLHttpRequest:new()
    self._xhr:retain()
    self._xhr.responseType = cc.XMLHTTPREQUEST_RESPONSE_STRING
    self._xhr:registerScriptHandler(function() 
            self:onHttpResponse(self._xhr)
            self._xhr:unregisterScriptHandler()    
    end)

    return true
end

function RankScene:onEnter()
    BaseScene.onEnter(self)

    self:checkName()
end

function RankScene:onCleanup()
    self._xhr:unregisterScriptHandler()
    self._xhr:release()

    wps.unloadSpriteFrames("rank")
    BaseScene.onCleanup(self)
end

function RankScene:checkName()
    if Data.Nickname == "" then
        RenameForm.create(function() self:checkName() end):show()
    else
        local gamename = "turnaround"
        local nickname = Data.Nickname
        local udid = GBUtils.getOpenUDID()
        local secret = "wanpishu"
        local score = Data.getBestScore()
        local token = md5.sumhexa(gamename..nickname..udid..secret)

        local url = string.format("http://120.27.183.37/rank/ranking.php?action=add&gameName=%s&userName=%s&udid=%s&score=%d&token=%s", 
            gamename, string.urlencode(nickname), udid, score, token)                
        self._xhr:open("POST", url)
        self._xhr:send()
    end
end

function RankScene:refreshList(response)
    self._list:removeAllItems()

    local data = Json.decode(response)
    local rank = data.rank
    for i = 1, #data.top10 do
        self._list:pushBackCustomItem(self:createItem(data.top10[i], i, i == rank))        
    end
    if rank > 10 then
        self._list:pushBackCustomItem(self:createItem({name = Data.Nickname, score = Data.getBestScore()}, rank, true))
    end
end

function RankScene:createItem(data, index, isSelf)
    local item = ccui.ImageView:create(isSelf and "rank_item_focus" or "rank_item_unfocus", ccui.TextureResType.plistType)

    local labelColor = isSelf and cc.c3b(6, 46, 71) or cc.c3b(131, 27, 2)
    local labelRank = cc.Label:createWithTTF(index, TTF_FONT, 48)
    labelRank:setColor(labelColor)
    wps.addChild2Pos(item, labelRank, cc.p(85, wps.h(item) / 2))

    local labelName = cc.Label:createWithSystemFont(data.name, SYS_FONT, 36)
    labelName:setColor(labelColor)
    wps.addChild2Center(item, labelName)

    local labelScore = cc.Label:createWithTTF(data.score, TTF_FONT, 48)
    labelScore:setColor(labelColor)
    wps.addChild2Pos(item, labelScore, cc.p(570, wps.h(item) / 2))

    return item
end

function RankScene:onHttpResponse(xhr)
    if xhr.readyState == 4 and (xhr.status >= 200 and xhr.status < 207) then
        print(xhr.response)
        self:refreshList(xhr.response)
    else
        print("xhr.readyState is:", xhr.readyState, "xhr.status is: ",xhr.status)
    end   
end