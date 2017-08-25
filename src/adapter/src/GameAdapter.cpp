/*
 * GameAdapter.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#include "../includes/GameAdapter.h"

namespace networkingLab {
GameAdapter::GameAdapter(AbstractAPI* oAPI) {
	this->api= oAPI;
	this->playing = false;
	logged_in = false;
	initCLICommandList();
	this->connect = false;
}

GameAdapter::~GameAdapter() {
	// TODO Auto-generated destructor stub
}

void GameAdapter::run()
{
	string command="";
	string name,pwd="";
	printWelcome();
	printServerInstructions();
	cout << "Would you like to connect to the main server ? <connect> " <<endl;
	cin >>command;
	int connect_cmd = parseCLICommand(command);
	if(connect_cmd == CONNECT_TO_SERVER)
	{
		connect = api->connect(SERVER_IP, SERVER_PORT);
		if(!connect)
		{
			cout << "[Game:] Coldnt Connect to Main server." <<endl;
			return;
		}
		else
		{
			cout << "Success connecting to server! "<<endl;
		}
	}
	else
	{
		cout <<"Exisining now ... First command must be <connect>"<<endl;
		return;
	}
	/**
	 * authenthicate
	 */
	do{
	cout << "### Choose Action: ###" <<endl;
	cout << "######################" <<endl;
	cout <<"Login: <login>"<<endl;
	cout <<"Register: <register>"<<endl;
		cin >> command;
		cout <<"Enter name: " <<endl;
		cin >>name;
		cout <<"Enter password:" <<endl;
		cin >>pwd;
		cout << "######################" <<endl;
		int loginCMD = parseCLICommand(command);
		switch(loginCMD)
		{
			case LOGIN_TO_SERVER:
			{
				logged_in = api->login(name, pwd);
				break;
			}
			case REGISTER_TO_SERVER:
			{
				logged_in = api->newRegister(name, pwd);
				break;
			}
		}
		if(!logged_in)
			cout << "Couldn't register/login "<<endl;
	}while(!logged_in);

	api->startAsyncCommunication();
	/**
	 * Actual client-server protocol
	 */
	printServerInstructions();
	cout <<"###############################################"<<endl;
	cout << "Hi " <<name << " you are logged in !  "<<endl;
	cout <<"###############################################"<<endl;
	while(logged_in)
	{
		cout << "Enter Command (for help -> <help>)"<<endl;
		cin >>command;
		int parsed_cmd = parseCLICommand(command);
		switch(parsed_cmd)
		{
		// <quit>
		case SESSION_QUIT_LOST:
		{
			cout << "Redirecting  back to server, score : " << _game->getScore()<<endl;
			playing=false;
			string myScore = utils.intToString(SESSION_SCORE_UPDATE);
			myScore+=":";
			myScore += utils.sizeToString(_game->getScore());
			UserDetails winner("",last_offer_ip,utils.castToSize(last_offer_port));
			api->sendToPeer(winner,myScore.c_str(),myScore.length());
			api->endGame(utils.sizeToString(_game->getScore()));
			break;
		}
		case PLAY_WITH_Y:
		{
			if(!playing)
			{
				string uName,uPort;
				cout <<"Enter User name:"<<endl;
				cin >> uName;
				cout <<"Enter User port: "<<endl;
				cin>>uPort;
				api->startGame(uName, uPort);
			}
			else
			{
				cout << "[Game:] Cannot use this command while playing with a peer" <<endl;
			}
			break;
		}
		case RANDOM_MATCH:
		{
			if(!playing)
			{
				cout << "Trying Random... "<<endl;
				api->startRandom();
				break;
			}
			else
			{
				cout << "[Game:] Cannot use this command while playing with a peer" <<endl;
			}
		}
		case GET_LIST:
		{
			if(!playing)
			{
				api->getUserList();
			}
			else
			{
				cout << "[Game:] Cannot use this command while playing with a peer" <<endl;
			}
			break;
		}
		case CONFIRM_OTHER_USER_PLAY:
		{
			if(!playing)
			{
				api->confirmPlay(last_offer_name, last_offer_port);
				api->startAsyncPeerCommunication();
			}
			else
			{
				cout << "[Game:] Cannot use this command while playing with a peer" <<endl;
			}
			break;
		}
		case DECLINE_OTHER_USER_PLAY:
		{
			if(!playing)
			{
				api->declineOffer(last_offer_name, last_offer_port);
			}
			else
			{
				cout << "[Game:] Cannot use this command while playing with a peer" <<endl;
			}
			break;
		}
		case SESSION_INIT:
		{
			string word="";
			// user picks a word for the opponent
			do
			{
				cout << "Enter [3,6] length word to guess: "<<endl;
				cin >>word;
			}while(!(word.length() >=3 && word.length() <=6));
			// user sends the word to the opponent
			api->startAsyncPeerCommunication();
			UserDetails peer("",last_offer_ip,utils.castToSize(last_offer_port));
			if(!(word.length() >=3 || word.length() <=6))
			{
				cout <<"Wrong word length, hit <init> and try again:" <<endl;
				break;
			}
			cout << "### Wait for opponents word to be sent ###" <<endl;
			string final_msg = utils.intToString(SESSION_START)+ ":" + word;
			api->sendToPeer(peer, final_msg.c_str(), final_msg.length());
			break;
		}
		// this = replaying to initiater with a word
		case SESSION_REPLY:
		{
			string word="";
			// user picks a word for the opponent
			do
			{
				cout << "Enter [3,6] length word to guess: "<<endl;
				cin >>word;
			}while(!(word.length() >=3 && word.length() <=6));
			// user sends the word to the opponent
			UserDetails peer("",last_offer_ip,utils.castToSize(last_offer_port));
			if(!(word.length() >=3 || word.length() <=6))
			{
				cout <<"Wrong word length, hit <init> and try again:" <<endl;
				break;
			}
			string final_msg = utils.intToString(SESSION_REPLY)+ ":" + word;
			api->sendToPeer(peer, final_msg.c_str(), final_msg.length());
			printGameInstructions();
			cout << "=============================================================="<<endl;
			cout << "======== >>>>>>>>>> start guessing !!!<<<<<<<<<==========" <<endl;
			cout << "=============================================================="<<endl;
			cout <<_game ->getState() <<endl;
			playing = true;
			startPlayingSession();
			cout << "[Game:Main loop:] out of game session loop " <<endl;
				break;
		}
		case SESSION_START_GUESS:
		{
			cout << "User Length Word : " << strlen(_game->getSecretWord()) <<endl;
			playing = true;
			startPlayingSession();
			break;
		}
		case FINISH_GAME_SESSION:
		{
			break;
		}
		case DISSCONECT_SERVER:
		{
			api->exit();
			connect = false;
			logged_in = false;
			break;
		}
		case HELP:
		{
			printServerInstructions();
			break;
		}
		default:
		{
			printServerInstructions();
			cout <<"[Main Loop:] Unrecognized Command: "<<command<<endl;
			break;
		}
		}
	}
	cout << "[Main Game Loop:] Disconnected from server"<<endl;
}

