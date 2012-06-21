//=======================================================================
// Copyright 2012.
// Authors: S Roychowdhury
// Project: Syllables
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <iostream>
#include <string>
#include <list>
#include <boost/foreach.hpp>
#include "Syllables.hh"

int main()
{
	std::string word;
	Syllables D;
	do {
		std::cout << "Please Enter word:  " << std::flush;
		std::cin >> word;
		typedef std::list<std::string> sList;
		typedef std::list<sList> sListList;
		sListList ss;
		bool k = D.syllabify(word,ss);
		if (k) {
			BOOST_FOREACH(sList sa, ss) {
				BOOST_FOREACH(std::string sb, sa) {
					std::cerr << sb << "-";
				}
				std::cerr << std::endl;
			}
		} else
			std::cout << word  << " failed" << std::endl;
	} while (word.size());
	exit(0);
}
