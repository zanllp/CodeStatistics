#include <iostream>
#include <fstream>
#include <map>
#include <io.h> 
#include <array>
#include <functional>
#include "MyString.h";

// lang_suffix = js | jsx | ts | tsx | html
// scand_dir = C:\Users\w1598\Desktop\ioflow.link\Ç°¶Ë\Ioflow.link - console\src | C:\Users\w1598\Desktop\ioflow.link\ºó¶Ë\Ioflow.link - backend\app
// show_detail = true

using namespace std;
vector<MyString> target_lang;
vector<MyString> target_dir;
map<MyString, MyString> conf;

MyString conf_str;
MyString lang = "lang_suffix";
MyString dir = "scand_dir";
MyString detail = "show_detail";
int all_num = 0;
bool CheckLang(MyString file_name)
{

	for (auto i : target_lang)
	{
		i = "." + i;
		if (file_name.length() >= i.length() &&
			file_name.find(i) == file_name.length() - i.length())
		{
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
	all_num += num;
	if (conf[detail] == "true")
	{
		cout << file_name << " " << num << endl;
	}
}

void main()
{
	fstream conf_file("conf.ini", ios::in);
	if (!conf_file.good())
	{
		fstream create_new("conf.ini", ios::app);
		create_new << lang + "=\n" + dir + "=\n" + detail + "=";
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
		target_dir.push_back(x.Trim());
	}
	function<void(MyString)> Find = [&](MyString filename)
	{
		_finddata_t file_info;
		auto handle = _findfirst(filename.c_str(), &file_info);
		while (handle != -1)
		{
			if (file_info.name[0] != '.')
			{
				if (file_info.attrib == 16)
				{
					MyString sub_dir = filename;
					sub_dir.Replace("*", MyString(file_info.name) + "\\*");
					Find(sub_dir);
				}
				else if (CheckLang(file_info.name))
				{
					MyString code_file = filename;
					code_file.Replace("*", file_info.name);
					CalcLineNum(code_file);
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
		Find(x.c_str());
	}
	cout << all_num << endl;
	system("pause");
}