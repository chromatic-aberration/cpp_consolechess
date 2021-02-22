#include "chessmech.h"


using namespace std;


char to_char(Coords p) {return 1+ p.y*8 + p.x; }
Coords to_Coords(char c) { return {char((c-1)/8), (c-1)%8}; }
bool Coords::operator==(const Coords& p) {return ((this->x==p.x)&&(this->y==p.y));}
bool Coords::operator!=(const Coords& p) {return ((this->x!=p.x)||(this->y!=p.y));}

//------FIGURE-CONSTRUCTORS---------
Figure::Figure(Board* brd, Coords p, bool c, FIG t)
    : board(brd), pos(p), color(c), type(t)
{}
Figure::~Figure() {
    this->board->eraseFig(this);
}

Pawn::Pawn(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,PAWN)
{}
Bishop::Bishop(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,BISHOP)
{}
Knight::Knight(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,KNIGHT)
{}
Rook::Rook(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,ROOK)
{}
Queen::Queen(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,QUEEN)
{}
King::King(Board* brd, Coords p, bool c)
    : Figure(brd,p,c,KING)
{}
//----------------------------------



//------FIGURE----------------------

char Figure::getFig() const{
    return color ? BLACKFIGS[type] : WHITEFIGS[type];
}
bool Figure::mv(Coords p, Match *m, bool undoable){
    enum FIG taken = EMPTY;
    string s = " ";
    if ((this->getPos() != p) && this->validate(p)) {
        m->setSaved(false);
        if (board->check(p)) { // was any enemy figure taken?

                if(board->figAddr(p)->getColor()) m->appendBlacktable(s + board->figAddr(p)->getFig());// + board->figAddr(p)->getFig());
            else m->appendWhitetable(s + board->figAddr(p)->getFig());
            cout << "owo " + board->figAddr(p)->getFig();
            taken=board->figAddr(p)->getType();
            delete(board->figAddr(p));
        }
        if(undoable) new Undo(this,this->getPos(),taken);
        board->mvFig(this, p);
        this->setPos(p);
        return true;
    }


    return false;
}


//----------------------------------

//------VALIDATE--------------------
bool Pawn::validate(Coords p){
    return 1;
}
bool Bishop::validate(Coords p){
    return 1;
}
bool Knight::validate(Coords p){
    return 1;
}
bool Rook::validate(Coords p){
    return 1;
}
bool Queen::validate(Coords p){
    return 1;
}
bool King::validate(Coords p){
    return 1;
}
//----------------------------------
FIG Board::check(Coords pos){
    return (pieces.find(to_char(pos)) != pieces.end()) ? pieces.at(to_char(pos))->getType() : EMPTY;
}
char Board::fig(Coords pos){
    return check(pos) ? pieces.at(to_char(pos))->getFig() : ' ';
}Figure* Board::figAddr(Coords pos){
    return check(pos) ? pieces.at(to_char(pos)) : nullptr;
}
void Board::eraseFig(Figure* fig) {
    pieces.erase(to_char(fig->getPos()));
}
void Board::mvFig(Figure* fig, Coords pos) {
    eraseFig(fig);
    pieces.insert(pair<char,Figure*>(to_char(pos),fig));
}
void Board::addFig(Coords p, bool c, FIG t){
    switch(t){
        case PAWN:
            pieces.insert(pair<char,Figure*>(to_char(p),new Pawn(this,p,c)));
            break;
        case BISHOP:
            pieces.insert(pair<char,Figure*>(to_char(p),new Bishop(this,p,c)));
            break;
        case ROOK:
            pieces.insert(pair<char,Figure*>(to_char(p),new Rook(this,p,c)));
            break;
        case KNIGHT:
            pieces.insert(pair<char,Figure*>(to_char(p),new Knight(this,p,c)));
            break;
        case QUEEN:
            pieces.insert(pair<char,Figure*>(to_char(p),new Queen(this,p,c)));
            break;
        case KING:
            pieces.insert(pair<char,Figure*>(to_char(p),new King(this,p,c)));
    }
}
void Board::clearBoard(){
    for (auto const x : pieces) delete x.second;
    pieces.clear();
}
void Board::select(Coords pos){
    selected = this->figAddr(pos);
}

//----------------------------------

Match::Match() {Undo::setMatch(this);}

