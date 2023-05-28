#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H
#include <chrono>
#include <string>
#include "Transaction.h"
using namespace std;

class Block{
private:
    /*
     * time : time_t
     * transactions : information
     * previus_hash : string ? link
     * nonce : num -> 5 zeros 00000...sfrsf4rasf23fdfas
     * hash : string
     */
public:
    Block() = default;
    ~Block() = default;
    static Block genesisBlock() {
        string genesisData = "Datos del bloque génesis";
        string genesisHash = "0";  // Hash del bloque anterior para el bloque génesis es 0
        return Block(0, time(0), genesisData, genesisHash, 0); // Suponiendo que Nonce sea 0 en el bloque génesis
    }
    static Block newBlock(int index, string previousHash, string data, int nonce) {
        return Block(index, time(0), data, previousHash, nonce);
    }
    void generar_hash(){}
    void toString(){}
    void print_hash(){}
    string getPreviousHash(){}
    string getHash(){}
    int getIndex(){}
    void incrementNonce(){
        this->nonce += 1;
    }
    void addTransaction(Transaction newTransaction) {
        this->transactions.push_back(newTransaction);
    }
    string calculateHash() const {
        std::hash<std::string> hasher;
        std::stringstream ss;
        ss << index << timestamp << data << prevHash << nonce;
        return std::to_string(hasher(ss.str()));
    }

};
#endif //BLOCKCHAIN_AED_BLOCK_H
