#include <iostream>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

class Parser
{

public:
	string line;
	bool started = false;
	int firstEol = -1;
	bool ready = false;
	tokenizer<char_separator<char>> * mTokens;

	void setDelim(char delim)
	{
		mDelim = delim;
	}

	void setEOL(char eol)
	{
		mEol = eol;
	}

	void AddStream(std::string  s)
	{

		char_separator<char> sep(";");	
		mTokens=new tokenizer<char_separator<char>>(s, sep);
	//	mTokens = tokens;



		for (auto a : *mTokens)
		{
			cout << a << " ";
		}

		cout << endl;
	}


	void AddStream2(string  s)
	{
		
		int loc = s.find(mEol);


		if (!started && loc == -1)
		{
			//if we haven't started and we don't find EOL then jut dont do anything
			return;
		}

		if (!started && loc > -1 && firstEol==-1) //this is the first EOL
		{
			started = true;//now 
			//take everything to the right of 
			firstEol = loc;
			line += s.substr(loc, s.size() - loc);
			line.erase(0, 1);
			//cout << "so far:" << line << endl;	
			return;

		}

		if (started && loc == -1)//no eol but we are inside the string;
		{

			line += s;
			return;

		}

		

		if (started && loc > -1 && firstEol>-1) //this is the terminating EOL, we have a line
		{

	//		firstEol = -1;
//			started = false;
	//		lastEol = loc;

			//split the string on the eol and process the left
			line += s.substr(0, loc);
			//s.pop_back();
			Process();

			//is there anything to the right?
			line = s.substr(loc + 1, s.size() - 1);
			firstEol = loc;


		

		}




	}

	void Process()
	{
		cout << "line:" << line << endl;
	}
		char mDelim;
		char mEol;
		int lastEol=-1;
};

void test()
{
	Parser p;
	p.setEOL(';');

	p.AddStream("one");
	p.AddStream(";two");
	p.AddStream(",three,");
	p.AddStream("four;five");
	p.AddStream(";six,sevon;");
//	p.AddStream("flooby");

	cout << "remaining is:" << p.line << endl;

}