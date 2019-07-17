#pragma once

#include <string>
#include <vector>
#include <iostream>

class ArgumentParser final
{
public:
	ArgumentParser(int argc, char ** argv);
	std::vector <std::string> get_options();
private:
	void parametr_parser();
	void option_parser();
	std::vector<std::string> tokens_;
	std::vector<std::string> options_;
};