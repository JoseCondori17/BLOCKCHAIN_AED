#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include "Block.h"
#define DIFFICULTY 4
#define CAPACITY 20
template<typename T>
class Blockchain{
public:
    typedef class Block<T> Block;
private:
    Block * chain;
    BST<Block*> * index;
public:
    // Inicio
    explicit Blockchain(CircularArray<T>* genesisData) {
        this->chain = nullptr;
        this->index = new BST<Block*>();
        auto* new_block = createGenesisBlock(genesisData);
        new_block->prevBlock = chain;
        chain = new_block;
        index->insert(new_block);
    };
    Blockchain() = default;
    ~Blockchain() {
        delete index;
        Block* temp = chain;
        while (temp != nullptr) {
            Block* current = temp;
            temp = temp->prevBlock;
            delete current;
        }
    };
    // Minar el bloque
    void mine_block(Block*& block){
        proof_of_work(block);
    }

    /*
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
    */
    void printBlockMaxTransaction(){
        maxHeapTransaction();
    }
    void printBlockMinTransaction(){
        minHeapTransaction();
    }
    void printMinBlock(){}
    void printMaxBlock(){}
    void printIndexBST(){
        index->printBST();
    }
    // Agregar información a un nuevo bloque
    void add_block(CircularArray<T>* data)
    {
        Block* lastBlock = chain;
        int newIndex = lastBlock->getIndex() + 1;
        // Crear el nuevo bloque
        auto* new_block = new Block(newIndex, data, lastBlock->getHash(),0);
        mine_block(new_block);
        // lista simple bloques
        new_block->prevBlock = chain;
        chain = new_block;
        // bst index
        index->insert(new_block);
    }
private:
    // Heap
    Block* minHeapTransaction(){
        Heap<T> * heap;
        Block* current = chain;
        Block* min_block = chain;
        CircularArray<T>* transactions;
        T min_transaction = (*chain->getTransactions())[0];
        while (current != nullptr){
            heap = new Heap<T>(CAPACITY, Heap<T>::MIN_HEAP);
            transactions = current->getTransactions();
            for (int i = 0; i < transactions->size(); i++){
                heap->push((*transactions)[i]);
            }
            if (heap->top() < min_transaction){
                min_transaction = heap->top();
                min_block = current;
                cout<<heap->top().getSender()<<endl;
                cout<<heap->top().getReceiver()<<endl;
            }
            current = current->prevBlock;
            delete heap;
            delete transactions;
        }
        cout << min_block << endl;
        cout << min_transaction << endl;
        return min_block;
    }
    Block* maxHeapTransaction(){
        Heap<T> * heap;
        Block* current = chain;
        Block* max_block = chain;
        CircularArray<T>* transactions;
        T max_transaction = (*chain->getTransactions())[0];
        while (current != nullptr){
            heap = new Heap<T>(CAPACITY, Heap<T>::MAX_HEAP);
            transactions = current->getTransactions();
            for (int i = 0; i < transactions->size(); i++){
                heap->push((*transactions)[i]);
            }
            if (heap->top() > max_transaction){
                max_transaction = heap->top();
                max_block = current;
                cout<<heap->top().getSender()<<endl;
                cout<<heap->top().getReceiver()<<endl;
            }
            current = current->prevBlock;
            delete heap;
            delete transactions;
        }
        cout << max_block << endl;
        cout << max_transaction << endl;
        return max_block;
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
    // Start
    Block* createGenesisBlock(CircularArray<T>* genesisData) {
        auto * genesisBlock=new Block(0, genesisData, string(64, '0'), 0);
        mine_block(genesisBlock);
        return genesisBlock;
    }

};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
