/*
 * TestHandler.h
 *
 *  Created on: Aug 22, 2017
 *      Author: user
 */

#ifndef TESTS_TESTHANDLER_H_
#define TESTS_TESTHANDLER_H_
#include "MThread.h"
#include <string.h>
#include <strings.h>
#include <SDKUtilsClient/SDKUtilsClient.h>
#include <string>
#include <iostream>
#include "API/includes/AbstractAPI.h"
#include "API/includes/ServerHandler.h"
using namespace std;
namespace networkingLab {

class TestHandler : public ServerHandler
{
public:
	TestHandler();
	virtual ~TestHandler();
public:
	virtual void handleUDP(const char * msg){cout << "MSG:" <<endl; cout <<msg <<endl;};
	virtual void handleTCP(const char* msg,int command){};
};
} /* namespace networkingLab */

#endif /* TESTS_TESTHANDLER_H_ */
