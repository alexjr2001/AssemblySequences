import networkx as nx
import matplotlib.pyplot as plt

def read_paths(filename):
    paths = []
    with open(filename, 'r') as f:
        current_path = []
        for line in f:
            line = line.strip()
            if line:
                node, edge = line.split()
                current_path.append((node, edge))
            else:
                paths.append(current_path)
                current_path = []
    return paths

filename = 'paths.txt'
paths = read_paths(filename)

length = 0
for path in paths:
    for node, edge in path:
        length += len(node)
        length -= int(edge)

pivot = 0
temp = 0
for path in paths:
    for node, edge in path:
        line = length
        while(line>0):
            if pivot == length - line:
                print(node, end='')
                line -= len(node) + int(edge)
                temp = pivot + len(node) - int(edge)
            else:
                print("-", end='')
                line -= 1
        print()
        pivot=temp

G = nx.Graph()
for path in paths:
    for i in range(len(path)-1):
        node1, _ = path[i]
        node2, edge = path[i+1]
        G.add_edge(node1, node2, label=edge)

pos = nx.spring_layout(G)  # Layout del grafo
labels = nx.get_edge_attributes(G, 'label')
nx.draw(G, pos, with_labels=True, node_size=500, node_color='lightblue', font_size=10, font_weight='bold')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels, font_color='red')

plt.title('Grafo de Caminos')
plt.show()