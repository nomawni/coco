#pragma once

#include <iostream>
#include <boost/asio.hpp>

class HTTPResponse
{
	friend class  HTTPRequst;
	

public:
	HTTPResponse();
	~HTTPResponse();

	unsigned int get_status_code() const;

	const std::string& get_status_message() const;

	const std::map<std::string, std::string>& get_headers() const;

	const std::istream& get_response() const;

private:

	boost::asio::streambuf& get_response_buf();

	void set_status_code(unsigned int status_code);

	void set_status_message(const std::string& status_message);

	void add_header(const std::string& name,
		const std::string& value);

private:

	unsigned int m_status_code;
	std::string m_status_message;

	// Response headers

	std::map<std::string, std::string> m_headers;
	
	boost::asio::streambuf m_response_buf;

	std::istream m_response_stream;


};

