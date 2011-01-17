--Worker action script
--[[Possible worker actions:
Bring food to queen
Bring food to warehouse
Get food from fungus plot
Place new fungus plot
--]]


--Deliver food to queen objective--

local deliverFoodQueen_Condition = Condition:new()

function deliverFoodQueen_Condition:conditionMet(ant, blackboard)
	if ant:GetFood() > 10 then
		--print("In deliver condition, current action is: "..blackboard.curAction)
		--print("Queen food: "..factory:GetQueen():GetFood().." and worker food "..ant:GetFood())
	end
	return ant:GetFood() > 10 and factory:GetQueen():GetFood() < 10 and blackboard.curAction == 0
end

local deliverFoodQueen_c = deliverFoodQueen_Condition:new()


--reuse the getQueen_Action action
--reuse the moveToTarget_Action action
--reuse the deliverFoodAnt_Action action

--End deliver food to queen objective--


--Deliver food to warehouse objective--

local deliverFoodWarehouse_Condition = Condition:new()

function deliverFoodWarehouse_Condition:conditionMet(ant, blackboard)
	--print("Warehouse has "..factory:GetWarehouse():GetStoredFood())
	return ant:GetFood() > 10 and blackboard.curAction == 0
end

local deliverFoodWarehouse_c = deliverFoodWarehouse_Condition:new()

--reuse getWarehouse_Action action
--reuse moveToTarget_Action action

local deliverFoodWarehouse_Action = Action:new()

function deliverFoodWarehouse_Action:running(ant, blackboard)
	--only takes a second
	return false
end

function deliverFoodWarehouse_Action:run(ant, blackboard)
	--print("Ant has "..ant:GetFood())
	local withdrawal = ant:WithdrawFood(50)
	--print("Ant has "..ant:GetFood().." and removed "..withdrawal.." food")
	factory:GetWarehouse():DepositFood(withdrawal)
	--print("Delivered "..withdrawal.." food to warehouse")
end

local deliverFoodWarehouse_a = deliverFoodWarehouse_Action:new()
--End Deliver food to warehouse objective--


--Get food from fungus plot--

local getFoodWorker_Condition = Condition:new()

function getFoodWorker_Condition:conditionMet(ant, blackboard)
	return ant:GetFood() <= 10 and blackboard.plot ~= -1 and blackboard.curAction == 0
end

local getFoodWorker_c = getFoodWorker_Condition:new()


local getPlot_Action = Action:new()

function getPlot_Action:running(ant, blackboard)
	--instant
	return false
end

function getPlot_Action:run(ant, blackboard)
	local plot = factory:GetPlotByID(blackboard.plot)
	blackboard.target.x = plot:GetX()
	blackboard.target.y = plot:GetY()
	blackboard.target.ID = blackboard.plot
end

local getPlot_a = getPlot_Action:new()

--reuse moveToTarget_a

local harvestFood_Action = Action:new()

function harvestFood_Action:running(ant, blackboard)
	return false
end

function harvestFood_Action:run(ant, blackboard)
	local plot = factory:GetPlotByID(blackboard.plot)
	local withdrawal = plot:WithdrawFood(100)
	--print("Worker withdrew "..withdrawal.." food from the fungus farm.")
	ant:AddFood(withdrawal)
	--print("Worker has "..ant:GetFood().." food and farm has "..plot:GetFood())
	if plot:GetFood() == 0 then
		RemovePlot(blackboard.plot)
		blackboard.plot = -1
	end
end

local harvestFood_a = harvestFood_Action:new()

--End Get food from fungus plot--

--Place new plot--

local placePlot_Condition = Condition:new()

function placePlot_Condition:conditionMet(ant, blackboard)
	return blackboard.plot == -1 and blackboard.curAction == 0
end

local placePlot_c = placePlot_Condition:new()


local getNewPlotLoc_Action = Action:new()

function getNewPlotLoc_Action:running(ant, blackboard)
	return false
end

function getNewPlotLoc_Action:run(ant, blackboard)
	local seed = os.clock()
	math.randomseed(seed)
	math.random()

	--random position near worker (plots may end up stacked, but it shouldn't be too big a problem;
	--besides, ants put things in piles, too!)
	local x = math.random(ant:GetX() - 10, ant:GetX() + 10)
	local y = math.random(ant:GetY() - 10, ant:GetY() + 10)
	
	blackboard.target.x = x
	blackboard.target.y = y
	--print("New fungus farm being placed at "..x.." "..y)
end

local getNewPlotLoc_a = getNewPlotLoc_Action:new()

--reuse moveToTarget_a

local placePlot_Action = Action:new()

function placePlot_Action:running(ant, blackboard)
	return false
end

function placePlot_Action:run(ant, blackboard)
	local id = CreatePlot(blackboard.target.x, blackboard.target.y)
	if id ~= -1 then
		blackboard.plot = id
		--print("Planted new farm, id is "..id)
	end
end

local placePlot_a = placePlot_Action:new()

--End Place new plot--


--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant, blackboard)
	--worker eats every 8 seconds
	return ant:GetFood() > 0 and blackboard.delta_sum >= 8 and blackboard.curAction == 0
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant, blackboard)
	--eating is...fast
	return false
end

function eat_Action:run(ant, blackboard)
	ant:Eat()
	--print("Worker just ate")
	blackboard.delta_sum = 0
end

local eat_a = eat_Action:new()

--end Eat--


--Actual worker ant behavior tree--
local DeliverFoodQueen = { condition = deliverFoodQueen_c, actions = { getQueen_a, moveToTarget_a, deliverFoodAnt_a } }
local DeliverFoodWarehouse = { condition = deliverFoodWarehouse_c, actions = { getWarehouse_a, moveToTarget_a,
	deliverFoodWarehouse_a } }
local GetFoodPlot = { condition = getFoodWorker_c, actions = { getPlot_a, moveToTarget_a, harvestFood_a } }
local PlacePlot = { condition = placePlot_c, actions = { getNewPlotLoc_a, moveToTarget_a, placePlot_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local WorkerBT = { Eat, DeliverFoodQueen, DeliverFoodWarehouse, GetFoodPlot, PlacePlot }


--blackboard tables for worker ants
local workerBB = {}


function WorkerRun(ID, dt)
	if workerBB[ID] == nil then
		workerBB[ID] = { actions = {}, curAction = 0, target = {x = nil, y = nil, ID = -1}, delta_sum = 0,
			movement_speed = 0.5, plot = -1 }
	end

	local ant = factory:GetAntByID(ID)
	
	if workerBB[ID].curAction == 0 then
		for key, val in pairs(WorkerBT) do
			local behavior = WorkerBT[key]
			local result = behavior.condition:conditionMet(ant, workerBB[ID])
			if result then
				--print("Worker: Behavior chosen: "..key)
				workerBB[ID].actions = behavior.actions
				workerBB[ID].curAction = 1
				break;
			end
		end		
	end

	local curAction = workerBB[ID].curAction
	--run the current action
	if curAction ~= 0 and workerBB[ID].actions ~= nil then
		workerBB[ID].actions[curAction]:run(ant, workerBB[ID], dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = workerBB[ID].actions[curAction]:running(ant, workerBB[ID])
		if status == false then
			if workerBB[ID].actions[curAction + 1] ~= nil then
				workerBB[ID].curAction = curAction + 1
			else
				workerBB[ID].actions = nil
				workerBB[ID].curAction = 0
				workerBB[ID].target.x = nil
				workerBB[ID].target.y = nil
				workerBB[ID].target.ID = -1
			end
		end
	end

	workerBB[ID].delta_sum = workerBB[ID].delta_sum + dt
end