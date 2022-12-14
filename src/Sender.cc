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

#include "Sender.h"
#include "MyMassage_m.h"
#include <fstream>
Define_Module(Sender);


void Sender::initialize()
{
    //counter when time out is happend he is looping in retransmit frames
    counter=0;
    //intialize variables
    WS=getParentModule()->par("WS").intValue();
    ack=1;
    frame_next=0;
    nbuffered=0;
    //read input file
    std::ifstream inFile;
    inFile.open("C:\\omnetpp-5.6.2\\samples\\MiniProject\\input1.txt");
    if (!inFile) {
        EV << "Unable to open file datafile.txt";
    }
    std::string x;

    while(std::getline(inFile, x)){
        EV<<x<<endl;
    s.push_back(x);
    }
    inFile.close();










}
bool Sender::printTXT(std::string o){
    //write output
    std:: ofstream outFile("C:\\omnetpp-5.6.2\\samples\\MiniProject\\output.txt",  std::ios::in | std::ios::out | std::ios::ate);

          if (outFile.is_open()) {
              outFile<<o<<"\n";
              outFile.close();
              return true;
          }
          EV<<"error out"<<endl;
          return false;
}
//function for check error and transmit frame
double Sender::sendError(std::string error,std::string payload,std::bitset<8>trailer){
    //variables for output file
    std::string mod="-1";
    std::string lost="No";
    std::string delayPrint="0";
    std::string dupPrint="0";

    double delay=par("TD").doubleValue()+par("PT").doubleValue();

//modification error check first bit
     if(error[0]=='1'){
        mod="1";
        std::bitset<8> r(payload[1]);
        std::bitset<8> e(1);
        r=e^r;

        std::string q(1,(char)r.to_ulong());
        payload=payload[0]+q+payload.substr(2, payload.size());
        //EV<<" payload mod="<<payload;
    }
    //new message to transmit to rec
    MyMassage_Base *mmsg=new MyMassage_Base ("rrrrrrrrrr");
    mmsg->setB(trailer);
    mmsg->setSeq_Num(frame_next);    // modify
    mmsg->setM_Payload(payload.c_str());
    //delay error check
    if(error[3]=='1'){
        delayPrint=std::to_string(par("ED").doubleValue());
        delay+=par("ED").doubleValue();
    }
    //start timer for this message
    MyMassage_Base *mmsgg=mmsg->dup();
    mmsgg->setName("TimeOut");

    timeOut.push_back(mmsgg);
    scheduleAt(simTime()+par("TO").doubleValue()+par("PT").doubleValue(),mmsgg);
    //loss error check
    if(error[1]=='1'){
        lost="Yes";

        delay-=par("TD").doubleValue();
        printTXT("At time["+std::to_string(simTime().dbl()+par("PT").doubleValue())+"],Node["+node+"][sent] frame with seq_num=["+std::to_string(frame_next)+"] and payload=["+payload.c_str()+"] and trailer=["+trailer.to_string()+"] , Modified ["+mod+" ] , Lost [Yes], Duplicate ["+dupPrint+"], Delay["+delayPrint+"]");

        //EV<<"delay="<<delay;
           return delay;
    }
    //send first message with process delay + tansmation delay
    sendDelayed(mmsg,delay,"out");
    //duplicatation error check
    if(error[2]=='1'){
        dupPrint="1";

        delay+=par("DD").doubleValue();

        MyMassage_Base *mmsgDup=mmsg->dup();

        sendDelayed(mmsgDup,delay,"out");
        printTXT("At time["+std::to_string(simTime().dbl()+par("PT").doubleValue())+"],Node["+node+"][sent] frame with seq_num=["+std::to_string(frame_next)+"] and payload=["+payload.c_str()+"] and trailer=["+trailer.to_string()+"] , Modified ["+mod+" ] , Lost [No], Duplicate ["+dupPrint+"], Delay["+delayPrint+"]");
        dupPrint="2";
    }
    //add to output file
    if (dupPrint=="2")
        printTXT("At time["+std::to_string(simTime().dbl()+par("PT").doubleValue()+par("DD").doubleValue())+"],Node[0][sent] frame with seq_num=["+std::to_string(frame_next)+"] and payload=["+payload.c_str()+"] and trailer=["+trailer.to_string()+"] , Modified ["+mod+" ] , Lost [No], Duplicate ["+dupPrint+"], Delay["+delayPrint+"]");

    else
    printTXT("At time["+std::to_string(simTime().dbl()+par("PT").doubleValue())+"],Node["+node+"][sent] frame with seq_num=["+std::to_string(frame_next)+"] and payload=["+payload.c_str()+"] and trailer=["+trailer.to_string()+"] , Modified ["+mod+" ] , Lost [No], Duplicate ["+dupPrint+"], Delay["+delayPrint+"]");
    //inc frame next
    frame_next++;
    if(frame_next==WS){
        frame_next=0;
    }
    //EV<<"delay="<<delay;
    return delay;
}

