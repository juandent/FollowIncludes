#pragma once


#include <string>

inline std::string print_tab(int depth)
{
	std::string tabbed_str;
	for (auto count = 0; count < depth; ++count)
	{
		tabbed_str += "\t";
	}
	return tabbed_str;
}

