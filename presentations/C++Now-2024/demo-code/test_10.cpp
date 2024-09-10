#include <iostream>
#include <boost/flags.hpp>

enum class lbs_enum {
    LBS_NOTIFY = 0x0001L,
    LBS_SORT = 0x0002L,
    LBS_NOREDRAW = 0x0004L,
    LBS_MULTIPLESEL = 0x0008L,
};
BOOST_FLAGS_ENABLE_LOGICAL_AND(lbs_enum)




int main()
{
    //if (lbs_enum::LBS_NOTIFY) {

    //}

    std::cout << "\ntest 8 started\n";


    std::cout << "\n\ntest 8 finished\n\n";
}