#include <iostream>
#include "controller.h"

int main()
{
    controller con(std::make_unique<model>("EMAIL_SERVICE", "user", "user_pass"),
                   std::make_unique<view>());
    con.contr_startMenu();

    return 0;
}
