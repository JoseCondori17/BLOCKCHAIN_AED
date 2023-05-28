#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
using namespace std;
class Transaction {
private:
    string sender;
    string receiver;
    double amount;

public:
    Transaction(string sender, string receiver, double amount) : sender(sender), receiver(receiver), amount(amount) {}

    string getSender() {
        return sender;
    }

    string getReceiver() {
        return receiver;
    }

    double getAmount() {
        return amount;
    }
};
#endif //BLOCKCHAIN_AED_BLOCK_H
