#include "pch.h"
#include "HTTPClient.h"
#include "HTTPRequest.h"


HTTPClient::HTTPClient()
{
	m_work.reset(new boost::asio::io_service::work(m_ios));
	m_thread.reset(new std::thread([this]() {
		m_ios.run();
	}));
}

std::shared_ptr<HTTPRequest>
HTTPClient::create_request(unsigned int id)
{

	return std::shared_ptr<HTTPRequest>(
		new HTTPRequest(m_ios, id));
}
void HTTPClient::close() {
	// Destroy the work object.
	m_work.reset(NULL);
	// Waiting for the I/O thread to exit.
	m_thread->join();
}


HTTPClient::~HTTPClient()
{
}
