#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
#include <vector>
#include "Block.h"
#include "Transaction.h"

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
    ~Blockchain() {};
    void create_genesis_block(){
        // Crear datos para el bloque génesis.
        // En una blockchain real, los datos generalmente serían las transacciones,
        // pero por simplicidad, aquí sólo usamos una cadena de texto.
        string genesisData = "Datos del bloque génesis";

        // Crear el bloque génesis.
        Block genesisBlock(0, time(0), genesisData, "0");

        // Añadir el bloque génesis a la cadena de bloques.
        this->chain.push_back(genesisBlock);
    } // inicio
    void add_block(string data) // verificar si es valido la prueba NONCE - Referencia al previo block
    {
        // Obtener el último bloque en la cadena
        Block lastBlock = this->chain.back();

        // Calcular el índice para el nuevo bloque
        int newIndex = lastBlock.getIndex() + 1;

        // Calcular el nonce y el hash para el nuevo bloque
        /*int newNonce;
        string newHash;
        tie(newNonce, newHash) = this->proof_of_work(lastBlock.getHash());

        // Crear el nuevo bloque
        Block newBlock(newIndex, time(0), data, lastBlock.getHash(), newNonce);*/
        std::pair<int, std::string> powResult = this->proof_of_work(lastBlock.getHash());
        Block newBlock(newIndex, time(0), data, lastBlock.getHash(), powResult.first, powResult.second);
        // Añadir el nuevo bloque a la cadena
        this->chain.push_back(newBlock);
    }
    void proof_of_work(Block &block){
        while(!is_valid_proof(block)){
            block.incrementNonce();
        }
        // Cuando salimos del bucle, hemos encontrado un nonce que hace que el hash del bloque sea válido,
        // es decir, hemos "minado" un nuevo bloque.
        cout << "Prueba de trabajo completada: " << block.getHash() << endl;
    } // realiza la prueba de trabajo NONCE
    bool is_valid_proof(const Block& block){
            // Calcula el hash de los datos del bloque y el nonce
            string hash = block.calculateHash();
            // Comprueba si el hash tiene un número específico de ceros al inicio
            for (int i = 0; i < difficulty; i++){
                if (hash[i] != '0'){
                    return false;
                }
            }
            return true;
    } // verificar que sea valido la prueba de trabajo PAOLA
    bool verify_chain() { // verificar la cadena de bloques si es valido
        for (size_t i = 1; i < this->chain.size(); i++) {
            Block currentBlock = this->chain[i];
            Block previousBlock = this->chain[i-1];

            // Verificar que el hash del bloque actual es correcto
            if (currentBlock.getHash() != currentBlock.calculateHash()) {
                return false;
            }

            // Verificar que el hash del bloque anterior es correcto
            if (currentBlock.getPrevHash() != previousBlock.getHash()) {
                return false;
            }
        }

        return true;
    }

    void add_transaccion(Transaction newTransaction){
        // Consigue el último bloque en la cadena
        Block& lastBlock = this->chain.back();
        // Añade la transacción al último bloque
        lastBlock.addTransaction(newTransaction);
    }
    void minar(Block &block){
        while(!is_valid_proof(block)){
            block.incrementNonce(); // Asume que incrementNonce incrementa el valor del nonce en el bloque
        }
        cout << "Bloque minado: " << block.getHash() << endl;
        this->add_block(block);
    } //PAOLA agregar las transacciones averiguando la prueba de trabajo
    void print_blocks(){
        for (const auto& block : this->chain) {
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