// Author: Isaac Van Horn
// Date: 10/15/24
#include <iostream>
#include <memory>
#include <string>
#include "MsgStream.h"
#include "DurableStream.h"
#include "PartitionStream.h"

using namespace std;

void testMsgStream() {
    MsgStream msgStream1(5);
    MsgStream msgStream2(4);
    MsgStream msgStream3(5);
    msgStream1.Append("Message 1");
    msgStream1.Append("Message 2");
    msgStream3.Append("Message 1");
    msgStream3.Append("Message 2");

    cout << "Subscript operator \"Message 1\": " << msgStream1[0] << endl;
    cout << "Comparison operator (Expected Equal): " << (msgStream1 == msgStream3 ? "Equal" : "Not Equal") << endl;
    cout << "Comparison operator (Expected Not Equal): " << (msgStream1 == msgStream2 ? "Equal" : "Not Equal") << endl;
    cout << "Comparison operator (Expected Not Equal): " << (msgStream1 == msgStream3 ? "Equal" : "Not Equal") << endl;
    cout << "Negation operator (Expected True): " << (msgStream1 != msgStream2 ? "True" : "False") << endl;
    cout << "Negation operator (Expected False): " << (msgStream1 != msgStream1 ? "True" : "False") << endl;
    msgStream2 = msgStream1 + msgStream3;
    cout << "Addition Expected Resuls:" << endl;
    cout << "Message 1" << endl;
    cout << "Message 2" << endl;
    cout << "Message 1" << endl;
    cout << "Message 2" << endl;
    cout << "Auctual Results:" << endl;
   shared_ptr<string[]> results  = msgStream2.ReadRange(0,4);
    cout << results[0] << endl;
    cout << results[1] << endl;
    cout << results[2] << endl;
    cout << results[3] << endl;
    -msgStream1;
    try {
        msgStream1[0];
    } catch (exception &e) {
        cout << "Exception Reset good" << endl;
    }
}

void testDurableStream() {
    DurableStream durableStream(5, "test.txt");
    durableStream.Append("Durable 1");
    durableStream.Append("Durable 2");

    cout << "DurableStream content: " << durableStream[0] << ", " << durableStream[1] << endl;

    durableStream.Reset();
    try {
        durableStream[0];
    } catch (invalid_argument& e) {
        cout << "DurableStream Reset Good" << endl;
    }
}

void testPartitionStream() {
    shared_ptr<MsgStream> msg1 = make_shared<MsgStream>(5);
    msg1 ->Append("Partition 1 ");
    msg1->Append("Partition 2 ");
    shared_ptr<shared_ptr<MsgStream>[]> array1 = make_shared<shared_ptr<MsgStream>[]>(5);
    array1[0] = msg1;
    shared_ptr<shared_ptr<MsgStream>[]> array2 = make_shared<shared_ptr<MsgStream>[]>(5);
    array2[0] = msg1;
    PartitionStream p1(3, array1);
    PartitionStream p2(5, array1);
    PartitionStream p3(3, array2);
    p1.add(0, "Partition 3");

    cout << "Subscript operator \"Partition 1\": " << p1[0] -> ReadRange(0,1) << endl;
    cout << "Comparison operator (Expected Equal): " << (p1 == p1 ? "Equal" : "Not Equal") << endl;
    cout << "Comparison operator (Expected Not Equal): " << (p1 == p2 ? "Equal" : "Not Equal") << endl;
    cout << "Negation operator (Expected True): " << (p1 != p2 ? "True" : "False") << endl;
    cout << "Negation operator (Expected True): " << (p1 != p2 ? "True" : "False") << endl;
    p2 = p1 + p3;
    cout << "Addition Expected Resuls:" << endl;
    cout << "Partition 1" << endl;
    cout << "Partition 2" << endl;
    cout << "Partition 3" << endl;
    cout << "Auctual Results:" << endl;
    shared_ptr<string[]> results  = p2.read(3,0,3);
    cout << results[0] << endl;
    cout << results[1] << endl;
    cout << results[2] << endl;
    -p1;
    try {
        p1[0] -> ReadRange(0,1);
    } catch (const exception& e) {
        cout << "PartitionStream Reset Good " << endl;
    }
}

int main() {
    cout << "Testing MsgStream:" << endl;
    testMsgStream();

    cout << endl << "Testing DurableStream:" << endl;
    testDurableStream();

    cout <<  endl << "Testing PartitionStream:" << endl;
    testPartitionStream();
    return 0;
}