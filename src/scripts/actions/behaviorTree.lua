--Conditions are just simple checks to see if the actor should or should not do something
Condition = {}

function Condition:new(obj)
	--create object if not provided
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

function Condition:conditionMet(ant)
	return true
end


--Actions carry out objectives like moving someplace, eating, etc.
Action = {}

function Action:new(obj)
	--create object if not provided
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

--can contain further actions, conditions, tasks, etc.
Action.childactions = {}

function Action:running()
	--override this to return true if starting or in progress, false if it failed
	return true
end

function Action:run(ant)
end


--Get food from warehouse action--
retrieveFood_Action = Action:new()

function retrieveFood_Action:running(ant)
	if ant:GetFood() ~= 0 then
		return false
	end
	
	return true
end

function retrieveFood_Action:run(ant)
	--remove 25 food by default, less is taken out if there isn't enough
	local withdraw = factory:GetWarehouse():WithdrawFood(25)
	ant:AddFood(withdraw)
end

retrieveFood_a = retrieveFood_Action:new()
--end Get food from warehouse action--


--Move to target action--
moveToTarget_Action = Action:new()

function moveToTarget_Action:running(ant)
	local delta_x, delta_y
	delta_x = ant.blackboard.target_x - ant:GetX()
	delta_y = ant.blackboard.target_y - ant:GetY()
	if math.abs(delta_x) < 0.5 and math.abs(delta_y) < 0.5 then
		return false
	end
	return true
end

function moveToTarget_Action:run(ant, dt)
	local delta_x, delta_y
	--calculate the delta values to the target
	delta_x = ant.blackboard.target_x - ant:GetX()
	delta_y = ant.blackboard.target_y - ant:GetY()

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
	ant:PositionChange(ant.blackboard.movement_speed * dt * plusminus_x, ant.blackboard.movement_speed * dt * plusminus_y)
end

--move to target action obj
moveToTarget_a = moveToTarget_Action:new()

--end Move to target action--


--Get warehouse target action--
getWarehouse_Action = Action:new()

function getWarehouse_Action:running(ant)
	--setting coords can be done in one call
	return false
end

function getWarehouse_Action:run(ant)
	local warehouse = factory:GetWarehouse()
	ant.blackboard.target_x = warehouse:GetX()
	ant.blackboard.target_y = warehouse:GetY()
end

--warehouse coords action obj
getWarehouse_a = getWarehouse_Action:new()

--end Get warehouse target action--


--getQueen_Action--

getQueen_Action = Action:new()

function getQueen_Action:running(ant)
	return false
end

function getQueen_Action:run(ant)
	local queen = factory:GetQueen()
	ant.blackboard.target_x = queen:GetX()
	ant.blackboard.target_y = queen:GetY()
	ant.blackboard.target_ID = queen:GetID()
end

getQueen_a = getQueen_Action:new()

--end getQueen_Action--


--Deliver food ant action--
deliverFoodAnt_Action = Action:new()

function deliverFoodAnt_Action:running(ant)
	return false
end

function deliverFoodAnt_Action:run(ant)
	if factory:GetAntByID(ant.blackboard.target_ID) ~= nil then
		local withdrawnFood = ant:WithdrawFood(10)
		factory:GetAntByID(ant.blackboard.target_ID):AddFood(withdrawnFood)
	end
end

deliverFoodAnt_a = deliverFoodAnt_Action:new()
--end deliver food ant action--