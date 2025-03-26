package A2;

import ned.IdealChannel;

simple Client { gates: input in; output out; }
simple Server { gates: input in; output out; }

network My_Network {
    submodules:
        client0: Client;
        client1: Client;
        client2: Client;
        client3: Client;
        server0: Server;
        server1: Server;

    connections:
        client0.out --> IdealChannel --> server0.in;
        server0.out --> IdealChannel --> client0.in;
        client0.out --> IdealChannel --> server1.in;
        server1.out --> IdealChannel --> client0.in;
        client1.out --> IdealChannel --> server0.in;
        server0.out --> IdealChannel --> client1.in;
        client1.out --> IdealChannel --> server1.in;
        server1.out --> IdealChannel --> client1.in;
        client2.out --> IdealChannel --> server0.in;
        server0.out --> IdealChannel --> client2.in;
        client2.out --> IdealChannel --> server1.in;
        server1.out --> IdealChannel --> client2.in;
        client3.out --> IdealChannel --> server0.in;
        server0.out --> IdealChannel --> client3.in;
        client3.out --> IdealChannel --> server1.in;
        server1.out --> IdealChannel --> client3.in;
        client3.out --> IdealChannel --> client1.in;
        client1.out --> IdealChannel --> client3.in;
        client1.out --> IdealChannel --> client0.in;
        client0.out --> IdealChannel --> client1.in;
        client0.out --> IdealChannel --> client2.in;
        client2.out --> IdealChannel --> client0.in;
        client2.out --> IdealChannel --> client3.in;
        client3.out --> IdealChannel --> client2.in;
        client0.out --> IdealChannel --> client3.in;
        client3.out --> IdealChannel --> client0.in;
        client1.out --> IdealChannel --> client2.in;
        client2.out --> IdealChannel --> client1.in;
}
