--Queen action script

function QueenRun(ID, dt)
	ant = factory:GetAntByID(ID)
	if dt > 1 then
		ant:Eat()
		print("Queen just ran. Food: "..ant:GetFood().." Larvae: "..ant:GetNumAvailLarvae())
	end
end