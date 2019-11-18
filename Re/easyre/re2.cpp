#include<iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

int xor1[5] = { 12,34,56,78,90 };


string base64_decryption(const string in)
{
	string value("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	string out0;
	string out1;
	string deciToBin_6bit(int);

	for (auto x = in.cbegin(); (*x) != '=' && x != in.cend(); ++x)    //排除等号
		out0 += deciToBin_6bit(value.find(*x));

	const int n = out0.length();

	for (int i = 0; i < n; i += 8)           //每8位提取一次
	{
		string subs = out0.substr(i, 8);    //提取8位
		int j = 0;
		int sum = 0;

		for (auto& x : subs)
		{
			sum += (x - '0') * pow(2, 7 - j);
			++j;
		}                                   //按权展开，得到ASCII码

		out1 += static_cast<char>(sum);     //转换为相应字符
	}

	return out1;

}
string deciToBin_6bit(int deci)                                //10进制转2进制函数
{

	string hexStr(6, ' ');
	int Value = 0;
	int i = 0;

	for (; deci != 0; ++i, deci /= 2)
	{
		Value = deci % 2;
		hexStr.at(i) = Value + '0';
	}

	hexStr = hexStr.substr(0, i);

	reverse(hexStr.begin(), hexStr.end());

	if (hexStr.length() < 6)
	{
		string add(6 - hexStr.length(), '0');
		hexStr = add + hexStr;
	}

	return hexStr;
}
int func1(string a)
{
	__asm
	{
		jz lab1;
		jnz lab1;
		_emit 0xe8;
	}
lab2:
	int list1[12] = { 0,2,4,8,9,11,13,14,15,16,17,19 };
	for (int i = 0; i < 21; i++)
	{
		a[i] ^= xor1[i % 5];
	}
	for (int i = 0; i < 12; i++)
	{
		a[list1[i]] += 50;
	}
	__asm
	{
		ja lab3;
		jna lab3;
		_emit 0x01;
		_emit 0x02;
		_emit 0x03;
	}
lab1:
	xor1[0] ^= 71;
	xor1[1] ^= 67;
	xor1[2] ^= 72;
	xor1[3] ^= 126;
	xor1[4] ^= 49;
	__asm
	{
		jg lab2;
		jng lab2;
		_emit 0xe8;
	}
lab3:
	string base = base64_decryption("TlE5b0R7NC81N3tILzNQVzxFX0F4");
	for (int i = 0; i < 21; i++)
	{
		if (a[i] != base[i])
			return 0;
	}
	return 1;
}



int main()
{
	string s1 = "Kap0k{";
	cout << "input your flag:";
	string s;
	cin >> s;
	string a="123456781234567812345";
	for (int i = 0; i < 21; i++)
		a[i] = s[i + 6];
	if (s1.compare(0, 5, s, 0, 5) != 0)
	{
		goto End;
	}
	if (s[s.size() - 1] != '}')
	{
		goto End;
	}
	if (s.length() > 28 &&s.length()<28)
	{
		cout << "I don't likt this length!" << endl;
		goto End;
	}
	if (!func1(a))
		goto End;
	printf("wow!You found me!");
	return 0;
End:
	printf("Error\n");
	return 0;
}