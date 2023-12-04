
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

local matchCounts = {}
local cardInstances = {}

for line in content:gmatch("[^\r\n]+") do
    local points = 0
    local matches = 0
    local winners = {}
    local lineIter = line:gmatch("[^|:]+")
    local cardNumber = tonumber(lineIter():match("%d+"))
    if cardNumber == nil then
        os.exit(1)
    end

    cardInstances[cardNumber] = 1
    local winnersIter = lineIter():gmatch("%d+")
    local numbersIter = lineIter():gmatch("%d+")
    for winnerStr in winnersIter do
        winners[tonumber(winnerStr)] = true
    end
    for numberStr in numbersIter do
        local number = tonumber(numberStr)
        if winners[number] ~= nil then
            matches = matches + 1
            if points == 0 then
                points = 1
            else
                points = points * 2
            end
        end
    end

    matchCounts[cardNumber] = matches

    printf("Card: %d %d \n", cardNumber, points)

    totalPoints = totalPoints + points
end


printf("totalPoints: %d \n", totalPoints)

local totalCards = 0
for card, count in ipairs(cardInstances) do
    totalCards = totalCards + count
    for i = card+1, card+matchCounts[card], 1 do
        cardInstances[i] = cardInstances[i]+count
    end
end

printf("totalCards: %d \n", totalCards)
