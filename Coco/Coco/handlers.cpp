
#include <iostream>
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include <boost/asio.hpp>

void HTTPHandler(const HTTPRequest& request,
	const HTTPResponse& response,
	const boost::system::error_code& ec)
{
	if (ec == 0) {
		std::cout << "Request #" << request.get_id()
			<< " has completed. Response: "
			<< response.get_response().rdbuf();
	}
	else if (ec == boost::asio::error::operation_aborted) {
		std::cout << "Request #" << request.get_id()
			<< " has been cancelled by the user."
			<< std::endl;
	}
	else {
		std::cout << "Request #" << request.get_id()
			<< " failed! Error code = " << ec.value()
			<< ". Error message = " << ec.message()

			<< std::endl;
	}
	return;
}