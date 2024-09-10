#include <iostream>

enum lbs_enum {
    LBS_NOTIFY = 0x0001L,
    LBS_SORT = 0x0002L,
    LBS_NOREDRAW = 0x0004L,
    LBS_MULTIPLESEL = 0x0008L,
};

enum bs_enum {
    BS_RADIOBUTTON = 0x00000004L,
    BS_MULTILINE = 0x00002000L,
    BS_NOTIFY = 0x00004000L,
};

void create_listbox(unsigned int listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void create_listbox_BROKEN(unsigned int listbox_styles) {
    std::cout << "\ncreating broken listbox...";
    // creating ...
    if (listbox_styles & BS_NOTIFY) { std::cout << " sending notification"; }
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
    create_listbox(listbox_styles);
    create_listbox_BROKEN(listbox_styles);
    create_button(button_styles);
    create_button(listbox_styles);
}


int main()
{
    std::cout << "\ntest 4 started\n";

    test();

    std::cout << "\n\ntest 4 finished\n\n";
}
