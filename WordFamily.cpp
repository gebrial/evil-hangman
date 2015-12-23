//WordFamily.cpp

#ifndef __WORDFAMILY_CPP
#define __WORDFAMILY_CPP

#include "WordFamily.hpp"
#include <string>
#include <algorithm>

WordFamily::WordFamily(int size){
	wordSize = size;
	partition = NULL;
	letter = '\0';

	wordHint = std::string(size, '_');
}

WordFamily::WordFamily(int size, char l){
	wordSize = size;
	partition = NULL;
	letter = l;

	wordHint = std::string(size, '_');
}

WordFamily::~WordFamily(){
	if(partition != NULL)
		delete partition;
	partition = NULL;
	words.clear();
	positions.clear();
}

bool WordFamily::addPositions(int position){
	if(position < wordSize){
		positions.insert(position);
		return true;
	} else {
		return false;
	}
}

bool WordFamily::addWord(std::string word){
	if((int) word.length() == wordSize){
		words.insert(word);
		return true;
	} else {
		return false;
	}
}

std::string WordFamily::addLetter(char l){
	if(l == letter)
		return wordHint;

	if(partition != NULL)
		return partition->addLetter(l);


	// sub families varying by letter positions
	std::vector<WordFamily *> pos;

	// sort words into sub families
	for(auto word : words){

		// get positions of letter in word, put in locs:
		std::set<int> locs;
		int loc = word.find_first_of(l, 0);
		while(loc != -1){
			locs.insert(loc);
			loc = word.find_first_of(l, loc + 1);
		}

		// search through sub families:
		bool added = false;
		for(auto sub : pos){

			if(sub->getLetterFreq() == (int) locs.size()){
				// letter frequencies match

				// check that all positions are the same:
				bool same = true;
				for(auto p : sub->getPositions()){
					if(locs.find(p) == locs.end()){
						// could no find p, position
						same = false;
						break;
					}
				}

				if(same){
					// all positions match
					sub->addWord(word);
					added = true;
					break;
				}
			}
		}

		if(!added){
			// found no matching sub family for word
			WordFamily * newFam = new WordFamily(wordSize, l);
			newFam->addWord(word);
			for(auto p : locs){
				newFam->addPositions(p);
			}

			newFam->setWordHint(makeWordHint(l, newFam->getPositions()));
			pos.push_back(newFam);
		}
	}

	// set next word family to sub family with largest size
	for(auto sub : pos){
		sub->setWordHint(makeWordHint(l, sub->getPositions()));
		if(partition == NULL)
			partition = sub;
		else if(sub->numOfWords() > partition->numOfWords())
		//else if(sub->missingLetters() > partition->missingLetters())
			partition = sub;
		else if(sub->numOfWords() == partition->numOfWords()
				&& sub->missingLetters() > partition->missingLetters())
			partition = sub;
	}

	// delete unused sub families
	WordFamily * done;
	while(pos.size() > 1 && pos.back() != partition){
		done = pos.back();
		pos.pop_back();
		delete done;
	}
	std::swap(pos[pos.size() - 1], pos[0]);
	while(pos.size() > 1){
		done = pos.back();
		pos.pop_back();
		delete done;
	}

	//partition->setWordHint(makeWordHint(l, partition->getPositions()));
	return partition->addLetter(l);
}

int WordFamily::missingLetters(){
	if(partition == NULL)
		return std::count(wordHint.begin(), wordHint.end(), '_');

	return partition->missingLetters();
}

int WordFamily::getSize(){
	return wordSize;
}

int WordFamily::getLetterFreq(){
	return positions.size();
}

std::set<int> WordFamily::getPositions(){
	return positions;
}

int WordFamily::numOfWords(){
	if(partition != NULL)
		return partition->numOfWords();

	return words.size();
}

std::string WordFamily::makeWordHint(char newLetter, std::set<int> pos){
	std::string newWHint = wordHint;

	for(auto posi : pos){
		newWHint[posi] = newLetter;
	}

	return newWHint;
}

void WordFamily::setWordHint(std::string wHint){
	wordHint = wHint;
}

std::string WordFamily::getWordHint(){
	if(partition != NULL)
		return partition->getWordHint();
	else
		return wordHint;
}

void WordFamily::reset(){
	if(partition != NULL)
		delete partition;

	partition = NULL;
}

std::string WordFamily::getWord(){
	if(partition != NULL)
		return partition->getWord();

	return *(words.begin());
}

#endif