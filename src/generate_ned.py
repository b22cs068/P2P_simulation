import random

TOPO_FILE = "topo.txt"
NED_FILE = "My_Network.ned"

def read_topo_file(filename):
    """ Reads topo.txt to get the number of clients and servers. """
    with open(filename, "r") as f:
        lines = f.readlines()
    
    num_clients = int(lines[0].split(":")[1].strip())
    num_servers = int(lines[1].split(":")[1].strip())

    return num_clients, num_servers

def generate_client_connections(num_clients):
    """ Generates a fully connected, randomized client-to-client topology. """
    if num_clients < 2:
        return []
    
    connections = []
    remaining_clients = list(range(num_clients))
    random.shuffle(remaining_clients)

    # Ensure every client is connected to at least one other (form a cycle)
    for i in range(num_clients - 1):
        connections.append((remaining_clients[i], remaining_clients[i + 1]))
    
    # Connect the last client to the first, ensuring full connectivity
    connections.append((remaining_clients[-1], remaining_clients[0]))

    # Add extra random connections for variation
    extra_connections = num_clients // 2
    all_possible_pairs = [(i, j) for i in range(num_clients) for j in range(i + 1, num_clients)]
    random.shuffle(all_possible_pairs)

    for i, j in all_possible_pairs:
        if (i, j) not in connections and (j, i) not in connections and len(connections) < num_clients + extra_connections:
            connections.append((i, j))

    return connections

def update_topo_file(filename, num_clients, num_servers, client_connections):
    """ Updates topo.txt with generated client-to-client connections. """
    with open(filename, "w") as f:
        f.write(f"clients: {num_clients}\n")
        f.write(f"servers: {num_servers}\n\n")
        f.write("clientLinks:\n")
        for c1, c2 in client_connections:
            f.write(f"client{c1} <-> client{c2}\n")

    print(f"Updated {filename} with client-client connections.")

def generate_ned_file(num_clients, num_servers, client_connections):
    """ Generates My_Network.ned with clients, servers, and connections. """
    with open(NED_FILE, "w") as f:
        f.write("package A2;\n\n")
        f.write("import ned.IdealChannel;\n\n")
        f.write("module Client { gates: input in; output out; @class(Server); }\n")
        f.write("module Server { gates: input in; output out; @class(Client); }\n\n")
        f.write("network My_Network {\n")
        f.write("    submodules:\n")

        # Define clients
        for i in range(num_clients):
            f.write(f"        client{i}: Client;\n")

        # Define servers
        for j in range(num_servers):
            f.write(f"        server{j}: Server;\n")

        f.write("\n    connections:\n")

        # Connect all clients to all servers
        for i in range(num_clients):
            for j in range(num_servers):
                f.write(f"        client{i}.out --> IdealChannel --> server{j}.in;\n")
                f.write(f"        server{j}.out --> IdealChannel --> client{i}.in;\n")

        # Connect clients based on randomized topology
        for c1, c2 in client_connections:
            f.write(f"        client{c1}.out --> IdealChannel --> client{c2}.in;\n")
            f.write(f"        client{c2}.out --> IdealChannel --> client{c1}.in;\n")

        f.write("}\n")

    print(f"Generated {NED_FILE}.")

def main():
    num_clients, num_servers = read_topo_file(TOPO_FILE)
    client_connections = generate_client_connections(num_clients)
    update_topo_file(TOPO_FILE, num_clients, num_servers, client_connections)
    generate_ned_file(num_clients, num_servers, client_connections)

if __name__ == "__main__":
    main()