#pragma once

#include <boost/asio.hpp>
#include <iostream>

#include "HTTPResponse.h"

typedef void(*Callback)(const HTTPRequest& request,
	const HTTPResponse& response,
	const boost::system::error_code& ec);



class HTTPRequest
{
	friend class HTTPResponse;

	static const unsigned int DEFAULT_PORT = 80;

public:

	HTTPRequest(boost::asio::io_service& ios, unsigned int id);

	void set_host(const std::string& host);

	void set_port(unsigned int port);

	void set_uri(const std::string& uri);

	void set_callback(Callback callback);

	std::string get_host() const ;

	unsigned int get_port() const;

	const std::string& get_uri() const;

	unsigned int get_id() const;

	void execute();

	void cancel();


	~HTTPRequest();

private:

	void on_host_name_resolved(
		const boost::system::error_code& ec,
		boost::asio::ip::tcp::resolver::iterator iterator);

	void on_connection_established(
		const boost::system::error_code& ec,
		boost::asio::ip::tcp::resolver::iterator iterator);

	void on_request_sent(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_status_line_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_headers_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_response_body_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_Finish();

private:

	// Request parameters

	std::string m_host;
	unsigned int m_port;
	std::string m_uri;

	// Object unique identifier

	unsigned int m_id;

	Callback m_callback;

	// Buffer containing the request line

	std::string m_request_buf;

	boost::asio::ip::tcp::socket m_sock;

	boost::asio::ip::tcp::resolver m_resolver;

	HTTPResponse m_response;

	bool m_was_cancelled;
	
	std::mutex m_cancel_mux;

	boost::asio::io_service& m_ios;
};

