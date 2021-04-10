#ifndef APIEXCEPTION_HPP
#define APIEXCEPTION_HPP

#include<stdexcept>

class APIException : public std::exception {
public:
	explicit APIException(const std::string& message) : exception{ message }
	{}

	const char* what() const noexcept override {return message.what()}
};


#endif // APIEXCEPTION_HPP