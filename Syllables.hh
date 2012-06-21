//=======================================================================
// Copyright 2012.
// Authors: S Roychowdhury
// Project: Syllables
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#ifndef _SYLLABLES_
#define _SYLLABLES_
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>

class Syllables {
private:
	typedef std::set<char> cSet;
	typedef std::map<char,char> ccMap;
	typedef std::list<std::string> sList;
	typedef std::list<sList> sListList;
	struct ChkLst {
		ChkLst(std::string _s, bool _retpos) : retpos(_retpos) {
			for (std::string::const_iterator it=_s.begin(); it!=_s.end(); ++it) {
				cs.insert(*it);
			}
		}
		bool operator[] (char c) {
			cSet::const_iterator it = cs.find(c);
			if (it!=cs.end()) return retpos;
			return (!retpos);
		}
	private:
		bool retpos;
		cSet cs;
	};
	struct RepChar {
		RepChar(std::string _str,std::string _rep) {
			int sz = ( _rep.size() < _str.size() ) ? _rep.size() : _str.size() ;
			for (int i=0; i<sz; ++i) {
				cm.insert(std::make_pair<char,char>(_str.at(i),_rep.at(i)));
			}
		}
		char operator[] (char c) {
			ccMap::const_iterator it = cm.find(c);
			if (it!=cm.end()) return it->second;
			return c;
		}
	private:
		ccMap cm;
	};
	ChkLst space;
	ChkLst vowel;
	ChkLst nasal;
	ChkLst approximant;
	ChkLst latApproximant;
	ChkLst flap;
	ChkLst affricate;
	ChkLst fricative;
	ChkLst highVowel;
	ChkLst dipthong;
	ChkLst plosive;
	ChkLst hTester;
	ChkLst rTester;
	ChkLst sTester;
	ChkLst dental;
	ChkLst alveolar;
	ChkLst obstruent;
	ChkLst sonorant;
	RepChar evowels;

	int sonority(char a);

public:
	Syllables();
	~Syllables();
	bool syllabify(std::string word, sListList& oList);
};
#endif /** _SYLLABLES_ */
