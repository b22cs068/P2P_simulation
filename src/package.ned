package a2;

@license(LGPL);
//package A2;


simple Client
{
    gates:
        input in;
        output out;
}
simple Server
{
    gates:
        input in;
        output out;
}
network DynamicNetwork {
    submodules:
        client0: Client;
        client1: Client;
        client2: Client;
        server0: Server;
        server1: Server;
        server2: Server;
        server3: Server;
    connections:
        client0.out --> server0.in;
        client0.out --> server1.in;
        client1.out --> server1.in;
        client1.out --> server2.in;
        client2.out --> server2.in;
        client2.out --> server3.in;
}
