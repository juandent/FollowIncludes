#pragma once



#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <format>

#include "printer.h"

enum class IncludeType
{
	Root,
	Child
};


class Include :  public std::enable_shared_from_this<Include>
{

	std::vector<std::shared_ptr<Include>> duplicate_dependencies;
	inline static std::vector<std::shared_ptr<Include>> module_seen;
public:
	std::vector<std::shared_ptr<Include>> dependencies;
	inline static std::vector<std::shared_ptr<Include>> all_roots;
	inline static std::vector<std::shared_ptr<Include>> all_Includes;

	static void print(std::shared_ptr<Include> ancestor, std::shared_ptr<Include> mod, std::shared_ptr<Include> root)
	{
		std::cout << "Ancestor '" << ancestor->name << "'  module '" << mod->name << "'  root '" << root->name << "'" << std::endl;
	}
	bool operator==(std::shared_ptr<Include> another)
	{
		return another->name == this->name;
	}

	std::shared_ptr<Include> getptr()
	{
		return shared_from_this();
	}
private:
	// ensure name is a Include in all_Includes
	static std::shared_ptr<Include> ensure_include(std::string name)
	{
		auto ret = std::find_if(all_Includes.begin(), all_Includes.end(),
			[&name](std::shared_ptr<Include> potential_include)
			{
				return potential_include->name == name;
			});
		if (ret == all_Includes.end())
		{
			auto m = std::shared_ptr<Include>(new Include{ name });
			all_Includes.push_back(m);
			return m;
		}
		else
		{
			return *ret;
		}
	}
	bool marked_open = false;
public:
	static bool is_root(const std::shared_ptr<Include>& potential_root)
	{
		auto ret = std::find_if(all_roots.begin(), all_roots.end(), [&potential_root](std::shared_ptr<Include>& root)
			{
				return root->name == potential_root->name;
			});
		return ret != all_roots.end();
	}

	static bool exists(std::string name)
	{
		auto ret = std::find_if(all_Includes.begin(), all_Includes.end(),
			[&name](std::shared_ptr<Include> potential_include)
			{
				return potential_include->name == name;
			});
		
		return ret != all_Includes.end();
	}

	static std::shared_ptr<Include> find_or_create(std::string name, IncludeType type )
	{
		auto m = ensure_include(name);

		if( type== IncludeType::Root)
		{
			bool is_a_root = is_root(m);

			if( ! is_a_root ) //not found
			{
				all_roots.push_back(m);
			}
		}
		return m;
	}
	void addDependency(std::string name)
	{
		auto module = Include::find_or_create(name, IncludeType::Child);
		bool exists = exists_already(module);
		if (!exists)
		{
			dependencies.push_back(module);
		}
		else
		{
			duplicate_dependencies.push_back(module);
		}
	}
	bool has_been_opened()
	{
		return marked_open;
	}
	void mark_opened()
	{
		marked_open = true;
	}
	void mark_processed()
	{
		marked_processed = true;
	}

	void print(int depth=0)
	{
		using namespace std;
		auto tab = print_tab(depth);
		cout << boolalpha;
		cout << tab << name << endl;					//  << " is root " << is_root(this->getptr()) << endl;
		if (marked_processed && depth >0 )	return;
		++depth;
		ranges::for_each(dependencies, [&depth](const auto& module)
			{
				auto name = module->name;
				if (module->name == "alias")
				{
					int i = 0;
				};
				module->print(depth);
			});
	}
	const std::string name;
private:

	bool marked_processed = false;

	Include(std::string n) : name{n} {}

	// in dependencies
	bool exists_already(std::shared_ptr<Include> candidate)
	{
		auto ret = std::find_if(dependencies.begin(), dependencies.end(), 
			[&candidate](const std::shared_ptr<Include>& potential_dependency)
			{
				return potential_dependency == candidate;
			});
		return ret != dependencies.end();
	}

};

