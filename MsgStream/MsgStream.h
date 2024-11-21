//Author: Isaac Van Horn
//Date: 10/15/24
#ifndef MSGSTREAM_H
#define MSGSTREAM_H
#include <string>
// Class Invariant: This class assumes that client handles thrown errors by code.
// The capacity is decided when passing into constructor, max operations is double the capacity
//Max String length is decided in the class declaration
class MsgStream {
protected:
    const int MaxStringLength = 100;
    int maxCapacity;
    int maxOperations;
    int messageCount;
    std::unique_ptr<std::string[]> messages;
    int operationsCount;
    void IsFull();
    void IsOperationLimit();
    void copy(const MsgStream& src);
public:
    MsgStream(int);
    MsgStream(const MsgStream& src);
    MsgStream& operator=(const MsgStream& src);
    MsgStream(MsgStream&& src);
    MsgStream& operator=(MsgStream&& src);
    MsgStream operator+(const MsgStream& src);
    void operator-();
    bool operator==(MsgStream& src);
    bool operator!=(MsgStream& src);
    std::string operator[](int key);
    virtual ~MsgStream() = default;
    // Precondition: Message passed to function must not exceed the Max String Length
    // Postcondition: Message added to the stream, operation and message count increases 1
    virtual void Append(std::string message);
    // Precondition: Start and count passed and must be within the range of the stream
    // Postcondition: Operation and message count increases 1, Client must assume responsibility
    //of the string array
    std::unique_ptr<std::string[]> ReadRange(int start, int count);
    // Postcondition: Array is cleared, operation and message count reset to 0;
    virtual void Reset();
};
//Implementation Invariant: Using C++ this class implements copy and move semantics
//The messages are stored in a pointer to an array and ints are used to track operations and messages as well
//as the maximum for each.
#endif
