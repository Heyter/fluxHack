-- Lua supports the following bitwise operators:
-- &: bitwise AND
-- |: bitwise OR
-- ~: bitwise exclusive OR
-- >>: right shift
-- <<: left shift
-- ~: unary bitwise NOT

hook.Add("CreateMove", "CreateMoveTest", function(cmd, frameTime)
	if iEngine:IsInGame() then
		local ply = LocalPlayer()

		if cmd:IsKeyDown(IN_JUMP) and (ply:GetFlags() & FL_ONGROUND) == 0 and (ply:WaterLevel() & FL_ONGROUND) == 0 and ply:GetMoveType() ~= MOVETYPE_LADDER then
			cmd:RemoveKey(IN_JUMP)
		end
	end
end)