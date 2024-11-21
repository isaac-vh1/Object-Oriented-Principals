//Author: Isaac Van Horn
//Date: 10/15/24
#ifndef PARTITIONSTREAM_H
#define PARTITIONSTREAM_H
#include<string>
#include"DurableStream.h"
#include"MsgStream.h"

using namespace std;
//Client Invariant: The client must handle thrown errors and the memory ownership
//of the string array is transfered in the read function
//max capacity is decided by the client when MsgStrean Objects are passed to the constructor
class PartitionStream {
private:
    shared_ptr<shared_ptr<MsgStream>[]> messages;
    int maxCapacity;
    int partitionCount;
    const int DefaultCapacity = 5;
    void isFull();
    //Precondition: Src must be a valid PartitionStream object
    //Postcondiditon: Src data is copied to this Object
    void copy(const PartitionStream& src);
    PartitionStream(const PartitionStream& src);
    PartitionStream& operator=(const PartitionStream& src);
public:
    PartitionStream(int capacity);
    PartitionStream(int capacity, shared_ptr<shared_ptr<MsgStream>[]> newMessages);
    PartitionStream(PartitionStream&& src);
    PartitionStream& operator=(PartitionStream&& src);
    PartitionStream operator+(PartitionStream& src);
    bool operator==(PartitionStream& src);
    bool operator!=(PartitionStream& src);
    shared_ptr<MsgStream> operator[](int key);
    void operator-();

    //Precondition: key add is valid and must not exceed the max string length defined in DurableStream
    //Postcondition: DurableStream Object can be created and added to messages or a new message
    //is added to existing Object
    int add(int keyAdd, std::string messageAdd);
    //Precondition: key , start, and count must be valid and not out of range
    //Postcondition: Client must take ownership of string* memory and handle appropriately
    unique_ptr<string[]> read(int key, int start, int count);
    //pre: key must be valid
    //post: resets the DurableStream object at key
    void reset(int key);
    //post: resets all DurableStream Objects in messages
    void reset();
};
//Implementation invariant: Using c++ this class implements move and copy semantics.
//Using pointers to arrays for keys and messages and ints for the rest of the variables
//The dynamically allocated memory is properly handled in the destructor
#endif