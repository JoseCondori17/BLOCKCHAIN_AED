#ifndef BLOCKCHAIN_AED_BLOCKCHAIN_H
#define BLOCKCHAIN_AED_BLOCKCHAIN_H
#include <iostream>
using namespace std;
class Blockchain{
private:

public:
    Blockchain() = default;
    ~Blockchain() = default;
    void create_genesis_block(){} // inicio
    void add_block(){} // verificar si es valido la prueba NONCE - Referencia al previo block
    void proof_of_work(){} // realiza la prueba de trabajo NONCE
    void is_valid_proof(){} // verificar que sea valido la prueba de trabajo
    void verify_chain(){} // verificar la cadena de bloques si es valido
    void add_transaccion(){}
    void minar(){} // agregar las transacciones averiguando la prueba de trabajo
    void print_blocks(){} // mostrar los bloques
    int getNumOfBlocks(){}
    string getLatestBlockHash(){} // obtenemos el previ hash

};
#endif //BLOCKCHAIN_AED_BLOCKCHAIN_H
