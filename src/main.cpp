#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <wex.h>
#include "cGUI.h"
#include <cxy.h>
#include "cPile.h"

cPile thePile; // the pile of string

main()
{
    // process input data file
    thePile.process("../dat/data1.txt");

    // construct the GUI
    cGUI theGUI;

    return 0;
}
