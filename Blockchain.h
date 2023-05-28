#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include <vector>
#include "Block.h"
#include "Transaction.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
using namespace std;
class Blockchain{
private:
    std::vector<Block> chain;  // Vector que almacena los bloques de la cadena
    int difficulty;  // Dificultad de la prueba de trabajo

    Block createGenesisBlock();  // Crear el bloque génesis

    bool is_valid_proof(const Block& block);  // Verificar si un bloque tiene una prueba de trabajo válida

    std::pair<int, std::string> proof_of_work(const std::string& previousHash);  // Realizar la prueba de trabajo

public:
    Blockchain() {
        this->chain.push_back(createGenesisBlock());
        this->difficulty = 4;  // Dificultad inicial de la prueba de trabajo
    };
    //IMPLEMENTÉ LOS CRITERIOS DE BÚSQUEDA CON AYUDA DE INTERNET PERO NO ESTOY SEGURA SI SE ADAPTA
    // A LO QUE QUEREMOS PRESENTAR (MARGIORY)

    //Criterio de Búsqueda por clave (TK Key)
    Block searchByKey(const string& key){ //Realiza una búsqueda por clave en la cadena y devuelve el bloque correspondiente si se encuentra
        for(const auto& block : chain){
            if(block.getKey() == key){
                return block;
            }
        }
        // Devolver un bloque vacío o generar una excepción si no se encuentra la clave
        return Block(); // O lanzar una excepción según tus necesidades
    }

    // Criterio de Búsqueda de patrones en el vector (vector<TV> contains(string pattern))
    vector<Block> searchByPattern(const string& pattern){ //Realiza una búsqueda en los datos de los bloques y devuelve un vector con los que coinciden con el pattern.
        vector<Block> results;
        for(const auto& block : chain){
            if(block.getData().find(pattern) != string::npos){
                results.push_back(block);
            }
        }
        return results;
    }


    ~Blockchain() {};

    //Margiory
    Block createGenesisBlock(const std::string& genesisData) {
        // Se utiliza para calcular el hash del bloque génesis.
        // El propósito de calcular el hash es proporcionar una representación única de los datos del bloque génesis
        // para su posterior verificación e integridad en la cadena de bloques.
        std::string genesisHash = calculateGenesisHash(genesisData); // Debe implementarse calculateGenesisHash

        // Crea el bloque génesis.
        // Suponemos que la clase Block tiene un constructor adecuado.
        Block genesisBlock(0, time(0), genesisData, genesisHash, 0); // Debe ajustarse según la implementación de la clase Block

        // Establece los datos del bloque génesis.
        genesisBlock.setKey("Clave del bloque génesis"); // Debe ajustarse según la implementación de la clase Block
        return genesisBlock;
    }

    std::string calculateGenesisHash(const string& genesisData) {
        std::hash<std::string> hasher;
        std::stringstream ss;
        ss << 0 << time(0) << genesisData << "0" << 0;
        return std::to_string(hasher(ss.str()));
    }

    void create_genesis_block(const string& genesisData) {// Crear datos para el bloque génesis
        Block genesisBlock = createGenesisBlock(genesisData);
        //Y lo agrega a la cadena de bloques
        this->chain.push_back(genesisBlock);
    }

    void add_block(const string& data) // verificar si es valido la prueba NONCE - Referencia al previo block
    {
        if (chain.empty()) {
            std::cout << "La cadena está vacía. No puedes añadir un bloque a una cadena vacía." << std::endl;
            return;
        }

        // Obtener el último bloque en la cadena
        Block lastBlock = this->chain.back();

        // Calcular el índice para el nuevo bloque
        int newIndex = lastBlock.getIndex() + 1;

        // Calcular el nonce y el hash para el nuevo bloque
        std::pair<int, std::string> powResult = this->proof_of_work(lastBlock.getHash());

        // Crear el nuevo bloque
        Block newBlock(newIndex, time(0), data, lastBlock.getHash(), powResult.second, powResult.first);

        // Valida el nuevo bloque
        if (!validateBlock(newBlock)) {
            throw std::runtime_error("Bloque inválido. No se puede añadir a la cadena. Índice: " + std::to_string(newIndex));
        }

        // Verifica que el bloque se conecta correctamente a la cadena
        if (newBlock.getPrevHash() != lastBlock.getHash()) {
            throw std::runtime_error("El bloque no se conecta correctamente a la cadena. Índice: " + std::to_string(newIndex));
        }

        // Si todas las verificaciones pasan, añade el bloque a la cadena
        chain.push_back(newBlock);
    }


