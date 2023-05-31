#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include "Block.h"
#include "Transaction.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#define DIFFICULTY 4
using namespace std;
template<typename T>
class Blockchain{
public:
    typedef class Block<T> Block;
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
    Node* root; // tree
    ForwardList<Block*> chain;
public:
    Blockchain() = default;
    ~Blockchain() {
        chain.clear();
        root->KillSelf();
        delete root;
        root = nullptr;
    };
    // Inicio
    explicit Blockchain(CircularArray<T>& genesisData) {
        this->root = nullptr;
        this->chain.push_back(createGenesisBlock(genesisData));
        insert(root,createGenesisBlock(genesisData));
    };
    Block* createGenesisBlock(CircularArray<T>& genesisData) {
        auto * genesisBlock=new Block(0, genesisData, string(64, '0'), 0);
        mine_block(genesisBlock);
        return genesisBlock;
    }
    // Minar el bloque
    void mine_block(Block*& block){
        proof_of_work(block);
    }
    // Prueba de trabajo - validación de bloque
    static bool proof_of_work(Block*& block){
        std::string target(DIFFICULTY, '0');
        do {
            block->calculateHash();
            block->incrementNonce();
        } while (block->getHash().substr(0, DIFFICULTY) != target);
        return true;
    }
    // Muestra el bloque con la transacción mayor y menor
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
    // Muestra los bloques haciendo recorridos
    void printBlock(){
        for (auto& block : chain){
            block->print_bloque();
        }
    }
    void printInorder(){
        printInorder(root);
    }
    void printLevels(){
        printTree(root);
    }
    // Contar transacciones de un usuario
    void countTransactionsUser(string user){
        int value = 0;
        countTransactionsUser(root,user,value);
        cout<<"User: "<<user<<endl;
        cout<<"Amount: "<<value<<endl;
    }
    // Agregar información a un nuevo bloque
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
    }

private:
    // BSTree
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

    void countTransactionsUser(Node* node,string user,int& value){
        if (node == nullptr) {
            return;
        }
        CircularArray<T> transaction_ = node->block->getTransactions();
        for (int i = 0; i<transaction_.size();i++){
            if (user == transaction_[i].getSender()){
                value += transaction_[i].getAmount();
            }
        }
        countTransactionsUser(node->left, user, value);
        countTransactionsUser(node->right, user, value);
    }
    void countTransactionsPerUser(Node* node,T totalTransactionsUser){
        if (node == nullptr){
            return;
        }
        T transactions = getTransactions(node->block);
        for(const auto& transaction : transactions){
            int userId= getIndex(transaction);
            if(validateTransaction(transaction)){
                totalTransactionsUser[userId]++;
            }
        }
        countTransactionsUser(node->left, totalTransactionsUser);
        countTransactionsUser(node->right, totalTransactionsUser);
    }
    //Block - corregir
    bool validateBlock(Block*& block){
        // Verificar que el bloque no está vacío
        if (block->emptyTransactions()) {
            return true;
        }
        return false;
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
};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
