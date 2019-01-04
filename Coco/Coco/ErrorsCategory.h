#pragma once

#include <boost/asio.hpp>
#include <iostream>

#include <thread>
#include <mutex>
#include <memory>

namespace boost {
	namespace system {
		template<>
		struct is_error_code_enum
		<httpErrors::httpErrorsCodes>
		{
			BOOST_STATIC_CONSTANT(bool, value = true);
		};
	}
}

namespace httpErrors {

	enum httpErrorsCodes
	{
		INVALID_RESPONSE = 1
	};


	class ErrorsCategory : public boost::system::error_category
	{
	public:
		ErrorsCategory();

		const char* name() const BOOST_SYSTEM_NOEXCEPT;

		std::string message(int e) const;

		const boost::system::error_category& getErrorsCategory();

		boost::system::error_code makeErrorCode(httpErrorsCodes e);

		~ErrorsCategory();
	};

}

