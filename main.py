import networkx as nx
import matplotlib.pyplot as plt
from networkx.readwrite import json_graph
import scipy as sp
import numpy as np
import json
import random
import math


# Opening JSON file (loading graph) #########################

with open('outputs/graphs/n10p04.json') as f:
    data = json.load(f)

must_nodes = data['-1']
del data['-1']

# Opening JSON file for covers #########################
with open('outputs/covers/n10p04.json') as f:
    cover = json.load(f)


# cover = data_covers['genetyczny']  # picking algorithm (aproksymacyjny, brute_force, genetyczny)

# print(data)

# Graph ########################
G = nx.Graph()

color_map = []

for i in data:
    # print(i)
    if int(i) in cover:
        if int(i) in must_nodes:
            color_map.append("red")
        else:
            color_map.append("green")
    else:
        color_map.append("gray")

    G.add_node(i)

for i in data:
    for j in range(int(i),len(data[i])):
        if data[i][j] == 1:
            G.add_edge(i,str(j))


# Showing graph #####################
nx.draw(G, with_labels=True, node_color=color_map)
plt.show()
