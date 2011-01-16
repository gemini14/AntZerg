local antIDTable = {}

function AddAnt(type, x, y)
	local id = factory:CreateAnt(type, x, y)
	if id ~= -1 then
		table.insert(antIDTable, id)
		renderer:AddAnt(id, type, x, y)
	end
end

function RemoveAnt(ID)
	if ID ~= nil then
		factory:RemoveAnt(ID)
		renderer:RemoveAnt(ID)
		for ID, val in pairs(antIDTable) do
			print(val)
		end
		--table.remove(antIDTable, ID)
		for key, val in pairs(antIDTable) do
			if val == ID then
				print("Removing key "..key.." and ID "..val)
				table.remove(antIDTable, key)
				break
			end
		end
		for ID, val in pairs(antIDTable) do
			print(val)
		end
	end
end

function RenderUpdateAllAnts()
	for key, ID in pairs(antIDTable) do
		local ant = factory:GetAntByID(ID)
		if ant ~= nil then
			renderer:UpdateAnt(ID, ant:GetX(), ant:GetY())
		else
			for key, ID in pairs(antIDTable) do
				print("Oops..key "..key.." ID "..ID)
			end
		end
	end
end

function CreateWarehouse(x, y)
	factory:CreateWarehouse(x, y)
	local warehouse = factory:GetWarehouse()
	if warehouse ~= nil then
		renderer:AddWarehouse(warehouse:GetX(), warehouse:GetY())
	end
end

function CreatePlot(x, y)
	local id = factory:CreateFungusPlot(x, y)
	if id ~= -1 then
		renderer:AddFungusPlot(id, x, y)
		return id
	else
		return -1
	end
end

function RemovePlot(ID)
	if ID ~= -1 then
		factory:RemoveFungusPlot(ID)
		renderer:RemoveFungusPlot(ID)
	end
end