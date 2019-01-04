#include "pch.h"
#include "Acceptor.h"
#include "Service.h"


Acceptor::Acceptor(boost::asio::io_service& ios, unsigned short port_num ) :
	
	m_ios(ios),
	m_acceptor(m_ios,
		boost::asio::ip::tcp::endpoint(
			boost::asio::ip::address_v4::any(),
			 port_num
			  )
		),
	m_isStopped(false)
{

}

// Start accepting incoming requests

void Acceptor::Start() {
	m_acceptor.listen();
	InitAccept();
}

// Stop accepting incoming requests

void Acceptor::Stop() {
	m_isStopped.store(true);
}

void Acceptor::InitAccept() {

	std::shared_ptr<boost::asio::ip::tcp::socket>
		sock(new boost::asio::ip::tcp::socket(m_ios));

	m_acceptor.async_accept(*sock.get(),
		[this, sock](
			const boost::system::error_code& ec
			) {
		onAccept(ec, sock);
	});
}

void Acceptor::onAccept(const boost::system::error_code& ec,
	                   std::shared_ptr<boost::asio::ip::tcp::socket> sock) {

	if (ec == 0) {

		(new Service(sock))->StartHandling();
	}
	else {
		std::cout << " Error occured! Error Message = "
			<< ec.value()
			<< ". Message: " << ec.message();
	}

	// Init next async accept operation if
	// acceptor has not been stopped yet

	if (!m_isStopped.load()) {

		InitAccept();
	}
	else {
		// Stop accepting incoming request 

		m_acceptor.close();
	}
}

Acceptor::~Acceptor()
{
}
