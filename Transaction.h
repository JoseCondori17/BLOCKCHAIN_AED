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
    Transaction(string sender, string receiver, double amount)
            : sender(std::move(sender)), receiver(std::move(receiver)), amount(amount) {}
    ~Transaction() = default;
    string getSender() const {
        return sender;
    }
    string getReceiver() const {
        return receiver;
    }
    double getAmount() const {
        return amount;
    }
    void setSender(string sender_){
        this->sender = std::move(sender_);
    }
    void setReceiver(string receiver_){
        this->receiver = std::move(receiver_);
    }
    void setAmount(double amoun_){
        this->amount = amoun_;
    }
    //  para poder comparar fácilmente dos transacciones:
    bool operator==(const Transaction& other) const {
        return sender == other.sender
               && receiver == other.receiver
               && amount == other.amount;
    }
    //  por si se planea usar las transacciones en un conjunto (std::set) o como clave en un mapa (std::map)
    bool operator<(const Transaction& other) const {
        return amount < other.amount;
    }
    bool operator>(const Transaction& other) const {
        return amount > other.amount;
    }
};
#endif //BLOCKCHAIN_AED_BLOCK_H
