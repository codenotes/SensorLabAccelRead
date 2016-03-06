#pragma once

#include <vector>
#include <iostream>
#include <stdio.h>
#include "medianfilter.h"


using namespace std;

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
	float deadBand = 0;
	bool feedAccel = false;
	int sampleIndex = 0;

	bool useMedian = false;

	MedianFilter<float> * mpfilter=0;

	void createMedianFilter(int sz)
	{

		if (mpfilter)
		{
			delete mpfilter;
			mpfilter = 0;
		}

		if (!mpfilter)
			mpfilter = new MedianFilter<float>(sz);

		useMedian = true;

	}

	void stopMedianFilter()
	{
		useMedian = false;

	}


	enum CutDirection
	{
		Negative,
		Positive,
		None
	};


	enum CutDirection mCutDir = CutDirection::None;

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


	RK4(vector<float> samples_, float sampleRate_, float timeInitial = 0, float initialX = 0, float initialV = 0)
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

	Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
	{
		State state;
		state.x = initial.x + d.dx*dt;
		state.v = initial.v + d.dv*dt;
		Derivative output;
		output.dx = state.v;
		output.dv = acceleration(state, t + dt);
		return output;
	}

	void integrate(State &state, float t, float dt)
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




	float acceleration(State &state, float t)
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
				ret = (float)samples[sampleIndex++ % 4];
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

