// Test_Tenzor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h" 
#include <iostream>
#include "File_cpp.h" 
#include <fstream> 
#include <filesystem>
#include <regex>
#include <string>


namespace fs = std::filesystem;

std::pair<fs::path, std::vector<fs::path>> Input(int &argc, char *argv[])//функция для ввода команды, и заполнение путей в паре
{
	if (argc < 2)
	{
		throw std::runtime_error("Please, enter parameters\n");
	}

	std::vector<fs::path> input_vec_path;
	fs::path input_path = fs::canonical(argv[1]);//получает полный путь относительно программы, вырабатывает исключение при не нахождении файла

	for (int i = 2; i < argc; i++)
	{
		if (std::string(argv[i]) == "-I")
		{
			i++;
			input_vec_path.push_back(fs::canonical(argv[i]));
		}
	}
	return { input_path , input_vec_path };
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "ru");
	std::pair <fs::path, std::vector<fs::path>> path_dir;

	try
	{
		path_dir = Input(argc, argv);
	}
	catch (fs::filesystem_error f)
	{
		std::cout << f.what();
		return 404;
	}
	catch (std::exception &s)
	{
		std::cout << s.what();
		return 2;
	}

	try
	{
		File_cpp file1("Tree_file", path_dir.first); //создает дерево файлов, второй параметр указывает путь начала поисков
		file1.set_path_h(path_dir.second);//вводит данные с параметров коммандной строки
		file1.build_tree(path_dir.first);//Начало построенние дерева файлов
		file1.out_vec(); // вывод данных в консоль
		file1.sort_sum_h(); //вывод отсортированного списка частот включения файлов
	}
	catch (fs::filesystem_error f)
	{
		std::cout << f.what();
		return 1;
	}
	catch (...)
	{
		std::cout << "Error";
	}

	return 0;
}

