package miniproject;

@license(LGPL);
network Network
{
    parameters:
        int WS = default(5);
        int prob = default(0);

        @display("bgb=609,344");
    submodules:
        sender: Sender {
            @display("p=71,122");


        }
        rec: Recevier {
            @display("p=464,122");


        }
        coordinator: Coordinator {
            @display("p=267,205");
        }
    connections:
        sender.out --> rec.in;
        rec.out --> sender.in;
        coordinator.out --> rec.in1;
        coordinator.out1 --> sender.in1;

}
