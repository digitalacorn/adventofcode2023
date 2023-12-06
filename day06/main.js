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

let totalCombinations = 1;
for (let index = 0; index < raceTimes.length; index++) {
	const time = raceTimes[index];
	const winningTime = raceRecords[index] + 1;
	const factor = Math.sqrt(time * time - 4 * winningTime);
	const min = Math.ceil(0.5 * (time - factor));
	const max = Math.floor(0.5 * (time + factor));
	combinations = max - min + 1;
	console.log(
		`Race ${index + 1}: ${min}-${max} = ${combinations} combinations`
	);
	totalCombinations *= combinations;
}

console.log('totalCombinations', totalCombinations);
