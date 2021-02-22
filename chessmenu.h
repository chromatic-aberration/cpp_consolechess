#ifndef CHESSMENU
#define CHESSMENU

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <cstdlib>
#include <conio.h>


class Button;

class Menu{
    std::string header;
    std::vector<Button*> buttons;
public:
    Menu(std::string h = "");
    void open();
    void add_button(Button* b);

};

class Button{
    std::string text;
public:
    Button(std::string t = "", std::function<void()> f = nullptr);
    Button(std::string t = "", Menu *m = nullptr);

    std::string const get_text(){return text;};
    std::function<void()> trigger;
};

#endif
