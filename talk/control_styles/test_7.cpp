#include <iostream>
#include <type_traits>

enum class lbs_enum {
    LBS_NOTIFY = 0x0001L,
    LBS_SORT = 0x0002L,
    LBS_NOREDRAW = 0x0004L,
    LBS_MULTIPLESEL = 0x0008L,
};
using enum lbs_enum;

enum class bs_enum {
    BS_RADIOBUTTON = 0x00000004L,
    BS_MULTILINE = 0x00002000L,
    BS_NOTIFY = 0x00004000L,
};
using enum bs_enum;

void create_listbox(lbs_enum listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void create_listbox_BROKEN(lbs_enum listbox_styles) {
    std::cout << "\ncreating broken listbox...";
    // creating ...
    if (listbox_styles & BS_NOTIFY) { std::cout << " sending notification"; }
    //    if (static_cast<int>(listbox_styles) & static_cast<int>(BS_NOTIFY)) { std::cout << " sending notification"; }
}

void create_button(bs_enum button_styles) {
    std::cout << "\ncreating button...";
    // creating ...
    if (button_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    lbs_enum listbox_styles = static_cast<lbs_enum>(LBS_SORT | LBS_NOTIFY);
    //    lbs_enum listbox_styles = static_cast<lbs_enum>(static_cast<int>(LBS_SORT) | static_cast<int>(LBS_NOTIFY));

    listbox_styles = static_cast<lbs_enum>(listbox_styles | LBS_MULTIPLESEL);
    //    listbox_styles &= ~LBS_SORT;
    listbox_styles = static_cast<lbs_enum>(listbox_styles & ~LBS_SORT);

    bs_enum button_styles = static_cast<bs_enum>(BS_RADIOBUTTON | BS_MULTILINE);
    //    button_styles |= BS_NOTIFY;
    button_styles = static_cast<bs_enum>(button_styles | BS_NOTIFY);

    //     create_listbox(button_styles);
    create_listbox(listbox_styles);
    create_listbox_BROKEN(listbox_styles);
    create_button(button_styles);
    //     create_button(listbox_styles);
}


int main()
{
    std::cout << "\ntest 7 started\n";

    test();

    std::cout << "\n\ntest 7 finished\n\n";
}
