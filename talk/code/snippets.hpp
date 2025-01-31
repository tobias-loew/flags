
#include <WinUser.h>


/* Listbox Styles */
#define LBS_NOTIFY            0x0001L
#define LBS_SORT              0x0002L
#define LBS_NOREDRAW          0x0004L
#define LBS_MULTIPLESEL       0x0008L

// basic list-box options: sorting, notification
unsigned int opts = LBS_SORT | LBS_NOTIFY;
 
// add multi-selection
opts |= LBS_MULTIPLESEL;
 
// remove sorting
opts &= ~LBS_SORT;
 
// notifications wanted?
if (opts & LBS_NOTIFY) { ... }



