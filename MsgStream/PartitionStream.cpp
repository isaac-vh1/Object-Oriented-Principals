//Author: Isaac Van Horn
//Date: 10/15/24
#include<iostream>
#include<string>
#include"DurableStream.h"
#include"PartitionStream.h"
#include"MsgStream.h"

using namespace std;

void PartitionStream::isFull() {
    if (partitionCount >= maxCapacity)
        throw invalid_argument("Partition stream is full RESET a partition to add new key");
}
void PartitionStream::copy(const PartitionStream& src) {
    partitionCount = src.partitionCount;
    maxCapacity = src.maxCapacity;
    messages = make_shared<shared_ptr<MsgStream>[]>(maxCapacity);
    for (int i = 0; i < partitionCount; i++) {
        messages[i] = make_shared<MsgStream>(*src.messages[i]);
    }
}

PartitionStream::PartitionStream(int capacity) {
    partitionCount = 0;
    maxCapacity = capacity;
    messages = make_shared<shared_ptr<MsgStream>[]>(maxCapacity);
}

PartitionStream::PartitionStream(int capacity, shared_ptr<shared_ptr<MsgStream>[]> newMessages) {
    maxCapacity = capacity;
    messages = make_shared<shared_ptr<MsgStream>[]>(maxCapacity);
    partitionCount = 0;
    for(int i = 0; i < capacity; i++){
        if(newMessages[i] != nullptr) {
            messages[i] = newMessages[i];
            partitionCount++;
        } else {
            messages[i] = make_shared<MsgStream>(DefaultCapacity);
            partitionCount++;
        }
    }
}

PartitionStream::PartitionStream(const PartitionStream& src) {
    copy(src);
}
PartitionStream& PartitionStream::operator=(const PartitionStream& src) {
    if(this == &src) return *this;
    copy(src);
    return *this;
}
PartitionStream::PartitionStream(PartitionStream&& src) {
    partitionCount = src.partitionCount;
    maxCapacity = src.maxCapacity;
    messages = src.messages;
    src.partitionCount = 0;
    src.maxCapacity = 0;
    src.messages = nullptr;
}

PartitionStream& PartitionStream::operator=(PartitionStream&& src) {
    if(this == &src) return*this;
    swap(partitionCount, src.partitionCount);
    swap(maxCapacity, src.maxCapacity);
    swap(messages, src.messages);
    return *this;
}

PartitionStream PartitionStream::operator+(PartitionStream& src) {
    int newMaxCapacity = maxCapacity + src.partitionCount;
    shared_ptr<shared_ptr<MsgStream>[]> newMessages = make_shared<shared_ptr<MsgStream>[]>(newMaxCapacity);
    for (int i = 0; i < src.partitionCount; i++) {
        newMessages[i] = make_shared<MsgStream>(*src.messages[i]);
    }
    for (int i = 0; i < partitionCount; i++) {
        newMessages[i + src.partitionCount] = make_shared<MsgStream>(*messages[i]);
    }
    return PartitionStream(newMaxCapacity, newMessages);
}

shared_ptr<MsgStream> PartitionStream::operator[](int key) {
    if (key < 0 || key >= maxCapacity)
        throw out_of_range("Partition index out of range");
    if (messages[key] == nullptr)
        throw out_of_range("Partition message is not initialized");
    return messages[key];
}

void PartitionStream::operator-() {
    reset();
}

bool PartitionStream::operator==(PartitionStream& src) {
    if (partitionCount != src.partitionCount) {
        return false;
    }
    for (int i = 0; i < partitionCount; i++) {
        if(messages[i] != src.messages[i]) {
            return false;
        }
    }
    return true;
}
bool PartitionStream::operator!=(PartitionStream& src) {
    if (partitionCount != src.partitionCount) {
        return true;
    }
    for (int i = 0; i < partitionCount; i++) {
        if(messages[i] != src.messages[i]) {
            return true;
        }
    }
    return false;
}

int PartitionStream::add(int keyAdd, string messageAdd) {
    if(keyAdd < 0 || keyAdd >= partitionCount) {
        isFull();
        keyAdd = partitionCount;
        partitionCount++;
    }
    if (messages[keyAdd] == nullptr) {
        messages[keyAdd] = make_shared<MsgStream>(DefaultCapacity);
    }
    messages[keyAdd] -> Append(messageAdd);
    return keyAdd;
}

unique_ptr<string[]> PartitionStream::read(int key, int start, int count) {
    if (key == -1 || messages[key] == nullptr)
        throw invalid_argument("Partition key not found");
    return messages[key] -> ReadRange(start, count);
}

void PartitionStream::reset(int key) {
    if (key < 0 || key >= partitionCount)
        throw invalid_argument("Partition key not found");
    messages[key] -> Reset();
}

void PartitionStream::reset() {
    for(int i = 0; i < partitionCount; i++) {
        messages[i] -> Reset();
    }
}
