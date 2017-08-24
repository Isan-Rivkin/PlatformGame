//============================================================================
// Name        : PlatformGAME.cpp
// Author      : Isan and Daniel
// Version     :
// Copyright   : Isan Rivkin and Daniel Hake
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "Tester.h"
#include "SDKUtilsClient/SDKUtilsClient.h"
#include "API/includes/AbstractAPI.h"
#include <TCPServer/AbstractServer.h>
#include <TCPServer/ServerConnector.h>
#include <UDP/AbstractPeer.h>
#include <UDP/UDPPeer.h>
#include "API/includes/ServerHandler.h"
#include "API/includes/GAPI.h"
#include "tests/TestGame.h"
#include "game/WordGame.h"
#include "adapter/includes/GameAdapter.h"
using namespace std;
using namespace networkingLab;

int main()
{
	SDKUtilsClient utils;
	/**
	 * original
	 */
	AbstractAPI    * api = new GAPI(SERVER_PORT);
	AbstractServer * server = new ServerConnector(api);
	api->initServer(server);
	GameAdapter  * adapter = new GameAdapter(api);
	api->initClientHandler(adapter);
	adapter->start();
	adapter->waitForThread();
	cout <<"main = dead " <<endl;
	/**
	 * test
	 */
//	TestGame test;
//	test.start();
//	test.waitForThread();
//	cout << "sleeping " <<endl;
//	sleep(15);
//	TestGame t1("1"),t2("2");
//	t1.start();
//	t2.start();
//	t1.waitForThread();
//	t2.waitForThread();
	return 0;
}
