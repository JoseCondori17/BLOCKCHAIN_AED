#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include <vector>
#include "Block.h"
#include "Transaction.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#define DIFFICULTY 4
using namespace std;
class Blockchain{
private:
    ForwardList<Block*> chain; // cambiar estructura
public:
    Blockchain() = default;
    ~Blockchain() = default;
    explicit Blockchain(ForwardList<Transaction>& genesisData) {
        this->chain.push_back(createGenesisBlock(genesisData));
    };

    static Block* createGenesisBlock(ForwardList<Transaction>& genesisData) {
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
    void add_block(ForwardList<Transaction>& data)
    {
        if (chain.empty()) {
            throw std::invalid_argument("La cadena está vacía. No puedes añadir un bloque a una cadena vacía.");
        }
        Block* lastBlock = this->chain.back();
        int newIndex = lastBlock->getIndex() + 1;
        // Crear el nuevo bloque
        auto* newBlock = new Block(newIndex, data, lastBlock->getHash(),0);
        mine_block(newBlock);
        // Valida el nuevo bloque
        if (!validateBlock(newBlock)) {
            throw std::runtime_error("Bloque inválido. No se puede añadir a la cadena. Índice: " + std::to_string(newIndex));
        }
        // Verifica que el bloque se conecta correctamente a la cadena
        if (newBlock->getPrevHash() != lastBlock->getHash()) {
            throw std::runtime_error("El bloque no se conecta correctamente a la cadena. Índice: " + std::to_string(newIndex));
        }
        this->chain.push_back(newBlock);
    }
    static bool validateBlock(Block* block) {
        ForwardList<Transaction> transactions = block->getTransactions();
        if (transactions.empty()) {
            return false;
        }
        for (const Transaction& transaction : transactions) {
            if (!validateTransaction(transaction)) {
                return false;
            }
        }
        return true;
    }
    static bool validateTransaction(const Transaction& newTransaction)  {
        // Verificar que el remitente y el receptor no están vacíos
        if (newTransaction.getSender().empty() || newTransaction.getReceiver().empty())
            return false;
        // Verificar que la cantidad es mayor que cero
        if (newTransaction.getAmount() <= 0)
            return false;
        // Si todas las verificaciones pasaron, la transacción es válida
        return true;
    }
};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
