#include "pch.h"
#include "File_cpp.h"

std::vector<std::filesystem::path> File_cpp::path_h = {};
std::map<std::string, int> File_cpp::sum_h;

const std::regex File_cpp::rx_th("^#include\\s{0,}(\")([A-Z]|[a-z]|[0-9]|_){1,}\\.h(\")");//поиск заголовочных фалов типа #include "file.h"
const std::regex File_cpp::rx_arg("^#include\\s{0,}(<)([A-Z]|[a-z]|[0-9]|_){1,}\\.h(>)");//поиск заголовочных фалов типа #include <file.h>
const std::regex File_cpp::rx_comment("\\/\\*(?!.*\\*\\/)");//поиск начала коментирования
const std::regex File_cpp::rx_comment2("\\*\\/(?!.*\\/\\*)");// поиск окончания коментирования

File_cpp::File_cpp(std::filesystem::path _filename, std::filesystem::path _path, std::set<std::filesystem::path> set_recurs, std::string lvl) : _filename(_filename), _path(_path), set_recurs(set_recurs)
{
	this->lvl = lvl + "..";
}

namespace fs = std::filesystem;

void File_cpp::build_tree(const fs::path &path_h, const fs::path &file_h)
{
	for (const auto &entry : fs::recursive_directory_iterator(_path))//рекурсивный поиск
	{
		if (entry.path().filename().extension() == ".cpp")// проверка формата файла
		{
			vec_h.push_back(File_cpp(entry.path().filename(), entry.path().parent_path(), set_recurs, this->lvl));//добавление ветви, когда нашел в директории файл .срр
			set_sum_h(entry.path().filename(), 0);//заполнение списка, 0 потому что cpp найден не в файле
			find_in_file(entry.path());//поиска в файле 
		}
	}
}

void File_cpp::find_in_file(const fs::path &file_h)
{
	std::ifstream is(file_h);
	std::string str;

	bool comment = false;//флаг открытости коментариев
	for (size_t line = 1; getline(is, str); line++)//проходим файл построчно
	{
		//здесь реализуется поиск внутри файла
		str.erase(remove(str.begin(), str.end(), '\0'), str.end()); // Костыль, Почему-то при чтении из файла добавляет перед символами \0
		fs::path new_name;

		if (!comment)
		{
			comment = regex_search(begin(str), end(str), rx_comment);//поиск, открываются ли комментарии

			if (regex_search(begin(str), end(str), rx_th))//поиск файла типа #include "xxx.h"
			{
				new_name = str.substr(str.find('\"') + 1, str.find('\"', str.find('\"') + 1) - str.find('\"') - 1);//ввод имени файла
				vec_h.back().bild_tree_h(new_name, vec_h.back().get_path());//добавлении ветви дерева, два аргумента потому-что сначало нужно найти в той же директории

			}

			if (regex_search(begin(str), end(str), rx_arg))//поиск файла типа #include <xxx.h>
			{
				new_name = str.substr(str.find('<') + 1, str.find('>') - str.find('<') - 1);//ввод имени файла
				vec_h.back().bild_tree_h(new_name);//добавлении ветви дерева

			}
		}
		else
		{
			if (regex_search(begin(str), end(str), rx_comment2))//поиск закрытия коментариев
			{
				comment = false;
			}
		}

	}
}

bool File_cpp::bild_tree_h(const std::filesystem::path &file_h, const std::filesystem::path &path_h)
{
	if (set_recurs.insert(file_h).second)//проверка рекурсии
	{
		if (path_h != "") //если указан путь то ищет сначало в нем
		{
			if (fs::exists(path_h / file_h))//существует ли файл в указаной директории
			{
				set_sum_h(file_h, 1);//заполнение списка файлов
				vec_h.push_back(File_cpp(file_h, path_h, set_recurs, this->lvl));//добавление ветви
				find_in_file(path_h / file_h);
				return true;
			}
		}

		for (auto &path_arg : this->path_h)//поиск во всех путях указаной в параметрах
		{
			if (fs::exists(path_arg / file_h))//существует ли файл в указаной директории
			{
				set_sum_h(file_h, 1);//заполнение списка файлов
				vec_h.push_back(File_cpp(file_h, path_arg, set_recurs, this->lvl));//добавление ветви
				find_in_file(path_arg / file_h);
				return true;
			}
		}
		//если не нашел файл
		set_sum_h(file_h, 1);//заполнение списка файлов
		vec_h.push_back(File_cpp(file_h.string() + " (!)", "", set_recurs, this->lvl));//добавляет файл с пометкой
	}
	return false;
}

void File_cpp::set_sum_h(std::filesystem::path name, int i)
{
	std::pair<std::map<std::string, int>::iterator, bool> res = sum_h.insert({ name.string(), i });//пара итератора на файл и флага добавления
	if (res.second == false) //Если добавить не получилось
	{
		(res.first)->second++; //увеличить
	}
}

void File_cpp::sort_sum_h()//вывод отсортированного списка
{
	std::multimap<int, std::string> _sort_sum_h;
	for (std::pair<std::string, int> pair : this->sum_h) //заполнение карты, уже отсортированного по алфовиту
	{
		_sort_sum_h.insert({ pair.second , pair.first });
	}
	std::cout << std::endl;
	for (std::multimap<int, std::string>::iterator i = _sort_sum_h.end(); i != _sort_sum_h.begin(); )//вывод с конца, т.к. сортирует по увеличению
	{
		i--;
		std::cout << i->second << " " << i->first << std::endl;
	}
}



void File_cpp::set_path_h(const std::vector<std::filesystem::path> &vec)
{
	path_h = vec;
}

fs::path File_cpp::get_path() const
{
	return _path;
}

void File_cpp::out_vec()//вывод дерева файлов
{
	std::cout << this->lvl.substr(2) << this->_filename.string() << std::endl;
	for (auto & fil : vec_h)
	{
		fil.out_vec();
	}
}

File_cpp::~File_cpp()
{
}
