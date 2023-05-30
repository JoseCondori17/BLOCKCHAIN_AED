#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include "ForwardList.h"
#include "Transaction.h"
#include "CircularArray.h"
#include "HeapMinMax.h"
#include <openssl/evp.h>
using namespace std;

template<typename T>
class Block {
private:
    int index; //posición del bloque
    string prevHash; //hash del bloque anterior a la cadena
    string currentHash;
    long long int nonce; //el proof of work (valor que se ajusta
    string timestamp; //fecha y hora
    CircularArray<T> transactions;
public:
    Block() = default;
    Block(int index, CircularArray<T> &transactions, const string &prevHash, long long int nonce) {
        this->index = index;
        this->timestamp = currentTime();
        this->transactions = transactions;
        this->prevHash = prevHash;
        this->nonce = nonce;
        this->currentHash = calculateHash();
    }
    ~Block() = default;
    void setPreviousHash(string prev){
        this->prevHash = prev;
    }
    void setIndex(long long int i){
        this->index = i;
    }
    void setTransactions(CircularArray<T>& transaction) {
        this->transactions = transaction;
    }
    void incrementNonce() {
        this->nonce += 1;
    }
    void addTransaction(Transaction& newTransaction) {
        transactions.push_back(newTransaction);
    }
    bool emptyTransactions(){
        return transactions.is_empty();
    }
    bool existTransaction(Transaction& newTransaction){
        for (size_t i = 0;i < transactions.size(); i++){
            if (transactions[i] == newTransaction){
                std::cout << "Error: La transaccion ya existe en el bloque.\n";
                return true;
            }
        }
        return false;
    }
    bool invalidTransaccion(Transaction& newTransaction){
        if (newTransaction.getSender().empty() || newTransaction.getReceiver().empty() ||
            newTransaction.getAmount() <= 0) {
            std::cout << "Error: Campos de transaccion invalidos.\n";
            return true;
        }
        return false;
    }
    bool checkLimitTransaction(){
        if (transactions.size() >= 29) {
            std::cout << "Error: Se ha alcanzado el límite de transacciones para este bloque.\n";
            return true;
        }
        return false;
    }
    bool verifySenderFunds(Transaction& newTransaction){
        if (getBalance(newTransaction.getSender()) < newTransaction.getAmount()) {
            std::cout << "Error: El remitente no tiene fondos suficientes para la transacción.\n";
            return true;
        }
        return false;
    }
    T MaxHeap(){
        auto * heap = new Heap<T>(30, Heap<T>::MAX_HEAP);
        auto * transArray = new Transaction[transactions.size()];
        for (int i = 0; i < transactions.size(); i++) {
            transArray[i] = transactions[i];
        }
        heap->buildFromArray(transArray, transactions.size());
        delete[] transArray;
        return heap->top();
    }
    T MinHeap(){
        auto * heap = new Heap<T>(30, Heap<T>::MIN_HEAP);
        auto * transArray = new Transaction[transactions.size()];
        for (int i = 0; i < transactions.size(); i++) {
            transArray[i] = transactions[i];
        }
        heap->buildFromArray(transArray, transactions.size());
        delete[] transArray;
        return heap->top();
    }
    CircularArray<T> getTransactions() const {
        return transactions;
    }
    string getHash() {
        return currentHash;
    }
    std::string calculateHash() {
        // Concatena los datos relevantes del bloque
        std::string blockData = std::to_string(index) + timestamp + toStringTransacciones() + prevHash + std::to_string(nonce);

        // Calcula el hash del bloque utilizando SHA-256 u otra función hash
        unsigned char hash[EVP_MAX_MD_SIZE]; // Almacenar el resultado del hash
        EVP_MD_CTX* ctx = EVP_MD_CTX_new(); // Calculo hash
        EVP_DigestInit(ctx, EVP_sha256()); // Calculo hash usando sha256
        EVP_DigestUpdate(ctx, blockData.c_str(), blockData.size());
        unsigned int hashLength = 0;
        EVP_DigestFinal(ctx, hash, &hashLength);
        EVP_MD_CTX_free(ctx);
        // cout<<"HashLength: "<<hashLength<<endl;
        // Convierte el hash binario a una representación hexadecimal
        std::stringstream ss;
        for (unsigned int i = 0; i < hashLength; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
        }
        this->currentHash = ss.str();
        return ss.str();
    }
    void print_bloque(){
        cout<<"Block: "<<index<<endl;
        cout<<"Time: "<<timestamp<<endl;
        cout<<"Prev hash: "<<prevHash<<endl;
        cout<<"Hash: "<<currentHash<<endl;
        cout<<"Nonce: "<<nonce<<endl;
        cout<<"Data: \n"<<
            getData()<<endl;
        cout<<endl;
    }
    double getBalance(const string& sender) {
        double balance = 0.0;
        for (auto& transaction : transactions) {
            if (transaction.getReceiver() == sender) {
                balance += transaction.getAmount();
            }
            if (transaction.getSender() == sender) {
                balance -= transaction.getAmount();
            }
        }
        return balance;
    }
    std::string getData() {
        return toStringData();
    }
    std::string getPrevHash() const {
        return prevHash;
    }
    std::string getTimestamp() const {
        return timestamp;
    }
    long long int getNonce() const {
        return nonce;
    }
    int getIndex() const {
        return index;
    }
private:
    static string currentTime(){
        std::time_t currentTime = time(nullptr);
        std::tm* localTime = localtime(&currentTime);
        std::ostringstream oss; char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
        oss << buffer;
        return oss.str();
    }
    string toStringTransacciones() {
        ostringstream oss;
        for (size_t i = 0; i < transactions.size(); i++) {
            oss << transactions[i].getSender() << transactions[i].getReceiver() << transactions[i].getAmount();
        }
        return oss.str();
    }
    string toStringData() {
        ostringstream oss;
        for (size_t i = 0; i < transactions.size(); i++) {
            oss << "Sender: " << transactions[i].getSender()
                << "  Receiver: " << transactions[i].getReceiver()
                << "  $" << transactions[i].getAmount() << "\n";
        }
        return oss.str();
    }
};

#endif //BLOCKCHAIN_AED_BLOCK_H
