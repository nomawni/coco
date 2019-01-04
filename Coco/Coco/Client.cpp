#include "pch.h"
#include "Client.h"

#include <boost/asio.hpp>

Client::Client(unsigned char num_of_threads)
{
	m_work.reset(new boost::asio::io_service::work(m_ios));

	for (unsigned char i = 1; i < num_of_threads; i++) {

		std::unique_ptr<std::thread> th(
			new std::thread([this]() {

			m_ios.run();
		}));

		m_threads.push_back(std::move(th));
	}



	//m_thread.reset(new std::thread([this]() {

	//	m_ios.run();
	//}));
}

void Client::emulate(

	unsigned int dur,
	const std::string& raw_ip_address,
	unsigned short port_num,
	Callback callback,
	unsigned int request_id

) {

	// Preparing the request
	std::string request = "EMULATE LONG_CALL_OP "
		+ std::to_string(dur)
		+ "\n";

	std::shared_ptr<Session> session =
		std::shared_ptr<Session>(new Session(m_ios,
			raw_ip_address,
			port_num,
			request,
			request_id,
			callback));

	session->m_sock.open(session->m_endpoint.protocol());

	std::unique_lock<std::mutex> lock(m_active_session_guard);

	m_active_sessions[request_id] = session;

	lock.unlock();

	session->m_sock.async_connect(session->m_endpoint,
		[this, session](const boost::system::error_code& ec) {

		if (ec.value() != 0) {
			
			session->m_ec = ec;
			onRequestComplete(session);
			return;
		 }

		std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

		if (session->m_was_cancelled) {
			onRequestComplete(session);
			return;
		}

		boost::asio::async_write(
			session->m_sock,
			boost::asio::buffer(session->m_request),
			[this, session](const boost::system::error_code& ec,
				std::size_t byte_transferred) {

			if (ec.value() != 0) {
				session->m_ec = ec;
				onRequestComplete(session);
				return;
			}

			std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);

			if (session->m_was_cancelled) {
				onRequestComplete(session);
				return;
			}

			boost::asio::async_read_until(session->m_sock,
				session->m_response_buf,
				'\n',
				[this, session](const boost::system::error_code& ec,
					std::size_t bytes_transferred) {

				if (ec.value() != 0) {
					session->m_ec = ec;
				}
				else {
					std::istream strm(&session->m_response_buf);
					std::getline(strm, session->m_response);
				}

				onRequestComplete(session);




		});

		});

	});

};

void Client::cancelRequest(unsigned int request_id) {

	std::unique_lock<std::mutex> lock(m_active_session_guard);

	auto it = m_active_sessions.find(request_id);

	if (it != m_active_sessions.end()) {

		std::unique_lock<std::mutex> cancel_lock(it->second->m_cancel_guard);

		it->second->m_was_cancelled = true;

		try {

			it->second->m_sock.cancel();

		//	it->second->m_sock.release();

		}
		catch (std::exception e) {
			std::cout << " Error : " << e.what() << std::endl;
		}
	}
}

void Client::close() {

	// Destroy work object

	m_work.reset(NULL);

	// Waiting for the I/O to exit

	for (auto &threads: m_threads )
          
		m_thread->join();

}

void Client::onRequestComplete(std::shared_ptr<Session> session) {

	// Shutting down the connection

	boost::system::error_code err;

	session->m_sock.shutdown(
		boost::asio::ip::tcp::socket::shutdown_both,
		err
	);

	// Remove the session from the active one

	std::unique_lock<std::mutex> lock(m_active_session_guard);

	auto it = m_active_sessions.find(session->m_id);

	if (it != m_active_sessions.end()) {

		m_active_sessions.erase(it);
	   }
	lock.unlock();

	boost::system::error_code ec;

	if (session->m_ec.value() == 0 && session->m_was_cancelled) {

		ec = boost::asio::error::operation_aborted;
	}
	else {
		ec = session->m_ec;
	}

	// Call the callback provided by the user 

	session->m_callback(session->m_id,
		session->m_response,
		ec);
}


Client::~Client()
{
}
