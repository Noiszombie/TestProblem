#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <mutex>
#include <deque>
class FileManager
{
public:
	explicit FileManager(const std::string & dir_path);

	FileManager(const FileManager &) = delete;
	FileManager & operator=(const FileManager &) = delete;
	FileManager(FileManager &&) = delete;
	FileManager & operator=(FileManager &&) = delete;


	size_t get_size() const noexcept;
	const std::vector<std::filesystem::path> & get_files() const noexcept;
private:
	std::vector <std::string> options_;
	std::vector <std::filesystem::path> files_;
};

