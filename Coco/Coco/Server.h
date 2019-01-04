#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include "Acceptor.h"

class Server
{
public:
	Server();

	void Start(unsigned short port_num,
		       unsigned int thread_pool_size);

	void Stop();
	~Server();

private:
	boost::asio::io_service m_ios;
	std::unique_ptr<boost::asio::io_service::work> m_work;
	std::unique_ptr<Acceptor> m_accept;
	std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

