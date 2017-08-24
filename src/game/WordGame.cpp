/*
 * WordGame.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#include "WordGame.h"

namespace networkingLab {

WordGame::WordGame() {
	score=0;
	secret_word = NULL;
	word_len =0;
	guess_state = NULL;
}

WordGame::~WordGame()
{
	delete secret_word;
	delete guess_state;
}

void WordGame::setSecretWord(const string oSecret)
{
	score =0;
	word_len = oSecret.length();
	secret_word = new char[oSecret.length()];
	guess_state = new char[oSecret.length()];
	for(size_t i=0;i<oSecret.length();++i)
	{
		secret_word[i] = oSecret[i];
		guess_state[i] = BLANK_CHAR;
	}
}

const char * WordGame::getSecretWord() const
{
	return secret_word;
}

bool WordGame::guess(char sub_char)
{
	for(size_t i=0;i<word_len;++i)
	{
		if(secret_word[i] == sub_char)
		{
			guess_state[i]=sub_char;
			secret_word[i]=BLANK_CHAR;
			return true;
		}
	}
	return false;
}

bool WordGame::isWon()
{
	for(size_t i=0;i<word_len;++i)
	{
		if(BLANK_CHAR != secret_word[i])
		{
			return false;
		}
	}
	score = 1;
	return true;
}

void WordGame::setHint(const string hint)
{
	this->_hint = hint;
}

const string& WordGame::getHint() const {
	return _hint;
}

string WordGame::getState() const
{
	string g = "";
	for(size_t i=0;i<word_len;++i)
	{
		g+= guess_state[i];
	}
	return g;
}

size_t WordGame::getScore() const {
	return score;
}

} /* namespace networkingLab */
