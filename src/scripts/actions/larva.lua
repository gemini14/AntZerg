--Larva action script
--[[Possible larva actions
Eat
Morph
--]]


--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant)
	--larva eats every 4 seconds
	return ant:GetFood() > 0  and ant.blackboard.delta_sum >= 4
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant)
	return false
end

function eat_Action:run(ant)
	ant:Eat()
	ant.blackboard.delta_sum = -1
end

local eat_a = eat_Action:new()

--end Eat--

--Morph--

local morph_Condition = Condition:new()

function morph_Condition:conditionMet(ant)
	return ant:CanMorph()
end

local morph_c = morph_Condition:new()


local morph_Action = Action:new()

function morph_Action:running(ant)
	return false
end

function morph_Action:run(ant)
	local seed = os.clock()
	math.randomseed(seed)
	math.random()

	local morphTarget = math.random()
	if morphTarget >= 0 and morphTarget < 0.4 then
		QueueAddAnt("worker", ant:GetX(), ant:GetY())
	elseif morphTarget >= 0.4 and morphTarget <= 1 then
		QueueAddAnt("nurse", ant:GetX(), ant:GetY())
	end

	ant.blackboard.dead = true
end

local morph_a = morph_Action:new()

--End Morph--


--Actual larva behavior tree--
local Eat = { condition = eat_c, actions = { eat_a } }
local Morph = { condition = morph_c, actions = { morph_a } }

local LarvaBT = { Eat, Morph }


function LarvaRun(ID, dt)
	local ant = factory:GetAntByID(ID)

	if ant.blackboard.curAction == 0 then
		for key, val in pairs(LarvaBT) do
			if LarvaBT[key].condition:conditionMet(ant) then
				ant.blackboard.behavior = key
				ant.blackboard.curAction = 1
				break;
			end
		end		
	end

	--run the current action
	if ant.blackboard.curAction ~= 0 and LarvaBT[ant.blackboard.behavior] ~= nil then
		LarvaBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:run(ant, dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = LarvaBT[ant.blackboard.behavior].actions[ant.blackboard.curAction]:running(ant)
		if status == false then
			if LarvaBT[ant.blackboard.behavior].actions[ant.blackboard.curAction + 1] ~= nil then
				ant.blackboard.curAction = ant.blackboard.curAction + 1
			else
				ant.blackboard.behavior = 0
				ant.blackboard.curAction = 0
			end
		end
	end

	ant.blackboard.delta_sum = dt

	if ant.blackboard.dead then
		RemoveAnt(ID)
	end
end