void Match::display(char selected_row){
    system("CLS");

    char spacer1, spacer2, selected1, selected2, selected = 0xB1;
    if (!board->getFlip()) {
        spacer1 = 0xFF;
        spacer2 = 0xDB;
        selected1 = 0xB0;
        selected2 = 0xB2;
    }
    else {
        spacer1 = 0xDB;
        spacer2 = 0xFF;
        selected1 = 0xB2;
        selected2 = 0xB0;
    }

    cout << (board->getFlip() ? "    h    g    f    e    d    c    b    a   | " : "    a    b    c    d    e    f    g    h   | ") << endl;
    Coords figpos;
    for(int i=0; i<24; i++) {
        for(int j=0; j<8; j++){

            if (!j) {
                if(!((i-1)%3)) cout << (board->getFlip() ? 1+(i/3) : 8-(i/3)) << (char)0xFF;
                else cout << (char)0xFF << (char)0xFF;
            }

            figpos = board->getFlip() ? (struct Coords){(i/3), 7-j} : (struct Coords){7-(i/3), j}; //for some reason ternary op. with struct reqruires typecasting   // ALTERNATIVELY to_Coords(56-(8*((i+1)/3))+j+1);

            if (selected_row == figpos.x) { //(board->getSelected() && ((board->getSelected()->getPos().x) == figpos.x)) {
                if ((j+i/3)%2){cout << (char)selected1 << (char)selected1 << ((!((i-1)%3) && board->check(figpos)) ? board->fig(figpos) : ((!((i-2)%3) && board->check(figpos) && board->figAddr(figpos)->getColor() == current_player) ? char(49+figpos.y) : (char)selected1) ) << (char)selected1 << (char)selected1;}
                else {cout << (char)selected2 << (char)selected2 << ((!((i-1)%3) && board->check(figpos)) ? board->fig(figpos) : ((!((i-2)%3) && board->check(figpos) && board->figAddr(figpos)->getColor() == current_player) ? char(49+figpos.y) : (char)selected2) ) << (char)selected2 << (char)selected2;}
            }
            else {
                if ((pointer == figpos) || (board->getSelected() && (board->getSelected()->getPos() == figpos))) { //((board->getSelected()->getPos().x) == figpos.x) && ((board->getSelected()->getPos().y) == figpos.y))) {
                    cout << (char)selected << (char)selected << ((!((i-1)%3) && board->check(figpos)) ? board->fig(figpos) : (char)selected) << (char)selected << (char)selected;
                }
                else {
                    if ((j+i/3)%2){cout << (char)spacer1 << (char)spacer1 << ((!((i-1)%3) && board->check(figpos)) ? board->fig(figpos) : (char)spacer1) << (char)spacer1 << (char)spacer1;}
                    else {cout << (char)spacer2 << (char)spacer2 << ((!((i-1)%3) && board->check(figpos)) ? board->fig(figpos) : (char)spacer2) << (char)spacer2 << (char)spacer2;}
                }

            }

            if (j==7) {
              cout << " | ";
              switch(i) {
                case 0:
                    cout << "   PLAYER " << ((current_player) ? "BLACK" : "WHITE") << " TURN";
                    break;
                case 2:
                    cout << "White pieces taken:" << whitetable;
                    break;
                case 3:
                    cout << "Black pieces taken:" << blacktable;
                    break;
                case 4:
                    cout << "Match saved: " << (match_saved ? "true" : "false");
                    break;
                case 7:
                    cout << "   INSTRUCTIONS: ";
                    break;
                case 8:
                    cout << "Use digits 1-8 to first select row, then column";
                    break;
                case 9:
                    cout << "Use w-a-s-d to move pointer and confirm with spacebar";
                    break;
                case 11:
                    cout << "c: cancel selection";
                    break;
                case 12:
                    cout << "f: flip board";
                    break;
                case 13:
                    cout << "q: quit";
                    break;
                case 14:
                    cout << "o: save";
                    break;
                case 15:
                    cout << "r: restart";
                    break;
              }
            }
        }
        cout << endl;
    }
    if (board->getSelected()) cout << "Selected: " << board->getSelected()->getFig() << " on " << char(board->getSelected()->getPos().y + 97) << char(board->getSelected()->getPos().x + 49) << endl;
    if (pointer.x != -1 && pointer.y != -1 && pointer != board->getSelected()->getPos()) cout << "Move: " << ((board->getSelected()->validate(pointer)) ? "VALID" : "INVALID") << endl;
}

void Match::initializeBoard(){
    FIG secondrow[8] = {ROOK,KNIGHT,BISHOP,QUEEN,KING,BISHOP,KNIGHT,ROOK};
    board->clearBoard();
    for(int i=0;i<8;i++){
        board->addFig(to_Coords(i+9),WHITE,PAWN);
        board->addFig(to_Coords(i+49),BLACK,PAWN);
        board->addFig(to_Coords(i+1),WHITE,secondrow[i]);
        board->addFig(to_Coords(i+57),BLACK,secondrow[i]);
    }
}

