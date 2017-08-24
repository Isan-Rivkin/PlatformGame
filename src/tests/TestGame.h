/*
 * TestGame.h
 *
 *  Created on: Aug 19, 2017
 *      Author: user
 */

#ifndef TESTS_TESTGAME_H_
#define TESTS_TESTGAME_H_
#include "MThread.h"
#include <string.h>
#include <strings.h>
#include <SDKUtilsClient/SDKUtilsClient.h>
#include <string>
#include <iostream>
#include "API/includes/AbstractAPI.h"
#include "API/includes/ServerHandler.h"
#include "API/includes/GAPI.h"
#include "TCPServer/AbstractServer.h"
#include "TCPServer/ServerConnector.h"
#include "../game/WordGame.h"
#include "UDP/AbstractPeer.h"
#include "UDP/UDPPeer.h"
#include "TestHandler.h"
using namespace std;

namespace networkingLab{
class TestGame : public MThread
{
	ServerHandler * test_handler;
	SDKUtilsClient utils;
	string id;
public:
	TestGame();
	TestGame(string thread_id);
	virtual ~TestGame();
	virtual void run();
	// test methods
	bool connectPlayerTest();
	bool registerPlayerTest();
	bool loginPlayerTest();
	bool matchGameTest();
	void playLocal();
	// udp
	bool echo2PeersTest();
	// other
	bool runThreadTester();
};

} /* namespace networkingLab */

#endif /* TESTS_TESTGAME_H_ */
