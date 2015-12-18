// g++ -g -Wall -std=c++11 ehm.cpp WordFamily.cpp

#include <iostream>
#include <fstream>
#include <iomanip>	// provides std::setw()
#include <cstdlib>	// provides atoi()
#include <cassert>      // provides assert
#include <functional>   // provides std::function
#include <time.h>
#include <stdlib.h>
#include <limits>

#include "WordFamily.hpp"

int main(){
	std::ifstream in("dictionary.txt");

	// build word families
	// each word family contains unique word sizes
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

	char ready = 'y';
	int wSize;
	WordFamily * current;
	while(ready == 'y'){

		ready = 'n';
		// get word size to use
		while(ready == 'n'){
			// list and choose word size
			std::cout << "Pick one of the following word size:" << std::endl;
			for(auto wf : sizes){
				std::cout << wf->getSize() << ' ';
			}
			std::cout << std::endl;
			std::cout << "Please enter number only(i.e., no words)." << std::endl;
			while(!(std::cin >> wSize)){
				std::cout << "Please enter numbers only." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			// find word size corresonding to word size
			for(auto wf : sizes){
				if(wf->getSize() == wSize){
					current = wf;
					ready = 'y';
					break;
				}
			}

			if(ready == 'n'){
				// chose unavailable word size
				std::cout << "Word size not available.\nPick one of the following:" << std::endl;
				for(auto wf : sizes){
					std::cout << wf->getSize() << ' ';
				}
				std::cout << std::endl;
			}
		}


		// start guessing game
		int tries = wSize;
		tries = 10;
		std::string wHint = current->getWordHint();
		std::set<char> guessedLetters;
		while(tries > 0 && wHint.find('_') != std::string::npos){
			// present info
			std::cout << std::endl;
			std::cout << "You have " << tries << " tries left." << std::endl;
			std::cout << "Your guessed letters are:" << std::endl;
			for(auto l : guessedLetters){
				std::cout << l << ' ';
			}
			std::cout << std::endl;
			std::cout << "Your word is: " << current->getWordHint() << std::endl;
			std::cout << "What is your next guess?" << std::endl;

			// guess a letter
			char guess;
			std::cin >> guess;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if(guess < 'A' || guess > 'z' || (guess > 'Z' && guess < 'a')){
				std::cout << "Please enter an english letter." << std::endl;
			} else if (guessedLetters.find(guess) != guessedLetters.end()){
				std::cout << "You have already tried that letter, try another." << std::endl;
			} else {
				// proper guess
				current->addLetter(guess);
				guessedLetters.insert(guess);
				if(wHint == current->getWordHint()){
					tries--;
				}
				wHint = current->getWordHint();
			}
		}

		std::cout << std::endl << "Your word was " << current->getWord() << std::endl;

		guessedLetters.clear();
		current->reset();

		ready = 'c';
		while(ready != 'y' && ready != 'n'){
			std::cout << "Ready to play again? (y/n)" << std::endl;
			std::cin >> ready;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	return 0;
}