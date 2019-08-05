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

	static void set_path_h(const std::vector<std::filesystem::path> &vec);//���������� ������� ����� ��������� � ���������� ������
	void sort_sum_h();//���������� ������ ��������� ������

	void build_tree(const std::filesystem::path &file_h, const std::filesystem::path &path_h = ".cpp");//���������� ������

	std::filesystem::path get_path() const;//��������� ���� �����, ����� ����� ����� ��� ������ ������������ �����
	void out_vec();//����� ������ �� �������

private:
	//std::string name;
	std::filesystem::path _filename;//��� �����
	std::filesystem::path _path;//���� �����
	std::vector<File_cpp> vec_h;//����� �� ����� �����
	std::string lvl = "";//������� ������

	std::set<std::filesystem::path> set_recurs;//��������� ��� ������ ��������

	//���� ������� ���������� ��������� 
	const static std::regex rx_th;//����� ������������ ����� ���� #include "file.h"
	const static std::regex rx_arg;//����� ������������ ����� ���� #include <file.h>
	const static std::regex rx_comment;//����� ������ ��������������
	const static std::regex rx_comment2; // ����� ��������� ��������������

	static std::vector<std::filesystem::path> path_h; //���� �������� ��� ��������� ���������� ������

	bool bild_tree_h(const std::filesystem::path &file_h, const std::filesystem::path &path_h = "");//����� ��������� ����� ������
	void find_in_file(const std::filesystem::path &file_h);//����� �������������� ����� ������������ ������ ������ �����

	static std::map<std::string, int> sum_h; //����� ����� ��������� �� ��������������� ������ ��������� ������
	void set_sum_h(std::filesystem::path name, int i);//���������� ������

};

