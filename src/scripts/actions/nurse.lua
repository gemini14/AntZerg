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


local function ReachedTarget(delta_x, delta_y)
	return delta_x <= 0.5 and delta_y <= 0.5
end

local function SetTarget(x, y)
	target.x = x
	target.y = y
end


function NurseRun(ID, dt)
	--ok to get ant each time, since some action will always be taken
	--(movement or action)

	ant = factory:GetAntByID(ID)

	--between 0 and 2 seconds, ant will only move (behaviors are decided after the 2s+ mark)
	if delta >= 0 and delta < 2 then
		--if ant has a target and the current action is to reach the target, move toward it
		if target.x ~= nil and target.y ~= nil and action[1] == "Reach target" then
			local delta_x, delta_y
			--calculate the delta values to the target
			delta_x = target.x - ant:GetX()
			delta_y = target.y - ant:GetY()

			--if we're not within range (0.5) of the target, calc which way to move and update the position
			if ReachedTarget(delta_x, delta_y) ~= true then
				--this just calculates forwards/backwards or left/right
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

				--update the ant's position, using the set movement speed and delta_t
				ant:PositionChange(movementspeed_nurse * dt * plusminus_x, movementspeed_nurse * dt * plusminus_y)
			else
				--otherwise, we're within range of the target, so don't move anymore
				--remove "Moving" action and reset target values to nil
				table.remove(action, 1)
				target.x = nil
				target.y = nil
			end

		end
		--add dt to stored delta value
		delta = delta + dt
	
	--2s+ mark: all actions are decided, changed, cancelled, etc.
	else
		--Eat first to keep up her strength!
		ant:Eat()
		print("Nurse just ran. Food: "..ant:GetFood())
		
		--if there are no current actions, make a decision
		if actions == nil then
			--set warehouse as target
			local warehouse = factory:GetWarehouse()
			SetTarget(warehouse:GetX(), warehouse:GetY())
			print("Nurse set warehouse target: "..target.x.." "..target.y)
			
			--set actions
			table.insert(action, "Set warehouse as target")
			table.insert(action, "Reach target")
			table.insert(action, "Get food")			
		else
			--there are some existing actions, so we need to continue carrying them out
		end

		--reset delta
		delta = 0
	end
end