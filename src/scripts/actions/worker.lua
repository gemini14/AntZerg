--Worker action script
--[[Possible worker actions:
Bring food to queen
Bring food to warehouse
Get food from fungus plot
Place new fungus plot
--]]


--Deliver food to queen objective--

local deliverFoodQueen_Condition = Condition:new()

function deliverFoodQueen_Condition:conditionMet(ant)
	return ant:GetFood() > 10 and factory:GetQueen():GetFood() < 10
end

local deliverFoodQueen_c = deliverFoodQueen_Condition:new()


--reuse the getQueen_Action action
--reuse the moveToTarget_Action action
--reuse the deliverFoodAnt_Action action

--End deliver food to queen objective--


--Deliver food to warehouse objective--

local deliverFoodWarehouse_Condition = Condition:new()

function deliverFoodWarehouse_Condition:conditionMet(ant)
	return ant:GetFood() > 10
end

local deliverFoodWarehouse_c = deliverFoodWarehouse_Condition:new()

--reuse getWarehouse_Action action
--reuse moveToTarget_Action action

local deliverFoodWarehouse_Action = Action:new()

function deliverFoodWarehouse_Action:running(ant)
	--only takes a second
	return false
end

function deliverFoodWarehouse_Action:run(ant)
	local withdrawal = ant:WithdrawFood(50)
	factory:GetWarehouse():DepositFood(withdrawal)
end

local deliverFoodWarehouse_a = deliverFoodWarehouse_Action:new()
--End Deliver food to warehouse objective--


--Get food from fungus plot--

local getFoodWorker_Condition = Condition:new()

function getFoodWorker_Condition:conditionMet(ant)
	return ant:GetFood() <= 10 and ant.blackboard.plot ~= -1
end

local getFoodWorker_c = getFoodWorker_Condition:new()


local getPlot_Action = Action:new()

function getPlot_Action:running(ant)
	--instant
	return false
end

function getPlot_Action:run(ant)
	local targetPlot = factory:GetPlotByID(ant.blackboard.plot)

	ant.blackboard.target_x = targetPlot:GetX()
	ant.blackboard.target_y = targetPlot:GetY()
	ant.blackboard.target_ID = ant.blackboard.plot
end

local getPlot_a = getPlot_Action:new()

--reuse moveToTarget_a

local harvestFood_Action = Action:new()

function harvestFood_Action:running(ant)
	return false
end

function harvestFood_Action:run(ant)
	local retrievedPlot = factory:GetPlotByID(ant.blackboard.plot)
	if retrievedPlot ~= nil then
		local withdrawal = retrievedPlot:WithdrawFood(100)
		ant:AddFood(withdrawal)
		
		if retrievedPlot:GetFood() == 0 then
			RemovePlot(ant.blackboard.plot)	
			ant.blackboard.plot = -1
		end
	end
end

local harvestFood_a = harvestFood_Action:new()

--End Get food from fungus plot--

--Place new plot--

local placePlot_Condition = Condition:new()

function placePlot_Condition:conditionMet(ant)
	return ant.blackboard.plot == -1
end

local placePlot_c = placePlot_Condition:new()


local getNewPlotLoc_Action = Action:new()

function getNewPlotLoc_Action:running(ant)
	return false
end

function getNewPlotLoc_Action:run(ant)
	local seed = os.clock()
	math.randomseed(seed)
	math.random()
	math.random()

	local x = math.random(ant:GetX() - 10, ant:GetX() + 10)
	local y = math.random(ant:GetY() - 10, ant:GetY() + 10)
	ant.blackboard.target_x = x
	ant.blackboard.target_y = y
end

local getNewPlotLoc_a = getNewPlotLoc_Action:new()

--reuse moveToTarget_a

local placePlot_Action = Action:new()

function placePlot_Action:running(ant)
	return false
end

function placePlot_Action:run(ant)
	local id = CreatePlot(ant.blackboard.target_x, ant.blackboard.target_y)
	if id ~= -1 then
		ant.blackboard.plot = id
	end
end

local placePlot_a = placePlot_Action:new()

--End Place new plot--


--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant)
	--worker eats every 8 seconds
	return ant:GetFood() > 0 and ant.blackboard.delta_sum >= 8
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


--Actual worker ant behavior tree--
local DeliverFoodQueen = { condition = deliverFoodQueen_c, actions = { getQueen_a, moveToTarget_a, deliverFoodAnt_a } }
local DeliverFoodWarehouse = { condition = deliverFoodWarehouse_c, actions = { getWarehouse_a, moveToTarget_a,
	deliverFoodWarehouse_a } }
local GetFoodPlot = { condition = getFoodWorker_c, actions = { getPlot_a, moveToTarget_a, harvestFood_a } }
local PlacePlot = { condition = placePlot_c, actions = { getNewPlotLoc_a, moveToTarget_a, placePlot_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local WorkerBT = { Eat, DeliverFoodQueen, DeliverFoodWarehouse, GetFoodPlot, PlacePlot }



function WorkerRun(ID, dt)
	local ant = factory:GetAntByID(ID)

	if ant.blackboard.curAction == 0 then
		for key, val in pairs(WorkerBT) do
			if WorkerBT[key].condition:conditionMet(ant) then
				ant.blackboard.behavior = key
				ant.blackboard.curAction = 1
				break;
			end
		end		
	end

	--run the current action
	if ant.blackboard.curAction ~= 0 and WorkerBT[ant.blackboard.behavior] ~= nil then
		WorkerBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:run(ant, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = WorkerBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:running(ant)
		if status == false then
			if WorkerBT[ant.blackboard.behavior].actions[ant.blackboard.curAction + 1] ~= nil then
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

	ant.blackboard.delta_sum = ant.blackboard.delta_sum + dt
end