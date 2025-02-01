#include <iostream>
#include <Windows.h>
#include <WinUser.h>

void create_listbox(unsigned int listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void create_button(unsigned int button_styles) {
    std::cout << "\ncreating button...";
    // creating ...
    if (button_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    unsigned int listbox_styles = LBS_SORT | LBS_NOTIFY;
    listbox_styles = listbox_styles | LBS_MULTIPLESEL;
    listbox_styles &= ~LBS_SORT;

    unsigned int button_styles = BS_RADIOBUTTON | BS_MULTILINE;
    button_styles |= BS_NOTIFY;

    create_listbox(button_styles);
    create_button(listbox_styles);
}


int main()
{
    std::cout << "\ntest 3 started\n";

    test();

    std::cout << "\n\ntest 3 finished\n\n";
}
