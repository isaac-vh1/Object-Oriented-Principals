//Author: Isaac Van Horn
//Date: 10/15/24
#include <iostream>
#include <stdexcept>
#include <string>
#include "MsgStream.h"
using namespace std;

void MsgStream::IsFull() {
    if (messageCount >= maxCapacity)
        throw invalid_argument("Message stream is full RESET to add new message");
}

void MsgStream::IsOperationLimit() {
    if (operationsCount >= maxOperations)
        throw invalid_argument("Operation limit exceeded RESET to continue operations");
}

void MsgStream::copy(const MsgStream& src) {
    maxCapacity = src.maxCapacity;
    maxOperations = src.maxOperations;
    messageCount = src.messageCount;
    operationsCount = src.operationsCount;
    messages =  make_unique<string[]>(maxCapacity);
    for(int i = 0; i < messageCount; i++){
        messages[i] = src.messages[i];
    }
}

MsgStream::MsgStream(int capacity) {
    if (capacity <= 0)
        throw invalid_argument("Capacity must be positive");
    maxCapacity = capacity;
    maxOperations = capacity * 2;
    messages = make_unique<string[]>(maxCapacity);
    operationsCount = 0;
    messageCount = 0;
}

MsgStream::MsgStream(const MsgStream& src) {
    copy(src);
}

MsgStream& MsgStream::operator=(const MsgStream& src) {
    if(this == &src) return*this;
    copy(src);
    return *this;
}

MsgStream::MsgStream(MsgStream&& src) {
    maxCapacity = src.maxCapacity;
    maxOperations = src.maxOperations;
    messageCount = src.messageCount;
    operationsCount = src.operationsCount;
    messages = move(src.messages);
    src.maxCapacity = 0;
    src.maxOperations = 0;
    src.messageCount = 0;
    src.operationsCount = 0;
    src.messages = nullptr;
}

MsgStream& MsgStream::operator=(MsgStream&& src) {
    if(this == &src) return*this;
    swap(maxCapacity, src.maxCapacity);
    swap(maxOperations, src.maxOperations);
    swap(messageCount, src.messageCount);
    swap(operationsCount, src.operationsCount);
    swap(messages, src.messages);
    return *this;
}

MsgStream MsgStream::operator+(const MsgStream& src) {
    int totalCapacity = src.maxCapacity + maxCapacity;
   MsgStream ret = MsgStream(totalCapacity);

    for (int i = 0; i < messageCount; i++) {
        ret.Append(messages[i]);
    }

    for (int i = 0; i < src.messageCount; i++) {
        ret.Append(src.messages[i]);
    }
    return ret;
}

void MsgStream::operator-() {
    Reset();
}

bool MsgStream::operator==(MsgStream& src) {
    if (messageCount != src.messageCount) {
        return false;
    }
    for (int i = 0; i < messageCount; i++) {
        if(messages[i] != src.messages[i]) {
            return false;
        }
    }
    return true;
}

bool MsgStream::operator!=(MsgStream& src) {
    if (messageCount != src.messageCount) {
        return true;
    }
    for (int i = 0; i < messageCount; i++) {
        if(messages[i] != src.messages[i]) {
            return true;
        }
    }
    return false;
}

string MsgStream::operator[](int key) {
    if (key < 0 || key >= messageCount) {
        throw invalid_argument("Key out of range");
    }
    return messages[key];
}

void MsgStream::Append(string message) {
    IsFull();
    IsOperationLimit();
    if (message.length() > MaxStringLength)
        throw invalid_argument("Message cannot exceed " + to_string(MaxStringLength)
        + " characters");
    messages[messageCount] = message;
    messageCount++;
    operationsCount++;
}

unique_ptr<string[]> MsgStream::ReadRange(int start, int count) {
    if (start < 0 && start >= messageCount)
        throw invalid_argument("Start index out of range");
    if (count <= 0)
        throw invalid_argument("Count cannot be 0");
    if (start + count > messageCount)
        throw invalid_argument("Requested range out of stream");
    IsOperationLimit();
    unique_ptr<string[]> result = make_unique<string[]>(count);
    for(int i = 0; i < count; i++ ) {
        result[i] = messages[start + i];
    }
    operationsCount++;
    return result;
}

void MsgStream::Reset() {
    messages = make_unique<string[]>(maxCapacity);
    messageCount = 0;
    operationsCount = 0;
}