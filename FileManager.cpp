#include "pch.h"
#include "FileManager.h"
#include <direct.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <deque>

FileManager::FileManager(const std::string & dir_path)
{
	const auto path = std::filesystem::path{ dir_path };
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("path doesnt exist");
	}
	for (const auto & file : std::filesystem::directory_iterator(path))
	{
		//! if dir than ignore + print warning
		files_.emplace_back(file.path());
	}
}

size_t FileManager::get_size() const noexcept
{
	return files_.size();
}

const std::vector<std::filesystem::path> & FileManager::get_files() const noexcept
{
	return files_;
}

