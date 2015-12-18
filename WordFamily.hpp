// WordFamily.hpp

#ifndef __WORDFAMILY_HPP
#define __WORDFAMILY_HPP

#include <vector>
#include <string>
#include <set>

class WordFamily{
public:
	WordFamily(int size);
	WordFamily(int size, char l);
	~WordFamily();
	bool addPositions(int position);
	bool addWord(std::string word);
	std::string addLetter(char l);
	int getSize();
	int getLetterFreq();
	std::set<int> getPositions();
	int numOfWords();
	void setWordHint(std::string wHint);
	std::string getWordHint();

private:
	std::vector<std::string> words;
	std::set<int> positions;
	int wordSize;
	char letter;
	WordFamily * partition;
	std::string wordHint;
	std::string makeWordHint(char newLetter, std::set<int> pos);
};

#endif