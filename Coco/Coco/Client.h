#pragma once
#include <boost/asio.hpp>
#include <boost/predef.h>

#ifdef BOOST_OS_WINDOWS

#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502 // windows server 2003 or earlier

#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO

#endif
#endif // BOOST_OS_WINDOWS

#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <string>

typedef void(*Callback) (unsigned int request_id, const std::string& response, const boost::system::error_code& ec);


// Structure representing a context of a single request

struct Session
{
	Session(boost::asio::io_service& ios, 
		    const std::string& raw_ip_address,
		    unsigned short port_num,
		    const std::string& request,
		    unsigned int id,
		    Callback callback):

		    m_sock{ios},
		    m_endpoint{boost::asio::ip::address::from_string(raw_ip_address), port_num},
		    
		    m_request{request},
		    m_id{id},
		    m_callback{callback},
		    m_was_cancelled(false) {}

	boost::asio::ip::tcp::socket m_sock;
	boost::asio::ip::tcp::endpoint m_endpoint;
	std::string m_request;

	boost::asio::streambuf m_response_buf;
	std::string m_response;

	boost::system::error_code m_ec;

	unsigned int m_id; // Unique id assign to the request

	Callback m_callback;
	bool m_was_cancelled;
	std::mutex m_cancel_guard;

};


class Client :
	public boost::asio::noncopyable
{
public:
	Client(unsigned char num_of_threads);
	~Client();

	void emulate(

		unsigned int dur,
		const std::string& raw_ip_address,
		unsigned short port_num,
		Callback callback,
		unsigned int request_id

	);

	// Cancel the request

	void cancelRequest(unsigned int request_id);

	void close();

private:

	void onRequestComplete(std::shared_ptr<Session> session);

private:
	boost::asio::io_service m_ios;
	std::map<int, std::shared_ptr<Session>> m_active_sessions;
	std::mutex m_active_session_guard;
	std::unique_ptr<boost::asio::io_service::work> m_work;
	std::unique_ptr<std::thread> m_thread;
	std::list<std::unique_ptr<std::thread>> m_threads;
};

