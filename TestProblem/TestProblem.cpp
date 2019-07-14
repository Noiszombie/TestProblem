
#include "pch.h"
#include "ArgumentParser.h"
#include "ThreadPool.h"
#include "FileManager.h"
#include <iostream>
#include <mutex>
#include <filesystem>
#include <fstream>
void do_parallel(std::filesystem::path file)
{
	std::cout << file << std::endl;
}

std::string str_sub(std::string str1, std::string str2)
{
	std::string result;
	if (str1.length() < str2.length())
		return "";
	for (auto i = 0u; i < str1.length(); i++)
	{
		if (str1[i] != str2[i])
			result.push_back(str1[i]);
	}
	result.push_back('\0');
	return result;
}

auto sort = [](std::vector <int> vect) {
	int key;
	int j;
	for (auto i = 1u; i < vect.size(); i++)
	{
		key = vect[i];
		j = i - 1;
		while (j >= 0 && vect[j] > key)
		{
			vect[j + 1] = vect[j];
			j--;
		}
		vect[j + 1] = key;
	}
	for (auto each : vect)
		std::cout << each << std::endl;
	return vect;
};


std::vector <int> reverse(std::vector <int> vect)
{
	std::vector <int> reversed_vect_;
	for (auto i = 0u; i < vect.size(); i++)
		reversed_vect_.emplace_back(vect[vect.size()-i-1u]);
	return reversed_vect_;
}

int main(int argc, char ** argv)
{
	ArgumentParser argument_parser{ argc, argv };
	FileManager dir(argument_parser.get_options());
	auto options_ = argument_parser.get_options();

	auto task = [](std::filesystem::path atom, std::vector <std::string> options)
	{
		char buff[50];
		std::vector <int> raw_vect_;
		std::ifstream fin(atom);
		while (fin.getline(buff, 50))
		{
			raw_vect_.emplace_back(std::stoi(buff));
		}
		fin.close();
		std::vector <int> sorted_vect_ = sort(raw_vect_);
		if (options[3u] == "d")
			sorted_vect_ = reverse(sorted_vect_);
		std::ofstream fout(options[1u]+atom.string());
		std::cout << options[1u] + str_sub(atom.string(), options[1u])  << std::endl;
		for (auto elem : sorted_vect_)
			fout << elem << std::endl;
	};
	std::deque <std::filesystem::path> files = dir.get_files_();


	std::vector <int> a = { 1, 2 , 3, 4, 5 };
	a = reverse(a);
	for (auto & elem : a)
		std::cout << elem;

	ThreadPool thread_pool{ 2u };


	for (auto & atom : files)
	{
		auto result = thread_pool.add_task(task, atom, options_);
	}
	std::this_thread::sleep_for(std::chrono::seconds{ 1u });
	
	thread_pool.stop();
	std::cout << "End game" << std::endl;
	return 0;
}
