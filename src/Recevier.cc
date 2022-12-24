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

#include "Recevier.h"
#include "MyMassage_m.h"
#include <fstream>
Define_Module(Recevier);

void Recevier::initialize()
{
    //inatialize varibles
    frame_exp=0;
    ack=0;
    WS=getParentModule()->par("WS").intValue();
}
//function to write output file
bool Recevier::printTXT(std::string o){

    std::ofstream outFile("C:\\omnetpp-5.6.2\\samples\\MiniProject\\output.txt",  std::ios::in | std::ios::out | std::ios::ate);

          if (outFile.is_open()) {
              outFile<<o<<"\n";
              outFile.close();
              return true;
          }
          EV<<"error out"<<endl;
          return false;
}
void Recevier::handleMessage(cMessage *msg)
{
    //from coordanitor
    std::string y1=msg->getName();
    if(y1=="0" or y1=="1"){
            int r=1-std::stoi(msg->getName());
             node=std::to_string(r);
             cancelEvent(msg);
             return;
         }
    std::string a="ACK";
    // TODO - Generated method body
    MyMassage_Base *mmsg=check_and_cast<MyMassage_Base*>(msg);
    EV<<" got frame="<<mmsg->getSeq_Num()<<" data="<<mmsg->getM_Payload()<<" frameExp="<<frame_exp;
    //check if frame is frame exp
    if(mmsg->getSeq_Num()==frame_exp){
        EV<<"got frame="<<frame_exp;
        //check parity byte
        std::string g=mmsg->getM_Payload();
        if(g[0]=='$' and g[g.size()-1]=='$'){

            std::bitset<8> b=mmsg->getB();
            std::bitset<8> total(0);
            for(int i=1;i<g.size()-1;i++){

                std::bitset<8> r(g[i]);
                total=total ^r;
            }
            EV<<" total="<<total;
            if(total==b){
                //correct then send ack
                EV<<" correct";
                frame_exp+=1;
                if(frame_exp==WS){
                    frame_exp=0;
                }
                ack+=1;   //window size
                if(ack==WS)
                      ack=0;
                mmsg->setFrametype(1);
                mmsg->setACK(ack);

            }
            //not same partiy send Nack
            else{
                a="NACK";
                //notcorrect
                mmsg->setFrametype(2);
                mmsg->setACK(ack);
            }
        }
        ////not same flags(first char and last char not $) send Nack
        else{
            a="NACK";
            mmsg->setFrametype(2);
            mmsg->setACK(ack);

        }
    }
    //not frame exp send Nack
    else{
        a="NACK";
        mmsg->setFrametype(2);
        mmsg->setACK(ack);
//        a="ACK";
//        mmsg->setFrametype(1);
//        mmsg->setACK(mmsg->getSeq_Num()+1);

    }
    //probability to send ack or Nack
    double ww=simTime().dbl()+par("PT").doubleValue();
    //EV<<"send time node 1="<<ww<<endl;

    int rand=uniform(0,1)*10;
    EV<<"prob="<<rand<<endl;
    if(rand>=getParentModule()->par("prob").intValue()){
      EV<<"got ack for"<<mmsg->getACK()<<" payload="<<mmsg->getM_Payload();
      sendDelayed(mmsg,par("TD").doubleValue()+par("PT").doubleValue(),"out");
      std::string out1="At time["+std::to_string(ww)+" ], Node["+node+"] Sending ["+a+"] with number ["+std::to_string(ack)+"] , loss [No]";
      printTXT(out1);
    }
    else{
        //miss send ack
        EV<<" miss send ack";
        std::string out1="At time["+std::to_string(ww)+" ], Node["+node+"] Sending ["+a+"] with number ["+std::to_string(ack)+"] , loss [No], ACK Loss";
            printTXT(out1);
    }



}
