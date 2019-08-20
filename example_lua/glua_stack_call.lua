local curtime_fn = 0
function CurTime()
	iGLua:GetLuaInterface(enum('stack', 'CLIENT'))
    iGLua:PushSpecial(enum('stack', 'SPECIAL_GLOB'))
    iGLua:GetField(-1, "CurTime")
    iGLua:Call(0, 1)
	curtime_fn = iGLua:GetNumber(-1)
    iGLua:Pop(2) -- Curtime, SPECIAL_GLOB
	return curtime_fn
end

-- https://github.com/danielga/garrysmod_common/blob/master/include/GarrysMod/Lua/LuaShared.h#58