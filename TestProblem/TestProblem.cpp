
#include "pch.h"
#include "ArgumentParser.h"
#include "ThreadPool.h"
#include "FileManager.h"
#include <iostream>
#include <mutex>
#include <functional>
#include <filesystem>
#include <fstream>

enum class ContentType : std::uint8_t
{
	Integer,
	String,
};

auto str_sub = [](std::string str1, std::string str2)
{
	std::string result = "";
	if ((str1.length() < str2.length()) && (str1.length() == 0u || str2.length() == 0u))
	{
		return result;
	}

	for (auto i = str2.length(); i < str1.length(); i++)
	{
		result.push_back(str1[i]);
	}
	return result;
};

auto str_sort = [](const std::string & l, const std::string & r) { return l.size() < r.size(); };
auto int_sort = [](const int & l, const int & r) { return l < r; };

template<typename T, typename U = std::greater<>>
auto sort(std::vector<T> & input_data, const U & comparator = U())
{
	T key;
	int j;
	for (auto i = 1u; i < input_data.size(); i++)
	{
		key = input_data[i];
		j = i - 1;
		while (j >= 0 && comparator(input_data[j], key))
		{
			input_data[j + 1] = input_data[j];
			--j;
		}
		input_data[j + 1] = key;
	}
	return input_data;
};

template<typename T>
auto reverse(std::vector <T> &vect)
{
	std::vector <T> reversed_vect_;
	for (auto i = 0u; i < vect.size(); i++)
		reversed_vect_.emplace_back(vect[vect.size() - i - 1u]);
	return reversed_vect_;
};

auto sort_task(
	std::filesystem::path file,
	std::filesystem::path dir,
	const std::string out_prefix,
	const ContentType content_type,
	const std::string sort_mode)
{
	std::vector <std::string> str_data;
	std::vector <int> int_data;

	// Reading the data from files in directory
	{
		size_t counter = 0;
		std::ifstream fin(file);
		for (std::string line; std::getline(fin, line); ) {
			str_data.push_back(line);
			counter++;
			if (counter > 100)
				throw std::runtime_error("Wrong! Number of elements in file is more than 100!!!");

		}
		if (content_type == ContentType::Integer)
			for (auto & elem : str_data)
				int_data.push_back(std::stoi(elem));
		fin.close();
	}
	// Sort of the data
	{
		if (content_type == ContentType::Integer)
			sort(int_data, int_sort);
		if (content_type == ContentType::String)
			sort(str_data, str_sort);

		if (sort_mode == "d" && content_type == ContentType::Integer)
			int_data = reverse(int_data);
		if (sort_mode == "d" && content_type == ContentType::String)
			str_data = reverse(str_data);
	}
	// Writing the data
	{
		auto name = str_sub(file.string(), dir.string() + "\\");
		//std::cout << dir.string() + "\\" + out_prefix + name << std::endl;
		std::ofstream fout(dir.string() + "\\" + out_prefix + name);

		if (content_type == ContentType::Integer)
			for (const auto & element : int_data)
			{
				fout << element << std::endl;
			}
		if (content_type == ContentType::String)
			for (const auto & element : str_data)
			{
				fout << element << std::endl;
			}
		fout.close();
	}	
};

int main(int argc, char ** argv) try
{
	ArgumentParser argument_parser{ argc, argv };
	const auto options = argument_parser.get_options();
	const auto dir_path = [&options]() -> std::string {
		const auto path = options.size() != 0u ? options[0u] : "";
		return std::filesystem::exists(path)
			? path
			: "";
	}();
	if (dir_path.empty())
	{
		throw std::runtime_error("Wrong dir");
	}

	FileManager file_manager(dir_path);
	const auto & files = file_manager.get_files();

	const auto number_of_files_ = files.size();
	const auto dir = options[0u];
	const auto out_prefix = options[1u];
	const auto parsed_string = options[2u];
	const auto content_type = "i" == parsed_string ? ContentType::Integer : ContentType::String;
	const auto sort_mode = options[3u];
	ThreadPool thread_pool{ 10u };
	auto sort = int_sort;
	for (auto & file : files)
	{
		thread_pool.add_task(sort_task, file, dir, out_prefix, content_type, sort_mode);
	}
	return 0;
}

	catch (const std::exception & exception)
{
	std::cerr << exception.what();
}
