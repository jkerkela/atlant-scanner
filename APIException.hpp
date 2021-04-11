#ifndef APIEXCEPTION_HPP
#define APIEXCEPTION_HPP

#include<stdexcept>

class APIException : public std::exception {
public:
	explicit APIException(const std::string& message) : api_exception(message){};

	const char* what() const noexcept override { return api_exception.what(); };

private:
	std::runtime_error api_exception;
};


#endif // APIEXCEPTION_HPP