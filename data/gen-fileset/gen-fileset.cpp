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

using namespace std;
using namespace boost;


void _create_parent_directories(const char* filename);
void _create_directories(const string& dir);


const char* FILELIST = "../../filelist-size";
const string OUTPUT_DIR = "/usr/local/flecs/no-cnst";

vector<pair<string, int> > name_size_list;


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
	for (vector<pair<string, int> >::iterator i = name_size_list.begin(); i != name_size_list.end(); ++ i)
	{
		// cout << i->first << " " << i->second << "\n";

		string filename = OUTPUT_DIR + "/" + i->first;
		int filesize = i->second;

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
	copy_file(FILELIST, OUTPUT_DIR + "/../no-cnst-filelist", copy_option::overwrite_if_exists);
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


int main(int argc, char* argv[])
{
	read_file_name_size();
	
	create_files();

	return 0;
}
