#include <thread>
#include <atomic>
#include <memory>

#include <boost/asio.hpp>
#include <iostream>

#pragma once
class Service
{
public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

	void StartHandling();

	~Service();

private:

	void onRequestReceived( const boost::system::error_code& ec,
		               std::size_t bytes_transferred
	                 );

	void onResponseSent(const boost::system::error_code& ec, std::size_t bytes_transferred);

	void onFinish();

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
	std::string m_response;
	boost::asio::streambuf m_request;
};

