#include "pch.h"
#include "FileManager.h"
#include <direct.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <deque>

FileManager::FileManager(std::vector <std::string> args) : options_(args)
{



	std::cout << "Check dir" << std::endl;
	if (!std::filesystem::exists(options_[0u]))
	{
		std::cout << "Wrong dir" << std::endl;
	}
	//std::filesystem::path dir(options_[0u]);
	for (auto& p : std::filesystem::directory_iterator(options_[0u]))
		file_queue_.emplace_back(p.path());

	file_count_ = file_queue_.size();
}

size_t FileManager::get_size()
{
	return file_count_;
}

void FileManager::do_parallel(std::filesystem::path file)
{
std::cout << file << std::endl;
}


std::deque <std::filesystem::path> FileManager::get_files_()
{
	return file_queue_;
}

FileManager::~FileManager()
{
}
