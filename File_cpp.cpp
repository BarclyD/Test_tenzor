#include "pch.h"
#include "File_cpp.h"

std::vector<std::filesystem::path> File_cpp::path_h = {};
std::map<std::string, int> File_cpp::sum_h;

const std::regex File_cpp::rx_th("^#include\\s{0,}(\")([A-Z]|[a-z]|[0-9]|_){1,}\\.h(\")");//����� ������������ ����� ���� #include "file.h"
const std::regex File_cpp::rx_arg("^#include\\s{0,}(<)([A-Z]|[a-z]|[0-9]|_){1,}\\.h(>)");//����� ������������ ����� ���� #include <file.h>
const std::regex File_cpp::rx_comment("\\/\\*(?!.*\\*\\/)");//����� ������ ��������������
const std::regex File_cpp::rx_comment2("\\*\\/(?!.*\\/\\*)");// ����� ��������� ��������������

File_cpp::File_cpp(std::filesystem::path _filename, std::filesystem::path _path, std::set<std::filesystem::path> set_recurs, std::string lvl) : _filename(_filename), _path(_path), set_recurs(set_recurs)
{
	this->lvl = lvl + "..";
}

namespace fs = std::filesystem;

void File_cpp::build_tree(const fs::path &path_h, const fs::path &file_h)
{
	for (const auto &entry : fs::recursive_directory_iterator(_path))//����������� �����
	{
		if (entry.path().filename().extension() == ".cpp")// �������� ������� �����
		{
			vec_h.push_back(File_cpp(entry.path().filename(), entry.path().parent_path(), set_recurs, this->lvl));//���������� �����, ����� ����� � ���������� ���� .���
			set_sum_h(entry.path().filename(), 0);//���������� ������, 0 ������ ��� cpp ������ �� � �����
			find_in_file(entry.path());//������ � ����� 
		}
	}
}

void File_cpp::find_in_file(const fs::path &file_h)
{
	std::ifstream is(file_h);
	std::string str;

	bool comment = false;//���� ���������� �����������
	for (size_t line = 1; getline(is, str); line++)//�������� ���� ���������
	{
		//����� ����������� ����� ������ �����
		str.erase(remove(str.begin(), str.end(), '\0'), str.end()); // �������, ������-�� ��� ������ �� ����� ��������� ����� ��������� \0
		fs::path new_name;

		if (!comment)
		{
			comment = regex_search(begin(str), end(str), rx_comment);//�����, ����������� �� �����������

			if (regex_search(begin(str), end(str), rx_th))//����� ����� ���� #include "xxx.h"
			{
				new_name = str.substr(str.find('\"') + 1, str.find('\"', str.find('\"') + 1) - str.find('\"') - 1);//���� ����� �����
				vec_h.back().bild_tree_h(new_name, vec_h.back().get_path());//���������� ����� ������, ��� ��������� ������-��� ������� ����� ����� � ��� �� ����������

			}

			if (regex_search(begin(str), end(str), rx_arg))//����� ����� ���� #include <xxx.h>
			{
				new_name = str.substr(str.find('<') + 1, str.find('>') - str.find('<') - 1);//���� ����� �����
				vec_h.back().bild_tree_h(new_name);//���������� ����� ������

			}
		}
		else
		{
			if (regex_search(begin(str), end(str), rx_comment2))//����� �������� �����������
			{
				comment = false;
			}
		}

	}
}

bool File_cpp::bild_tree_h(const std::filesystem::path &file_h, const std::filesystem::path &path_h)
{
	if (set_recurs.insert(file_h).second)//�������� ��������
	{
		if (path_h != "") //���� ������ ���� �� ���� ������� � ���
		{
			if (fs::exists(path_h / file_h))//���������� �� ���� � �������� ����������
			{
				set_sum_h(file_h, 1);//���������� ������ ������
				vec_h.push_back(File_cpp(file_h, path_h, set_recurs, this->lvl));//���������� �����
				find_in_file(path_h / file_h);
				return true;
			}
		}

		for (auto &path_arg : this->path_h)//����� �� ���� ����� �������� � ����������
		{
			if (fs::exists(path_arg / file_h))//���������� �� ���� � �������� ����������
			{
				set_sum_h(file_h, 1);//���������� ������ ������
				vec_h.push_back(File_cpp(file_h, path_arg, set_recurs, this->lvl));//���������� �����
				find_in_file(path_arg / file_h);
				return true;
			}
		}
		//���� �� ����� ����
		set_sum_h(file_h, 1);//���������� ������ ������
		vec_h.push_back(File_cpp(file_h.string() + " (!)", "", set_recurs, this->lvl));//��������� ���� � ��������
	}
	return false;
}

void File_cpp::set_sum_h(std::filesystem::path name, int i)
{
	std::pair<std::map<std::string, int>::iterator, bool> res = sum_h.insert({ name.string(), i });//���� ��������� �� ���� � ����� ����������
	if (res.second == false) //���� �������� �� ����������
	{
		(res.first)->second++; //���������
	}
}

void File_cpp::sort_sum_h()//����� ���������������� ������
{
	std::multimap<int, std::string> _sort_sum_h;
	for (std::pair<std::string, int> pair : this->sum_h) //���������� �����, ��� ���������������� �� ��������
	{
		_sort_sum_h.insert({ pair.second , pair.first });
	}
	std::cout << std::endl;
	for (std::multimap<int, std::string>::iterator i = _sort_sum_h.end(); i != _sort_sum_h.begin(); )//����� � �����, �.�. ��������� �� ����������
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

void File_cpp::out_vec()//����� ������ ������
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
