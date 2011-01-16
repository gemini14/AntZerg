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
--reuse getFood_Condition, getWarehouse_Action, moveToTarget_Action, retrieveFood_Action
--end Get food objective--

--Deliver food to queen objective--

local deliverFoodQueen_Condition = Condition:new()

function deliverFoodQueen_Condition:conditionMet(ant, blackboard)
	--if ant:GetFood() > 10 then
		--print("In deliver condition, current action is: "..blackboard.curAction)
		--print("Queen food: "..factory:GetQueen():GetFood())
	--end
	return ant:GetFood() > 10 and factory:GetQueen():GetFood() < 10 and blackboard.curAction == 0
end

local deliverFoodQueen_c = deliverFoodQueen_Condition:new()


local getQueen_Action = Action:new()

function getQueen_Action:running(ant, blackboard)
	return false
end

function getQueen_Action:run(ant, blackboard)
	local queen = factory:GetQueen()
	--print("Setting queen as target x: "..queen:GetX().." y: "..queen:GetY())
	blackboard.target.x = queen:GetX()
	blackboard.target.y = queen:GetY()
	blackboard.target.ID = queen:GetID()
end

local getQueen_a = getQueen_Action:new()


--reuse the moveToTarget_Action action


local deliverFoodAnt_Action = Action:new()

function deliverFoodAnt_Action:running(ant, blackboard)
	return false
end

function deliverFoodAnt_Action:run(ant, blackboard)
	print("Target has "..factory:GetAntByID(blackboard.target.ID):GetFood().." food")
	print("Ant has "..ant:GetFood())
	local withdrawnFood = ant:WithdrawFood(10)
	print("Ant has "..ant:GetFood().." and removed "..withdrawnFood.." food")
	factory:GetAntByID(blackboard.target.ID):AddFood(withdrawnFood)
	--factory:GetQueen():AddFood(withdrawnFood)
	print("Delivered "..withdrawnFood.." food to target")
end

local deliverFoodAnt_a = deliverFoodAnt_Action:new()

--End deliver food to queen objective--


--Extract larva objective--

local extractLarva_Condition = Condition:new()

function extractLarva_Condition:conditionMet(ant, blackboard)
	return factory:GetQueen():GetNumAvailLarvae() > 0 and ant:GetFood() > 10 and
		ant:IsCarryingLarva() ~= true and blackboard.curAction == 0
end

local extractLarva_c = extractLarva_Condition:new()


--reuse getQueen_Action action

--reuse moveToTarget_Action action

local extractLarva_Action = Action:new()

function extractLarva_Action:running(ant, blackboard)
	--removal of larva from queen is instant
	return false
end

function extractLarva_Action:run(ant, blackboard)
	local queen = factory:GetQueen()
	local larva = queen:ExtractLarvae()
	if larva ~= 0 then
		ant:SetLarvaCarry(true)
	end
end

local extractLarva_a = extractLarva_Action:new()

--end Extract larva objective--


--Place larva objective--

local placeLarva_Condition = Condition:new()

function placeLarva_Condition:conditionMet(ant, blackboard)
	return ant:IsCarryingLarva() and ant:GetFood() > 10 and blackboard.curAction == 0
end

local placeLarva_c = placeLarva_Condition:new()


local getNewLarvaLoc_Action = Action:new()

function getNewLarvaLoc_Action:running(ant, blackboard)
	--target acquisition is instant
	return false
end

function getNewLarvaLoc_Action:run(ant, blackboard)
	local queen = factory:GetQueen()
	local seed = os.clock()
	math.randomseed(seed)
	math.random()

	--random position near queen (larvae may end up stacked, but it shouldn't be too big a problem;
	--besides, ants put things in piles, too!)
	local x = math.random(queen:GetX() - 10, queen:GetX() + 10)
	local y = math.random(queen:GetY() - 10, queen:GetY() + 10)
	
	blackboard.target.x = x
	blackboard.target.y = y
	print("new larva being placed at "..x.." "..y)
end

local getNewLarvaLoc_a = getNewLarvaLoc_Action:new()


--reuse moveToTarget_Action action


local placeLarva_Action = Action:new()

function placeLarva_Action:running(ant, blackboard)
	--placement is instant
	return false
end

function placeLarva_Action:run(ant, blackboard)
	AddAnt("larva", blackboard.target.x, blackboard.target.y)
	ant:SetLarvaCarry(false)
end

local placeLarva_a = placeLarva_Action:new()

--end Place larva objective--

--DeliverFoodLarva--

local deliverFoodLarva_Condition = Condition:new()

function deliverFoodLarva_Condition:conditionMet(ant, blackboard)
	return ant:GetFood() > 10 and factory:LarvaNeedsFood() ~= -1 and blackboard.curAction == 0
end

local deliverFoodLarva_c = deliverFoodLarva_Condition:new()


local getLarva_Action = Action:new()

function getLarva_Action:running(ant, blackboard)
	--larva acquisition is instant
	return false
end

function getLarva_Action:run(ant, blackboard)
	local ID = factory:LarvaNeedsFood()
	blackboard.target.ID = ID
	local larva = factory:GetAntByID(ID)
	blackboard.target.x = larva:GetX()
	blackboard.target.y = larva:GetY()
end

local getLarva_a = getLarva_Action:new()


--reuse moveToTarget_Action action
--reuse deliverFoodAnt_Action action

--end DeliverFoodLarva--

--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant, blackboard)
	--nurse eats every 10 seconds
	return ant:GetFood() > 0 and blackboard.delta_sum >= 10 and blackboard.curAction == 0
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant, blackboard)
	--eating is...fast
	return false
end

function eat_Action:run(ant, blackboard)
	ant:Eat()
	print("Nurse just ate")
	blackboard.delta_sum = 0
end

local eat_a = eat_Action:new()

--end Eat--
--------

--Actual nurse ant behavior tree--
local GetFood = { condition = getFood_c, actions = { getWarehouse_a, moveToTarget_a, retrieveFood_a } } 
local DeliverFoodQueen = { condition = deliverFoodQueen_c, actions = { getQueen_a, moveToTarget_a, deliverFoodAnt_a } }
local ExtractLarva = { condition = extractLarva_c, actions = { getQueen_a, moveToTarget_a, extractLarva_a } }
local PlaceLarva = { condition = placeLarva_c, actions = { getNewLarvaLoc_a, moveToTarget_a, placeLarva_a } }
local DeliverFoodLarva = { condition = deliverFoodLarva_c, actions = { getLarva_a, moveToTarget_a, deliverFoodAnt_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local NurseBT = { GetFood, DeliverFoodQueen, ExtractLarva, PlaceLarva, Eat }


--blackboard tables for individual ants (or nurses, rather)
local nurseBB = {}


function NurseRun(ID, dt)
	if nurseBB.ID == nil then
		nurseBB.ID = { actions = {}, curAction = 0, target = {x = nil, y = nil, ID = -1}, delta_sum = 0, movement_speed = 0.4 }
	end

	local ant = factory:GetAntByID(ID)
	
	if nurseBB.ID.curAction == 0 then
		--print("NurseBT size: "..#NurseBT)
		for key, val in pairs(NurseBT) do
			local behavior = NurseBT[key]
			local result = behavior.condition:conditionMet(ant, nurseBB.ID)
			if result then
				print("Nurse: Behavior chosen: "..key)
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