void Sender::handleMessage(cMessage *msg)
{
    //check if message from my self (time out or send message)  or from rec or coordinator
    if(msg->isSelfMessage()){
        //EV<<"name="<<msg->getName();
        std::string y=msg->getName();
        //Time out happend
        if(y=="TimeOut" ){

            MyMassage_Base *msgt=check_and_cast<MyMassage_Base*>(msg);

            EV<<"TimeOut in frame = "<<msgt->getSeq_Num()<<" data="<<msgt->getM_Payload();
            //check if send all frames get out
            if(counter==wait.size()){
                EV<<"endTransmition";
                for(int i=0;i<timeOut.size();i++){
                    EV<<"timeout["<<i<<"]="<<timeOut[i]->getArrivalTime();
                }
                counter=0;
                cancelEvent(msg);
                cMessage* msgg=new cMessage("asdfgas",0);
                scheduleAt(simTime()+par("PT").doubleValue(),msgg);
                return;
            }

            // if first time time out happend
            if(counter==0){
                printTXT("Time out event at time ["+std::to_string(simTime().dbl())+"], at Node["+node+"] for frame with seq_num=["+std::to_string(msgt->getSeq_Num())+"]");
                EV<<" fk inside ";
                //pause all timers
                while(timeOut.size()>0){
                    timeOut.erase(timeOut.begin());

                    cMessage*m=timeOut[0];

                    cancelEvent(m);

                }

                timeOut.clear();
                //change frame next
                if (ack==0){
                    frame_next=WS-1;
                }
                else
                    frame_next=ack-1;
            }
            //retransmit
            EV<<"size timeOut ="<<timeOut.size();
            std::string d=wait[counter];     //read order message
            std::string payload="$"+d.substr(5, d.size()-5)+"$";
            std::string error=d.substr(0, 4);
            std::bitset<8> trailer(0);
            //get parity and add escape chars
              for(int i=1;i<payload.size()-1;i++){

                  if(payload[i]=='$' or payload[i]=='/'){
                      payload=payload.substr(0, i)+'/'+payload.substr(i, payload.size()-i);
                      EV<<"add"<<payload[i]<<endl;
                      std::bitset<8> r(payload[i]);
                      trailer=trailer ^r;
                      i++;
                  }
                  //EV<<"normalcase"<<payload[i]<<endl;
                  std::bitset<8> r(payload[i]);
                  trailer=trailer ^r;
              }
              //if first frame send make error 0000
              if(counter==0)
                  error="0000";

              EV<<" retransmit payload="<<payload <<" frame "<<frame_next<<" error="<<error;
              //sended
              sendError( error, payload,trailer);
              counter++;
              scheduleAt(simTime()+par("PT").doubleValue(),msg);


        }
        else{
    // TODO - Generated method body
            //normal case  check if send all in input file or not
               EV<<" size="<<s.size();
                if(s.size()>0){

                    //get first frame and get parity and error and send it
                    std::string d=*s.begin();     //read order message
                    std::string payload="$"+d.substr(5, d.size()-5)+"$";
                    std::string error=d.substr(0, 4);
                    std::bitset<8> trailer(0);
                    double e=simTime().dbl();
                    std::string out="At["+std::to_string(e)+"], Node ["+node+"] , Introducing channel error with code =["+error+"]"+"\n";
                    printTXT(out);
                       for(int i=1;i<payload.size()-1;i++){

                           if(payload[i]=='$' or payload[i]=='/'){
                               payload=payload.substr(0, i)+'/'+payload.substr(i, payload.size()-i);
                               EV<<"add"<<payload[i]<<endl;
                               std::bitset<8> r(payload[i]);
                               trailer=trailer ^r;
                               i++;
                           }
                           EV<<"normalcase"<<payload[i]<<endl;
                           std::bitset<8> r(payload[i]);
                           trailer=trailer ^r;
                       }

                    EV<<" payload="<<payload<<" error="<<error<<"  parity="<<trailer << "frame="<<frame_next;
                    sendError( error, payload,trailer);
                    //inc buffer 1
                    nbuffered++;

                    wait.push_back(*s.begin());
                    s.erase(s.begin());
                    EV<<" nbuffered="<<nbuffered;
                    //check if buffer has empty size, send another frame
                    if(nbuffered<WS){

                        cancelEvent(msg);
                        cMessage *msgg=new cMessage("dfghdfh");
                        scheduleAt(simTime()+par("PT").doubleValue(),msgg);
                    }

             }

            else{

                EV<<"end";
                cancelAndDelete(msg);
            }
        }
    }
    else{
        //from coordinator
        std::string y1=msg->getName();
        if(y1=="0" or y1=="1"){
            node=msg->getName();
            msg->setName("sedfsdg");
            scheduleAt(simTime(),msg);
            return;
        }
        MyMassage_Base *msgt=check_and_cast<MyMassage_Base*>(msg);
        //from rec check ackExp
        if(msgt->getACK()==ack and msgt->getFrametype()==1){
            //inc ackExp
            ack++;
            if(ack==WS )
                ack=0;
            EV<<"got ack for sender"<<msgt->getACK();
            //if buffer is full and came ack from rec we can send now another frame
            if(nbuffered==WS){
                scheduleAt(simTime(),msg);
            }
            nbuffered--;
            //stop timer
            wait.erase(wait.begin());
            cMessage*m=timeOut[0];

            cancelEvent(m);
            timeOut.erase(timeOut.begin());

        }
        else{
            //if ack and not exp then its accmulative
            if ( msgt->getFrametype()==1){
                EV<<"got ack for"<<msgt->getACK()<<" now ack expect="<<ack;
                int temp=msgt->getACK();
                if(nbuffered==WS){
                    scheduleAt(simTime(),msg);
                }
                while(temp!=ack){
                    std::string out1="sender recived ack="+std::to_string(ack)+"ACK Lossed";
                    printTXT(out1);
                    nbuffered--;
                    wait.erase(wait.begin());
                    cMessage*m=timeOut[0];

                    cancelEvent(m);
                    timeOut.erase(timeOut.begin());
                   ack++;
                   if(ack==WS )
                          ack=0;
                }
                nbuffered--;
                wait.erase(wait.begin());
                cMessage*m=timeOut[0];

                cancelEvent(m);
                timeOut.erase(timeOut.begin());
               ack++;
               if(ack==WS )
                      ack=0;

            }
            else
                //if not ack then Nack
                EV<<"wrong ack"<<msgt->getACK()<<" need="<<ack;
        }
    }

}
