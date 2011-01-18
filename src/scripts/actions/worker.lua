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
	return ant:GetFood() > 10 and factory:GetQueen():GetFood() < 10
end

local deliverFoodQueen_c = deliverFoodQueen_Condition:new()


--reuse the getQueen_Action action
--reuse the moveToTarget_Action action
--reuse the deliverFoodAnt_Action action

--End deliver food to queen objective--


--Deliver food to warehouse objective--

local deliverFoodWarehouse_Condition = Condition:new()

function deliverFoodWarehouse_Condition:conditionMet(ant, blackboard)
	return ant:GetFood() > 10
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
	local withdrawal = ant:WithdrawFood(50)
	factory:GetWarehouse():DepositFood(withdrawal)
end

local deliverFoodWarehouse_a = deliverFoodWarehouse_Action:new()
--End Deliver food to warehouse objective--


--Get food from fungus plot--

local getFoodWorker_Condition = Condition:new()

function getFoodWorker_Condition:conditionMet(ant, blackboard)
	return ant:GetFood() <= 10 and blackboard.plot ~= -1
end

local getFoodWorker_c = getFoodWorker_Condition:new()


local getPlot_Action = Action:new()

function getPlot_Action:running(ant, blackboard)
	--instant
	return false
end

function getPlot_Action:run(ant, blackboard)
	if ant:GetID() == 3 then
		print("\nGetting plot target (worker ID: "..ant:GetID()..", plot ID: "..blackboard.plot..")")
	end
	local plot = factory:GetPlotByID(blackboard.plot)
	local newtarget = blackboard.plot
	blackboard.target.x = plot:GetX()
	blackboard.target.y = plot:GetY()
	blackboard.target.ID = newtarget
	if ant:GetID() == 3 then
		print("\nGetting plot target (worker ID: "..ant:GetID()..", plot ID: "..blackboard.plot..")")
	end
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
	ant:AddFood(withdrawal)

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
	if blackboard.plot == -1 then
		print("Conditional: ant: "..ant:GetID().." plot: "..blackboard.plot)
	end

	return blackboard.plot == -1
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

	local x = math.random(ant:GetX() - 10, ant:GetX() + 10)
	local y = math.random(ant:GetY() - 10, ant:GetY() + 10)
	
	blackboard.target.x = x
	blackboard.target.y = y
end

local getNewPlotLoc_a = getNewPlotLoc_Action:new()

--reuse moveToTarget_a

local placePlot_Action = Action:new()

function placePlot_Action:running(ant, blackboard)
	return false
end

function placePlot_Action:run(ant, blackboard)
	local id = CreatePlot(blackboard.target.x, blackboard.target.y)
	if ant:GetID() == 3 then
		print("Worker 3 plot ID is "..blackboard.plot.."\n")
	end
	if id ~= -1 then
		blackboard.plot = id
	end
	if ant:GetID() == 3 then
		print("Worker 3 plot ID is "..blackboard.plot.."\n")
	end
end

local placePlot_a = placePlot_Action:new()

--End Place new plot--


--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant, blackboard)
	--worker eats every 8 seconds
	return ant:GetFood() > 0 and blackboard.delta_sum >= 8
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant, blackboard)
	--eating is...fast
	return false
end

function eat_Action:run(ant, blackboard)
	ant:Eat()
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

local WorkerBT = { Eat, DeliverFoodQueen, DeliverFoodWarehouse, PlacePlot, GetFoodPlot }


--blackboard tables for worker ants
local workerBB = {}


function WorkerRun(ID, dt)
	if workerBB[ID] == nil then
		workerBB[ID] = { actions = {}, curAction = 0, target = {x = nil, y = nil, ID = -1}, delta_sum = 0,
			movement_speed = 0.6, plot = -1 }
		for k, v in pairs(workerBB) do
			print("Ant "..k..": target: x: "..v.target.x.." y: "..v.target.y.." plot: "..v.plot)
		end
	end

	local ant = factory:GetAntByID(ID)
	
	if workerBB[ID].curAction == 0 then
		for key, val in pairs(WorkerBT) do
			local behavior = WorkerBT[key]
			local result = behavior.condition:conditionMet(ant, workerBB[ID])
			if result then
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