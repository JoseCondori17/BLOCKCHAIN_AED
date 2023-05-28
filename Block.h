#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H
#include <chrono>
#include <string>
#include <sstream>
#include "Transaction.h"
using namespace std;

class Block{
private:

    //Margio: implemente los atributos privados para agregar un constructor block
    string key; //clave del bloque
    string data; // data del bloque
    string prevHash; //hash del bloque anterior a la cadena
    string nonce; //el proof of work (valor que se ajusta
    string index; //posición del bloque
    time_t timestamp; //momento en el que el bloque ha sido creado
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

    string getKey()const{
        return key;
    }
    string getData() const{
        return data;
    }
    Block(int index, time_t timestamp, const string& genesisData, const string& prevHash) {
        this->index = to_string(index);
        this->timestamp = timestamp;
        this->data = genesisData;
        this->prevHash = prevHash;
    }

    void setKey(const string& blockKey) {
        key = blockKey;
    }
    void setData(const string& blockData) {
        data = blockData;
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