void GameAdapter::printServerInstructions() const {

	cout << "==============================================================" <<endl;
	cout << "================== Server Help ===============================" <<endl;
	cout <<"Connect to server:                       <connect>"<<endl;
	cout <<"Login:                                   <login>"<<endl;
	cout <<"Register:                                <register>"<<endl;
	cout <<"Show Players List:                       <list>"<<endl;
	cout <<"Match specific player:                   <match>" <<endl;
	cout <<"Random Player :                          <random>"<<endl;
	cout <<"Confirm Play offer:                      <confirm>"<<endl;
	cout <<"Decline Play offer:                      <decline>" <<endl;
	cout <<"Finish Current Game Session (if Active): <finish>"<<endl;
	cout <<"Disconnect from the server:              <exit>"<<endl;
	cout <<"Help :                                   <help>"<<endl;
	cout << "==============================================================" <<endl;
}

void GameAdapter::handleUDP(const char* msg)
{
	// protocol {2_chars_command_id:buffer}
	// xy,xy:... =ok
	// x,0,xy:   =bad
	if(strlen(msg) < 2 || (strlen(msg) == 3))
		return;
	string str_cmd ="";
	 str_cmd+=msg[0];
	 str_cmd+=msg[1];
	size_t command = utils.castToSize(str_cmd);
	switch(command)
	{
	// the initial requester start conversation
	case SESSION_START:
	{
		string peers_word = "";
		if(msg[2] != ':')
			return;
		for(size_t i=3;i<strlen(msg);++i)
			peers_word += msg[i];
		/**
		 * parse word to send back + send
		 */
		_game = new WordGame();
		_game->setSecretWord(peers_word);
		cout << "=============================================================="<<endl;
		cout << "User sent you a " << peers_word.length() << " chars word." <<endl;
		cout << _game->getState()<<endl;
		cout << "Enter <reply> to reply" <<endl;
		cout << "=============================================================="<<endl;
		break;
	}
	// accepter replies to SESSION_START
	case SESSION_REPLY:
	{
		string peers_word = "";
		if(msg[2] != ':')
			return;
		for(size_t i=3;i<strlen(msg);++i)
			peers_word += msg[i];
		_game = new WordGame();
		_game->setSecretWord(peers_word);
		printGameInstructions();
		cout << "=============================================================="<<endl;
		cout << "======== >>>>>>>>>> start guessing !!! <g> <<<<<<<<<==========" <<endl;
		cout << "=============================================================="<<endl;
		cout << "User sent you a " << peers_word.length() << " chars word." <<endl;
		cout <<_game ->getState() <<endl;
		break;
	}
	// other peer has asked to terminate the game.
	case SESSION_END_LOOP:
	{
		cout << "#########################################################################"<<endl;
		cout << "                  USER HAVE TERMINATED THE GAME                   " <<endl;
		cout << "#########################################################################"<<endl;
		playing=false;
		string peers_word = "";
		if(msg[2] != ':')
			return;
		for(size_t i=3;i<strlen(msg);++i)
			peers_word += msg[i];
		cout << "# OTHER PEERS SCORE: " <<endl;
		cout << "# "<<last_offer_ip << " : " <<last_offer_port << " SCORE = " << peers_word<<endl;
		cout <<"# Enter <quit> to finish and redirect to server ! " <<endl;
		cout << "#########################################################################"<<endl;
		break;
	}
	// other peer has won -> terminate the game.
	case SESSION_WIN:
	{
		cout << " ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{"<<endl;
		cout << "                  YOU HAVE LOST THE GAME                   " <<endl;
		cout << " ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{ ':{"<<endl;
		playing=false;
		string peers_word = "";
		if(msg[2] != ':')
			return;
		for(size_t i=3;i<strlen(msg);++i)
			peers_word += msg[i];
		cout << "#########################################################################"<<endl;
		cout << "# OTHER PEERS SCORE: " <<endl;
		cout << "# "<<last_offer_ip << " : " <<last_offer_port << " SCORE = " << peers_word<<endl;
		cout <<"# Enter <quit> to finish and redirect to server ! " <<endl;
		cout << "#########################################################################"<<endl;
		break;
	}
	case SESSION_SCORE_UPDATE:
	{
		string peers_word = "";
		if(msg[2] != ':')
			return;
		for(size_t i=3;i<strlen(msg);++i)
			peers_word += msg[i];
		cout << "#########################################################################"<<endl;
		cout << "# OTHER PEERS SCORE: " <<endl;
		cout << "#"<<last_offer_ip << " : " <<last_offer_port << " SCORE = " << peers_word<<endl;
		cout << "# YOUR SCORE: " << _game->getScore() <<endl;
		cout << "#########################################################################"<<endl;
		break;
	}
	default:
	{
		cout <<"[Game:] Unrecognized command from peer:" << command<<endl;
		break;
	}
	}
	/**
	 * parse buffer (game_protocol)
	 * send begin + word
	 * read begin + word
	 * send status: {win,stop,again?}
	 */
}
void GameAdapter::playWithPeer(const UserDetails & _peer_details)
{

}
void GameAdapter::printWelcome() const
{
	cout << "==============================================================" <<endl;
	cout << "========================= WELCOME ============================" <<endl;
	cout << "==============================================================" <<endl;
}

