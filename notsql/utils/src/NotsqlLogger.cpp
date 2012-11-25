/*
 * NotsqlLogger.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: danielblack
 */

#include "NotsqlLogger.h"
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;


void NotsqlLogger::write(string content){
	this->fin << "[msg] " << content << endl;
}

void NotsqlLogger::write(string header, string content){
	this->fin << "[" << header << "] " << content << endl;
}



