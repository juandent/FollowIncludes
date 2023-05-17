// FindDependencies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>

#include "DirectoryExplorer.h"
#include "SortModules.h"
#include "Tests.h"




int main(int argc, char* argv[])
{

    if (argc < 3)   return -1;

    auto directory_dir = argv[1];
    bool verbose = argv[2][0] == '1';


    DirectoryExplorer explorer{verbose};
    explorer.Execute(directory_dir);

#if 0
    TopologicalSort sorter;


    for (auto& mod : Include::all_Includes)
    {
        // is mod in Vertex::all_vertices
        auto ret = Vertex::find(mod);
        assert(ret.get() != nullptr);
    }

    sorter.Execute();
    sorter.PrintOrderedVertices();
    sorter.PrintImportExportAll();
    sorter.Verify();
    //sorter.Save(Compiler::Microsoft, directory_dir, destination_dir);
#endif
}

