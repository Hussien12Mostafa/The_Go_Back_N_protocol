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

#ifndef __MINIPROJECT_SENDER_H_
#define __MINIPROJECT_SENDER_H_
#include<bitset>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Sender : public cSimpleModule
{
    std::string node;
    std:: vector<std::string>s;
    std:: vector<std::string>wait;
    std:: vector<cMessage*>timeOut;
    int counter;
    std:: vector<std::string>outputFile;
    //cMessage*m[10];
    int frame_next;
    int ack;
    int nbuffered;
    int WS;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    bool printTXT(std::string o);

    double sendError(std::string error,std::string payload,std::bitset<8>trailer);
};

#endif
