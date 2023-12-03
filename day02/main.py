
import argparse
import sys
import functools 

total_cubes = {"red": 12, "green": 13, "blue": 14}

def main(filename):
    file1 = open(filename, 'r')
    lines = file1.readlines()
    sum = 0
    powerSum = 0
    for line in lines:
        valid = True
        minimum_cubes = {"red": 0, "green": 0, "blue": 0}
        [info, reveal_data] = line.split(":", 2)
        [_, gameId] = info.strip().split(" ", 2)
        reveals = reveal_data.split(";")
        for reveal in reveals:
            sets = reveal.split(',')
            for set in sets:
                [countStr, colourStr] = set.strip().split(" ", 2)
                count = int(countStr.strip())
                colour = colourStr.strip()
                minimum_cubes[colour] = max(minimum_cubes[colour], count)

                if total_cubes.get(colour, -1) < count:
                    valid = False
        if valid:
            sum += int(gameId.strip())
        power = functools.reduce(lambda a,b: a*b, minimum_cubes.values(), 1)
        powerSum += power

    print("Sum:", sum)
    print("Total power:", powerSum)

    return 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='main')
    parser.add_argument('filename')  

    args = parser.parse_args()

    sys.exit(main(args.filename)) 
