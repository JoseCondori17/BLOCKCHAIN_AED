#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H
#include "Librerias.h"

template<typename T>
class Block {
private:
    uint16_t index; //posición del bloque
    string prevHash; //hash del bloque anterior a la cadena
    string currentHash; // hash actual del bloque
    uint64_t nonce; //el proof of work
    string timestamp; //fecha y hora
    CircularArray<T>* transactions; // transacciones
public:
    // constructores
    Block() = default;
    Block(uint16_t _index, CircularArray<T>* _transactions, const string &_prevHash, uint64_t _nonce) {
        this->index = _index;
        this->timestamp = currentTime();
        this->transactions = _transactions;
        this->prevHash = _prevHash;
        this->nonce = _nonce;
        this->currentHash = calculateHash();
        this->prevBlock = nullptr;
    }
    Block* prevBlock;
    // destructores
    ~Block() { delete prevBlock; delete transactions; };

    // setters
    void setPreviousHash(const string& prev){ this->prevHash = prev; }
    void setIndex(uint64_t idx){ this->index = idx; }
    void setTransactions(CircularArray<T>* transaction) { this->transactions = transaction; }

    // getters
    uint16_t getIndex() const { return index; }
    uint64_t getNonce() const { return nonce; }
    string getHash() { return currentHash; }
    string getData() { return toStringData(); }
    string getPrevHash() const { return prevHash; }
    string getTimestamp() const { return timestamp; }
    CircularArray<T>* getTransactions() const { return transactions; }

    // methods
    void incrementNonce() { this->nonce += 1; }
    void addTransaction(Transaction& newTransaction) {
        if (verifyTransaction(newTransaction))
            transactions->push_back(newTransaction);
        else
            cout << "Invalid transaction" << endl;
    }  // uso para el blockchain
    bool emptyTransactions(){ return transactions->is_empty(); }

    // verificar
    bool verifyTransaction(Transaction& newTransaction){
        if (invalidTransaction(newTransaction))
            return false;
        if (existTransaction(newTransaction))
            return false;
        if (verifySenderFunds(newTransaction))
            return false;
        return true;
    }
    bool existTransaction(Transaction& newTransaction){
        for (size_t i = 0;i < transactions->size(); i++){
            if ((*transactions)[i] == newTransaction){
                return true;
            }
        }
        return false;
    }
    bool invalidTransaction(Transaction& newTransaction){
        if (newTransaction.getSender().empty() ||
            newTransaction.getReceiver().empty() ||
            newTransaction.getAmount() <= 0) {
            return true;
        }
        return false;
    }
    bool verifySenderFunds(Transaction& newTransaction){
        if (getBalance(newTransaction.getSender()) < newTransaction.getAmount()) {
            return true;
        }
        return false;
    }
    bool checkLimitTransaction(){
        return (transactions->size() >= 15);
    }

    // corregir ...
    T MaxHeap(){
        auto * heap = new Heap<T>(10, Heap<T>::MAX_HEAP);
        auto * transArray = new Transaction[transactions->size()];
        for (int i = 0; i < transactions->size(); i++) {
            transArray[i] = (*transactions)[i];
        }
        heap->buildFromArray(transArray, transactions->size());
        T maxim = heap->top();
        delete[] transArray;
        delete heap;
        return maxim;
    }
    T MinHeap(){
        auto * heap = new Heap<T>(10, Heap<T>::MIN_HEAP);
        auto * transArray = new Transaction[transactions->size()];
        for (int i = 0; i < transactions->size(); i++) {
            transArray[i] = (*transactions)[i];
        }
        heap->buildFromArray(transArray, transactions->size());
        T minim = heap->top();
        delete[] transArray;
        delete heap;
        return minim;
    }

    // esencial
    string calculateHash() {
        // Concatena los datos relevantes del bloque
        string blockData = to_string(index) + timestamp + toStringTransacciones() + prevHash + to_string(nonce);

        // Calcula el hash del bloque utilizando SHA-256 u otra función hash
        unsigned char hash[EVP_MAX_MD_SIZE]; // Almacenar el resultado del hash
        EVP_MD_CTX* ctx = EVP_MD_CTX_new(); // Calculo hash
        EVP_DigestInit(ctx, EVP_sha256()); // Calculo hash usando sha256
        EVP_DigestUpdate(ctx, blockData.c_str(), blockData.size());
        unsigned int hashLength = 0;
        EVP_DigestFinal(ctx, hash, &hashLength);
        EVP_MD_CTX_free(ctx);
        std::stringstream ss;
        for (unsigned int i = 0; i < hashLength; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
        }
        this->currentHash = ss.str();
        return ss.str();
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

    // prints
    void print_bloque(){
        cout<<"Block    : "<<index<<endl
            <<"Time     : "<<timestamp<<endl
            <<"Prev hash: "<<prevHash<<endl
            <<"Hash     : "<<currentHash<<endl
            <<"Nonce    : "<<nonce<<endl
            <<"Data     : "<<endl;
    }
    void print_table(){
        cout<<left<<index<<setw(10)<<nonce<<setw(10)<<prevHash<<setw(10)<<currentHash<<endl;
    }

    //sobrecarga
    bool operator<(const Block<T>* other) const { return this->currentHash < other->currentHash; }
    bool operator>(const Block<T>* other) const { return this->currentHash > other->currentHash; }
    friend ostream & operator<<(ostream& out, Block<T>* block){
        out<<block->getIndex();
        return out;
    }
private:
    static string currentTime(){
        time_t currentTime = time(nullptr);
        tm* localTime = localtime(&currentTime);
        ostringstream oss; char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
        oss << buffer;
        return oss.str();
    }
    // arreglar
    string toStringTransacciones() {
        ostringstream oss;
        for (size_t i = 0; i < transactions->size(); i++) {
            oss << (*transactions)[i].getSender() << (*transactions)[i].getReceiver() << (*transactions)[i].getAmount();
        }
        return oss.str();
    }
    string toStringData() {
        ostringstream oss;
        for (size_t i = 0; i < transactions->size(); i++) {
            oss << "Sender  : " << (*transactions)[i].getSender()
                << "Receiver: " << (*transactions)[i].getReceiver()
                << "Monto $ : " << (*transactions)[i].getAmount() << endl;
        }
        return oss.str();
    }
public:
    void results(){
        cout<<toStringTransacciones();
    }
};

#endif //BLOCKCHAIN_AED_BLOCK_H
