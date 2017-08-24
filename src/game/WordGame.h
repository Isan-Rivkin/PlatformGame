/*
 * WordGame.h
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#ifndef GAME_WORDGAME_H_
#define GAME_WORDGAME_H_
#include <string.h>
#include <strings.h>
#include <string>
#include <iostream>
#define BLANK_CHAR '_'
using namespace std;
namespace networkingLab {

class WordGame
{
	char * secret_word, * guess_state;
	size_t word_len;
	string _hint;
	size_t score;
public:
	WordGame();
	virtual ~WordGame();
public:
	void setSecretWord(const string oSecret);
	void setHint(const string hint);
	const string& getHint()const;
	const char* getSecretWord()const;
	bool guess(char sub_char);
	bool isWon();
	string getState()const;
	size_t getScore()const;
};

} /* namespace networkingLab */

#endif /* GAME_WORDGAME_H_ */
