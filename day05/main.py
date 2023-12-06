
import argparse
import sys
import functools 

categories = ["seed", "soil", "fertilizer", "water", "light", "temperature", "humidity", "location"]
mapping_definitions = ["{}-to-{}".format(categories[i], categories[i+1]) for i in range(len(categories)-1)]

def main(filename):
    seed_ranges=[]   # now storing a tuple of (start, end) range of seeds
    mappings={}
    read_section=None
    file1 = open(filename, 'r')
    lines = file1.readlines()
    for line_number, line in enumerate(lines):
        parts = line.split()
        if not len(parts):
            continue
        if parts[0] == "seeds:":
            seed_ranges_info = [int(p) for p in parts[1:]]
            if len(seed_ranges_info)%2 != 0:
                raise Exception("Invalid seed ranges on line {}".format(line_number))
            for i in range(0, len(seed_ranges_info), 2):
                seed_ranges.append((seed_ranges_info[i],seed_ranges_info[i]+seed_ranges_info[i+1]-1))

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
            [destination_range_start, source_range_start, range_length] = [int(p) for p in parts]
            source_range_end = source_range_start + range_length - 1
            offset = destination_range_start-source_range_start

            mappings[read_section].append((source_range_start, source_range_end, offset))
        else:
            raise Exception("Unexpected input on line {}".format(line_number))
    
    closest_location = None

    def mapper(category, item_range):

        unmapped_ranges = [item_range]
        mapped_ranges = []
        while len(unmapped_ranges):
            (input_start,input_end) = unmapped_ranges.pop()
            local_mapped_ranges = []
            for (start, end, offset) in mappings[category]:
                # print("                 map", start, end, offset)
                if input_start>=start and input_end<=end:
                    # entirely contained in the range
                    local_mapped_ranges.append((input_start+offset, input_end+offset))
                elif input_start>end or input_end<start:
                    # no overlap 
                    pass
                elif input_start<start and input_end>=start:
                    # input strides the start
                    unmapped_ranges.append((input_start,start-1))
                    local_mapped_ranges.append((start+offset,input_end+offset))
                elif input_start<=end and input_end>end:
                    # input strides the end
                    local_mapped_ranges.append((input_start+offset,end+offset))
                    unmapped_ranges.append((end+1,input_end))
                else:
                    raise Exception("Unexpected case while dividing ranges")
        
            if len(local_mapped_ranges)==0:
                local_mapped_ranges.append((input_start,input_end))
            
            mapped_ranges.extend(local_mapped_ranges)

        return mapped_ranges



    print("num seed ranges", len(seed_ranges))
    locations = []
    for seed_range in seed_ranges:
        print("seed_range", seed_range)
        item_ranges = [seed_range]
        for category in categories[:-1]:
            print("   cat", category)
            next_item_ranges = []
            for item_range in item_ranges:
                next_item_ranges.extend([i for i in mapper(category, item_range)])
            item_ranges = next_item_ranges
        
        location = min([start for (start,end) in item_ranges])
        locations.append(location)

    print("Closest location {}".format(min(locations)))

    return 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='main')
    parser.add_argument('filename')  

    args = parser.parse_args()

    sys.exit(main(args.filename)) 
