#include <iostream>
#include <fstream>
#include <map>
#include <io.h> 
#include <array>
#include <functional>
#include "MyString.h"

/*
	// conf.ini
	lang_suffix = js | jsx | ts | tsx | html | css | cpp | h
	scand_dir = $dir_0 | $dir_1
	ignore_dir = $dir_2 | $dir_3
	show_detail = true
	dir_0 = C:\Users\w1598\Desktop\ioflow.link
	dir_1 = C:\Users\w1598\source\repos
	dir_2 = C:\Users\w1598\Desktop\ioflow.link\前端\Ioflow.link-console\node_modules
	dir_3 = C:\Users\w1598\Desktop\ioflow.link\后端\Ioflow.link-backend\node_modules
*/

using namespace std;
vector<MyString> target_lang;
vector<MyString> target_dir;
vector<MyString> ignore_dir;
map<MyString, MyString> conf;

MyString conf_str;
const MyString lang = "lang_suffix";
const MyString dir = "scand_dir";
const MyString ignore_ = "ignore_dir";
const MyString detail = "show_detail";

MyString lang_now;
map<MyString, long> lang_num;
int all_num = 0;

bool ShowDetail()
{
	return conf[detail] == "true";
}

bool CheckIgnore(MyString file_name)
{
	file_name.Replace("\\*", "");
	for (auto& i : ignore_dir)
	{
		if (file_name==i)
		{
			return false;
		}
	}
	return true;
}

bool CheckLang(MyString file_name)
{
	auto name_split = file_name.Split(".", 1);
	if (name_split.size() != 2)
	{
		return false;
	}
	MyString suffix = name_split[1];
	for (auto& i : target_lang)
	{
		if (suffix == i)
		{
			lang_now = i;
			return true;
		}
	}
	return false;
};

void CalcLineNum(MyString file_name)
{
	fstream code_file(file_name, ios::in);
	MyString code;
	while (code_file.good())
	{
		array<char, 3000> rbq = { 0 };
		code_file.read(&rbq.at(0), rbq.size() - 1);
		code += MyString(&rbq.at(0)) + "\n";
	}
	auto num = code.FindAll("\n").size();
	lang_num[lang_now] += num;
	all_num += num;
	if (ShowDetail())
	{
		cout << file_name << " " << num << endl;
	}
}

void InitConf()
{
	for (auto x : conf_str.Split("\n"))
	{
		auto item = x.Split("=", 1);
		auto propetry = item[0].Trim();
		auto value = item.size() == 1 ? "" : item[1].Trim();
		conf[propetry] = value;
	}
	for (auto x : conf[lang].Split("|"))
	{
		target_lang.push_back(x.Trim());
	}
	for (auto x : conf[dir].Split("|"))
	{
		x = x.Trim();
		if (x[0] == '$')
		{
			target_dir.push_back(conf[x.substr(1)]);
		}
		else
		{
			target_dir.push_back(x);
		}
	}
	for (auto x : conf[ignore_].Split("|"))
	{
		x = x.Trim();
		if (x[0] == '$')
		{
			ignore_dir.push_back(conf[x.substr(1)]);
		}
		else
		{
			ignore_dir.push_back(x);
		}
	}
}

int ReadConfFile()
{
	fstream conf_file("conf.ini", ios::in);
	if (!conf_file.good())
	{
		fstream create_new("conf.ini", ios::app);
		create_new
			<< lang << " = " << endl
			<< dir << " = " << endl
			<< ignore_ << " = " << endl
			<< detail << " = true";
		cout << "找不到配置文件，已新建一个，先进行配置" << endl;
		return -1;
	}
	while (conf_file.good())
	{
		array<char, 300> rbq = { 0 };
		conf_file.read(&rbq.at(0), rbq.size() - 1);
		conf_str += MyString(&rbq.at(0)) ;
	}
}

void CoutLine()
{
	cout << "-----------------------------------------------------------------------------------------------" << endl;
}

int main()
{
	if (ReadConfFile() == -1)
	{
		system("pause");;
		return 0;
	}
	InitConf();
	function<void(MyString)> Find = [&](MyString filename)
	{
		_finddata_t file_info;
		auto handle = _findfirst(filename.c_str(), &file_info);
		while (handle != -1)
		{
			if (file_info.name[0] == '.')
			{
				if (_findnext(handle, &file_info) == -1)
				{
					break;
				}
				continue;
			}
			if (file_info.attrib == 16)
			{
				MyString sub_dir = filename;
				sub_dir.Replace("*", MyString(file_info.name) + "\\*");
				if (CheckIgnore(sub_dir))
				{
					Find(sub_dir);
				}
			}
			else if (CheckLang(file_info.name))
			{
				MyString code_file = filename;
				code_file.Replace("*", file_info.name);
				CalcLineNum(code_file);
			}
			if( _findnext(handle, &file_info) == -1)
			{
				break;
			}
		}
		_findclose(handle);
	};
	for (auto x : target_dir)
	{
		if (ShowDetail())
		{
			CoutLine();
			cout << x << endl;
			CoutLine();
		}
		x += "\\*";
		Find(x.c_str());
	}
	CoutLine();
	for (auto& i : lang_num)
	{
		cout << i.first << "  " << i.second << endl;
	}
	cout <<"all    "<< all_num << endl;
	system("pause");
}