void GameAdapter::printGameInstructions() const
{
		cout << "==============================================================" <<endl;
		cout << "================== GAME Help =================================" <<endl;
		cout << "Pick a word for the opponent to guess."<<endl;
		cout << "Guess the oppoenents word ! " <<endl;
		cout << "==============================================================" <<endl;
		cout <<"Guess a letter:                       <g>"<<endl;
		cout <<"To initialize accepted offer game:    <init>"<<endl;
		cout <<"Terminate the game session:           <force>" <<endl;
		cout << "Exit the server:                     <exit>" <<endl;
		cout << "==============================================================" <<endl;
}

void GameAdapter::initCLICommandList()
{
	pair<string,int> cmd1;
	pair<string,int> cmd2;
	pair<string,int> cmd3;
	pair<string,int> cmd4;
	pair<string,int> cmd5;
	pair<string,int> cmd6;
	pair<string,int> cmd7;
	pair<string,int> cmd8;
	pair<string,int> cmd9;
	pair<string,int> cmd10;
	pair<string,int> cmd11;
	pair<string,int> cmd12;
	pair<string,int> cmd13;
	pair<string,int> cmd14;
	pair<string,int> cmd15;
	pair<string,int> cmd16;
	pair<string,int> cmd17;
	pair<string,int> cmd18;

	cmd1.first ="connect";
	cmd1.second=CONNECT_TO_SERVER;
	cmd2.first="login";
	cmd2.second =LOGIN_TO_SERVER;
	cmd3.first= "register";
	cmd3.second =REGISTER_TO_SERVER;
	cmd4.first="list";
	cmd4.second =GET_LIST;
	cmd5.first="match";
	cmd5.second =PLAY_WITH_Y;
	cmd6.first= "random";
	cmd6.second =RANDOM_MATCH;
	cmd7.first= "finish";
	cmd7.second =FINISH_GAME_SESSION;
	cmd8.first= "exit";
	cmd8.second =DISSCONECT_SERVER;
	cmd9.first="help";
	cmd9.second =HELP;
	cmd10.first="confirm";
	cmd10.second=CONFIRM_OTHER_USER_PLAY;
	cmd11.first="decline";
	cmd11.second=DECLINE_OTHER_USER_PLAY;
	cmd12.first ="init";
	cmd12.second=SESSION_INIT;
	cmd13.first ="reply";
	cmd13.second=SESSION_REPLY;
	cmd14.first ="end";
	cmd14.second=SESSION_TERMINATE;
	cmd15.first ="g";
	cmd15.second=SESSION_START_GUESS;
	cmd16.first ="quit";
	cmd16.second=SESSION_QUIT_LOST;
	cmd17.first ="force";
	cmd17.second=SESSION_END_LOOP;
	cmd18.first ="gHelp";
	cmd18.second=SESSION_HELP_GAME;
	this->cli_commands_map.push_back(cmd1);
	this->cli_commands_map.push_back(cmd2);
	this->cli_commands_map.push_back(cmd3);
	this->cli_commands_map.push_back(cmd4);
	this->cli_commands_map.push_back(cmd5);
	this->cli_commands_map.push_back(cmd6);
	this->cli_commands_map.push_back(cmd7);
	this->cli_commands_map.push_back(cmd8);
	this->cli_commands_map.push_back(cmd9);
	this->cli_commands_map.push_back(cmd10);
	this->cli_commands_map.push_back(cmd11);
	this->cli_commands_map.push_back(cmd12);
	this->cli_commands_map.push_back(cmd13);
	this->cli_commands_map.push_back(cmd14);
	this->cli_commands_map.push_back(cmd15);
	this->cli_commands_map.push_back(cmd16);
	this->cli_commands_map.push_back(cmd17);
	this->cli_commands_map.push_back(cmd18);

//
}

