--Larva action script
--[[Possible larva actions
Eat
Morph
--]]


--Eat--

local eat_Condition = Condition:new()

function eat_Condition:conditionMet(ant, blackboard)
	--larva eats every 4 seconds
	return ant:GetFood() > 0  and blackboard.delta_sum >= 4 and blackboard.curAction == 0
end

local eat_c = eat_Condition:new()


local eat_Action = Action:new()

function eat_Action:running(ant, blackboard)
	return false
end

function eat_Action:run(ant, blackboard)
	ant:Eat()
	--print("Larva just ate.  Delta sum: "..blackboard.delta_sum)
	blackboard.delta_sum = 0
end

local eat_a = eat_Action:new()

--end Eat--

--Morph--

local morph_Condition = Condition:new()

function morph_Condition:conditionMet(ant, blackboard)
	return ant:CanMorph() and blackboard.curAction == 0
end

local morph_c = morph_Condition:new()


local morph_Action = Action:new()

function morph_Action:running(ant, blackboard)
	return false
end

function morph_Action:run(ant, blackboard)
	local seed = os.clock()
	math.randomseed(seed)
	math.random()

	local morphTarget = math.random(1, 2)
	--print("Morph randomizer gave "..morphTarget)
	if morphTarget == 1 then
		AddAnt("worker", ant:GetX(), ant:GetY())
	elseif morphTarget == 2 then
		AddAnt("nurse", ant:GetX(), ant:GetY())
	end

	blackboard.dead = true
end

local morph_a = morph_Action:new()

--End Morph--

--Actual larva behavior tree--
local Eat = { condition = eat_c, actions = { eat_a } }
local Morph = { condition = morph_c, actions = { morph_a } }

local LarvaBT = { Eat, Morph }


--blackboard table
local larvaBB = {}


function LarvaRun(ID, dt)
	if larvaBB[ID] == nil then
		larvaBB[ID] = { actions = {}, curAction = 0, delta_sum = 0, larva_sum = 0, dead = false }
	end

	local ant = factory:GetAntByID(ID)

	if larvaBB[ID].curAction == 0 then
		for key, val in pairs(LarvaBT) do
			local behavior = LarvaBT[key]
			local result = behavior.condition:conditionMet(ant, larvaBB[ID])
			if result then
				--print("Larva: Behavior chosen: "..key)
				larvaBB[ID].actions = behavior.actions
				larvaBB[ID].curAction = 1
				break;
			end
		end		
	end

	local curAction = larvaBB[ID].curAction
	--run the current action
	if curAction ~= 0 and larvaBB[ID].actions ~= nil then
		larvaBB[ID].actions[curAction]:run(ant, larvaBB[ID], dt)
	
		--see if it's done and update current action and actions tree for ant
		local status = larvaBB[ID].actions[curAction]:running(ant, larvaBB[ID])
		if status == false then
			if larvaBB[ID].actions[curAction + 1] ~= nil then
				larvaBB[ID].curAction = curAction + 1
			else
				larvaBB[ID].actions = nil
				larvaBB[ID].curAction = 0
			end
		end
	end

	larvaBB[ID].delta_sum = larvaBB[ID].delta_sum + dt

	if larvaBB[ID].dead then
		RemoveAnt(ID)
		larvaBB[ID] = nil
	end
end