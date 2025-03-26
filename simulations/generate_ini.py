import os

# Define the appropriate directories
SRC_DIR = "src"
SIM_DIR = "simulations"

# Ensure the directories exist
os.makedirs(SRC_DIR, exist_ok=True)
os.makedirs(SIM_DIR, exist_ok=True)

def generate_ini(topo_file=f"{SRC_DIR}/topo.txt", ini_file=f"{SIM_DIR}/omnetpp.ini"):
    # Read topo.txt
    with open(topo_file, "r") as f:
        lines = f.readlines()

    # Extract client and server counts
    num_clients = num_servers = 0
    for line in lines:
        if line.startswith("clients="):
            num_clients = int(line.strip().split("=")[1])
        elif line.startswith("servers="):
            num_servers = int(line.strip().split("=")[1])

    # Write to omnetpp.ini in the simulations directory
    with open(ini_file, "w") as f:
        f.write("[General]\n")
        f.write("network = My_Network\n")
        f.write("sim-time-limit = 100s\n\n")
        
        f.write(f"*.numClients = {num_clients}\n")
        f.write(f"*.numServers = {num_servers}\n\n")

        f.write("# Server settings\n")
        f.write(f"*.server[*].processingTime = uniform(1s, 3s)\n")
        f.write(f"*.server[*].bufferSize = 100\n\n")

        f.write("# Client settings\n")
        f.write(f"*.client[*].taskGenerationInterval = exponential(5s)\n")
        f.write(f"*.client[*].subtaskCount = {num_servers}\n\n")

        f.write("# Network connection delays\n")
        f.write(f"*.client[*].out.channel.delay = uniform(10ms, 50ms)\n")
        f.write(f"*.server[*].in.channel.delay = uniform(10ms, 50ms)\n")

    print(f"Generated {ini_file} with {num_clients} clients and {num_servers} servers.")

# Run the script
generate_ini()
