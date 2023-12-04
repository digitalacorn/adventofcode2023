
local printf = function(s,...)
  return io.write(s:format(...))
end

local inputFilename = arg[1]
printf("Opening %s\n", inputFilename)

local file = io.open(inputFilename, "rb")
if not file then os.exit(1) end
local content = file:read "*a"
file:close()

local totalPoints = 0

for line in content:gmatch("[^\r\n]+") do
    local points = 0
    local winners = {}
    local lineIter = line:gmatch("[^|:]+")
    local cardNumber = tonumber(lineIter():match("%d+"))
    local winnersIter = lineIter():gmatch("%d+")
    local numbersIter = lineIter():gmatch("%d+")
    for winnerStr in winnersIter do
        winners[tonumber(winnerStr)] = true
    end
    for numberStr in numbersIter do
        local number = tonumber(numberStr)
        if winners[number] ~= nil then
            if points == 0 then
                points = 1
            else
                points = points * 2
            end
        end
    end
    printf("Card: %d %d \n", cardNumber, points)

    totalPoints = totalPoints + points
end

printf("totalPoints: %d \n", totalPoints)

