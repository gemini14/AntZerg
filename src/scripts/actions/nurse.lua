--Nurse action script
--[[Possible Nurse actions:
Bring food to queen
Bring food to target larva
Get food from warehouse
Get larva from queen
Place larva somewhere
Eat
--]]


--------
--Nurse-specific conditions, actions, & tasks

--Get food objective

local getFood_Condition = Condition:new()

function getFood_Condition:conditionMet(ant)
	--nurse will get food if she has less than 10 food, unless there's another current action
	return ant:GetFood() == 0 and factory:GetWarehouse():GetStoredFood() > 0
end

--get food condition obj
local getFood_c = getFood_Condition:new()

--reuse getWarehouse_Action, moveToTarget_Action, retrieveFood_Action

--end Get food objective--


--Extract larva objective--

local extractLarva_Condition = Condition:new()

function extractLarva_Condition:conditionMet(ant)
	return factory:GetQueen():GetNumAvailLarvae() > 0 and ant:IsCarryingLarva() ~= true
end

local extractLarva_c = extractLarva_Condition:new()


--reuse getQueen_Action action

--reuse moveToTarget_Action action

local extractLarva_Action = Action:new()

function extractLarva_Action:running(ant)
	--removal of larva from queen is instant
	return false
end

function extractLarva_Action:run(ant)
	local larva = factory:GetQueen():ExtractLarvae()
	if larva ~= 0 then
		ant:SetLarvaCarry(true)
	end
end

local extractLarva_a = extractLarva_Action:new()

--end Extract larva objective--


--Place larva objective--

local placeLarva_Condition = Condition:new()

function placeLarva_Condition:conditionMet(ant)
	return ant:IsCarryingLarva()
end

local placeLarva_c = placeLarva_Condition:new()


local getNewLarvaLoc_Action = Action:new()

function getNewLarvaLoc_Action:running(ant)
	--target acquisition is instant
	return false
end

function getNewLarvaLoc_Action:run(ant)
	local queen = factory:GetQueen()
	local seed = os.clock()
	math.randomseed(seed)
	math.random()

	--random position near queen (larvae may end up stacked, but it shouldn't be too big a problem;
	--besides, ants put things in piles, too!)
	local x = math.random(queen:GetX() - 10, queen:GetX() + 10)
	local y = math.random(queen:GetY() - 10, queen:GetY() + 10)
	
	ant.blackboard.target_x = x
	ant.blackboard.target_y = y
end

local getNewLarvaLoc_a = getNewLarvaLoc_Action:new()


--reuse moveToTarget_Action action


local placeLarva_Action = Action:new()

function placeLarva_Action:running(ant)
	--placement is instant
	return false
end

function placeLarva_Action:run(ant)
	QueueAddAnt("larva", ant.blackboard.target_x, ant.blackboard.target_y)
	ant:SetLarvaCarry(false)
end

local placeLarva_a = placeLarva_Action:new()

--end Place larva objective--

--DeliverFoodLarva--

local deliverFoodLarva_Condition = Condition:new()

function deliverFoodLarva_Condition:conditionMet(ant)
	return ant:GetFood() > 0 and factory:LarvaNeedsFood(ant:GetID()) ~= -1
end

local deliverFoodLarva_c = deliverFoodLarva_Condition:new()


local getLarva_Action = Action:new()

function getLarva_Action:running(ant)
	--larva acquisition is instant
	return false
end

function getLarva_Action:run(ant)
	local id = factory:LarvaNeedsFood(ant:GetID())
	ant.blackboard.target_ID = id
	local larva = factory:GetAntByID(id)
	ant.blackboard.target_x = larva:GetX()
	ant.blackboard.target_y = larva:GetY()
end

local getLarva_a = getLarva_Action:new()


--reuse moveToTarget_Action action
--reuse deliverFoodAnt_Action action

--end DeliverFoodLarva--

--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant)
	return ant:GetFood() > 0 and ant.blackboard.delta_sum >= 10
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant)
	--eating is...fast
	return false
end

function eat_Action:run(ant)
	ant:Eat()
	ant.blackboard.delta_sum = -1
end

local eat_a = eat_Action:new()

--end Eat--
--------

--Actual nurse ant behavior tree--
local GetFood = { condition = getFood_c, actions = { getWarehouse_a, moveToTarget_a, retrieveFood_a } } 
local ExtractLarva = { condition = extractLarva_c, actions = { getQueen_a, moveToTarget_a, extractLarva_a } }
local PlaceLarva = { condition = placeLarva_c, actions = { getNewLarvaLoc_a, moveToTarget_a, placeLarva_a } }
local DeliverFoodLarva = { condition = deliverFoodLarva_c, actions = { getLarva_a, moveToTarget_a, deliverFoodAnt_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local NurseBT = { GetFood, Eat, DeliverFoodLarva, ExtractLarva, PlaceLarva }



function NurseRun(ID, dt)
	local ant = factory:GetAntByID(ID)

	if ant.blackboard.curAction == 0 then
		for key, val in pairs(NurseBT) do
			if NurseBT[key].condition:conditionMet(ant) then
				ant.blackboard.behavior = key
				ant.blackboard.curAction = 1
				break;
			end
		end		
	end

	--run the current action
	if ant.blackboard.curAction ~= 0 and NurseBT[ant.blackboard.behavior] ~= nil then
		NurseBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:run(ant, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = NurseBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:running(ant)
		if status == false then
			if NurseBT[ant.blackboard.behavior].actions[ant.blackboard.curAction + 1] ~= nil then
				ant.blackboard.curAction = ant.blackboard.curAction + 1
			else
				ant.blackboard.behavior = 0
				ant.blackboard.curAction = 0
				ant.blackboard.target_x = 0
				ant.blackboard.target_y = 0
				ant.blackboard.target_ID = 0
			end
		end
	end

	ant.blackboard.delta_sum = dt
end