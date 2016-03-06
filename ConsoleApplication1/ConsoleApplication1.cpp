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

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;


extern std::vector<float> nudge;

//struct  State;

//float acceleration(const State &state, float t);

bool testt(string s)
{
	cout << "process2:" << s << endl;
	return true;


}

extern unsigned char sample1[];


// Simple RK4 integration framework
// Copyright (c) 2004, Glenn Fiedler
// http://www.gaffer.org/articles

class RK4
{
public:



	struct State
	{
		float x;
		float v;
	};

	struct Derivative
	{
		float dx;
		float dv;
	};

	std::vector<float> samples;
	
	State mState;
	float t, dt;
	float sampleRate;
	float * sampleBlock = 0;
	float deadBand=0;
	bool feedAccel = false;
	int sampleIndex = 0;
	
	enum CutDirection
	{
		Negative,
		Positive,
		None
	};


	enum CutDirection mCutDir=CutDirection::None;

	void setCutDirection(enum CutDirection cut)
	{
		mCutDir = cut;
	}

	RK4(float * sampleBlock, float sampleRate_, float timeInitial = 0, float initialX = 0, float initialV = 0)
	{

		feedAccel = true;
		sampleBlock = sampleBlock;


		sampleRate = sampleRate_;
		t = timeInitial;
		dt = 1.0f / sampleRate_;
		mState.x = initialX;
		mState.v = initialV;

	}


	RK4(vector<float> samples_,float sampleRate_,float timeInitial=0, float initialX=0,float initialV=0)
	{

		setSample(samples_);
		sampleRate = sampleRate_;
		t = timeInitial;
		dt = 1.0f / sampleRate_;
		mState.x = initialX;
		mState.v = initialV;

	}

	//+- values to ignore...base reading for accelerometers
	void setDeadBand(float b)
	{
		deadBand = b;
	}

	void setSample(std::vector<float>&sample)
	{
		samples = sample;

	}

	Derivative evaluate(const State &initial, float t)
	{
		Derivative output;
		output.dx = initial.v;
		output.dv = acceleration(mState, t);
		return output;
	}

	Derivative evaluate( const State &initial, float t, float dt, const Derivative &d)
	{
		State state;
		state.x = initial.x + d.dx*dt;
		state.v = initial.v + d.dv*dt;
		Derivative output;
		output.dx = state.v;
		output.dv = acceleration(state, t + dt);
		return output;
	}

	void integrate(  State &state, float t, float dt)
	{
		Derivative a = evaluate(state, t);
		Derivative b = evaluate(state, t, dt*0.5f, a);
		Derivative c = evaluate(state, t, dt*0.5f, b);
		Derivative d = evaluate(state, t, dt, c);

		const float dxdt = 1.0f / 6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
		const float dvdt = 1.0f / 6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

		state.x = state.x + dxdt*dt;
		state.v = state.v + dvdt*dt;
	}


	void integrate(float * block, float t, float dt)
	{
		sampleBlock = block;
		integrate(mState, t, dt);

	}




	float acceleration( State &state, float t)
	{
		//	const float k = 10;
		//	const float b = 1;
		//	return - k*state.x - b*state.v;
		
		float ret;


	

		if (sampleIndex >= samples.size())
		{
			cout << "exceed sample index at:" << sampleIndex << endl;
			return 0;
		}

		try
		{
			//cout <<"*"<< i << endl;
			if (feedAccel)
			{
				ret = (float)samples[sampleIndex++%4];
			}
			else
				ret = (float)samples[sampleIndex++];

			if (abs(ret) <= deadBand)
			{
				return 0;
			}


			if (mCutDir == CutDirection::None)
			{
				return ret;
			}



			if (mCutDir == CutDirection::Negative) //then we are interested in only positive
			{

				if (ret > 0)
				{
					return ret;

				}
				else
				{
					return 0;
				}

			}
			else
			{

				if (ret < 0)
				{
					return ret;

				}
				else
				{
					return 0;
				}



			}

			


		}
		catch (const std::exception&)
		{
			printf("boom, index:%d", sampleIndex);
		}

	}

	void go()
	{

		float numsamples = samples.size();
		float looplen = numsamples / sampleRate / 4.0f;

		while (t < looplen)
		{
			printf("%.2f, %.2f\n", mState.x, mState.v);
			integrate(mState, t, dt);
			t += dt;
		}

	}

	


};

extern unsigned char sample1[];



int parsetest()
{
	ParseIt p('\n',',');
	string input;


	p.setCB(testt);
	//MyTestFunction f = boost::bind(testt);
	
//	sample1[225] = 0;
	string s((char*)sample1);

	//cout << s << endl;

	//for (auto c : s)
	//{
	//	cout << c << " " << (int)c << endl;
	//	if (c == '\n')
	//		cout << "found!" << endl;
	//}
//	printf("%s", (char*)sample1);


	p.feedstate((char*) sample1   );
	
	
	return 0;
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



int main()
{


//	State state;
//	state.x = 0; //was 100
//	state.v = 0;
//	float sampleratehz = 125;/// 125 hz = 1 / 125 =.008ms
//
//	float t = 0;
//	float dt = 1/sampleratehz;

	//sampple nudge is 39.5 seconds, 125hz which is 8ms or .008 of second. 
	//8*15=2,528ms...=2.5 seconds

	//while (fabs(state.x)>0.001f || fabs(state.v)>0.001f)
	//printf("size is:%d", nudge.size());
	//316 samples

	//return 0;

	RK4 rk4(nudge,125);
	rk4.setDeadBand(4);
	rk4.setCutDirection(RK4::CutDirection::None);
	//rk4.setSample(nudge);
	//rk4.sampleRate = 125;
	//rk4.t = 0;
	//rk4.dt = 1 / rk4.sampleRate;
	//rk4.mState.x = 0;
	//rk4.mState.v = 0;


	rk4.go();

	//getc(stdin);

	return 0;
}
