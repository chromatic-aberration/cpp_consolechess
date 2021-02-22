#ifndef CHESSMECH
#define CHESSMECH

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <conio.h>
#include <stack>



struct Coords {
    char x;
    char y;
    bool operator==(const Coords&);
    bool operator!=(const Coords&);
};
char to_char(Coords p);
Coords to_Coords(char c);

enum RETURNFLAG{ NONE, MENU, RESTART, QUIT};
enum DIRECTION { UP='w', LEFT='a', DOWN='s', RIGHT='d'};
enum COLOR { WHITE, BLACK };
enum FIG { EMPTY=0, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING};
const char WHITEFIGS[7] = {' ','p','b','k','r','q','x'};
const char BLACKFIGS[7] = {' ','P','B','K','R','Q','X'};

//forward declarations
class Figure;
class Undo;
class Board;
class Match;

class Chessfile {

public:
    static void save(Match* match);
    static Match* load();

};

class Board {
    std::map<char,Figure*> pieces;
    Figure* selected = nullptr;
    bool flip = false;
public:
    friend void Chessfile::save(Match* match);
    bool getFlip() const {return flip;}
    void setFlip(bool f) {flip = f;}
    void select(Coords pos);
    FIG check(Coords pos);
    char fig(Coords pos);
    Figure* getSelected() {return selected;};
    Figure* figAddr(Coords pos);
    void addFig(Coords p, bool c, FIG t); //position, color
    void eraseFig(Figure* fig);
    void clearBoard();
    void mvFig(Figure* fig, Coords pos);

};



class Match {
    Board* board = new Board;
    bool current_player = WHITE;
    Coords pointer = {-1,-1};
    std::string whitetable = " ";
    std::string blacktable = " ";
    bool match_saved = false;
public:
    friend void Chessfile::save(Match* match);
    friend Match* Chessfile::load();
    Match();
    void appendWhitetable(std::string s) {whitetable.append(s);}
    void appendBlacktable(std::string s) {blacktable.append(s);}
    void undoWhitetable() {whitetable.erase(whitetable.size() - 2, whitetable.size() - 1);}
    void undoBlacktable() {blacktable.erase(blacktable.size() - 2, blacktable.size() - 1);}
    void initializeBoard();
    void display(char selected_row = -1);
    void addFig(Coords p, bool c, FIG t) {board->addFig(p,c,t);}
    enum RETURNFLAG gameLoop();
    void setPointer(Coords pos) {pointer = pos;}
    void setSaved(bool ms) {match_saved = ms;}
    void clearPointer() {pointer = {-1,-1};}
    void movePointer(char dir);
    void flipPlayer() {current_player = !current_player;}
    bool getPlayer() const {return current_player;}
};






class Figure{
protected:
    const FIG type;
    Board* board;
    Coords pos;
    bool color;
public:
    virtual bool validate(Coords p) = 0;
    Figure(Board* brd, Coords p, bool c, FIG t);
    ~Figure();
    bool mv(Coords p, Match* m, bool undoable=false);
    //void select();
    void setPos(Coords p) {pos=p;}
    FIG getType() const {return type;}
    Coords getPos() const {return pos;}
    bool getColor() const {return color;}
    char getFig() const;
};

//---FIGURES---------
class Pawn : public Figure {
public:
    bool validate(Coords p);
    Pawn(Board* brd, Coords p, bool c);

};

class Bishop : public Figure {
public:
    bool validate(Coords p);
    Bishop(Board* brd, Coords p, bool c);
};

class Knight : public Figure {
public:
    bool validate(Coords p);
    Knight(Board* brd, Coords p, bool c);
};

class Rook : public Figure {
public:
    bool validate(Coords p);
    Rook(Board* brd, Coords p, bool c);
};

class Queen : public Figure {
public:
    bool validate(Coords p);
    Queen(Board* brd, Coords p, bool c);
};

class King : public Figure {
public:
    bool validate(Coords p);
    King(Board* brd, Coords p, bool c);
};
//------------------

class Undo {
    static std::stack<Undo*> undostack;
    static Match* match;

    Figure* fig;
    Coords prev_pos;
    enum FIG taken_piece;
public:
    Undo(Figure* f, Coords prev, enum FIG taken);
    static void setMatch(Match* m) {match=m;}
    static bool undo();
    static void clearUndo();
};




#endif
