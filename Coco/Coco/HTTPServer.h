#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>
#include <thread>
#include <iostream>

class HTTPServer
{

	static const std::map<unsigned int, std::string> http_status_table;
		
public:
	HTTPServer(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

	void on_request_line_received(
		const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_headers_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void process_request();

	void on_response_sent(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void on_finish();

	void send_response();

	void start_handling();
	~HTTPServer();

private:



private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	boost::asio::streambuf m_request;
	std::map<std::string, std::string> m_request_headers;
	std::string m_requested_resource;
	std::unique_ptr<char[]> m_resource_buffer;
	unsigned int m_response_status_code;
	std::size_t m_resource_size_bytes;
	std::string m_response_headers;
	std::string m_response_status_line;
};



