Data = {}

Data.RoleType = 
{
    FanQie = 1,
    YangCong = 2,
    TuDou = 3,
    LaJiao = 4,
    DouJiao = 5,
    NanGua = 6,
    MoGu = 7,
    LianOu = 8,
    QieZi = 9,

    Number = 9,
}

Data.ResType = 
{
    Gold = 1,

    Number = 1,
}

Data.BestScore = Value.new(GBSqlite.getNumber(UserKey.BestScore, UserDefault.BestScore))

Data.Res = {}
for i = 1, Data.ResType.Number do
    Data.Res[i] = Value.new(GBSqlite.getNumber(UserKey.Res..i, UserDefault.Res))   
end

Data.Role = {}
for i = 1, Data.RoleType.Number do
    Data.Role[i] = Value.new(GBSqlite.getBool(UserKey.Role..i, i == Data.RoleType.FanQie))
end
Data.SelectedRole = Value.new(GBSqlite.getNumber(UserKey.SelectedRole, Data.RoleType.FanQie))

Data.Nickname = GBSqlite.getString(UserKey.Nickname, "")

Data.IsMute = wps.UserDefault:getBoolForKey(UserKey.IsMute, UserDefault.IsMute)
audio.setMusicVolume(Data.IsMute and 0 or 1)
audio.setSoundsVolume(Data.IsMute and 0 or 1)

Data.RunningDuration = 0

Data.Scheduler = wps.Director:getScheduler():scheduleScriptFunc(function(dt) 
    Data.onUpdate(dt)
end, 0, false)

function Data.setNickname(nickname)
    if Data.Nickname == nickname then return end
    Data.Nickname = nickname

    GBSqlite.setString(UserKey.Nickname, nickname)
end

function Data.setSelectedRole(roleIndex)
    if Data.SelectedRole:toNumber() == roleIndex then
        return    
    end
    Data.SelectedRole:setValue(roleIndex)

    wps.dispatchCustomEvent(Event.SelectedRole)
    GBSqlite.setNumber(UserKey.SelectedRole, roleIndex)
end

function Data.getSelectedRole()
    return Data.SelectedRole:toNumber()
end

function Data.unlockRole(roleIndex)
    if Data.Role[roleIndex]:toBool() then
        return
    end
    Data.Role[roleIndex]:setValue(true)

    wps.dispatchCustomEvent(Event.UnlockRole, roleIndex)
    GBSqlite.setBool(UserKey.Role..roleIndex, true)
end

function Data.isRoleUnlock(roleIndex)
    return Data.Role[roleIndex]:toBool()
end

function Data.setBestScore(bestScore)
    if Data.BestScore:toNumber() >= bestScore then
        return
    end
    Data.BestScore:setValue(bestScore)

    wps.dispatchCustomEvent(Event.BestScore)
    GBSqlite.setNumber(UserKey.BestScore, bestScore)
end

function Data.getBestScore()
    return Data.BestScore:toNumber()
end

function Data.hasRes(resType, delta)
    if Data.getRes(resType) < delta then
        local label = cc.Label:createWithTTF(STR.NO_GOLD, TTF_FONT, 70)
        label:enableOutline(cc.c4b(0, 0, 0, 0xff), 2)        
        Toast.getInstance():show(label)
        return false
    end

    return true
end

function Data.changeRes(resType, delta)
    if delta == 0 then return end

    local res = Data.getRes(resType) + delta
    if res < 0 then return end
    Data.Res[resType]:setValue(res)

    wps.dispatchCustomEvent(Event.Res, resType)
    GBSqlite.setNumber(UserKey.Res..resType, res)
end

function Data.getRes(resType)
    return Data.Res[resType]:toNumber()
end

function Data.setIsMute(isMute)
    if Data.IsMute == isMute then
        return
    end
    Data.IsMute = isMute

    audio.setMusicVolume(isMute and 0 or 1)
    audio.setSoundsVolume(isMute and 0 or 1)

    wps.UserDefault:setBoolForKey(UserKey.IsMute, isMute)
end

function Data.playMusic(filename, isLoop)
    audio.playMusic(wps.Update:getRealPath(string.format("audio/%s.mp3", filename)), isLoop)
end

function Data.playSound(filename)
    audio.playSound(wps.Update:getRealPath(string.format("audio/%s.mp3", filename)), false)
end

function Data.onUpdate(dt)
    Data.RunningDuration = Data.RunningDuration + dt
end

function Data.formatTime(timestamp)
    local hour = math.floor(timestamp / 3600)
    local minute = math.floor((timestamp % 3600) / 60)
    local second = timestamp % 60

    local str = ""
    if hour > 0 then str = str..string.format(STR.HOUR, hour) end
    if minute > 0 then str = str..string.format(STR.MINUTE, minute) end
   
    return str..string.format(STR.SECOND, second)     
end
