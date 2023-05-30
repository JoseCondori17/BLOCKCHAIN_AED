#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include "Block.h"
#include "Transaction.h"
#include "HashTable.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#define DIFFICULTY 4
using namespace std;
#include <queue>
template<typename T>
class Blockchain{
public:
    typedef Block<T> Block;
private:
    struct Node{
        Block* block;
        Node* left;
        Node* right;
        explicit Node(Block* block_) : block(block_), left(nullptr), right(nullptr){}
        void KillSelf(Node* node){
            if (node == nullptr) return void();
            KillSelf(node->left);
            KillSelf(node->right);
            delete node;
        }
        void KillSelf(){
            KillSelf(left);
            KillSelf(right);
        }
    };
private:
    Node* root; // arbol
    ForwardList<Block*> chain;
public:
    Blockchain() = default;
    ~Blockchain() {
        chain.clear();
        root->KillSelf();
        delete root;
        root = nullptr;
    };
    explicit Blockchain(CircularArray<T>& genesisData) {
        this->root = nullptr;
        this->chain.push_back(createGenesisBlock(genesisData));
        insert(root,createGenesisBlock(genesisData));
    };
    static Block* createGenesisBlock(CircularArray<T>& genesisData) {
        auto * genesisBlock=new Block(0, genesisData, string(64, '0'), 0);
        mine_block(genesisBlock);
        return genesisBlock;
    }
    static void mine_block(Block*& block){
        if (!validateBlock(block)) {
            std::cout << "Bloque inválido. No se puede minar." << std::endl;
            return;
        }
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
    void printMaxHeapBlock(){ // operativo
        string hashBlock = root->block->getHash();
        T maxTransaction = root->block->MaxHeap();
        findMaxTransaction(root, maxTransaction,hashBlock);
        Block* block = findBlock(root,hashBlock);
        block->print_bloque();
    }
    void printMinHeapBlock(){ // operativo
        string hashBlock = root->block->getHash();
        T minTransaction = root->block->MinHeap();
        findMinTransaction(root, minTransaction,hashBlock);
        Block* block = findBlock(root,hashBlock);
        block->print_bloque();
    }
    void printBlock(){
        for (auto& block : chain){
            block->print_bloque();
        }
    } // forward
    void printInorder(){
        printInorder(root);
    } // bstree
    void printLevels(){
        printTree(root);
    } // bstree
    void add_block(CircularArray<T>& data)
    {
        if (chain.empty()) {
            throw std::invalid_argument("La cadena está vacía. No puedes añadir un bloque a una cadena vacía.");
        }
        Block* lastBlock = this->chain.back();
        int newIndex = lastBlock->getIndex() + 1;
        // Crear el nuevo bloque
        auto* newBlock = new Block(newIndex, data, lastBlock->getHash(),0);
        mine_block(newBlock);
        insert(root, newBlock);
        this->chain.push_back(newBlock);
        // Valida el nuevo bloque
        if (!validateBlock(newBlock)) {
            throw std::runtime_error("Bloque inválido. No se puede añadir a la cadena. Índice: " + std::to_string(newIndex));
        }
    }
    bool validateBlock(Block*& block){
        // Verificar que el bloque no está vacío
        if (block.getTransactions().empty()) {
            return false;
        }
        // Verificar que el hash del bloque es válido
        if (block.getHash() != block.calculateHash()){
            return false;
        }
        // Verificar la validez de las transacciones en el bloque
        for (Transaction* &transaction : block.getTransactions()) {
            if (!validateTransaccion(transaction)) {
                return false;
            }
        }
        // Si todas las verificaciones pasan, el bloque es válido
        return true;
    }
    bool validateTransaction(const Transaction& newTransaction)  {
        // Verificar que el remitente y el receptor no están vacíos
        if (newTransaction.getSender().empty() || newTransaction.getReceiver().empty()) {
            return false;
        }
        // Verificar que la cantidad es mayor que cero
        if (newTransaction.getAmount() <= 0) {
            return false;
        }
        // Si todas las verificaciones pasaron, la transacción es válida
        return true;
    }
    bool verify_chain() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block& currentBlock = chain[i];
            const Block& previousBlock = chain[i - 1];

            // Verifica que el hash del bloque anterior sea correcto
            if (currentBlock.getPrevHash() != previousBlock.getHash()) {
                return false;
            }
            // Verifica la validez de cada bloque en la cadena
            if (!validateBlock(currentBlock)) {
                return false;
            }
        }
        // Si todas las verificaciones pasan, la cadena es válida
        return true;
    }

private:
    void insert(Node* &node, Block* block){
        if (node == nullptr) node = new Node(block);
        else if (block->getHash() < node->block->getHash()) insert(node->left, block);
        else insert(node->right, block);
    }
    void printInorder(Node* node){
        if (node != nullptr){
            printInorder(node->left);
            node->block->print_bloque();
            printInorder(node->right);
        }
    }
    void printTree(Node* node, int level = 0, int count = 10) {
        if (node == nullptr)
            return;
        level++;
        printTree(node->right, level);
        cout << setw(level * count) << "Level " << level << ": " << node->block->getIndex() << endl;
        printTree(node->left, level);
    }
    void findMaxTransaction(Node* node, T& maxTransaction, string& currentHash) {
        if (node == nullptr) {
            return;
        }

        T transaction = node->block->MaxHeap();
        if (transaction > maxTransaction) {
            maxTransaction = transaction;
            currentHash = node->block->getHash();
        }

        findMaxTransaction(node->left, maxTransaction,currentHash);
        findMaxTransaction(node->right, maxTransaction,currentHash);
    }
    void findMinTransaction(Node* node, T& maxTransaction, string& currentHash) {
        if (node == nullptr) {
            return;
        }

        T transaction = node->block->MinHeap();
        if (transaction < maxTransaction) {
            maxTransaction = transaction;
            currentHash = node->block->getHash();
        }

        findMinTransaction(node->left, maxTransaction,currentHash);
        findMinTransaction(node->right, maxTransaction,currentHash);
    }
    Block* findBlock(Node* node,string hashBlock){
        if (node->block->getHash() == hashBlock) return node->block;
        else if (hashBlock < node->block->getHash()) return findBlock(node->left, hashBlock);
        else return findBlock(node->right, hashBlock);
    }
};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
