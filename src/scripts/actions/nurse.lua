--Nurse action script
--[[Possible Nurse actions:
Bring food to queen
Bring food to target larva (if have target larva)
Get food from warehouse
Get larva from queen (if larva present)
Place larva somewhere (if have)
--]]

local action = {}
local target = {x = nil, y = nil}
local delta = 0
local movementspeed_nurse = 0.2

function NurseRun(ID, dt)
	--ok to get ant each time, since some action will always be taken
	--(movement or action)

	ant = factory:GetAntByID(ID)

	if delta >= 0 and delta < 2 then
		if target.x ~= nil and target.y ~= nil and action[1] == "Reach target" then
			local delta_x, delta_y
			delta_x = target.x - ant:GetX()
			delta_y = target.y - ant:GetY()

			if delta_x > 0.5 or delta_y > 0.5 then
				local plusminus_x, plusminus_y
				if delta_x > 0 then
					plusminus_x = 1
				else
					plusminus_x = -1
				end

				if delta_y > 0 then
					plusminus_y = 1
				else
					plusminus_y = -1
				end

				ant:PositionChange(movementspeed_nurse * dt * plusminus_x, movementspeed_nurse * dt * plusminus_y)
			else
				--at the target, remove "Moving" action
				table.remove(action, 1)
				target.x = nil
				target.y = nil
			end

		end
		delta = delta + dt
	else
		ant:Eat()
		print("Nurse just ran. Food: "..ant:GetFood())
		
		--actions listed in order of priority

		if actions == nil then
			--get food
			local warehouse = factory:GetWarehouse()
			target.x = warehouse:GetX()
			target.y = warehouse:GetY()
			table.insert(action, "Reach target")
			table.insert(action, "Get food")
			print("Nurse set warehouse target: "..target.x.." "..target.y)
		end

		--food to queen

		--reset delta
		delta = 0
	end
end