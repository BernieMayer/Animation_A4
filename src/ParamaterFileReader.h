/*
 * ParamaterFileReader.h
 *
 *  Created on: Mar 28, 2017
 *      Author: bemayer
 */

#ifndef PARAMATERFILEREADER_H_
#define PARAMATERFILEREADER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#define GetCurrentDir getcwd
#include <streambuf>

using namespace std;

class ParamaterFileReader {
public:
	ParamaterFileReader(string filename);
	virtual ~ParamaterFileReader();

	void echoFileContents();


private:
	string fileContents;

};

#endif /* PARAMATERFILEREADER_H_ */
