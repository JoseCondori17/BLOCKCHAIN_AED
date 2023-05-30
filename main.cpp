#include "Blockchain.h"

int main(){
    ForwardList<Transaction> t;
    t.push_back(Transaction("Juan","Redt",2500));
    t.push_back(Transaction("Luca","Juan",1655));
    t.push_back(Transaction("Redt","Luca",2333));
    t.push_back(Transaction("Juan d","Pier",16));
    t.push_back(Transaction("Juan","Redt",2500));
    t.push_back(Transaction("Luca","Juan",1655));
    t.push_back(Transaction("Redt","Luca",2333));
    t.push_back(Transaction("Juan d","Pier",16));
    ForwardList<Transaction> s;
    s.push_back(Transaction("Juan","Redt",2500));
    s.push_back(Transaction("Luca","Juan",1655));
    s.push_back(Transaction("Redt","Luca",2333));
    s.push_back(Transaction("Juan d","Pier",16));
    s.push_back(Transaction("Juan","Redt",2500));
    s.push_back(Transaction("Luca","Juan",1655));
    s.push_back(Transaction("Redt","Luca",2333));
    s.push_back(Transaction("Juan d","Pier",16));
    ForwardList<Transaction> p;
    p.push_back(Transaction("Juan","Redt",22));
    p.push_back(Transaction("Luca","Juan",11));
    p.push_back(Transaction("Redt","Luca",34));
    p.push_back(Transaction("Juan d","Pier",12341));
    p.push_back(Transaction("Juan","Redt",433));
    p.push_back(Transaction("Luca","Juan",333));
    p.push_back(Transaction("Redt","Luca",55));
    p.push_back(Transaction("Juan d","Pier",77));
    Blockchain bloque1(t);
    bloque1.add_block(s);
    bloque1.add_block(p);
    bloque1.printBlock();
    //bloque1.printBlock();
}