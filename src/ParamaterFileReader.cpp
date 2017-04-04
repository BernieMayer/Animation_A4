/*
 * ParamaterFileReader.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: bemayer
 */

#include "ParamaterFileReader.h"


//Basic constructor, is used to read in the whole text file
ParamaterFileReader::ParamaterFileReader(string filename)
{

	/*
	 *  // ...
    std::ifstream file(filename.c_str());
    std::stringstream buffer;

    buffer << file.rdbuf();
    std::string str = buffer.str();
    std::cout << str;
	 */
	std::ifstream t(filename);
	if (t)
	{
		std::cout <<"File does exist now will read it in \n";
	} else
	{
		cout << "File does not exist \n";
		char buff[FILENAME_MAX];
		  GetCurrentDir( buff, FILENAME_MAX );
		  std::string current_working_dir(buff);
		  cout << current_working_dir << "\n";
	}
	std::stringstream buffer;

	buffer << t.rdbuf();
	fileContents = buffer.str();

	//split up the string by line...

	fileLines = splitString(fileContents, "\n");




}

vector<std::string> ParamaterFileReader::splitString(const std::string& str,
                                      const std::string& delimiter)
{
	int i;
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}



ParamaterFileReader::~ParamaterFileReader() {
	// TODO Auto-generated destructor stub
}

void ParamaterFileReader::echoFileContents()
{
	cout << fileContents << "\n";
}

string ParamaterFileReader::queryTag(string tagName)
{
	for (auto line:fileLines)
	{
		if (line.find(tagName) != string::npos)
		{
			return line;
		}
	}
}
