local antIDTable = {}

function AddAnt(type, x, y)
	local id = factory:CreateAnt(type, x, y)
	table.insert(antIDTable, id)
	renderer:AddAnt(id, type, x, y, 0)
end

function RemoveAnt(ID)
	factory:RemoveAnt(ID)
	renderer:RemoveAnt(ID)
	antIDTable.ID = nil
end

function RenderUpdateAllAnts()
	for ID, _ in pairs(antIDTable) do
		local ant = factory:GetAntByID(ID)
		renderer:UpdateAnt(ID, ant:GetX(), ant:GetY(), 0);
	end
end

function CreateWarehouse(x, y)
	factory:CreateWarehouse(x, y)
	local warehouse = factory:GetWarehouse()
	renderer:AddWarehouse(warehouse:GetX(), warehouse:GetY())
end