
#define TEST_2

#include <iostream>


#ifdef TEST_1
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

#endif // TEST_1



#ifdef TEST_2
#include <Windows.h>
#include <WinUser.h>

/* Button Control Styles */
// ...
//#define BS_VCENTER          0x00000C00L
//#define BS_PUSHLIKE         0x00001000L
//#define BS_MULTILINE        0x00002000L
//#define BS_NOTIFY           0x00004000L
// ...


void create_listbox(unsigned int listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    unsigned int listbox_styles = LBS_SORT | LBS_NOTIFY;
    listbox_styles = listbox_styles | LBS_MULTIPLESEL;
    listbox_styles &= ~LBS_SORT;

    create_listbox(listbox_styles);
}

#endif // TEST_2



#ifdef TEST_3
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

#endif // TEST_3



#ifdef TEST_4

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

#endif // TEST_4



#ifdef TEST_5

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

void create_listbox(lbs_enum listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void create_listbox_BROKEN(lbs_enum listbox_styles) {
    std::cout << "\ncreating broken listbox...";
    // creating ...
    if (listbox_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void create_button(bs_enum button_styles) {
    std::cout << "\ncreating button...";
    // creating ...
    if (button_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    lbs_enum listbox_styles = LBS_SORT | LBS_NOTIFY;
    listbox_styles = listbox_styles | LBS_MULTIPLESEL;
    listbox_styles &= ~LBS_SORT;

    bs_enum button_styles = BS_RADIOBUTTON | BS_MULTILINE;
    button_styles |= BS_NOTIFY;

    create_listbox(button_styles);
    create_listbox(listbox_styles);
    create_listbox_BROKEN(listbox_styles);
    create_button(button_styles);
    create_button(listbox_styles);
}

#endif // TEST_5



#ifdef TEST_6

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

void create_listbox(lbs_enum listbox_styles) {
    std::cout << "\ncreating listbox...";
    // creating ...
    if (listbox_styles & LBS_NOTIFY) { std::cout << " sending notification"; }
}

void create_listbox_BROKEN(lbs_enum listbox_styles) {
    std::cout << "\ncreating broken listbox...";
    // creating ...
    if (listbox_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void create_button(bs_enum button_styles) {
    std::cout << "\ncreating button...";
    // creating ...
    if (button_styles & BS_NOTIFY) { std::cout << " sending notification"; }
}

void test() {
    lbs_enum listbox_styles = static_cast<lbs_enum>(LBS_SORT | LBS_NOTIFY);
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

#endif // TEST_6



#ifdef TEST_7
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

#endif // TEST_6



int main()
{
    std::cout << "\ntest started\n";

    test();

    std::cout << "\n\ntest finished\n\n";
}
