#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include "ForwardList.h"
#include "Transaction.h"
#include <openssl/evp.h>
using namespace std;

class Block {
private:

    //Margio: implemente los atributos privados para agregar un constructor block
    string key; //clave del bloque
    string data; // data del bloque
    string prevHash; //hash del bloque anterior a la cadena
    int nonce; //el proof of work (valor que se ajusta
    int index; //posición del bloque
    string timestamp; //fecha y hora

    ForwardList<Transaction> transactions;
public:
    Block() = default;
    ~Block() = default;
    static Block newBlock(int index, const string &previousHash, const string &data, int nonce) {
        return Block{index, data, previousHash, nonce};  // create object
    }
    std::string getKey() const {
        return key;
    }

    std::string getData() const {
        return data;
    }

    std::string getPrevHash() const {
        return prevHash;
    }

    std::string getTimestamp() const {
        return timestamp;
    }

    int getNonce() const {
        return nonce;
    }

    int getIndex() const {
        return index;
    }

    Block(int index, const string &genesisData, const string &prevHash, int nonce) {
        this->index = index;
        this->timestamp = currentTime();
        this->data = genesisData;
        this->prevHash = prevHash;
        this->nonce = nonce;
    }

    void setKey(const string &blockKey) {
        key = blockKey;
    }

    void setData(const string &blockData) {
        data = blockData;
    }

    string getHash() const {
        return calculateHash();
    }

    //Incrementa el valor de nonce
    void incrementNonce() {
        this->nonce += 1;
    }

    double getBalance(const std::string& sender) {
        double balance = 0.0;
        for (const Transaction& transaction : transactions) {
            if (transaction.getReceiver() == sender) {
                balance += transaction.getAmount();
            }
            if (transaction.getSender() == sender) {
                balance -= transaction.getAmount();
            }
        }
        return balance;
    }

    bool addTransaction(const Transaction &newTransaction) {
        // Verificar si la transacción ya existe en el bloque
        for (const auto &transaccion: transactions) {
            if (transaccion == newTransaction) {
                std::cout << "Error: La transacción ya existe en el bloque.\n";
                return false;
            }
        }
        // Validación de la transacción.
        // Falta agregar mas reglas de validación.
        if (newTransaction.getSender().empty() || newTransaction.getReceiver().empty() ||
            newTransaction.getAmount() <= 0) {
            // Lanzar una excepción o devolver false en caso de que la transacción no sea válida.
            std::cout << "Error: Campos de transacción inválidos.\n";
            return false;
        }

        // Verificar el límite de transacciones (por ejemplo, 5000 transacciones por bloque)
        if (transactions.size() >= 5000) {
            std::cout << "Error: Se ha alcanzado el límite de transacciones para este bloque.\n";
            return false;
        }

        // Verificar la firma digital de la transacción
        //if (!verifySignature(newTransaction)) {
        //     std::cout << "Error: La firma de la transacción no es válida.\n";
        //     return false;
        //}

        // Verificar los fondos del remitente
         if (getBalance(newTransaction.getSender()) < newTransaction.getAmount()) {
             std::cout << "Error: El remitente no tiene fondos suficientes para la transacción.\n";
             return false;
         }

        // falta implementar la funcion verifySignature y getBalance (PAOLA)

        // Si todo esta bien, agregar la transaccion al bloque.
        transactions.push_back(newTransaction);
        return true;
    }

    ForwardList<Transaction> getTransactions() const {
        return transactions;
    }

    std::string calculateHash() const {
        // Concatena los datos relevantes del bloque
        std::string blockData = std::to_string(index) + timestamp + data + prevHash + std::to_string(nonce);

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
        for (unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
        }

        return ss.str();
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
};

#endif //BLOCKCHAIN_AED_BLOCK_H
