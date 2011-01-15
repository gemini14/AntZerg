--Nurse action script
--[[Possible Nurse actions:
Bring food to queen
Bring food to target larva
Get food from warehouse
Get larva from queen
Place larva somewhere
--]]


--------
--Nurse-specific conditions, actions, & tasks

--Get food objective--
getFood_Condition = Condition:new()

function getFood_Condition:conditionMet(ant, blackboard)
	--nurse will get food after the 5 second mark with less than 10 food, unless there's another current action
	--print("in getfood condition: cur food: "..ant:GetFood().." time delta: "..blackboard.delta_sum)
	return ant:GetFood() <= 10 and blackboard.curAction == 0 --blackboard.delta_sum >= 5 and
end

--get food condition obj
getFood_c = getFood_Condition:new()


getWarehouse_Action = Action:new()

function getWarehouse_Action:running(ant, blackboard)
	--setting coords can be done in one call
	return false
end

function getWarehouse_Action:run(ant, blackboard)
	local warehouse = factory:GetWarehouse()
	--print("Setting warehouse as target x: "..warehouse:GetX().." y: "..warehouse:GetY())
	blackboard.target.x = warehouse:GetX()
	blackboard.target.y = warehouse:GetY()
end

--warehouse coords action obj
getWarehouse_a = getWarehouse_Action:new()


moveToTarget_Action = Action:new()

function moveToTarget_Action:running(ant, blackboard)
	local delta_x, delta_y
	delta_x = blackboard.target.x - ant:GetX()
	delta_y = blackboard.target.y - ant:GetY()
	if math.abs(delta_x) < 0.5 and math.abs(delta_y) < 0.5 then
		if blackboard.target.x == 1 and blackboard.target.y == 1 then
			print("Arrived at target! BB-x: "..blackboard.target.x.." BB-y: "..blackboard.target.y)
			print("Current ant pos  x: "..ant:GetX().." y: "..ant:GetY())
		end
		return false
	end
	return true
end

function moveToTarget_Action:run(ant, blackboard, dt)
	local delta_x, delta_y
	--calculate the delta values to the target
	delta_x = blackboard.target.x - ant:GetX()
	delta_y = blackboard.target.y - ant:GetY()

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
	ant:PositionChange(blackboard.movement_speed * dt * plusminus_x, blackboard.movement_speed * dt * plusminus_y)
end

--move to target action obj
moveToTarget_a = moveToTarget_Action:new()


retrieveFood_Action = Action:new()

function retrieveFood_Action:running(ant, blackboard)
	return false
end

function retrieveFood_Action:run(ant, blackboard)
	local warehouse = factory:GetWarehouse()
	--remove 25 food by default, less is taken out if there isn't enough
	local withdraw = warehouse:WithdrawFood(25)
	--print("Nurse withdrew "..withdraw.." food from the warehouse.")
	ant:AddFood(withdraw)
	--print("Nurse has "..ant:GetFood().." food")
end

retrieveFood_a = retrieveFood_Action:new()

--End Get food objective--

--Deliver food to queen objective--

deliverFoodQueen_Condition = Condition:new()

function deliverFoodQueen_Condition:conditionMet(ant, blackboard)
	if ant:GetFood() > 10 then
		print("In deliver condition, current action is: "..blackboard.curAction)
		print("Queen food: "..factory:GetQueen():GetFood())
	end
	return ant:GetFood() > 10 and factory:GetQueen():GetFood() < 10 and blackboard.curAction == 0
end

deliverFoodQueen_c = deliverFoodQueen_Condition:new()


getQueen_Action = Action:new()

function getQueen_Action:running(ant, blackboard)
	return false
end

function getQueen_Action:run(ant, blackboard)
	local queen = factory:GetQueen()
	print("Setting queen as target x: "..queen:GetX().." y: "..queen:GetY())
	blackboard.target.x = queen:GetX()
	blackboard.target.y = queen:GetY()
	blackboard.target.ID = queen:GetID()
end

getQueen_a = getQueen_Action:new()


--this objective will reuse the moveToTarget_Action action


deliverFood_Action = Action:new()

function deliverFood_Action:running(ant, blackboard)
	return false
end

function deliverFood_Action:run(ant, blackboard)
	print("Queen has "..factory:GetQueen():GetFood().." food")
	print("Ant has "..ant:GetFood())
	local withdrawnFood = ant:WithdrawFood(10)
	print("Ant has "..ant:GetFood().." and removed "..withdrawnFood.." food")
	factory:GetQueen():AddFood(withdrawnFood)
	print("Delivered "..withdrawnFood.." food to queen")
end

deliverFood_a = deliverFood_Action:new()

--End deliver food to queen objective--
--------

--Actual nurse ant behavior tree--
local GetFood = { condition = getFood_c, actions = { getWarehouse_a, moveToTarget_a, retrieveFood_a } } 
local DeliverFoodQueen = { condition = deliverFoodQueen_c, actions = { getQueen_a, moveToTarget_a, deliverFood_a } }

local NurseBT = { GetFood, DeliverFoodQueen }


--blackboard tables for individual ants (or nurses, rather)
local nurseBB = {}


function NurseRun(ID, dt)
	if nurseBB.ID == nil then
		nurseBB.ID = { actions = {}, curAction = 0, target = {x = nil, y = nil, ID = -1}, delta_sum = 0, movement_speed = 0.2 }
	end

	local ant = factory:GetAntByID(ID)
	
	if nurseBB.ID.curAction == 0 then
		--print("NurseBT size: "..#NurseBT)
		for key, val in pairs(NurseBT) do
			local behavior = NurseBT[key]
			local result = behavior.condition:conditionMet(ant, nurseBB.ID)
			if result then
				print("Behavior chosen: "..key)
				--print("condition met--table key: "..key.." currentAction: "..nurseBB.ID.curAction)
				nurseBB.ID.actions = behavior.actions
				nurseBB.ID.curAction = 1
				break;
			end
		end		
	end

	local curAction = nurseBB.ID.curAction
	--run the current action
	if curAction ~= 0 and nurseBB.ID.actions ~= nil then
		nurseBB.ID.actions[curAction]:run(ant, nurseBB.ID, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = nurseBB.ID.actions[curAction]:running(ant, nurseBB.ID)
		if status == false then
			if nurseBB.ID.actions[curAction + 1] ~= nil then
				nurseBB.ID.curAction = curAction + 1
			else
				nurseBB.ID.actions = nil
				nurseBB.ID.curAction = 0
				nurseBB.ID.target.x = nil
				nurseBB.ID.target.y = nil
				nurseBB.ID.target.ID = -1
			end
		end
	end

	nurseBB.ID.delta_sum = nurseBB.ID.delta_sum + dt
	--print("current nurse delta: "..nurseBB.ID.delta_sum.." passed delta: "..dt)
end