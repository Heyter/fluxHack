for k = 1, iEngine:GetMaxClients() do
	local target = iEntity:GetEntity(k)
	print(target == LocalPlayer(), target:Nick(k))
end