int GameAdapter::parseCLICommand(const string& cmd) const
{
	for(size_t i=0;i<cli_commands_map.size();++i)
	{
		if(cmd == cli_commands_map[i].first)
			return cli_commands_map[i].second;
	}
	return -1;
}

void GameAdapter::handleTCP(const char* msg, int command)
{
	switch(command)
	{
		case AUTH_ACK_LOGIN:
		{
			if(msg == NULL)
				break;
			string port = "";
			for(size_t i = 0; i <strlen(msg);++i)
				port+= msg[i];
			port_self = utils.castToSize(port);
			api->initPeerHConnector(port_self);
			break;
		}
	case AUTH_NACK_LOGIN:
	{
		if(msg == NULL)
			break;
		string port = "";
		for(size_t i = 0; i <strlen(msg);++i)
			port+= msg[i];
		port_self = utils.castToSize(port);
		api->initPeerHConnector(port_self);
		break;
	}
	case MATCH_LIST:
	{
		cout <<msg <<endl;
		break;
	}
	case MATCH_OFFER_FROM_X:
	{
		// msg = {ip:name:port}
		updateCurrentOffer(msg);
		cout << "############################################"<<endl;
		cout <<"Offer to play from : " << msg <<endl;
		cout <<"<confirm> or <decline>" <<endl;
		cout << "############################################"<<endl;
		break;
	}
	case MATCH_OFFER_ACK_ACCEPTED_:
	{
		this->last_offer_ip="";
		this->last_offer_port="";
		// msg = {ip:port}
		size_t i=0,j=0;
		for(i=0;i<strlen(msg);++i)
		{
			if(msg[i] != ':')
			{
				this->last_offer_ip += msg[i];
			}
			else
			{
				break;
			}
		}
		for(j=i+1;j<strlen(msg);++j)
		{
			this->last_offer_port += msg[j];
		}
		playing = true;
		cout << "############################################"<<endl;
		cout <<last_offer_ip<<":"<<last_offer_port <<" Accepted your offer! "  <<endl;
		cout << "Enter <init> inorder to start the session ! " <<endl;
		cout << "############################################"<<endl;
		/**
		 * start a udp session with peer
		 * while playing:
		 * 	~
		 * 	terminating commands: {SESSION_START,SESSION_WIN,SESSION_REPLY,SESSION_TERMINATE}
		 */
		break;
	}
	case MATCH_OFFER_NACK_REJECTED_:
	{
		cout << "############################################"<<endl;
		cout  <<" Rejected your offer! "  <<endl;
		cout << "############################################"<<endl;
		break;
	}
	case BUSY_STOP_GAME:
	{
		cout << "############# High Scores ##################"<<endl;
		cout <<" [Game:] Game Stopped."  <<endl;
		cout <<msg <<endl;
		cout << "############################################"<<endl;
		break;
	}
	default:
	{
		break;
	}
	}
}

