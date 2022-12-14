//
// Generated file, do not edit! Created by nedtool 5.6 from MyMassage.msg.
//

#ifndef __MYMASSAGE_M_H
#define __MYMASSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include <bitset>
typedef std::bitset<8> bits;
// }}

/**
 * Class generated from <tt>MyMassage.msg:24</tt> by nedtool.
 * <pre>
 * packet MyMassage
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 * 
 *     int Seq_Num;
 *     int M_Type;
 *     string M_Payload;
 *     bits b;
 *     int Frametype;
 *     int ACK;
 * }
 * </pre>
 *
 * MyMassage_Base is only useful if it gets subclassed, and MyMassage is derived from it.
 * The minimum code to be written for MyMassage is the following:
 *
 * <pre>
 * class MyMassage : public MyMassage_Base
 * {
 *   private:
 *     void copy(const MyMassage& other) { ... }

 *   public:
 *     MyMassage(const char *name=nullptr, short kind=0) : MyMassage_Base(name,kind) {}
 *     MyMassage(const MyMassage& other) : MyMassage_Base(other) {copy(other);}
 *     MyMassage& operator=(const MyMassage& other) {if (this==&other) return *this; MyMassage_Base::operator=(other); copy(other); return *this;}
 *     virtual MyMassage *dup() const override {return new MyMassage(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from MyMassage_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(MyMassage)
 * </pre>
 */
class MyMassage_Base : public ::omnetpp::cPacket
{
  protected:
    int Seq_Num;
    int M_Type;
    ::omnetpp::opp_string M_Payload;
    bits b;
    int Frametype;
    int ACK;

  private:
    void copy(const MyMassage_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MyMassage_Base&);
    // make constructors protected to avoid instantiation

    MyMassage_Base(const MyMassage_Base& other);
    // make assignment operator protected to force the user override it
    MyMassage_Base& operator=(const MyMassage_Base& other);

  public:
    MyMassage_Base(const char *name=nullptr, short kind=0);
    virtual ~MyMassage_Base();
    virtual MyMassage_Base *dup() const override {return new MyMassage_Base(*this) ;}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSeq_Num() const;
    virtual void setSeq_Num(int Seq_Num);
    virtual int getM_Type() const;
    virtual void setM_Type(int M_Type);
    virtual const char * getM_Payload() const;
    virtual void setM_Payload(const char * M_Payload);
    virtual bits& getB();
    virtual const bits& getB() const {return const_cast<MyMassage_Base*>(this)->getB();}
    virtual void setB(const bits& b);
    virtual int getFrametype() const;
    virtual void setFrametype(int Frametype);
    virtual int getACK() const;
    virtual void setACK(int ACK);
};


#endif // ifndef __MYMASSAGE_M_H
