//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "coordinator.h"
#include <fstream>
Define_Module(Coordinator);

void Coordinator::initialize()
{
    //read file coordinator.txt
    std::ifstream inFile;
        inFile.open("C:\\omnetpp-5.6.2\\samples\\MiniProject\\coordinator.txt");
        if (!inFile) {
            EV << "Unable to open file datafile.txt";

        }
        //get node num of sender
        std::string x;

        std::getline(inFile, x);
        EV<<x<<endl;


        inFile.close();
        sender1=x[4];
        EV<<sender1<<endl;
        int i=16;
        //get start time
        while(x[i]-48>=0 and x[i]-48<=9){
            startTime=startTime+x[i];

            i++;
        }
        //send num node to sender and the another num for rec after delay
        EV<<startTime<<endl;
        int c=std::stoi(startTime);
        cMessage *msg=new cMessage(sender1.c_str());
        sendDelayed(msg,c,"out1");
        cMessage *msg2=msg->dup();
        sendDelayed(msg2,c,"out");
    // TODO - Generated method body
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
