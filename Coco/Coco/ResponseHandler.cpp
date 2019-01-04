#include "pch.h"
#include "ResponseHandler.h"


ResponseHandler::ResponseHandler()
{
}

ResponseHandler::ResponseHandler(unsigned int request_id,
	const std::string& response ,
	const boost::system::error_code& ec )
	: m_request_id(request_id),
	  m_response(response),
	  m_ec(ec)
{

}

void ResponseHandler::Handler(unsigned int request_id,
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

	return;
}


ResponseHandler::~ResponseHandler()
{
}
