#ifndef DETECTION_HPP
#define DETECTION_HPP

#include <string>
#include <optional>

class Detection
{

public:
	enum Category {
		SUSPICIOUS,
		PUA,
		UA,
		HARMFUL
	};

	Detection(const Category &category, const std::string &name, const std::optional<std::string> &member_name = std::nullopt)
		: category{ category }, name{ name }, member_name{ member_name } {};

	Category getCategory() { return category;  };

	std::string getName() { return name; };

	std::optional<std::string> getMemberName() { return member_name; };

private:
	Category category;
	std::string name;
	std::optional<std::string> member_name;

};

#endif // DETECTION_HPP
