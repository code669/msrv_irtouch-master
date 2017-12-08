#include "msrv_irtouch.h"
#include <iostream>
using namespace std;

int main()
{
    MSRV_IRTOUCH::GetInstance()->start();
    return 0;
}

