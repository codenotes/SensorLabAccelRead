// AccelSocketRead.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/iter_find.hpp>

//listense to sensorlog ipad app.

using boost::asio::ip::tcp;

boost::asio::streambuf b,b2;
using namespace std;

void handler(const boost::system::error_code& e, std::size_t size)
{
	if (!e)
	{
		std::istream is(&b);
		std::string line;
		std::getline(is, line);
	//	cout <<"read:"<<size<<"###"<< line << endl;
		
		std::list<std::string> stringList;
		std::vector<std::string> list2;
		//boost::iter_split(stringList, line, boost::first_finder(","));
		try
		{ 
			boost::iter_split(list2, line, boost::first_finder(","));
		}
		catch (...)
		{

			cout << "bad" << endl;
			return;
		}

		if (list2.size() >= 6)
		{
			try
			{
				cout << "x:" << list2.at(4) << "\ty:" << list2.at(5) << "\tz:" << list2.at(6) << endl;
			}
			catch (...)
			{
				
				cout << "boom size:" << list2.size() << " line:"<<line<< " insize:"<<size<<endl;
				
			}
		}
		
		/*BOOST_FOREACH(std::string token, stringList)
		{
			std::cout << token << '\n'; ;
		}
*/


	}
}

void handlerError(const boost::system::error_code& e)
{
	if (!e)
	{
		std::istream is(&b);
		std::string line;
		std::getline(is, line);
		//cout <<"read:"<<size<<"###"<< line << endl;

		std::list<std::string> stringList;
		boost::iter_split(stringList, line, boost::first_finder(","));

		BOOST_FOREACH(std::string token, stringList)
		{
			std::cout << token << '\n'; ;
		}



	}
}


//#define SYNC

float heapMedian3(float *a);
extern double datafuck[];
int median2test();
int main(int argc, char* argv[])
{

	for (int i = 0; i < 255; i++)
	{

		printf("%f\n", heapMedian3((float*)&datafuck[i]));

	}

	//median2test();

	return 0;
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: client <host> <port>" << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;
#ifdef SYNCH
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1],argv[2]);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;
#else

		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(argv[1]),boost::lexical_cast<int>(argv[2]) );
			//boost::asio::ip::address::from_string("1.2.3.4"), 12345);


#endif
		boost::system::error_code error = boost::asio::error::host_not_found;
		tcp::socket socket(io_service);

#ifdef SYNC
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);
#else			
			socket.async_connect(endpoint, handlerError)   ;
#endif

			for (;;)
			{
				//boost::array<char, 128> buf;
				boost::system::error_code error;

#ifdef SYNC
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
#else
			boost::asio::async_read_until(socket, b, '\n', handler);
#endif

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

#ifdef SYNC
			std::cout.write(buf.data(), len);
#else
			io_service.run();
#endif

		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}



//
//std::list<std::string> stringList;
//boost::iter_split(stringList, s, boost::first_finder("--"));
//
//BOOST_FOREACH(std::string token, stringList)
//{
//	std::cout << token << '\n'; ;
//}