#ifndef BLOCKCHAIN_AED_TRANSACTION_H
#define BLOCKCHAIN_AED_TRANSACTION_H
#include <iostream>
using namespace std;

class Transaction {
private:
    std::string sender;
    std::string receiver;
    double amount;
    bool isSignatureVerified;
public:
    Transaction(string sender, string receiver, double amount)
            : sender(std::move(sender)), receiver(std::move(receiver)), amount(amount) {}

    string getSender() const {
        return sender;
    }

    string getReceiver() const {
        return receiver;
    }

    double getAmount() const {
        return amount;
    }
    //  para poder comparar fácilmente dos transacciones:
    bool operator==(const Transaction& other) const {
        return sender == other.sender
               && receiver == other.receiver
               && amount == other.amount;
    }
    //  por si se planea usar las transacciones en un conjunto (std::set) o como clave en un mapa (std::map)
    bool operator<(const Transaction& other) const {
        if (sender != other.sender) return sender < other.sender;
        if (receiver != other.receiver) return receiver < other.receiver;
        return amount < other.amount;
    }

};
#endif //BLOCKCHAIN_AED_BLOCK_H
