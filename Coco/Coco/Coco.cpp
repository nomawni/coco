// Coco.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <boost/asio.hpp>
#include "Client.h"
#include "ResponseHandler.h"
#include "Server.h"

void handler(unsigned int request_id,
	const std::string& response,
	const boost::system::error_code& ec) {

	if (ec.value() == 0) {

		std::cout << "Request #" << request_id
			<< "Has completed: Response : "
			<< response << std::endl;
	}
	else if (ec == boost::asio::error::operation_aborted) {

		std::cout << " Request #" << request_id
			<< " Has been cancelled by the user" << std::endl;
	}
	else {
		std::cout << " Request #" << request_id
			<< "Failed! Error code = " << ec.value()
			<< " Error Message " << ec.message() << std::endl;

	}
}

// The default size of the thread => 
// Server

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

int main()
{

	unsigned short port_num = 3333;
	try {

		std::cout << " Welcome" << std::endl;

		auto num_threads = std::thread::hardware_concurrency();

		Client client(num_threads);

		//ResponseHandler handler;

		

		// Initiate a request with id 1

		client.emulate(10,
			"127.0.0.1",
			3333,
			handler,
			1
		);

		std::this_thread::sleep_for(std::chrono::seconds(5));

		// Initiate another request 

		client.emulate(11,
			"127.0.0.1",
			3333,
			handler,
			2);

		// Then decide to cancel the request with id 1

		client.cancelRequest(1);

		// Does nothing for 6 seconds 

		std::this_thread::sleep_for(std::chrono::seconds(6));

		// Initiate one more request with id 3

		client.emulate(12,
			"127.0.0.1",
			3333,
			handler,
			3);

		// Does nothing for 15 seconds

		std::this_thread::sleep_for(std::chrono::seconds(15));

		// Then decide to exit the application

		client.close();



	}
	catch (boost::system::system_error &ec) {

		std::cout << "Error occured: The error code is : " << ec.code()
			<< " Message " << ec.what();

			return ec.code().value();
	}

	try {
		Server server;

		unsigned int thread_pool_size =
			std::thread::hardware_concurrency() * 2;

		if (thread_pool_size == 0) {
			thread_pool_size = DEFAULT_THREAD_POOL_SIZE;

			server.Start(port_num, thread_pool_size);

			std::this_thread::sleep_for(std::chrono::seconds(60));

			server.Stop();
		}
	}
	catch (boost::system::system_error& e) {

		std::cout << " Error occured! Error code = "
			<< e.code() << " Message. "
			<< e.what();

	}

  //  std::cout << "Hello World!\n"; 

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
