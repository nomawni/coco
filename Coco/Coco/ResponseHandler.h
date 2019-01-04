#pragma once

#include <iostream>
#include <boost/asio.hpp>

class ResponseHandler
{
public:
	ResponseHandler();

	ResponseHandler(unsigned int request_id ,
		const std::string& response ,
		const boost::system::error_code& ec );

	void Handler(unsigned int request_id,
		const std::string& response,
		const boost::system::error_code& ec);

	~ResponseHandler();

private:

	unsigned int m_request_id;

	const std::string m_response;

	const boost::system::error_code m_ec;

};

