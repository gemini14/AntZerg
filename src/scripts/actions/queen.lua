--Queen action script
--[[Possible queen actions:
Create larva
Eat
--]]

--------

--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant)
	--queen eats every 5 seconds
	return ant:GetFood() > 0  and ant.blackboard.delta_sum >= 5
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

--Create larva--

local createLarva_Condition = Condition:new()

function createLarva_Condition:conditionMet(ant)
	return ant:GetFood() > 0 and ant.blackboard.larva_sum >= 3
end

local createLarva_c = createLarva_Condition:new()


local createLarva_Action = Action:new()

function createLarva_Action:running(ant)
	return false
end

function createLarva_Action:run(ant)
	ant:CreateLarva()
	ant.blackboard.larva_sum = -1
end

local createLarva_a = createLarva_Action:new()
--end Create larva--


--------

--Actual queen ant behavior tree--
local CreateLarva = { condition = createLarva_c, actions = { createLarva_a } }
local Eat = { condition = eat_c, actions = { eat_a } }

local QueenBT = { CreateLarva, Eat }


function QueenRun(ID, dt)
	local ant = factory:GetAntByID(ID)

	if ant.blackboard.curAction == 0 then
		for key, val in pairs(QueenBT) do
			if QueenBT[key].condition:conditionMet(ant) then
				ant.blackboard.behavior = key
				ant.blackboard.curAction = 1
				break;
			end
		end		
	end

	--run the current action
	if ant.blackboard.curAction ~= 0 and QueenBT[ant.blackboard.behavior] ~= nil then
		QueenBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:run(ant, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = QueenBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:running(ant)
		if status == false then
			if QueenBT[ant.blackboard.behavior].actions[ant.blackboard.curAction + 1] ~= nil then
				ant.blackboard.curAction = ant.blackboard.curAction + 1
			else
				ant.blackboard.behavior = 0
				ant.blackboard.curAction = 0
			end
		end
	end

	ant.blackboard.delta_sum = dt
	ant.blackboard.larva_sum = dt
end