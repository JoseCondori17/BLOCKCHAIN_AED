#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include "Block.h"
#include "Transaction.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#define DIFFICULTY 4
using namespace std;
template<typename T>
class Blockchain{
public:
    typedef Block<T> Block;
private:
    ForwardList<Block*> chain; // cambiar estructura
public:
    Blockchain() = default;
    ~Blockchain() = default;
    explicit Blockchain(CircularArray<T*>& genesisData) {
        this->chain.push_back(createGenesisBlock(genesisData));
    };

    static Block* createGenesisBlock(CircularArray<T*>& genesisData) {
        auto * genesisBlock=new Block(0, genesisData, string(64, '0'), 0);
        mine_block(genesisBlock);
        return genesisBlock;
    }
    static void mine_block(Block*& block){
        proof_of_work(block);
    }
    static bool proof_of_work(Block*& block){
        std::string target(DIFFICULTY, '0');
        do {
            block->calculateHash();
            block->incrementNonce();
        } while (block->getHash().substr(0, DIFFICULTY) != target);
        return true;
    }

    void printBlock(){
        for (auto& block : chain){
            block->print_bloque();
        }
    }
    void add_block(CircularArray<T*>& data)
    {
        if (chain.empty()) {
            throw std::invalid_argument("La cadena está vacía. No puedes añadir un bloque a una cadena vacía.");
        }
        Block* lastBlock = this->chain.back();
        int newIndex = lastBlock->getIndex() + 1;
        // Crear el nuevo bloque
        auto* newBlock = new Block(newIndex, data, lastBlock->getHash(),0);
        mine_block(newBlock);

        this->chain.push_back(newBlock);
    }
};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
