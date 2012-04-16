// Does a file have monotonically inceasing sequence number per client?
//
// Is a line intermingled by multiple requests?


#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>

using namespace std;

boost::program_options::variables_map povm;


void _get_filelist(const string& parent, vector<string>& filelist)
{
	DIR* dp;
	if ((dp  = opendir(parent.c_str())) == NULL)
		throw runtime_error(string("opendir: " + errno));

	while (true)
	{
		struct dirent* dirp = readdir(dp);
		if (dirp == NULL)
		{
			if (errno == EBADF)
				throw runtime_error(string("readdir: " + errno));
			else
				break;
		}

		if (! dirp->d_name)
			throw runtime_error("Unexpected");

		// skip hidden files and the directory itself and the parent directory.
		if (dirp->d_name[0] == '.')
			continue;

		string filepath(parent);
		filepath += "/";
		filepath += dirp->d_name;

		struct stat stat_;
		if (stat(filepath.c_str(), &stat_) == -1)
			throw runtime_error(string("stat: " + errno));

		// recurse if it is a directory
		if (S_ISDIR(stat_.st_mode))
			_get_filelist(filepath, filelist);
		else
			filelist.push_back(filepath);
	}

	if (closedir(dp) == -1)
		throw runtime_error(string("closedir: " + errno));
}


void check_integrity(const string& filename)
{
	string line;

	ifstream ifs(filename.c_str());

	if (! ifs.is_open())
		throw runtime_error(string("Unable to open file: ") + filename);

	map<string, string> hostname_seqno;

	while (ifs.good())
	{
		getline (ifs, line);

		// initial file content. ignore.
		if (line == "0000000000000000000000000000000000000000000000000000000000000000000000000000000")
			continue;

		boost::char_separator<char> sep(" ");

		boost::tokenizer<boost::char_separator<char> > tokens(line, sep);

		// if a line is intermingled, output the line.
		string t2 = "";

		BOOST_FOREACH(string t, tokens)
		{
			// check if all tokens are the same.
			if (t2 == "")
			{
				t2 = t;

				// check if there is a dot in the middle.
				boost::char_separator<char> sep2(".");
				boost::tokenizer<boost::char_separator<char> > tokens2(t, sep2);
				int num_tokens = 0;
				string hostname;
				string seqno;
				for (boost::tokenizer<boost::char_separator<char> >::iterator i = tokens2.begin(); i != tokens2.end(); ++ i)
				{
					if (num_tokens == 0)
						hostname = *i;
					else if (num_tokens == 1)
						seqno = *i;

					++ num_tokens;
				}

				if (num_tokens != 2)
					cout << "Unexpected tokenization with dot!: " << line << endl;

				// seqno should monotonically increase per host.
				map<string, string>::iterator j = hostname_seqno.find(hostname);
				if (j == hostname_seqno.end())
					hostname_seqno[hostname] = seqno;
				else
				{
					if (j->second >= seqno)
						cout << "seqno is not increasing!: " << j->second << " " << seqno << ": " << line << endl;

					hostname_seqno[hostname] = seqno;
				}
			}
			else
			{
				if (t2 != t)
				{
					// intermingled.
					cout << "Intermingled!: " << line << endl;
					break;
				}
			}
		}
	}
}


void parse_args(int argc, char* argv[])
{
	namespace po_ = boost::program_options;

	po_::options_description visible("Options");
	visible.add_options()
		("help", "produce help message")
		;

	po_::options_description hidden("Hidden options");
	hidden.add_options()
		("data-dir", po_::value< vector<string> >(), "data dir")
		;   

	po_::options_description cmdline_opt;
	cmdline_opt.add(visible).add(hidden);

	po_::positional_options_description pos;
	pos.add("data-dir", -1);

	po_::store(po_::command_line_parser(argc, argv).
			options(cmdline_opt).positional(pos).run(), povm);
	po_::notify(povm);

	if (povm.count("help")) {
		cout << argv[0] << " data-dir\n\n";
		cout << visible << "\n";
		exit(1);
	}

	if (! povm.count("data-dir"))
	{
		cout << argv[0] << " data-dir\n\n";
		cout << visible << "\n";
		exit(1);
	}

	cout << "Input directories are: ";
	vector<string> tmp(povm["data-dir"].as< vector<string> >());
	copy(tmp.begin(), tmp.end(), ostream_iterator<string>(cout, " "));
	cout << "\n";
}


int main(int argc, char* argv[])
{
	parse_args(argc, argv);

	vector<string> dirs(povm["data-dir"].as< vector<string> >());
	for (vector<string>::const_iterator i = dirs.begin(); i != dirs.end(); ++ i)
	{
		cout << "\n";
		cout << *i << " ..." << endl;	// endl flushes.

		vector<string> filelist;

		_get_filelist(*i, filelist);

		for (vector<string>::const_iterator j = filelist.begin(); j != filelist.end(); ++ j)
		{
			cout << *j << "\n";

			check_integrity(*j);
		}
	}

	return 0;
}
