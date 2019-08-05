#pragma once

#include <string>
#include <vector>
#include <fstream> 
#include <filesystem>
#include <regex>
#include <map>
#include <iterator>
#include <utility>
#include <iostream>
#include <set>


class File_cpp
{
public:
	File_cpp(std::filesystem::path _filename, std::filesystem::path _path, std::set<std::filesystem::path> set_recurs = {}, std::string lvl = "");
	~File_cpp();

	static void set_path_h(const std::vector<std::filesystem::path> &vec);//заполнение вектора путей введенной в коммандной строке
	void sort_sum_h();//Сортировка списка включения файлов

	void build_tree(const std::filesystem::path &file_h, const std::filesystem::path &path_h = ".cpp");//Построение дерева

	std::filesystem::path get_path() const;//получение пути файла, нужно чтобы знать где искать заголовочные файлы
	void out_vec();//вывод дерева на консоль

private:
	//std::string name;
	std::filesystem::path _filename;//Имя файла
	std::filesystem::path _path;//путь файла
	std::vector<File_cpp> vec_h;//ветви от этого файла
	std::string lvl = "";//уровень дерева

	std::set<std::filesystem::path> set_recurs;//множества для поиска рекурсии

	//ниже описаны регулярные выражения 
	const static std::regex rx_th;//поиск заголовочных фалов типа #include "file.h"
	const static std::regex rx_arg;//поиск заголовочных фалов типа #include <file.h>
	const static std::regex rx_comment;//поиск начала коментирования
	const static std::regex rx_comment2; // поиск окончания коментирования

	static std::vector<std::filesystem::path> path_h; //пути введеные как параметра коммандной строки

	bool bild_tree_h(const std::filesystem::path &file_h, const std::filesystem::path &path_h = "");//Здесь создается ветви дерева
	void find_in_file(const std::filesystem::path &file_h);//Здесь осуществляется поиск заголовочных файлов внутри файла

	static std::map<std::string, int> sum_h; //здесь будет собиратся не отсортированный список включения файлов
	void set_sum_h(std::filesystem::path name, int i);//заполнение списка

};

