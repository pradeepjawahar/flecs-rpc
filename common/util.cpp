#include "util.h"

#include <fstream>
#include <iostream>

using namespace std;


// TODO: generate flecs exceptions instead of boolean value.
//  FleCS::Error
//  FleCS::Error::Client
//  FleCS::Error::Server
bool _readfile(
		const char* filename,
		vector<unsigned char>& content)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);

	if (! file.is_open())
	{
		cerr << "Unable to open file " << filename << "\n";
		return false;
	}

	ifstream::pos_type size;
	size = file.tellg();

	char* memblock;
	memblock = new char [size];

	file.seekg (0, ios::beg);
	file.read (memblock, size);
	file.close();

	content.insert(content.begin(), memblock, memblock + size);

	delete[] memblock;

	return true;
}


void _writefile(
		const char* filename,
		const vector<unsigned char>& content)
{
	ofstream outfile(filename, ofstream::binary);

	outfile.write(reinterpret_cast<const char*>(&content[0]), content.size());
}
