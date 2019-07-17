#include "pch.h"
#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(const int argc, char ** argv)
{
	for (auto i = 1; i < argc; ++i)
	{
		tokens_.emplace_back(argv[i]);
	}
	parametr_parser();
	option_parser();
}	

std::vector <std::string> ArgumentParser::get_options()
{
	return options_;
}


void ArgumentParser::option_parser()
{
	if (options_.size() != 0u)
	{
		if ((options_[2u] != "i") && (options_[2u] != "s"))
		{
			throw std::runtime_error("Wrong type!");
		}
		if ((options_[3u] != "a") && (options_[3u] != "d"))
		{
			throw std::runtime_error("Wrong type!");
		}
	}
}


void ArgumentParser::parametr_parser()
{
	if ((tokens_.size() != 0u) && (tokens_.size() <= 4u))
	{
		if ((tokens_.size() == 1u) && (tokens_[0u] == "--help"))
		{
			std::cout << "Examples of cmd commands:" << std::endl;
			std::cout << "TestProblem.exe C:\check --out-prefix=sorted_ --content-type=i --sort-mode=a" << std::endl;
			std::cout << "TestProblem.exe C:\check --out-prefix=sorted_ --content-type=s --sort-mode=a" << std::endl;
			std::cout << "TestProblem.exe C:\check --out-prefix=sorted_ --content-type=i --sort-mode=d" << std::endl;
			std::cout << "TestProblem.exe C:\check --out-prefix=sorted_ --content-type=s --sort-mode=d" << std::endl;
		}
		options_.emplace_back(tokens_[0u]);
		if ((tokens_.size() >= 2u) && (tokens_[1u].substr(0u, 13u) == "--out-prefix="))
		{
			options_.emplace_back(tokens_[1u].substr(13u, tokens_[1u].size()));
		}
		else
		{
			throw std::runtime_error("Wrong first parametrs!");
		}
		if ((tokens_.size() >= 3u) && (tokens_[2u].substr(0u, 15u) == "--content-type="))
		{
			options_.emplace_back(tokens_[2u].substr(15u, tokens_[2u].size()));
		}
		else
		{
			throw std::runtime_error("Wrong second parametrs!");
		}
		if ((tokens_.size() >= 4u) && (tokens_[3u].substr(0u, 12u) == "--sort-mode="))
		{
			options_.emplace_back(tokens_[3u].substr(12u, tokens_[3u].size()));
		}
		else
		{
			throw std::runtime_error("Wrong third parametrs!");
		}
	}
	else
	{
		throw std::runtime_error("Wrong parametrs!");
	}
}
