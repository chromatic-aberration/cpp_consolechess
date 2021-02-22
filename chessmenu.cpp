#include "chessmenu.h"


using namespace std;

Button::Button(string t, function<void()> f)
    :   text(t), trigger(f)
{}
Button::Button(string t, Menu *m)
    :   text(t), trigger([m]{m -> open();})
{}

Menu::Menu(string h)
    :   header((h == "") ? h : h+'\n')
{}

void Menu::open(){
    if (buttons.empty()) return;
    system("CLS");
    cout << header;
    int i = 0;
    for(auto x: buttons) { cout << i+1 << ". " << x -> get_text() << endl; i++; }
    char k;
    while (true) {
        k = getch();
        if (k > 48 && k <= 48+ buttons.size()) break;
    }
    buttons.at(k-49) -> trigger();
}
void Menu::add_button(Button* b){
    buttons.push_back(b);
}
