#include "pch.h"
#include "Service.h"




Service::Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
	m_socket(sock)
{
}

void Service::StartHandling() {

	boost::asio::async_read_until(*m_socket.get(),
		m_request,
		'\n',
		[this](
			const boost::system::error_code& ec,
			std::size_t bytes_transferred
			) {

		onRequestReceived(ec,
			bytes_transferred);

	});
}

void Service::onRequestReceived(const boost::system::error_code& ec,
	std::size_t bytes_tansferred) {

	if (ec != 0) {
		std::cout << " Error occured! Error code ="
			<< ec.value()
			<< " . Message = " << ec.message();

		onFinish();
	    
		return;
	}

	// Process the request

	m_response = processRequest(m_request);

	// Initiate asynchronous writing

	boost::asio::async_write(*m_socket.get(),
		                     boost::asio::buffer(m_response,
								 [this](
									 const boost::system::error_code& ec,
									 std::size_t bytes_transferred
									 ) {
		onResponseSent(ec, bytes_tansferred);
	});
}

void Service::onResponseSent(const boost::system::error_code& ec,
	std::size_t bytes_transferred) {

	if (ec != 0) {
		
		std::cout << " Error occured! Error code = "
			<< ec.value()
			<< " Message: " << ec.message();
	}

	onFinish();

}

// Here we perform the cleanup

void Service::onFinish() {
	delete this;
}

std::string ProcessRequest(boost::asio::streambuf& request) {

	// We parse the request
	// Emulate CPU -consuming operations

	int i = 0;

	while (i < 1000000)
		i++;

	// Emulate operations that blocks the thread

	std::this_thread::sleep_for(std::chrono::microseconds(100));

	// Returning the response message

	std::string response = "Response\n";

	return response;
}


Service::~Service()
{
}
