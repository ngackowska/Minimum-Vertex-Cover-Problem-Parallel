import json


with open('outputs/output.json') as f:
    data = json.load(f)

print(data)

out = ""

for graph in data:
    # print(graph)
# out += str(data[graph]["nodes"]) + ";" + str(data[graph]["density"]) + ";" + str(data[graph]["time0"]) + ";" + str(data[graph]["time1"]) + ";" + str(data[graph]["time2"]) +"\n"
    out += str(data[graph]["nodes"]) + ";" + str(data[graph]["density"]) + ";" + str(data[graph]["time0"]) +"\n"
    # print(data[graph]["nodes"])
    # print(data[graph]["density"])
    # print(data[graph]["time"])

with open("output.txt", "w") as f:
  f.write(out)