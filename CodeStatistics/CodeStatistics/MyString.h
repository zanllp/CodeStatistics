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

	//ʹ���ַ����ָ�
	//flag �ָ��־,���ص��ַ����������޳�,flag��Ҫ��char�������ز���ȷ
	//num �ָ������Ĭ��0���ָ��������num=1,���ؿ�ͷ��flag,flag������size=2���ַ�������
	//skipEmpty �������ַ���������ѹ��length==0���ַ���
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
			{	//��������ֱ�ӽص�����
				PushData(substr(Pos[num - 1] + flag.size()));
				break;
			}
			if (i == 0 && Pos[0] != 0)
			{	//��һ������λ�ò���0�Ļ�����
				PushData(substr(0, Pos[0]));
			}
			if (i != Pos.size() - 1)
			{
				int Left = Pos[i] + flag.length();
				int Right = Pos[i + 1] - Left;
				PushData(substr(Left, Right));
			}
			else
			{	//���һ����־������
				PushData(substr(*(--Pos.end()) + flag.size()));
			}
		}
		return dataSet;
	}

	//���ǰ����ַ�
	//target ��Ҫ������ַ�Ĭ�Ͽո�
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

	//����������������λ��
	//flag ��λ��־
	//num ����������Ĭ��ֱ������
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


	//�ַ����滻
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
