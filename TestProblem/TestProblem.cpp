
#include "pch.h"
#include "ArgumentParser.h"
#include "ThreadPool.h"
#include "FileManager.h"
#include <iostream>
#include <mutex>
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
	//result.push_back('\0');
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

template<class T>
auto reverse(std::vector <T> &vect)
{
	std::vector <T> reversed_vect_;
	for (auto i = 0u; i < vect.size(); i++)
		reversed_vect_.emplace_back(vect[vect.size() - i - 1u]);
	return reversed_vect_;
};


//template<typename T>
auto sort_task = [](
	std::filesystem::path file,
	std::filesystem::path dir,
	const std::string out_prefix,
	const ContentType content_type,
	const std::string sort_mode)
{
	//std::cout << std::this_thread::get_id() << std::endl;


	std::vector<std::string> data;
	{
		std::ifstream fin(file);
		for (std::string line; std::getline(fin, line); ) {
			//if(content_type == ContentType::Integer)
				//data.push_back(std::stoi(line));
			if(content_type == ContentType::String)
				data.push_back(line);
		}
		fin.close();
	}



	//(content_type == ContentType::Integer) ? sort(data, int_sort) : sort(data, str_sort);
	sort(data, str_sort);

	if (sort_mode == "d")
		data = reverse(data);
	{
		auto name = str_sub(file.string(), dir.string() + "\\");
		std::cout << dir.string() + "\\" + out_prefix + name << std::endl;
		std::ofstream fout(dir.string() + "\\" + out_prefix + name);
		for (const auto & element : data)
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
	//std::vector <T>
	ThreadPool thread_pool{ 10u };
	for (auto & file : files)
	{
		thread_pool.add_task(sort_task, file, dir, out_prefix, content_type, sort_mode);
	}

	//thread_pool.stop();
	std::cout << "End game" << std::endl;
}



	catch (const std::exception & exception)
{
	std::cerr << exception.what();
}
