/*
 * GameAdapter.h
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#ifndef ADAPTER_INCLUDES_GAMEADAPTER_H_
#define ADAPTER_INCLUDES_GAMEADAPTER_H_
#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include "MThread.h"
#include "../../game/WordGame.h"
#include "API/includes/AbstractAPI.h"
#include "TCPServer/AbstractServer.h"
#include "API/includes/ServerHandler.h"
#include "API/Protocol.h"
#include "../../CLIProtocol.h"
#include "SDKUtilsClient/SDKUtilsClient.h"
#include <vector>
using namespace std;
namespace networkingLab
{

class GameAdapter : public MThread, public ServerHandler
{
	WordGame * _game;
	AbstractAPI * api;
	bool playing,connect,logged_in;
	vector<pair<string,int> > cli_commands_map;
	string last_offer_name,last_offer_port,last_offer_ip;
	size_t port_self;
	SDKUtilsClient utils;
public:
	GameAdapter(AbstractAPI * oAPI);
	virtual ~GameAdapter();
public:
	/*
	 * run the cli game
	 */
	virtual void run();
	void printServerInstructions()const;
	void printWelcome()const;
	void printGameInstructions()const;
	/**
	 * game session
	 */
	// unsued !! empty
	void playWithPeer(const UserDetails & _peer_details);
	void startPlayingSession();
	/**
	 * handler implementation for the api
	 */
public:
	virtual void handleUDP(const char * msg);
	virtual void handleTCP(const char* msg,int command);
private:
	void initCLICommandList();
	int parseCLICommand(const string& cmd)const;
	void updateCurrentOffer(const char * name_port);
};

} /* namespace networkingLab */

#endif /* ADAPTER_INCLUDES_GAMEADAPTER_H_ */
