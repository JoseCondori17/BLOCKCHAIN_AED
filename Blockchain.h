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
        create_genesis_block("Datos del bloque génesis");
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
        //El propósito de calcular el hash es proporcionar una representación única de los datos del bloque génesis
        // para su posterior verificación e integridad en la cadena de bloques.
        string genesisHash = calculateGenesisHash(genesisData);
        // Crea el bloque génesis.
        Block genesisBlock(0, time(0), genesisData, genesisHash, 0);

        // Establece los datos del bloque génesis.
        genesisBlock.setKey("Clave del bloque génesis");
        return genesisBlock;
    }

    string calculateGenesisHash(const string& genesisData) {
        hash<string> hasher;
        stringstream ss;
        ss << 0 << time(0) << genesisData << "0" << 0;
        return to_string(hasher(ss.str()));
    }

    void create_genesis_block(const string& genesisData) {// Crear datos para el bloque génesis
        Block genesisBlock = createGenesisBlock(genesisData);
        //Y lo agrega a la cadena de bloques
        this->chain.push_back(genesisBlock);}

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

    //Margiory
    void proof_of_work(Block &block){ //Realiza la prueba de trabajo para un bloque dado (nonce)
        while(!is_valid_proof(block)){
            block.incrementNonce();
        }
        // Cuando salimos del bucle, hemos encontrado un nonce que hace que el hash del bloque sea válido,
        // es decir, hemos "minado" un nuevo bloque.
        cout << "Prueba de trabajo completada: " << block.getHash() << endl;
    }
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

    //Margiory
    bool verify_chain() { // verifica si la cadena de bloques es válido
        for (size_t i = 1; i < this->chain.size(); i++) {
            Block currentBlock = this->chain[i];
            Block previousBlock = this->chain[i-1];

            // Verificar que el hash del bloque actual es correcto
            if (currentBlock.getHash() != currentBlock.calculateHash()) {
                return false;
            }

            // Verificar que el hash del bloque anterior es correcto
            if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
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
