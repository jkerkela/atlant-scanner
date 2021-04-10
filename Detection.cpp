#include "Detection.hpp"

Detection::Detection(const Category &category, const std::string &name, const std::string &member_name)
	: category{ category }, name{ name }, member_name{ member_name }
{}

Detection::Category Detection::getCategory()
{
	return category;
}

std::string Detection::getName()
{
	return name;
}

std::string Detection::getMemberName()
{
	return member_name;
}