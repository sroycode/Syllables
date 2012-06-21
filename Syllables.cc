//=======================================================================
// Copyright 2012.
// Authors: S Roychowdhury
// Project: Syllables
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <algorithm>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "Syllables.hh"
#define DEFAULT_NONE					""
#define DEFAULT_SPACE				" -_+"
#define DEFAULT_VOWEL				"ieauocyw*"
#define DEFAULT_NASAL				"mn"
#define DEFAULT_APPROXIMANT		"rjw"
#define DEFAULT_LATAPPROXIMANT	"l"
#define DEFAULT_FLAP					"j"
#define DEFAULT_AFFRICATE			"xj"
#define DEFAULT_FRICATIVE			"fvtdszh"
#define DEFAULT_HIGHVOWEL			"iu"
#define DEFAULT_DIPTHONG			"ywc"
#define DEFAULT_PLOSIVE				"pbtdkg"
#define DEFAULT_HTESTER				"h"
#define DEFAULT_RTESTER				"r"
#define DEFAULT_STESTER				"s"
#define DEFAULT_DENTAL				"ntdrlj"
#define DEFAULT_ALVEOLAR			"ntdszrjl"
#define DEFAULT_EVOWELS				"aeoy","uiuj"
#define UPPERCASE_CHAR_A			'A'

Syllables::Syllables() :
	space(DEFAULT_SPACE,true),
	vowel(DEFAULT_VOWEL,true),
	nasal(DEFAULT_NASAL,true),
	approximant(DEFAULT_APPROXIMANT,true),
	latApproximant(DEFAULT_LATAPPROXIMANT,true),
	flap(DEFAULT_FLAP,true),
	affricate(DEFAULT_AFFRICATE,true),
	fricative(DEFAULT_FRICATIVE,true),
	highVowel(DEFAULT_HIGHVOWEL,true),
	dipthong(DEFAULT_DIPTHONG,true),
	plosive(DEFAULT_PLOSIVE,true),
	hTester(DEFAULT_HTESTER,true),
	rTester(DEFAULT_RTESTER,true),
	sTester(DEFAULT_STESTER,true),
	dental(DEFAULT_DENTAL,true),
	alveolar(DEFAULT_ALVEOLAR,true),
	obstruent((DEFAULT_VOWEL DEFAULT_NASAL DEFAULT_LATAPPROXIMANT DEFAULT_APPROXIMANT),false),
	sonorant((DEFAULT_VOWEL DEFAULT_NASAL DEFAULT_LATAPPROXIMANT DEFAULT_APPROXIMANT),true),
	evowels(DEFAULT_EVOWELS)
{
}
Syllables::~Syllables() {}
int Syllables::sonority(char a)
{
	if (highVowel[a])
		return 9;
	else if (vowel[a] && !highVowel[a])
		return 9;
	else if (dipthong[a])
		return 9;
	else if (approximant[a] || latApproximant[a])
		return 7;
	else if (nasal[a])
		return 6;
	else if (fricative[a])
		return 5;
	else if (affricate[a])
		return 4;
	else if (flap[a])
		return 3;
	else if (plosive[a])
		return 2;
	else if (sTester[a])
		return 0;
	else
		return 11; //return value that cannot create false positives
}
bool Syllables::syllabify(std::string word,sListList& oList)
{
	//sListList oList;
	oList.clear();
	int ctr=0;
	boost::algorithm::to_lower(word);
	std::string nword;
	char _prevc =UPPERCASE_CHAR_A;
	for (std::string::const_iterator it=word.begin(); it!=word.end(); ++it) {
		if (*it == _prevc && ctr>0) {
			nword[nword.size()-1]=evowels[*it];
		} else
			nword.push_back(*it);
		_prevc=*it;
		++ctr;
	}
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sepA(DEFAULT_SPACE,DEFAULT_VOWEL);
	tokenizer tokensA(nword, sepA);
	typedef std::vector<std::string> sVec;
	typedef std::vector<bool> bVec;
	sVec StrVec;
	bVec HasVowelVec;
	ctr=0;
	BOOST_FOREACH(std::string tokA, tokensA) {
		if (vowel[tokA.at(0)] && StrVec.size()>0) {
			StrVec.back() += tokA;
			HasVowelVec.back() = true;
		} else {
			StrVec.push_back(tokA);
			HasVowelVec.push_back(false);
		}
		++ctr;
	}
	if (ctr<1) return false;
	if (ctr==1) {
		sList ts;
		ts.push_back(nword);
		oList.push_back(ts);
		return true;
	}
	for (int i=0; i<(int)StrVec.size(); ++i) {
		sList ostl;
		for (int j=0; j<i; ++j) ostl.push_back(StrVec.at(j));
		for (int j=i; j<(int)StrVec.size(); ++j) {
			std::string s = StrVec.at(j);
			int delpos=0;
			for (int k=1; k<(int)s.size()-1; ++k) {
				bool flag=false;
				if ((sonority(s.at(k-1)) > sonority(s.at(k))) && !sTester[s.at(k-1)]) flag=true;
				else if (obstruent[s.at(k-1)] && !sTester[s.at(k-1)]) {
					if (obstruent[s.at(k)] && !nasal[s.at(k)]) flag=true;
				} else if (dental[s.at(k-1)] && plosive[s.at(k-1)]) {
					if (latApproximant[s.at(k)]) flag=true;
				} else if (alveolar[s.at(k-1)] && fricative[s.at(k-1)]) {
					if (rTester[s.at(k)]) flag=true;
				} else if (hTester[s.at(k)]) flag=true;
				if (flag && (k-1)>delpos) delpos=k-1;
			}
			if (i==0) continue;
			if (!HasVowelVec.at(i-1)) {
				ostl.push_back(s);
				continue;
			}
			if (delpos==0 && s.size()>1) delpos=1; // lower vowel
			if (delpos>0) {
				ostl.back().append(s,0,delpos);
				s.erase(0,delpos);
				ostl.push_back(s);
			} else {
				ostl.back().append(s);
			}
		}
		oList.push_back(ostl);
	}
	bVec CharBreakVec(word.length()+1,false);
	CharBreakVec[0]=true;
	BOOST_FOREACH(sList sa, oList) {
		int parsed=0;
		BOOST_FOREACH(std::string sb, sa) {
			parsed+=sb.length();
			CharBreakVec[parsed]=true;
		}
	}
	sList pstl;
	for (std::size_t i=0; i<word.length(); ++i) {
		std::string xs ( 1, word[i]);
		if (CharBreakVec[i]) {
			pstl.push_back(xs);
		} else {
			pstl.back().append(xs);
		}
	}
	oList.push_back(pstl);
	return true;
}
