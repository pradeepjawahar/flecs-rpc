#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <linux/limits.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost;


void _create_parent_directories(const char* filename);
void _create_directories(const string& dir);


const char* FILELIST = "../../../data/gen-fileset/filelist-size";

vector<pair<string, int> > name_size_list;

boost::program_options::variables_map povm;


void read_file_name_size()
{
	ifstream file(FILELIST, ios::in);

	if (! file.is_open())
	{
		cerr << "Unable to open file " << FILELIST << "\n";
		exit(EXIT_FAILURE);
	}

	while ( file.good() )
	{
		string line;
		getline(file, line);
		if (file.eof())
			break;

		// cout << line << endl;

		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(" ");
		tokenizer tokens(line, sep);

		int filesize;
		string filename;

		int j = 0;
		for (tokenizer::iterator i = tokens.begin(); i != tokens.end(); ++ i, ++ j)
		{
			// cout << *i << "\n";

			if (j == 0)
				filesize = strtoul(i->c_str(), NULL, 10);
			else if (j == 1)
				filename = *i;
		}

		name_size_list.push_back(pair<string, int>(filename, filesize));
	}

	file.close();
}


void create_files()
{
	string output_dir;
	if (povm["storage"].as<string>() == "disk")
		output_dir = "/usr/local/flecs/data/rep-no-const";
	else if (povm["storage"].as<string>() == "memory")
		output_dir = "/dev/shm/flecs/data/rep-no-const";
	else
		throw runtime_error(string("Unknown storage: ") + povm["storage"].as<string>());

	for (vector<pair<string, int> >::iterator i = name_size_list.begin(); i != name_size_list.end(); ++ i)
	{
		// cout << i->first << " " << i->second << "\n";

		string filename = output_dir + "/" + i->first;
		int filesize = 0;

		if (povm["dist"].as<string>() == "zifian")
			filesize = i->second;
		else if (povm["dist"].as<string>() == "uniform")
			filesize = 10240;	// 10KB

		_create_parent_directories(filename.c_str());

		ofstream outfile(filename.c_str(), ios::out | ios::trunc);

		if (outfile.fail())
		{
			cerr << "Unable to open file " << filename << "\n";
			exit(EXIT_FAILURE);
		}

		for (int written = 0; written < filesize; ++ written)
		{
			if (written == filesize - 1)
				outfile << "\n";
			else
			{
				if (written % 80 == 79)
					outfile << "\n";
				else
					outfile << "0";
			}
		}
	}

	// copy file list
	using namespace boost::filesystem;
	copy_file(FILELIST, output_dir + "/../rep-no-const-filelist", copy_option::overwrite_if_exists);
}


void _create_parent_directories(const char* filename)
{
	char filename_[PATH_MAX];
	strcpy(filename_, filename);
	char* dir = dirname(filename_);

	if ((strcmp(dir, ".") != 0) && (strcmp(dir, "/") != 0))
		_create_directories(dir);
}


void _create_directories(const string& dir)
{
	boost::system::error_code ec;
	bool r = boost::filesystem::create_directories(dir, ec);
	if (!r && ec != boost::system::errc::success)
	{
		cerr << "create_directories. ec=" << ec << "\n";
		exit(EXIT_FAILURE);
	}
}


void parse_args(int argc, char* argv[])
{
	namespace po_ = boost::program_options;

	po_::options_description visible("Options");
	visible.add_options()
		("storage", po_::value<string>()->default_value("disk"), "disk or memory.")
		("dist", po_::value<string>(), "file size distribution. either zifian or uniform.")
		("help", "produce help message")
		;

	po_::options_description cmdline_opt;
	cmdline_opt.add(visible);

	po_::store(po_::command_line_parser(argc, argv).
			options(cmdline_opt).run(), povm);
	po_::notify(povm);

	if (povm.count("help"))
	{
		cout << visible << "\n";
		exit(EXIT_SUCCESS);
	}

	if (povm.count("dist") == 0)
	{
		cout << "You need to specify a distribution.\n\n";
		cout << visible << "\n";
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[])
{
	parse_args(argc, argv);

	read_file_name_size();
	
	create_files();

	return 0;
}
