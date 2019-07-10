#include <iostream>
#include "MyString.h"
#include <fstream>
#include <map>
#include <io.h> 
#include <array>
#include <functional>
using namespace std;
void main()
{
	fstream conf_file("conf.ini", ios::in);
	MyString conf_str;
	MyString lang = "lang_suffix";
	MyString dir = "scand_dir";
	if (!conf_file.good())
	{
		fstream create_new("conf.ini", ios::app);
		create_new << lang + "=\n" + dir + "=";
		return;
	}
	else
	{
		while (conf_file.good())
		{
			array<char, 300> rbq = { 0 };
			conf_file.read(&rbq.at(0), rbq.size() - 1);
			conf_str += MyString(&rbq.at(0)) + "\n";
		}
	}
	map<MyString, MyString> conf;
	for (auto x : conf_str.Split("\n"))
	{
		auto item = x.Split("=", 1);
		auto propetry = item[0].Trim();
		auto value = item[1].Trim();
		conf[propetry] = value;
	}
	vector<MyString> target_lang;
	for (auto x : conf[lang].Split("|"))
	{
		target_lang.push_back(x.Trim());
	}
	vector<MyString> target_dir;
	for (auto x : conf[dir].Split("|"))
	{
		target_dir.push_back(x.Trim());
	}
	auto check_lang = [&](MyString file_name)
	{
		for (auto i : target_lang)
		{
			i = "." + i;
			if (file_name.find(i) == file_name.length() - i.length() &&
				file_name.length() >= i.length())
			{
				return true;
			}
		}
		return false;
	};

	function<void(MyString)> find = [&](MyString filename)
	{
		_finddata_t file_info;
		auto handle = _findfirst(filename.c_str(), &file_info);
		while (handle != -1)
		{
			if (file_info.name[0] != '.')
			{
				if (file_info.attrib == 16)
				{
					MyString  sub_dir = filename;
					sub_dir.Replace("*", MyString(file_info.name) + "\\*");
					find(sub_dir);
				}
				else if (check_lang(file_info.name))
				{
					cout << file_info.name << endl;
				}
			}
			int status = _findnext(handle, &file_info);
			if (status == -1)
			{
				break;
			}
		}
		_findclose(handle);
	};
	for (auto x : target_dir)
	{
		x += "\\*";
		find(x.c_str());
	}
	system("pause");
}