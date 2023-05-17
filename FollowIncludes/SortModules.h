#pragma once

#include <memory>
#include <algorithm>
#include <list>
#include "TopologicalSort.h"

#if 0
class SortModules
{
	std::vector<std::shared_ptr<Vertex>>& all_vertices;
	std::vector<std::shared_ptr<Module>>& zero_vertices;
public:
	SortModules(std::vector<std::shared_ptr<Vertex>>&  all_vertices, std::vector<std::shared_ptr<Module>>& zero_vertices) :
		all_vertices(all_vertices), zero_vertices(zero_vertices)
	{

	}
	std::list<std::shared_ptr<Vertex>> find(std::shared_ptr<Vertex>& one, std::shared_ptr<Vertex>& two)
	{
		// find in one->dependencies if two appears in it
		auto ret = std::find_if(one->dependencies.begin(), one->dependencies.end(), [&two](std::shared_ptr<Vertex>& vertex)
			{
				return two == vertex;
			});
		//
		if (ret != one->dependencies.end())
		{
			// 
		}
		std::list<std::shared_ptr<Vertex>> lis{ one,two };
		return lis;
	}
	void Sort()
	{
		for (auto& mod: zero_vertices )
		{
			std::cout << mod->name << std::endl;
		}
	}

};

#endif
