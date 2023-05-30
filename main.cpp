#include "Blockchain.h"

int main(){
    CircularArray<Transaction> g;
    g.push_back(Transaction("Juan","Redt",22));
    g.push_back(Transaction("Luca","Juan",11));
    g.push_back(Transaction("Redt","Luca",34));
    g.push_back(Transaction("Juan d","Pier",123));
    g.push_back(Transaction("Juan","Redt",433));
    g.push_back(Transaction("Luca","Juan",333));
    g.push_back(Transaction("Redt","Luca",55));
    g.push_back(Transaction("Juan d","Pier",77));
    Blockchain<Transaction> bloque1(g);
    //bloque1.add_block(s);
    //bloque1.add_block(p);
    //bloque1.add_block(g);
    bloque1.printBlock();
    bloque1.printMaxHeapBlock();
    //bloque1.printBlock();
}