#include <iostream>

#include <thread>
#include <boost/asio.hpp>

#include "HTTPRequest.h"
#include "HTTPClient.h"

namespace http_client {

	try {

		HTTPClient client;

		std::shared_ptr<HTTPRequest> request_one = client.create_request(1);

		request_one->set_host("localhost");
		request_one->set_uri("/index.html");
		request_one->set_port(3333);
		request_one->set_callback(handler);
		request_one->execute();
		std::shared_ptr<HTTPRequest> request_two =
			client.create_request(1);
		request_two->set_host("localhost");
		request_two->set_uri("/example.html");
		request_two->set_port(3333);
		request_two->set_callback(handler);
		request_two->execute();
		request_two->cancel();
		// Do nothing for 15 seconds, letting the
		// request complete.
		std::this_thread::sleep_for(std::chrono::seconds(15));
		// Closing the client and exiting the application.
		client.close();
		
	}
	catch (std::system::system_error& e ) {

		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();
		return e.code().value();

	}

}

namespace syncSSLClient {

	int main()
	{
		const std::string raw_ip_address = "127.0.0.1";
		const unsigned short port_num = 3333;
		try {
			SyncSSLClient client(raw_ip_address, port_num);
			// Sync connect.
			client.connect();
			std::cout << "Sending request to the server... "
				<< std::endl;
			std::string response =
				client.emulate_long_computation_op(10);
			std::cout << "Response received: " << response
				<< std::endl;
			// Close the connection and free resources.
			client.close();
		}
		catch (system::system_error &e) {
			std::cout << "Error occured! Error code = " << e.code()
				<< ". Message: " << e.what();
			return e.code().value();
		}
		return 0;
	}
}