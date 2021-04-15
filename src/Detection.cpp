#include "Detection.hpp"

Detection::Detection(const Category &category, const std::string &name, const std::optional<std::string> &member_name)
	: category{ category }, name{ name }, member_name{ member_name }
{}