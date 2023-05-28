#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H

#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include "Transaction.h"
#include <openssl/sha.h>

using namespace std;

class Block {
private:

    //Margio: implemente los atributos privados para agregar un constructor block
    string key; //clave del bloque
    string data; // data del bloque
    string prevHash; //hash del bloque anterior a la cadena
    int nonce; //el proof of work (valor que se ajusta
    int index; //posición del bloque
    time_t timestamp; //momento en el que el bloque ha sido creado
    /*
     * time : time_t
     * transactions : information
     * previus_hash : string ? link
     * nonce : num -> 5 zeros 00000...sfrsf4rasf23fdfas
     * hash : string
     */

    std::vector<Transaction> transacciones;
public:
    Block() = default;

    ~Block() = default;

    static Block newBlock(int index, const string &previousHash, const string &data, int nonce) {
        return Block(index, time(0), data, previousHash, nonce);
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
        return std::to_string(timestamp);
    }

    int getNonce() const {
        return nonce;
    }

    int getIndex() const {
        return index;
    }

    //Constructor creado
    Block(int index, time_t timestamp, const string &genesisData, const string &prevHash, int nonce) {
        this->index = index;
        this->timestamp = timestamp;
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

    void generar_hash() {}

    void toString() {}

    void print_hash() {}

    string getHash() const {
        return calculateHash();
    }

    //Incrementa el valor de nonce
    void incrementNonce() {
        this->nonce += 1;
    }

    bool addTransaction(const Transaction &newTransaction) {
        // Verificar si la transacción ya existe en el bloque
        for (const auto &transaccion: transacciones) {
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
        if (transacciones.size() >= 5000) {
            std::cout << "Error: Se ha alcanzado el límite de transacciones para este bloque.\n";
            return false;
        }

        // Verificar la firma digital de la transacción (pseudocódigo)
        // if (!verifySignature(newTransaction)) {
        //     std::cout << "Error: La firma de la transacción no es válida.\n";
        //     return false;
        // }

        // Verificar los fondos del remitente (pseudocódigo)
        // if (getBalance(newTransaction.getSender()) < newTransaction.getAmount()) {
        //     std::cout << "Error: El remitente no tiene fondos suficientes para la transacción.\n";
        //     return false;
        // }

        // falta implementar la funcion verifySignature y getBalance (PAOLA)


        // Si todo esta bien, agregar la transaccion al bloque.
        transacciones.push_back(newTransaction);
        return true;
    }

    std::vector<Transaction> getTransactions() const {
        return transacciones;
    }

    std::string calculateHash() const {
        // Concatena los datos relevantes del bloque
        std::string blockData = std::to_string(index) + timestamp + data + prevHash + std::to_string(nonce);

        // Calcula el hash del bloque utilizando SHA-256 u otra función hash
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, blockData.c_str(), blockData.size());
        SHA256_Final(hash, &sha256);

        // Convierte el hash binario a una representación hexadecimal
        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
        }

        return ss.str();
    }




};

#endif //BLOCKCHAIN_AED_BLOCK_H
