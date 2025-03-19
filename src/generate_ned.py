def generate_ned(topo_file, output_ned):
    with open(topo_file, "r") as file:
        lines = file.readlines()
    
    num_clients = int(lines[0].split(":")[1].strip())
    num_servers = int(lines[1].split(":")[1].strip())
    
    connections = [line.strip() for line in lines[2:]]

    with open(output_ned, "w") as file:
        file.write("network DynamicNetwork {\n")
        file.write("    submodules:\n")
        
        for i in range(num_clients):
            file.write(f"        client{i}: Client;\n")
        
        for i in range(num_servers):
            file.write(f"        server{i}: Server;\n")

        file.write("    connections:\n")
        for conn in connections:
            c, s = conn.split()
            file.write(f"        {c}.out --> {s}.in;\n")
        
        file.write("}\n")

# Run script
generate_ned("topo.txt", "DynamicNetwork.ned")
