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
    CircularArray<Transaction> p;
    p.push_back( Transaction("Juan","Redt",22));
    p.push_back( Transaction("Luca","Juan",11));
    p.push_back( Transaction("Redt","Luca",34));
    p.push_back( Transaction("Juan d","Pier",12341));
    p.push_back( Transaction("Juan","Redt",433));
    p.push_back( Transaction("Luca","Juan",333));
    p.push_back( Transaction("Redt","Luca",55));
    p.push_back( Transaction("Juan d","Pier",77));
    CircularArray<Transaction> s;
    s.push_back( Transaction("Juan","Redt",2500));
    s.push_back( Transaction("Luca","Juan",1655));
    s.push_back( Transaction("Redt","Luca",2333));
    s.push_back( Transaction("Juan d","Pier",16));
    s.push_back( Transaction("Juan","Redt",2500));
    s.push_back( Transaction("Luca","Juan",1655));
    s.push_back( Transaction("Redt","Luca",2333));
    s.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> t;
    t.push_back( Transaction("Juan","Redt",2500));
    t.push_back( Transaction("Luca","Juan",1655));
    t.push_back( Transaction("Redt","Luca",2333));
    t.push_back( Transaction("Juan d","Pier",16));
    t.push_back( Transaction("Juan","Redt",2500));
    t.push_back( Transaction("Luca","Juan",1655));
    t.push_back( Transaction("Redt","Luca",2333));
    t.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> h;
    h.push_back( Transaction("Juan","Redt",2500));
    h.push_back( Transaction("Luca","Juan",1655));
    h.push_back( Transaction("Redt","Luca",2333));
    h.push_back( Transaction("Juan d","Pier",16));
    h.push_back( Transaction("Juan","Redt",2500));
    h.push_back( Transaction("Luca","Juan",1655));
    h.push_back( Transaction("Redt","Luca",2333));
    h.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> ts;
    ts.push_back( Transaction("Juan","Redt",2500));
    ts.push_back( Transaction("Luca","Juan",1655));
    ts.push_back( Transaction("Redt","Luca",2333));
    ts.push_back( Transaction("Juan d","Pier",16));
    ts.push_back( Transaction("Juan","Redt",2500));
    ts.push_back( Transaction("Luca","Juan",1655));
    ts.push_back( Transaction("Redt","Luca",2333));
    ts.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> ty;
    ty.push_back( Transaction("Juan","Redt",2500));
    ty.push_back( Transaction("Luca","Juan",1655));
    ty.push_back( Transaction("Redt","Luca",2333));
    ty.push_back( Transaction("Juan d","Pier",16));
    ty.push_back( Transaction("Juan","Redt",2500));
    ty.push_back( Transaction("Luca","Juan",1655));
    ty.push_back( Transaction("Redt","Luca",2333));
    ty.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> ui;
    ui.push_back( Transaction("Juan","Redt",2500));
    ui.push_back( Transaction("Luca","Juan",1655));
    ui.push_back( Transaction("Redt","Luca",2333));
    ui.push_back( Transaction("Juan d","Pier",16));
    ui.push_back( Transaction("Juan","Redt",2500));
    ui.push_back( Transaction("Luca","Juan",1655));
    ui.push_back( Transaction("Redt","Luca",2333));
    ui.push_back( Transaction("Juan d","Pier",16));
    CircularArray<Transaction> ree;
    ree.push_back( Transaction("Juan","Redt",4312431));
    ree.push_back( Transaction("Luca","Juan",23423));
    ree.push_back( Transaction("Redt","Luca",43242));
    ree.push_back( Transaction("Juan d","Pier",23423));
    ree.push_back( Transaction("Juan","Redt",23423));
    ree.push_back( Transaction("Luca","Juan",433));
    ree.push_back( Transaction("Redt","Luca",978987));
    ree.push_back( Transaction("Juan d","Pier",777));
    CircularArray<Transaction> qq;
    qq.push_back( Transaction("Juan","Redt",423));
    qq.push_back( Transaction("Luca","Juan",33));
    qq.push_back( Transaction("Redt","Luca",121));
    qq.push_back( Transaction("Juan d","Pier",11));
    qq.push_back( Transaction("Juan","Redt",2500));
    qq.push_back( Transaction("Luca","Juan",1655));
    qq.push_back( Transaction("Redt","Luca",2333));
    qq.push_back( Transaction("Juan d","Pier",16));
    Blockchain<Transaction> bloque1(g);
    bloque1.add_block(s);
    bloque1.add_block(p);
    bloque1.add_block(t);
    bloque1.add_block(ts);
    bloque1.add_block(h);
    bloque1.add_block(ty);
    bloque1.add_block(ui);
    bloque1.add_block(ree);
    bloque1.add_block(qq);

    //bloque1.printBlockchain();
    bloque1.printLevels();
    bloque1.printMaxHeapBlock();
    bloque1.printMinHeapBlock();
    //bloque1.add_block(g);
    //bloque1.printBlock();
    //bloque1.printMaxHeapBlock();
    //bloque1.printBlock();
}
