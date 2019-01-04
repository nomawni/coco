#include "pch.h"
#include "ErrorsCategory.h"

using namespace httpErrors;

ErrorsCategory::ErrorsCategory()
{
}


const char* ErrorsCategory::name() const BOOST_SYSTEM_NOEXCEPT {

	return "http_errors";
}

std::string ErrorsCategory::message(int e) const {

	switch (e)
	{
	case INVALID_RESPONSE:
		return " Server response can not be parsed !";
		break;
	default:
		return " Unknown Error ";
		break;
	}

}

 const boost::system::error_category&  ErrorsCategory::getErrorsCategory() {

	 static ErrorsCategory category;

	 return category;

}

 boost::system::error_code ErrorsCategory::makeErrorCode(httpErrorsCodes e) {

	 return boost::system::error_code(
		 static_cast<int> (e), getErrorsCategory());

 }


ErrorsCategory::~ErrorsCategory()
{
}
