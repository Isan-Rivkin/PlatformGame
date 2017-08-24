/*
 * TestGame.cpp
 *
 *  Created on: Aug 19, 2017
 *      Author: user
 */

#include "TestGame.h"

namespace networkingLab {

TestGame::TestGame()
{

}

TestGame::~TestGame() {

}

void TestGame::run() {
	runThreadTester();
}

bool TestGame::connectPlayerTest()
{
	string p_str;
	cout <<"Enter server port :" <<endl;
	cin >> p_str;
	size_t port = utils.castToSize(p_str);
	AbstractAPI * api = new GAPI(port);
	AbstractServer * server = new ServerConnector(api);
	api->initServer(server);
	bool ret = api->connect(SERVER_IP, port);
	if(ret == true)
		cout << "success "<<endl;
	else
		cout <<"failure" <<endl;
	return ret;
}

bool TestGame::registerPlayerTest()
{
		string p_str;
		cout <<"Enter server port :" <<endl;
		cin >> p_str;
		size_t port = utils.castToSize(p_str);
		AbstractAPI * api = new GAPI(port);
		AbstractServer * server = new ServerConnector(api);
		api->initServer(server);
		bool ret = api->connect(SERVER_IP, port);
		sleep(1);
		api->newRegister("dana", "123");
		api->waitForThread();
		sleep(25);
		if(ret == true)
			cout << "success "<<endl;
		else
			cout <<"failure" <<endl;
		return ret;
}

bool TestGame::loginPlayerTest()
{
	string p_str;
	cout <<"Enter server port :" <<endl;
	cin >> p_str;
	size_t port = utils.castToSize(p_str);
	AbstractAPI * api = new GAPI(port);
	AbstractServer * server = new ServerConnector(api);
	api->initServer(server);
	bool ret = api->connect(SERVER_IP, port);
	sleep(1);
	cout << "[TestGame:] should try login " <<endl;
	bool succeed =api->login("dana", "123");
	if(succeed)
		cout << "success "<<endl;
	else
		cout <<"failure" <<endl;
	return ret;
}

bool TestGame::matchGameTest()
{
	string p_str;
		cout <<"Enter server port :" <<endl;
		cin >> p_str;
		size_t port = utils.castToSize(p_str);
		AbstractAPI * api = new GAPI(port);
		AbstractServer * server = new ServerConnector(api);
		api->initServer(server);
		bool ret = api->connect(SERVER_IP, port);
		cout <<"Enter name: "<<endl;
		string name;
		cin >>name;
		cout <<"Enter Password:" <<endl;
		string password;
		cin >> password;
		bool logged_in = api->login(name, password);
		if(logged_in)
		{
			cout <<name << " Logged to the server! "<<endl;
		}
		else
		{
			cout << "Couldnlt log in "<<endl;
		}
		sleep(1);
		// start listening to the server
		api->startAsyncCommunication();
		sleep(1);
		api->start();
		sleep(1);
		// ask for list
		api->getUserList();
		cout << "Enter peer name:" <<endl;
		cin >>name;
		if(name != "none")
		{
			cout <<"enter port"<<endl;
			string p;
			cin >>p;
			api->startGame(name, p);
			cout << "asked to play with: " << name <<endl;
		}
		api->getServerConnector()->waitForThread();


}
void TestGame::playLocal() {
	string word;
	string hint;
	char guess;
	WordGame wordGame;
	cout << "Choose Word " <<endl;
	cin >>word;
	cout << "Choose hint " <<endl;
	cin >>hint;
	wordGame.setSecretWord(word.c_str());
	wordGame.setHint(hint);
	do
	{
		cout << "for hint: ^ , for guess just type, for stop: x" <<endl;
		cin >>guess;
		switch(guess)
		{
		case '^':
		{
			cout <<wordGame.getHint()<<endl;
			break;
		}
		default:
		{
			bool val = wordGame.guess(guess);
			if(val)
			{
				if(wordGame.isWon())
				{
					cout <<"you have won ! " <<endl;
					cout << wordGame.getState() <<endl;
				}
				else
				{
					cout <<"good!" <<endl;
					cout <<wordGame.getState()<<endl;
				}
			}
			else
			{
				cout <<"bad!"<<endl;
				cout <<wordGame.getState()<<endl;
			}
			break;
		}
		}
	}while(guess != 'x');
}
bool TestGame::echo2PeersTest()
{
	/**
	 * peer 1
	 */
	size_t port1 = utils.generateRandom(2300, 9000);
	size_t port2 = utils.generateRandom(2300, 9000);
	AbstractAPI * api = new GAPI(port1);
	AbstractServer * server = new ServerConnector(api);
	AbstractPeer * peer = new UDPPeer(api,port1);
	this->test_handler = new TestHandler();
	api->initServer(server);
	api->initPeerConnector(peer);
	api->initClientHandler(test_handler);
	api->startAsyncPeerCommunication();
	/**
	 * peer 2
	 */
	AbstractAPI * api2 = new GAPI(port2);
	AbstractServer * server2 = new ServerConnector(api2);
	AbstractPeer * peer2 = new UDPPeer(api2,port2);
	ServerHandler * test2_handler = new TestHandler();
	api2->initServer(server2);
	api2->initPeerConnector(peer2);
	api2->initClientHandler(test2_handler);
	api2->startAsyncPeerCommunication();
	sleep(0.3);
	api->start();
	api2->start();
	sleep(1);
	/**
	 * #1 send message
	 */
	char msg[5]="me11";
	UserDetails other("",SERVER_IP,port2);
	api->sendToPeer(other, msg, 5);
	/**
	 * #2 send message
	 */
	sleep(1);
	char msg1[5]="me22";
	UserDetails other1("",SERVER_IP,port1);
	api2->sendToPeer(other1, msg1, 5);
	char msg3[5]="misi";
	api2->sendToPeer(other1, msg3, 5);
	api2->waitForThread();
	api->waitForThread();
}
TestGame::TestGame(string thread_id) {
	id = thread_id;
}


bool TestGame::runThreadTester()
{
	while(true)
	{
		string inp;
		cout << " Enter input #"<<id <<endl;
		cin >>inp;
		cout << "input #" <<id << " " << inp<<endl;
	}
}
} /* namespace networkingLab */


