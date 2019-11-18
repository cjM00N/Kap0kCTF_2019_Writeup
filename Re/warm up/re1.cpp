#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

int number[50] = { 1,1,0,1,2,1,1,1,2,3,2,3,1,2,1,2 };
int num_or_char[50] = { 0,1,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,0,0,1,1,1 };
int count_0[10] = { 243,208,248,250,212,201,258,169 };
int count_1[10] = { 21317,15754,22206,22218,16456,14909,23540,9523 };
int count_2[10] = { 293,200,251,212,255,211,212,155};
int count_3[10] = { 97,96,59,106,63,107,106,53 };
int main()
{
	string s1 = "kap0k{";
	cout << "input your flag:";
	string s;
	cin >> s;
	if (s1.compare(0, 5, s, 0, 5) != 0)
	{
		goto End;
	}
	if (s[s.size()-1]!='}')
	{
		goto End;
	}
	if (s.length() > 32)
	{
		cout << "too long"<<endl;
		goto End;
	}
	if (s.length() < 31)
	{
		cout << "too short" << endl;
		goto End;
	}
	for (int i = 0; i < 10; i++)
	{
		int count = 0;
		char ch = '40';
		ch += i;
		for (int j = 6; j < s.length(); j++)
		{
			if (s[j] == ch)
				count++;
		}
		if (count != number[i])
			goto End;
	}
	for (int i = 0; i < 6; i++)
	{
		int count = 0;
		char ch = 'a';
		ch += i;
		for (int j = 6; j < s.length(); j++)
		{
			if (s[j] == ch)
				count++;
		}
		if (count != number[i+10])
			goto End;
	}
	for (int i = 0; i < 24; i++)
	{
		int num_char=-1;
		if (s[i+6] >= 'a' && s[i+6] <= 'f')
		{
			num_char = 0;
			if (num_char != num_or_char[i])
				goto End;
		}
		if (s[i + 6] >= '0' && s[i + 6] <= '9')
		{
			num_char = 1;
			if (num_char != num_or_char[i])
				goto End;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		int addcount = 0;
		int mulcount = 0;
		for (int j = 0; j < 3; j++)
		{
			addcount += s[i * 3 + j+6];
			mulcount += s[i * 3 + j+6] * s[i * 3 + j+6];
		}
		if (addcount != count_0[i])
			goto End;
		if (mulcount != count_1[i])
			goto End;
	}
	for (int i = 0; i < 8; i++)
	{
		int addcount = s[i + 6];
		int xorcount = s[i + 6];
		for (int j = 1; j < 3; j++)
		{
			addcount += s[i + j * 8 + 6];
			xorcount ^= s[i + j * 8 + 6];
		}
		if (addcount != count_2[i])
			goto End;
		if (xorcount != count_3[i])
			goto End;
	}
	cout << "Correct!";
	return 0;
End:
	cout << "Try again!";
	return 0;
}