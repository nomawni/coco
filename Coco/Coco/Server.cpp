#include "pch.h"
#include "Server.h"


Server::Server()
{
	m_work.reset(new boost::asio::io_service::work(m_ios));
}

void Server::Start(unsigned short port_num,
	               unsigned int thread_pool_size) {

	assert(thread_pool_size > 0);

	// Create and Start Acceptor

	m_accept.reset(new Acceptor(m_ios,
		port_num));

	m_accept->Start();

	// Create Specified number of thread to the pool

	for (unsigned int i = 0; i < thread_pool_size; i++) {

		std::unique_ptr<std::thread> th(
			new std::thread(
				[this]() {
			m_ios.run();

		}));

		m_thread_pool.push_back(std::move(th));
	}

}

void Server::Stop() {

	m_accept->Stop();

	m_ios.stop();

	for (auto& th : m_thread_pool) {

		th->join();
	}
}

Server::~Server()
{
}
