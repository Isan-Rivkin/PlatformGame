/*
 * CLIProtocol.h
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#ifndef CLIPROTOCOL_H_
#define CLIPROTOCOL_H_

/**********************************************************************
 ************************CLI INSTRUCTIONS***************************
 **********************************************************************/
//followed with a guessed word buffer
#define SESSION_START 22
#define SESSION_REPLY 25
#define SESSION_WIN 23
#define SESSION_TERMINATE 24

// after the other accepted - > use this to actually initiate connection with peer
#define SESSION_INIT 26
// used by the initiater to start while of guessing loops
#define SESSION_START_GUESS 27
// after losing the user input quit
#define SESSION_QUIT_LOST 28
#define SESSION_SCORE_UPDATE 29
// when stuck in game loop
#define SESSION_END_LOOP 30
// game session help instructions
#define SESSION_HELP_GAME 31
/**********************************************************************
 ************************SERVER PROTOCOL***************************
 **********************************************************************/
#define CONNECT_TO_SERVER 10
#define LOGIN_TO_SERVER 12
#define REGISTER_TO_SERVER 13
#define PLAY_WITH_Y 14
#define RANDOM_MATCH 15
#define GET_LIST 16
#define DISSCONECT_SERVER 17
#define FINISH_GAME_SESSION 18
#define HELP 19
#define CONFIRM_OTHER_USER_PLAY 20
#define DECLINE_OTHER_USER_PLAY 21
// start from 40
#endif /* CLIPROTOCOL_H_ */
