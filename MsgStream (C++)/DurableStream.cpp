//Isaac Van Horn
//Date: 10/27/24
#include<iostream>
#include"MsgStream.h"
#include"DurableStream.h"
#include<string>
#include<fstream>

using namespace std;

void DurableStream::InitializeFile() {
    string line;
    file.seekp(0, ios::beg);
    while (getline(file, line)) {
        if(line != "\n")
            MsgStream::Append(line);
    }
    file.flush();
}

DurableStream::DurableStream(int capacity, string newFilePath) : MsgStream(capacity) {
    filePath = newFilePath;
    InitializeFile();

    ifstream testFile(filePath);
    if (testFile.good())
    {
        file.open(filePath, ios::in | ios::out| ios::binary);
        InitializeFile();
        fileOrigin = file.tellp();
    }
    else
    {
        file.open(filePath, ios::in | ios::out | ios::binary | ios::trunc);
        fileOrigin = 0;
    }
}

DurableStream::DurableStream(const DurableStream& src) : MsgStream(src) {
    DeepCopy(src);
}

DurableStream& DurableStream::operator=(const DurableStream& src) {
    if (this == &src) return *this;
    DeepCopy(src);
    return *this;
}

DurableStream::DurableStream(DurableStream&& src) : MsgStream(std::move(src)) {
    filePath = src.filePath;
    file.open(filePath, ios::in | ios::out | ios::binary);
    fileOrigin = src.fileOrigin;
    src.file.close();
    src.fileOrigin = 0;
}

DurableStream& DurableStream::operator=(DurableStream&& src)  {
    if (this == &src) return *this;
    MsgStream::operator=(std::move(src));
    swap(src.filePath, filePath);
    swap(src.file, file);
    swap(src.fileOrigin, fileOrigin);
    return *this;
}

DurableStream::~DurableStream() {
    file.flush();
    file.close();
}

void DurableStream::DeepCopy(const DurableStream& src) {
    MsgStream::operator=(src);
    fileOrigin = src.fileOrigin;
    filePath = src.filePath;
    file.open(filePath, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file during deep copy.");
    }
}

void DurableStream::Append(string message) {
    if (!file.is_open()) throw runtime_error("File is null");
    file.seekp(0, ios::end);
    file << message << "\n";
    MsgStream::Append(message);
}

void DurableStream::Reset() {
    file.flush();
    file.seekp(fileOrigin);
    MsgStream::Reset();
    InitializeFile();
}