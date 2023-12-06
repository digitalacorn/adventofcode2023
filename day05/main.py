
import argparse
import sys
import functools 

categories = ["seed", "soil", "fertilizer", "water", "light", "temperature", "humidity", "location"]
mapping_definitions = ["{}-to-{}".format(categories[i], categories[i+1]) for i in range(len(categories)-1)]

def main(filename):
    seeds=[]
    mappings={}
    read_section=None
    file1 = open(filename, 'r')
    lines = file1.readlines()
    for line_number, line in enumerate(lines):
        parts = line.split()
        if not len(parts):
            continue
        if parts[0] == "seeds:":
            seeds = [int(p) for p in parts[1:]]
        elif len(parts)>1 and parts[1]=="map:":
            try:
                index = mapping_definitions.index(parts[0])
            except:
                raise Exception("{} is not recognised on line {}".format(parts[0], line_number))
            read_section = categories[index]
            mappings[read_section] = []
        elif len(parts)==3:
            if read_section is None:
                raise Exception("mapping defined before category specified on line {} - ".format(line_number))
            [destination_range_start, source_range_start, range_lngth] = [int(p) for p in parts]
            source_range_end = source_range_start + range_lngth - 1
            offset = destination_range_start-source_range_start

            mappings[read_section].append((source_range_start, source_range_end, offset))
        else:
            raise Exception("Unexpected input on line {}".format(line_number))
    
    closest_location = None

    def mapper(category, item):
        mapped_value = item
        for (start, end, offset) in mappings[category]:
            if item>=start and item<=end:
                mapped_value = item+offset
        return mapped_value

    print("seeds", seeds)
    locations = []
    for seed in seeds:
        item = seed
        for category in categories[:-1]:
            item = mapper(category, item)
        location = item
        print("seed: {} = location:{}".format( seed, location))
        locations.append(location)

    print("Closest location {}".format(min(locations)))

    return 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='main')
    parser.add_argument('filename')  

    args = parser.parse_args()

    sys.exit(main(args.filename)) 