enum RETURNFLAG Match::gameLoop() {
    char k;
    bool fig_found = false;
    char selected_row;
    while(true){
        k = getch();
        if(!board->getSelected()) {
            if(k>='1' && k<='8') {
                for (int i=0; i<8; i++){
                    if (board->check({k-49,i}) && ((board->figAddr({k-49,i})->getColor()) == current_player)) {
                        fig_found = true;
                        this->display(k-49);
                        break;
                    }
                }
                if (fig_found)
                {
                    selected_row = k-49;
                    k = getch();
                    if(k>='1' && k<='8') {
                        if (board->check({selected_row,k-49}) && (board->figAddr({selected_row,k-49})->getColor() == current_player)) {
                            board->select({selected_row,k-49});
                            setPointer({selected_row,k-49});
                        }
                    }
                    this->display();
                }
            }
            else {
                switch(k){
                    case 'f':
                        board->setFlip(!board->getFlip());
                        this->display();
                        break;
                    case 'u':
                        if (Undo::undo()) this->display();
                        break;
                    case 'o':
                        Chessfile::save(this);
                        match_saved = true;
                        this->display();
                        break;
                    case 'q':
                        cout << endl << "Are you sure you want to quit? [y/n]";
                        do {
                            k = getch();
                            if (k == 'y') return MENU;
                            else if (k == 'n') { this->display(); break;}

                        } while (true);
                        break;
                    case 'r':
                        cout << endl << "Are you sure you want to restart? [y/n]";
                        do {
                            k = getch();
                            if (k == 'y') return RESTART;
                            else if (k == 'n') { this->display(); break;}

                        } while (true);
                        break;;
                }
            }
            fig_found = false;
        }
        else {
            if(k==UP || k==LEFT || k==DOWN || k==RIGHT) {
                movePointer(k);
                this->display();
            }
            switch(k) {
                case ' ': //CONFIRM MOVE
                    if (board->getSelected()->mv(pointer, this, true)) { //IF MOVE WAS SUCCESFUL
                        board->select({-1,-1});
                        pointer={-1,-1};
                        current_player = !current_player;
                        this->display();
                    }
                    break;
                case 'c': //CANCEL POINTER
                    pointer={-1,-1};
                    board->select({-1,-1});
                    this->display();
                    break;
            }
        }
    }
}


void Match::movePointer(char dir) {
    switch(dir) {
        case UP:
            pointer.x = ((pointer.x+1-(board->getFlip()*2)) + 8) % 8;
            break;
        case DOWN:
            pointer.x = ((pointer.x-1+(board->getFlip()*2)) + 8) % 8;
            break;
        case LEFT:
            pointer.y = ((pointer.y-1+(board->getFlip()*2)) + 8) % 8;
            break;
        case RIGHT:
            pointer.y = ((pointer.y+1-(board->getFlip()*2)) + 8) % 8;
            break;

    }
}



//--------------------UNDO--------------

stack<Undo*> Undo::undostack;
Match* Undo::match;

Undo::Undo(Figure* f, Coords prev, enum FIG taken)
    : fig(f), prev_pos(prev), taken_piece(taken)
{ Undo::undostack.push(this); }

bool Undo::undo() {
    Coords pos;
    if(undostack.size()) {
        Undo* u = undostack.top();
        pos = u->fig->getPos();
        match->flipPlayer();
        u->fig->mv(u->prev_pos, match);
        if(u->taken_piece) {
            match->addFig(pos,!(u->fig->getColor()),u->taken_piece);
            if(match->getPlayer()) match->undoWhitetable();
            else match->undoBlacktable();
        }

        undostack.pop(); //pop should call destructor //delete(u);
        return true;
    }
    else return false;

}

void Undo::clearUndo() {
    while (!undostack.empty()) {
        undostack.pop();
    }
}


//------------

void Chessfile::save(Match* match) {
    map<char,Figure*>* pieces = &(match->board->pieces);
    char filename[] = "chess_save";
        ofstream file(filename, ofstream::trunc);
        file << "chess_save 1.0\n"
    << match->whitetable << '\n' << match->blacktable << '\n'
    << "player:" << match->current_player << '\n';
    for (auto const x : *pieces ) {
        file << "fig:" << int(x.second->getPos().x) << " " << int(x.second->getPos().y) << " "
        << x.second->getColor() << " " << x.second->getType() << '\n';
    }
    file.close();
}

Match* Chessfile::load() {
    Match* match = new Match;
    string line;
    int i = 0;
    char filename[] = "chess_save";
    ifstream file("chess_save");
    if(file.fail()) return nullptr;;
    while(getline(file,line)) {
        if (line == "\n") break;
        switch(i) {
        case 0:
            if (line != "chess_save 1.0") return nullptr;
            break;
        case 1:
            match -> whitetable = line;
            break;
        case 2:
            match -> blacktable = line;
            break;
        case 3:
            if (line.at(7)-48) match->flipPlayer();
        default:
            if (line.substr(0,4) == "fig:") {
                match->addFig({char(line.at(4)-48),char(line.at(6)-48)},bool(line.at(8)-48),FIG(line.at(10)-48));
            }
        }
        i++;
    }
    return match;



}



