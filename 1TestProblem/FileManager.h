#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <mutex>
#include <deque>
class FileManager
{
public:
	FileManager(std::vector <std::string>);
	~FileManager();
	size_t get_size();
	std::deque <std::filesystem::path> get_files_();
	//std::mutex get_mutex();
	void do_parallel(std::filesystem::path);
private:
	std::vector <std::string> options_;
	//std::vector <std::filesystem::path> files_;
	std::deque <std::filesystem::path> file_queue_;
	std::mutex queue_mutex_;
	size_t file_count_;
};

