#include <iostream>
#include <Windows.h>
#include <WinUser.h>

/* Listbox Styles */
// #define LBS_NOTIFY            0x0001L
// #define LBS_SORT              0x0002L
// #define LBS_NOREDRAW          0x0004L
// #define LBS_MULTIPLESEL       0x0008L
// ...

void create_listbox(unsigned int listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    // basic list-box options: sorting, notification
    unsigned int listbox_styles = LBS_SORT | LBS_NOTIFY;

    // add multi-selection
    listbox_styles = listbox_styles | LBS_MULTIPLESEL;

    // remove sorting
    listbox_styles &= ~LBS_SORT;

    create_listbox(listbox_styles);
}


int main()
{
    std::cout << "\ntest 1 started\n";

    test();

    std::cout << "\n\ntest 1 finished\n\n";
}
