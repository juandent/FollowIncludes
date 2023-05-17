// FindDependencies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>

#include "DirectoryExplorer.h"
// #include "SortModules.h"
// #include "Tests.h"




int main(int argc, char* argv[])
{

    if (argc < 3)   return -1;

    auto directory_dir = argv[1];
    bool verbose = argv[2][0] == '1';


    DirectoryExplorer explorer{verbose};
    explorer.Execute(directory_dir);
}

