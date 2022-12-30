#include "parse.hpp"

// Constructors
parse::parse()
{
	std::cout << "\e[0;33mDefault Constructor called of parse\e[0m" << std::endl;
}

parse::parse(const parse &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of parse\e[0m" << std::endl;
}


// Destructor
parse::~parse()
{
	std::cout << "\e[0;31mDestructor called of parse\e[0m" << std::endl;
}


// Operators
parse & parse::operator=(const parse &assign)
{
	(void) assign;
	return *this;
}

void parse::readFile()
{
	
}
