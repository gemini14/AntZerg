--Queen action script
--[[Possible queen actions:
Create larva
Eat
--]]

--------

--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant, blackboard)
	--queen eats every 5 seconds
	return ant:GetFood() > 0  and blackboard.delta_sum >= 5
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

--Create larva--

local createLarva_Condition = Condition:new()

function createLarva_Condition:conditionMet(ant, blackboard)
	return ant:GetFood() > 0 and blackboard.larva_sum >= 3
end

local createLarva_c = createLarva_Condition:new()


local createLarva_Action = Action:new()

function createLarva_Action:running(ant, blackboard)
	return false
end

function createLarva_Action:run(ant, blackboard)
	ant:CreateLarva()
	blackboard.larva_sum = 0
end

local createLarva_a = createLarva_Action:new()
--end Create larva--



--------

--Actual queen ant behavior tree--
local CreateLarva = { condition = createLarva_c, actions = { createLarva_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local QueenBT = { CreateLarva, Eat }

--blackboard table
local queenBB = {}

function QueenRun(ID, dt)
	if queenBB.ID == nil then
		queenBB.ID = { actions = {}, curAction = 0, delta_sum = 0, larva_sum = 0 }
	end

	local ant = factory:GetAntByID(ID)

	if queenBB.ID.curAction == 0 then
		for key, val in pairs(QueenBT) do
			local behavior = QueenBT[key]
			local result = behavior.condition:conditionMet(ant, queenBB.ID)
			if result then
				queenBB.ID.actions = behavior.actions
				queenBB.ID.curAction = 1
				break;
			end
		end		
	end

	local curAction = queenBB.ID.curAction
	--run the current action
	if curAction ~= 0 and queenBB.ID.actions ~= nil then
		queenBB.ID.actions[curAction]:run(ant, queenBB.ID, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = queenBB.ID.actions[curAction]:running(ant, queenBB.ID)
		if status == false then
			if queenBB.ID.actions[curAction + 1] ~= nil then
				queenBB.ID.curAction = curAction + 1
			else
				queenBB.ID.actions = nil
				queenBB.ID.curAction = 0
			end
		end
	end

	queenBB.ID.delta_sum = queenBB.ID.delta_sum + dt
	queenBB.ID.larva_sum = queenBB.ID.larva_sum + dt
end