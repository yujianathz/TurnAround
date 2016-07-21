Event = 
{
    ShowPanel = "show panel",
    HidePanel = "hide panel",

    StartGame = "start game",
    RestartGame = "restart game",
    ExitGame = "exit game",

    BestScore = "best score",
    Res = "res",

    SelectedRole = "selected role",
    UnlockRole = "unlock role"
}

UserKey = 
{
    BestScore = "best_score",
    Res = "res%d",
    Role = "role%d",
    SelectedRole = "selected role",
    Nickname = "nickname",

    IsMute = "is_mute",
}

UserDefault = 
{
    BestScore = 0,
    Res = 0,

    IsMute = false,
}

require "strings"

require "base.Init"

require "data.Data"
require "data.RoleInfo"
require "data.GiftInfo"
require "data.NameInfo"

require "scene.BaseScene"
require "scene.LogoScene"
require "scene.MainScene"
require "scene.GameScene"
require "scene.ShareScene"
require "scene.UnlockScene"
require "scene.RankScene"

require "widget.JellyButton"
require "widget.FinishPanel"
require "widget.RenameForm"
require "widget.VideoPanel"
require "widget.DialogForm"

TTF_FONT = wps.Update:getRealPath("font.ttf")
SYS_FONT = "Arial"
