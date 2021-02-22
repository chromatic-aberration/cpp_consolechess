#include <iostream>
#include <string>
#include "chessmenu.h"
#include "chessmech.h"


#include <windows.h>
using namespace std;


int main()
{
    enum RETURNFLAG returnflag=NONE;

    Match* match;
    Match* matchloaded = Chessfile::load();


    Menu main("CONSOLE CHESS GAME by Michal Kucharz");
    Menu* game;
    if(matchloaded) game = new Menu("Match save detected: continue, or start new game?");
    else game = new Menu("Start new game?");
    Menu settings("CONSOLE CHESS GAME by Michal Kucharz");
    Menu about("CONSOLE CHESS GAME by Michal Kucharz\nObject-Oriented Programming Project\nversion 1.0\n");
    Menu quit("CONSOLE CHESS GAME by Michal Kucharz\nAre you sure you want to quit?");


    main.add_button(new Button("Start game", [game]{ game->open(); } ));
    //main.add_button(new Button("Settings", [&settings]{ settings.open(); } ));
    main.add_button(new Button("About", [&about]{ about.open(); } ));
    main.add_button(new Button("Quit", [&quit]{ quit.open(); } ));

    if(matchloaded) game->add_button(new Button("Continue", [&match, &matchloaded, &returnflag]{
                                               match = matchloaded;
                                               match->display();
                                               returnflag = match->gameLoop();
                                            }));
    game->add_button(new Button("New game", [&match, &returnflag]{
                                                match = new Match;
                                                match->initializeBoard();
                                                match->display();
                                                returnflag = match->gameLoop();
                                            } ));
    game->add_button(new Button("Go back", [&main]{ main.open(); } ));

    //settings.add_button(new Button("setting 1", []{ cout << '\n'; } ));
    //settings.add_button(new Button("setting 2", []{ cout << '\n'; } ));
    //settings.add_button(new Button("Go back", [&main]{ main.open(); } ));

    about.add_button(new Button("Go back", [&main]{ main.open(); } ));

    quit.add_button(new Button("Quit", [&returnflag]{ returnflag = QUIT; } ));
    quit.add_button(new Button("Cancel", [&main]{ main.open(); } ));


    while(true)
    {
        switch(returnflag) {
        case NONE:
            main.open();
            break;
        case RESTART:
            delete(match);
            match = new Match;
            match->initializeBoard();
            match->display();
            match->gameLoop();
            break;
        case MENU:
            matchloaded = Chessfile::load();
            delete(match);
            main.open();
            break;
        case QUIT:
            return 0;
        }
    }





    return 0;
}









