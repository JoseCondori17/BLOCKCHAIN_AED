#ifndef BLOCKCHAIN_AED_BLOCK_H
#define BLOCKCHAIN_AED_BLOCK_H
#include <chrono>
#include <string>
using namespace std;

class Block{
private:
    /*
     * time : time_t
     * transactions : information
     * previus_hash : string ? link
     * nonce : num -> 5 zeros 00000...sfrsf4rasf23fdfas
     * hash : string
     */
public:
    Block() = default;
    ~Block() = default;
    void generar_hash(){}
    void toString(){}
    void print_hash(){}
    string getPreviousHash(){}
    string getHash(){}
    int getIndex(){}
};
#endif //BLOCKCHAIN_AED_BLOCK_H
