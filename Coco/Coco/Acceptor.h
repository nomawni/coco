#pragma once

#include <thread>
#include <iostream>
#include <boost/asio.hpp>
class Acceptor
{
public:
	Acceptor(boost::asio::io_service& ios, unsigned short port_num);
	~Acceptor();

	void Start();

	void Stop();

private:

	void InitAccept();

	void onAccept(const boost::system::error_code& ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock);

private:
	boost::asio::io_service& m_ios;
	boost::asio::ip::tcp::acceptor m_acceptor;
	std::atomic<bool> m_isStopped;
};

