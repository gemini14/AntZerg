--Queen action script

local delta = 0

function QueenRun(ID, dt)
	if delta < 2 then
		delta = delta + dt
	else
		--over 2 secs
		local ant = factory:GetAntByID(ID)
		ant:Eat()
		print("Queen just ran. Food: "..ant:GetFood().." Larvae: "..ant:GetNumAvailLarvae())
		delta = 0
	end
end