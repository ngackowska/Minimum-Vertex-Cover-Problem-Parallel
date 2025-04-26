import json
import sys

data = {}

nodes_start = 250
nodes_stop = 3000
nodes_step = 250

density_start = 0.3
density_stop = 0.8
density_step = 0.1

name = "inputs/inputBasic"

counter = 0


nodes_start = int(sys.argv[2])
nodes_stop = int(sys.argv[3])
nodes_step = int(sys.argv[4])

density_start = float(sys.argv[5])
density_stop = float(sys.argv[6])
density_step = float(sys.argv[7])

name = sys.argv[1]


for i in range(nodes_start, nodes_stop+1, nodes_step):
    for j in range(int(density_start*10), int(density_stop*10+1), int(density_step*10)):
        data[counter] = {}
        data[counter]["nodes"] = i
        data[counter]["density"] = j/10

        counter += 1


file = "inputs/" + name + ".json"

with open(file, 'w', encoding='utf-8') as f:
    json.dump(data, f, ensure_ascii=False, indent=4)