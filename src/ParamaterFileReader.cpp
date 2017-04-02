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



}

ParamaterFileReader::~ParamaterFileReader() {
	// TODO Auto-generated destructor stub
}

void ParamaterFileReader::echoFileContents()
{
	cout << fileContents << "\n";
}
