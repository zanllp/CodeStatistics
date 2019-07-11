#pragma once
#pragma warning(disable:4267)
#include <string>
#include <vector>
class MyString :public std::string
{
public:
	MyString() = default;
	~MyString() = default;
	template<class T>
	MyString(T&& arg) :std::string(std::forward<T>(arg))
	{

	}

	//使用字符串分割
	//flag 分割标志,返回的字符串向量会剔除,flag不要用char，会重载不明确
	//num 分割次数，默认0即分割到结束，例num=1,返回开头到flag,flag到结束size=2的字符串向量
	//skipEmpty 跳过空字符串，即不压入length==0的字符串
	std::vector<MyString> Split(MyString flag, int num = 0, bool skipEmpty = true)
	{
		std::vector<MyString> dataSet;
		auto PushData = [&dataSet, skipEmpty](MyString line)
		{
			if (line.length() != 0 || !skipEmpty)
			{
				dataSet.push_back(line);
			}
		};
		auto Pos = FindAll(flag, num != 0 ? num : -1);
		if (!Pos.size())
		{
			dataSet.push_back(*this);
			return dataSet;
		}
		for (int i = 0; i < Pos.size(); i++)
		{
			if (dataSet.size() == num && num != 0)
			{	//满足数量直接截到结束
				PushData(substr(Pos[num - 1] + flag.size()));
				break;
			}
			if (i == 0 && Pos[0] != 0)
			{	//第一个数的位置不是0的话补上
				PushData(substr(0, Pos[0]));
			}
			if (i != Pos.size() - 1)
			{
				int Left = Pos[i] + flag.length();
				int Right = Pos[i + 1] - Left;
				PushData(substr(Left, Right));
			}
			else
			{	//最后一个标志到结束
				PushData(substr(*(--Pos.end()) + flag.size()));
			}
		}
		return dataSet;
	}

	//清除前后的字符
	//target 需要清除的字符默认空格
	MyString Trim(char target = ' ')
	{
		if (size()==1||size()==0)
		{
			return *this;
		}
		auto left = find_first_not_of(target);
		auto right = find_last_not_of(target);
		return substr(left, right - left + 1);
	}

	//返回搜索到的所有位置
	//flag 定位标志
	//num 搜索数量，默认直到结束
	std::vector<int> FindAll(MyString flag, int num = -1)
	{
		std::vector<int> Result;
		auto Pos = find(flag);
		while (Pos != -1 && Result.size() != num)
		{
			Result.push_back(Pos);
			Pos = find(flag, *(--Result.end()) + 1);
		}
		return Result;
	}


	//字符串替换
	MyString& Replace(MyString oldStr, MyString newStr)
	{
		int pos = find(oldStr);
		if (pos != -1)
		{
			replace(pos, oldStr.length(), newStr);
		}
		return *this;
	}

};
