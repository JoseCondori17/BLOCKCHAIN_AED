#ifndef BLOCKCHAIN_AED_TRANSACTION_H
#define BLOCKCHAIN_AED_TRANSACTION_H
#include <iostream>
using namespace std;

class Transaction {
private:
    string sender;
    string receiver;
    double amount;
public:
    Transaction() = default;
    Transaction(const string& sender, const string& receiver, double amount = 0){
        this->sender = sender;
        this->receiver = receiver;
        this->amount = amount;
    }
    Transaction(const Transaction& other){
        this->sender = other.sender;
        this->receiver = other.receiver;
        this->amount = other.amount;
    }
    ~Transaction() = default;
    // setters
    void setSender(string sender_){ this->sender = std::move(sender_); }
    void setReceiver(string receiver_){ this->receiver = std::move(receiver_); }
    void setAmount(double amount_){ this->amount = amount_; }

    // getters
    string getSender() const { return sender; }
    string getReceiver() const { return receiver; }
    double getAmount() const { return amount; }

    // sobrecargas
    friend ostream& operator<<(ostream& out, Transaction& other){
        out << "Sender  : " << other.getSender() << endl
            << "Receiver: " << other.getReceiver() << endl
            << "Monto $ : " << other.getAmount() << endl;
        return out;
    }
    bool operator==(const Transaction& other) const {
        return sender == other.sender
               && receiver == other.receiver
               && amount == other.amount;
    }
    bool operator<(const Transaction& other) const { return amount < other.amount; }
    bool operator>(const Transaction& other) const { return amount > other.amount; }
    bool operator<=(const Transaction& other) const { return amount <= other.amount; }
    bool operator>=(const Transaction& other) const { return amount >= other.amount; }
};
#endif //BLOCKCHAIN_AED_BLOCK_H
