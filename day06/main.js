const fs = require('fs');
filename = process.argv[2];

const [line1, line2] = fs.readFileSync(filename, 'utf-8').split(/\r?\n/);

const parseLine = (line) =>
	line
		.split(/ +/)
		.slice(1)
		.map((i) => parseInt(i));

const raceTimes = parseLine(line1);
const raceRecords = parseLine(line2);
if (raceTimes.length !== raceRecords.length) {
	console.error('malformed input');
	process.exit(1);
}

const raceCombinations = (raceTime, currentRecord) => {
	const winningTime = currentRecord + 1;
	const factor = Math.sqrt(raceTime * raceTime - 4 * winningTime);
	const min = Math.ceil(0.5 * (raceTime - factor));
	const max = Math.floor(0.5 * (raceTime + factor));
	return max - min + 1;
};

let totalCombinations = 1;
for (let index = 0; index < raceTimes.length; index++) {
	combinations = raceCombinations(raceTimes[index], raceRecords[index]);
	console.log(`Race ${index + 1}: ${combinations} combinations`);
	totalCombinations *= combinations;
}

console.log('totalCombinations', totalCombinations);

const massiveRaceTime = parseInt(raceTimes.join(''));
const massiveRaceRecord = parseInt(raceRecords.join(''));

console.log(
	`Massive Race: ${raceCombinations(
		massiveRaceTime,
		massiveRaceRecord
	)} combinations`
);
