#ifndef DETECTION_HPP
#define DETECTION_HPP

#include<string>

class Detection
{

public:
	enum Category {
		SUSPICIOUS,
		PUA,
		UA,
		HARMFUL
	};

	Detection(const Category &category, const std::string &name, const std::string &member_name = "");

	Category getCategory();

	std::string getName();

	std::string getMemberName();

private:
	Category category;
	std::string name;
	std::string member_name;

};

#endif // DETECTION_HPP
