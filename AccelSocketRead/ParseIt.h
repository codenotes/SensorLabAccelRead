#pragma once

#include <string>
#include <iostream>
//#include <boost/tokenizer.hpp>
//#include <vector>
//#include <list>
//#include <queue>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace std;

typedef boost::function<bool(std::string)> MyTestFunction;

class ParseIt
{
public:
	string line;
	char mEol = ';';
	char mDelim = ',';

	bool Process(string &s)
	{
		cout << "process:" << s << endl;
		return true;
	}

	boost::function<bool(string &s)> callback;
	
	void setCB(MyTestFunction & fs)
	{
		callback = fs;

	}

	void state(char c)
	{

		static string st = "";
		static bool first = false;

		bool second = false;

		if (c == ';'&&!first)
		{
			first = true;
			return;
		}

		if (first)
		{
			if (c == ';')
			{
				if (!st.size())
					return;

				Process(st);
				callback(st);

				st.clear();
				//		first = false;
				return;
			}

			st += c;

		}



	}

	void feedstate(string s)
	{
		for (auto c : s)
		{
			state(c);
		}


	}
};