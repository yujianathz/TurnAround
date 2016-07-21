Value = class("Value")

local Secret = "WPSGAME"

local Code = function(vStr)
    local temp = vStr
    local bytes = {}
    for i = 1, #temp do
        local c = bit.bxor(temp:byte(i), Secret:byte((i - 1) % #Secret + 1))
        table.insert(bytes, c)        
    end

    return string.char(unpack(bytes))
end

function Value:ctor(v)
    self:setValue(v)
end

function Value:setValue(v)
    local t = type(v)
    if t == "boolean" then
        self._value = Code(v and "true" or "false")
    else
        self._value = Code(v.."")
    end
end

function Value:toBool()
    local v = Code(self._value)
    return v == "true"
end

function Value:toNumber()
    local v = Code(self._value)
    return tonumber(v)
end