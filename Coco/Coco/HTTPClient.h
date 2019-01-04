#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
class HTTPClient
{
public:
	HTTPClient();

	std::shared_ptr<HTTPRequest>
		create_request(unsigned int id);

	void close();

	~HTTPClient();

private:

	boost::asio::io_service m_ios;
	std::unique_ptr<boost::asio::io_service::work> m_work;
	std::unique_ptr<std::thread> m_thread;
};

