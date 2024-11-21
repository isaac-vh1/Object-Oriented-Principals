//Isaac Van Horn
//Date: 10/27/24
#ifndef DURABLESTREAM_H
#define DURABLESTREAM_H
#include"MsgStream.h"
#include<string>
#include<fstream>

//Class invarriant: this class assumes that client handles errors thrown by class
//capacity and backing file decided by the client by passing into constructor
class DurableStream : public MsgStream
{
    private:
    std::string filePath;
    std::fstream file;
    int fileOrigin;

    //Post: file contents appended to the message array in msgstream class
    void InitializeFile();
    //Pre: src must be a valid durable stream object
    //post: the src data is copied to the current msg stream object
    void DeepCopy(const DurableStream& src);
    DurableStream(const DurableStream& src);
    DurableStream& operator=(const DurableStream& src);
    public:
    DurableStream(int capacity, std::string filePath);
    DurableStream(DurableStream&& src);
    DurableStream& operator=(DurableStream&& src);
    ~DurableStream();
//pre: message must be of valid string type and not exceed max length
//post: message appended to internal array
    void Append(std::string message) override;
// post: resets the stream back to the initial values in the file that was passed in originally
    void Reset() override;
// post disposes the file and sets disposed to true so no other functions try to access the file
    //Implementation invarriant:
    //Using c# only deep copy changes ownership of the current stream
    //Message stream class is inherited by this class as the parent
    // reset and append are overrided in the class however they are called in the functions
};
#endif