    //Margiory
    std::pair<int, std::string> proof_of_work(const std::string& lastHash) {
        int nonce = 0;
        std::string hash;
        do {
            nonce++;
            std::string text = lastHash + std::to_string(nonce);
            hash = compute_hash(text); // compute_hash es una función que calcula el hash de una cadena.
        } while (hash.substr(0, DIFFICULTY) != std::string(DIFFICULTY, '0'));
        return std::make_pair(nonce, hash);
    }
    std::string compute_hash(const std::string& input) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, input.c_str(), input.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }

        return ss.str();
    }

    bool is_valid_proof(const Block& block) {
        // Crea un nuevo string que contiene el hash del bloque
        string hash = block.getHash();

        // Comprueba si el hash comienza con un cierto número de ceros
        // Este es el "problema" que el algoritmo de Prueba de Trabajo debe resolver
        for (int i = 0; i < DIFFICULTY; ++i) {
            if (hash[i] != '0') {
                return false;
            }
        }

        // Si el hash cumple con el requisito, la prueba es válida
        return true;

    }// verificar que sea valido la prueba de trabajo PAOLA

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


    void add_transaccion(const Transaction& newTransaction) {
        // Comprueba si la cadena está vacía
        if (this->chain.empty()) {
            // Si está vacía, crea un bloque génesis y añádelo a la cadena
            this->chain.push_back(Block());
        }

        // Consigue el último bloque en la cadena
        Block& lastBlock = this->chain.back();

        // Valida la transacción
        if (!validateTransaction(newTransaction)) {
            // Si la transacción no es válida, no se añade al bloque
            std::cout << "Transacción no válida, no se puede añadir al bloque." << std::endl;
            return;
        }

        // Añade la transacción al último bloque
        if (!lastBlock.addTransaction(newTransaction)) {
            // Si addTransaction devuelve false, la transacción no se pudo añadir al bloque
            std::cout << "No se pudo añadir la transacción al bloque." << std::endl;
        }
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

        // Añadir más reglas de validación aquí

        // Si todas las verificaciones pasaron, la transacción es válida
        return true;
    }

    bool validateBlock(const Block& block) const {
        // Verificar que el bloque no está vacío
        if (block.getTransactions().empty()) {
            return false;
        }

        // Verificar que el hash del bloque es válido
        if (block.getHash() != block.calculateHash()){
            return false;
        }

        // Verificar la validez de las transacciones en el bloque
        for (const Transaction& transaction : block.getTransactions()) {
            if (!validateTransaction(transaction)) {
                return false;
            }
        }

        // Verificar cualquier otra regla adicional que puedas tener

        // Si todas las verificaciones pasan, el bloque es válido
        return true;
    }


    void minar(Block &block){
        // Valida las transacciones en el bloque
        for (const auto& trans : block.getTransactions()) {
            if (!validateTransaction(trans)) {
                cout << "Transacción inválida en el bloque. No se puede minar." << endl;
                return;
            }
        }

        // Valida el bloque
        if (!validateBlock(block)) {
            cout << "Bloque inválido. No se puede minar." << endl;
            return;
        }

        // Intenta encontrar un nonce válido
        int maxAttempts = 1000000;  // Cambia esto según tus necesidades
        for (int i = 0; i < maxAttempts && !is_valid_proof(block); i++) {
            block.incrementNonce();
        }

        // Verifica si se encontró un nonce válido
        if (!is_valid_proof(block)) {
            cout << "No se encontró un nonce válido después de " << maxAttempts << " intentos." << endl;
            return;
        }

        // Si se encontró un nonce válido, añade el bloque a la cadena
        cout << "Bloque minado: " << block.getHash() << endl;
        this->add_block(block);
    } //PAOLA agregar las transacciones averiguando la prueba de trabajo

    void print_blocks(){
        for (const Block& block : this->chain) {
            cout << "----------------------" << endl;
            cout << "Index: " << block.getIndex() << endl;
            cout << "Timestamp: " << block.getTimestamp() << endl;
            cout << "Data: " << block.getData() << endl;
            cout << "Hash: " << block.getHash() << endl;
            cout << "Previous Hash: " << block.getPrevHash() << endl;
            cout << "Nonce: " << block.getNonce() << endl;
            cout << "----------------------" << endl;
        }
    } // mostrar los bloques


    int getNumOfBlocks(){
        return chain.size();
    } //PAOLA
    string getLatestBlockHash(){
        return chain.back().getHash();
    } // obtenemos el previ hash


};

#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
