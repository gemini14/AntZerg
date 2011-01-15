--Conditions are just simple checks to see if the actor should or should not do something
Condition = {}

function Condition:new(obj)
	--create object if not provided
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

function Condition:conditionMet(ant, blackboard)
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

function Action:run(ant, blackboard)
end


--generic clone function
function clone(o)
	local newobj = {}
	--retrieves first index of o and its value
	local i, v = next(o, nil)
	print("cloning")
	while i do
		print(i.." "..v)
		newobj.i = v
		i, v = next(o, i)
	end
	return newobj
end