void GameAdapter::startPlayingSession()
{
	/**
		 * while playing -
		 * -read cmd
		 * -!g -> guess
		 * -SESSION_TERMINATE
		 * -SESSION_WIN
		 */
	string cmd="";
	while(playing)
	{
		cout << "Enter guess: (<gHelp>) "<<endl;
		cin >> cmd;
		int command = parseCLICommand(cmd);
		switch(command)
		{
		case SESSION_HELP_GAME:
		{
			printGameInstructions();
			break;
		}
		// <quit>
		case SESSION_QUIT_LOST:
		{
			playing=false;
			string myScore = utils.intToString(SESSION_SCORE_UPDATE);
			myScore+=":";
		    myScore += utils.sizeToString(_game->getScore());
			UserDetails winner("",last_offer_ip,utils.castToSize(last_offer_port));
			api->sendToPeer(winner,myScore.c_str(),myScore.length());
			api->endGame(utils.sizeToString(_game->getScore()));
			break;
		}
		//<force>
		case SESSION_END_LOOP:
		{
			cout << "Redirecting  back to server, score : " << _game->getScore()<<endl;
			UserDetails winner("",last_offer_ip,utils.castToSize(last_offer_port));
			string msg = "";
			msg+=utils.intToString(SESSION_END_LOOP);
			msg+=":";
			msg+=utils.sizeToString(_game->getScore());
			playing = false;
			api->sendToPeer(winner, msg.c_str(), msg.length());
			api ->endGame(utils.sizeToString(_game->getScore()));
			break;
		}
		case DISSCONECT_SERVER:
		{
			playing =false;
			connect =false;
			logged_in = false;
			api->exit();
			break;
		}
		default:
		{
			if(cmd.length() != 1)
			{
				cout <<"[Game Loop:] Must enter 1 char only to guess ! " <<endl;
				break;
			}
			bool good = _game->guess(cmd.c_str()[0]);
			if(good)
			{
				if(_game->isWon())
				{
					UserDetails loser("",last_offer_ip,	utils.castToSize(last_offer_port));
					string msg = "";
					msg+=utils.intToString(SESSION_WIN);
					msg+=":";
					msg+=utils.sizeToString(_game->getScore());
					api->sendToPeer(loser, msg.c_str(), msg.length());
					api ->endGame(utils.sizeToString(_game->getScore()));
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "^^^^ YOU HAVE WON THE GAME ^^^^^^^" <<endl;
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "^^^^Word =" <<_game->getState()<<" ^^^^^^^" <<endl;
					cout <<"^^^^ Your Score: " << _game->getScore()<<"^^^^" <<endl;
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
					cout << "Redirected back to the main server..."<<endl;
					playing = false;
					break;
				}
				else
				{
					cout << "Great Guess keep up! " << endl;
					cout << _game->getState() <<endl;
				}
			}
			else if(!good)
			{
				cout <<"Bad Guess ! Don't give up!" <<endl;
				cout << _game->getState() <<endl;
			}
			break;
		}
		}
	}
}

void GameAdapter::updateCurrentOffer(const char* name_port)
{
	this->last_offer_name="";
	this->last_offer_port="";
	this->last_offer_ip ="";
	size_t i=0;
	for(i=0;i<strlen(name_port);++i)
	{
		if(name_port[i] != ':')
		{
			last_offer_ip+=name_port[i];
		}
		else
		{
			break;
		}
	}
	size_t j;
	for(j=i+1; j<strlen(name_port);++j)
	{
		if(name_port[j] != ':')
		{
			last_offer_name+=name_port[j];
		}
		else
		{
			break;
		}
	}
	for(size_t k=j+1;k<strlen(name_port);++k)
	{
		last_offer_port+=name_port[k];
	}
}

} /* namespace networkingLab */
