// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
//#include <boost/tokenizer.hpp>
//#include <vector>
//#include <list>
//#include <queue>
#include "..\AccelSocketRead\ParseIt.h"
using namespace std;



bool testt(string &s)
{
	cout << "process2:" << s << endl;
	return true;


}

int main()
{
	ParseIt p;
	string input;


	//p.setCB(boost::bind(testt));
	MyTestFunction f = boost::bind(testt);


	//p.feedstate("11;22;33;");
	//p.feedstate("4");
	//p.feedstate("5;6;777");
	//p.feedstate(";8;");

	//list<string> l;
	//l.push_back("hi");
	//l.back();
	//l.front();

//	return 0;
	//p.token("11;22;33;44");
	//p.token(";55;;6;777;0");
	//p.token("888;999;");

//	p.token("1;2");
//	p.token("3;4;5;66");// 4; 55; 66");// 55; 66; ");
//	p.token("777777;");
	//p.token("7;8;9;");
	


//	return 0;
	//input = "11;22;";

	//int first = input.find(';', 0);
	//cout << first << endl;
	//input = input.erase(0, first);
	//cout << input << endl;
	//int second = input.find(';', first + 1);
	//cout << second << endl;
	//cout << input.substr(first - 1, second - 1)<<endl;
	//cout<<  input.erase(first-1, second-1) << endl;
	//

	//return 0;

	while (1)
	{
		//p.AddStream(";hi there;how;are;you;today");
		getline(cin, input);
		p.feedstate(input);

	}
	
    return 0;
}

