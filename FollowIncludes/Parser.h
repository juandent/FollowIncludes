#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <exception>

#include "Include.h"

using namespace std::literals;


class Parser
{
	const bool verbose;
	std::ifstream my_input;
	std::pair<bool, std::string> find_include_name(const std::string& line, size_t offset_to_include)
	{
		auto rest = line.substr(offset_to_include + 9);
		// #include "gggg.h"
		// #include <hhh>
		// skip whitespace
		bool is_system_file = false;
		int i = 0;
		for( i=0; i < rest.size(); ++i)
		{
			char caracter = rest[i];
			if (caracter == '\"')
				continue;
			if (caracter == '<')
			{
				is_system_file = true;
				continue;
			}
			if (caracter != ' ' && caracter != '\t')
				break;
		}
		rest = rest.substr(i);
		// "gggg.h" or <hhhh>

		auto end = rest.find_first_of("\" >");
		auto include_name = rest.substr(0, end);
		return std::make_pair(is_system_file,  include_name);
	}
public:

	Parser(bool verbose) : verbose{verbose}
	{}

	bool ProcessFile(std::filesystem::path file)
	{
		using namespace std;
		namespace fs = std::filesystem;

		if( file.extension().string() != ".h" && file.extension().string() != ".hpp" && file.extension().string() != ".hxx")
		{
			return false;
		}
		auto parent = Include::find_or_create(file.string(), IncludeType::Root);
		if (parent->has_been_opened())  // this file has already been opened and analized
		{
			if (verbose)
			{
				std::cout << std::format("***** {} has already been opened before", file.string()) << std::endl;
			}
			return false;
		}

		my_input.open(file.string(), std::ios::in );
		if(!my_input.good())
		{
			string str = format("Could not open {}", file.string());
			throw exception(str.c_str());
		}

		parent->mark_opened();

		string line;
		do
		{
			getline(my_input, line);
			auto comment_start = line.find("//");
			auto start = line.find("#include ");
			if(comment_start != string::npos)
			{
				if(comment_start < start || start == string::npos)
				{
					continue;
				}
			}
			if (start != string::npos)
			{
				auto [sys_file, include_name] = this->find_include_name(line, start);
				//auto include_name = this->find_include_name(line, start);
				auto pp = file.parent_path().string();
				auto root = pp + "\\";
				auto concat = root + include_name;

				fs::path complete(concat);
				auto n = complete.lexically_normal().string();



				if (!sys_file)
				{
					parent->addDependency(n);
				}
			}
			
		} while (my_input.good());
		// mark parent as processed to prevent cycles in printing
		parent->mark_processed();
		parent->print();
		my_input.close();

		for (auto child : parent->dependencies)
		{
			fs::path p{ child->name };
			ProcessFile(p);
		}
		return true;
	}
};

