// g++ -g -Wall -std=c++11 ehm.cpp WordFamily.cpp

#include <iostream>
#include <fstream>
#include <iomanip>	// provides std::setw()
#include <cstdlib>	// provides atoi()
#include <cassert>      // provides assert
#include <functional>   // provides std::function
#include <time.h>
#include <stdlib.h>

#include "WordFamily.hpp"

int main(){
	std::ifstream in("dictionary.txt");

	std::vector<WordFamily *> sizes;
	std::string word;
	while (in >> word) {
		int size = word.length();
		bool added = false;

		for(unsigned int i = 0; i < sizes.size(); i++){
			added = sizes[i]->addWord(word);
			if(added){
				break;
			}
		}

		if(!added){
			WordFamily * newSize = new WordFamily(size);
			newSize->addWord(word);	
			sizes.push_back(newSize);		
		}
	}

	std::cout << sizes.size() << std::endl;

	for(unsigned int i = 0; i < sizes.size(); i++){
		std::cout << "Word size = " << sizes[i]->getSize();
		std::cout << ", # words = " << sizes[i]->numOfWords() << std::endl;
	}


	char ready = 'y';
	int wSize = 6;
	while(ready == 'y'){
		WordFamily * current;
		for(auto wf : sizes){
			if(wf->getSize() == wSize){
				current = wf;
				break;
			}
		}

		int tries = wSize * 2;
		while(tries-- > 0){
			std::cout << "You have " << tries << " tries left." << std::endl;
			std::cout << "Your letters are: " << current->getWordHint() << std::endl;
			std::cout << "What is your next guess?" << std::endl;

			char guess;
			std::cin >> guess;
			current->addLetter(guess);
		}

		std::cout << "Ready to play again? (y/n)" << std::endl;
		std::cin >> ready;
	}

	return 0;
}