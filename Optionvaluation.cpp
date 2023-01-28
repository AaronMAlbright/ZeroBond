

#include "ql/qldefines.hpp"
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif

#include <iostream>
#include "RAtes.h"
#include "NPV.h"



using namespace std;


int main() {

        cout<< "____________________________________________________" << endl;
        cout << "Zero coupon bond yield curve:" << endl;
        market();
        cout << "Choose date for option contract: " << endl;
        NPV();
    return 0;
}

