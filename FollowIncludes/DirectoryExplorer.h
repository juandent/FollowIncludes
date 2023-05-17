#pragma once
#include <filesystem>
#include <fstream>

#include "Parser.h"
#include "TopologicalSort.h"


class DirectoryExplorer
{
	const bool verbose;
public:

	DirectoryExplorer(bool verbose) : verbose{verbose} {}

	void Execute(std::string directory_path)
	{
		using namespace std;
		namespace fs = std::filesystem;

		int count = 0;
		Parser parse{verbose};
		for(auto it = fs::directory_iterator(directory_path); it != fs::directory_iterator{}; ++it)
		{
			if (parse.ProcessFile(*it))
			{
				++count;
			}
		}
		std::cout << std::format("{} include top level interface files processed", count) << std::endl;
	}
	void copy(std::filesystem::path source, std::filesystem::path destination)
	{
		namespace fs = std::filesystem;
		// must verify source is a file
		if (!is_regular_file(source))	return;

		ifstream in;
		in.open(source.string(), ios::in);
		if(! in.good())
		{
			throw std::exception("Error opening source file");
		}
		ofstream out;
		out.open(destination.string(), ios::trunc | ios::out);
		if(! out.good())
		{
			throw std::exception("Error opening destination file");
		}
		std::string line;
		do
		{
			std::getline(in, line);
			out.write(line.c_str(), line.size());
			out << endl;
		}
		while (in.good() && out.good());
		in.close();
		out.close();

		verify_copy(source, destination);
	}
	bool  verify_copy(std::filesystem::path source, std::filesystem::path destination)
	{
		namespace fs = std::filesystem;
		// must verify source is a file
		if (!is_regular_file(source))	return true;

		ifstream orig;
		orig.open(source.string(), ios::in);
		if (!orig.good())
		{
			throw std::exception("Error opening source file");
		}
		ifstream dest;
		dest.open(destination.string(), ios::in);
		if (!dest.good())
		{
			throw std::exception("Error opening destination file");
		}
		std::string line_orig;
		std::string line_dest;
		bool success = true;
		do
		{
			std::getline(orig, line_orig);
			std::getline(dest, line_dest);
			bool eq = line_orig == line_dest;
			if(!eq)
			{
				success = false;
			}
		} while (orig.good() && dest.good());
		orig.close();
		dest.close();

		return success;
	}
	void MicrosoftToGplusplusReplaceExtension( std::string source_dir, std::string dest_dir)
	{
		using namespace std;
		namespace fs = std::filesystem;

		for (auto it = fs::directory_iterator(source_dir); it != fs::directory_iterator{}; ++it)
		{
			std::filesystem::path source(*it);
			if(! source.has_filename())
			{
				continue;
			}
			if (source.extension().string() == ".ixx")
			{
				auto new_filename = source;
				auto new_name = new_filename.replace_extension(".cxx").filename();
				auto dest = fs::path{ dest_dir };
				new_filename = dest.append(new_name.c_str());
				fs::path nf{ new_filename };

				copy(source, nf);
			}
			else
			{
				auto dest = fs::path{ dest_dir };
				auto filename = source.filename();
				auto new_filename = dest.append(filename.c_str());
				fs::path nf{ new_filename };

				copy(source, nf);
			}
			std::ignore = 0;
		}

	}
};

