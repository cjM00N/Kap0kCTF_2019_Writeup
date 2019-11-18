#include <stdio.h>
#include <string.h>
/*aes_small.c*/
//辅助矩阵
/*s盒矩阵：The AES Substitution Table*/// 256 位的密匙256 位支持长度为32 个字符
static const unsigned char sbox[256] = {	//static:内部变量  const：只读，不可变常量
	0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,
	0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
	0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,
	0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
	0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,
	0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
	0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,
	0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
	0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,
	0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
	0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,
	0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
	0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,
	0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
	0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,
	0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
	0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,
	0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
	0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,
	0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
	0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,
	0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
	0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,
	0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
	0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,
	0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
	0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,
	0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
	0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,
	0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
	0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,
	0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16,
};
//逆向S 盒矩阵
static const unsigned char contrary_sbox[256] = {
	0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,
	0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
	0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,
	0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
	0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,
	0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,//0x4e
	0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,
	0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
	0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,
	0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
	0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,
	0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
	0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,
	0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
	0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,
	0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
	0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,
	0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
	0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,
	0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
	0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,
	0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
	0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,
	0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
	0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,
	0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
	0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,
	0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
	0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,
	0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
	0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,
	0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d,
};
/*轮常量表 The key schedule rcon table*/
static const unsigned char Rcon[10] = {
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36 };

//辅助函数
/*有限域*2乘法 The x2time() function */
static unsigned char x2time(unsigned char x)
{
	if (x & 0x80)
	{
		return (((x << 1) ^ 0x1B) & 0xFF);
	}
	return x << 1;
}
/*有限域*3乘法 The x2time() function */
static unsigned char x3time(unsigned char x)
{
	return (x2time(x) ^ x);
}
/*有限域*4乘法 The x4time() function */
static unsigned char x4time(unsigned char x)
{
	return (x2time(x2time(x)));
}
/*有限域*8乘法 The x8time() function */
static unsigned char x8time(unsigned char x)
{
	return (x2time(x2time(x2time(x))));
}
/*有限域9乘法 The x9time() function */
static unsigned char x9time(unsigned char x)	//9:1001
{
	return (x8time(x) ^ x);
}
/*有限域*B乘法 The xBtime() function */
static unsigned char xBtime(unsigned char x)	//B:1011
{
	return (x8time(x) ^ x2time(x) ^ x);
}
/*有限域*D乘法 The xDtime() function */
static unsigned char xDtime(unsigned char x)	//D:1101
{
	return (x8time(x) ^ x4time(x) ^ x);
}
/*有限域*E乘法 The xEtime() function */
static unsigned char xEtime(unsigned char x)	//E:1110
{
	return (x8time(x) ^ x4time(x) ^ x2time(x));
}

/****************************************************************************************************************/
/*第三类操作：列混合操作 MixColumns: Process the entire block*/
static void MixColumns(unsigned char* col)//列混合
{
	unsigned char tmp[4], xt[4];
	int i;
	for (i = 0; i < 4; i++, col += 4)  //col代表一列的基地址，col+4:下一列的基地址
	{
		/*
		xt[0]=x2time(col[0]);
		xt[1]=x2time(col[1]);
		xt[2]=x2time(col[2]);
		xt[3]=x2time(col[3]);
		//xt[n]代表*2   xt[n]^col[n]代表*3   col[n]代表*1
		tmp[0]=(xt[0])^(xt[1]^col[1])^col[2]^col[3];	//2 3 1 1
		tmp[1]=col[0]^(xt[1])^(xt[2]^col[2])^col[3];	//1 2 3 1
		tmp[2]=col[0]^col[1]^(xt[2])^(xt[3]^col[3]);	//1 1 2 3
		tmp[3]=(xt[0]^col[0])^col[1]^col[2]^(xt[3]);	//3 1 1 2
		*/
		tmp[0] = x2time(col[0]) ^ x3time(col[1]) ^ col[2] ^ col[3];	//2 3 1 1
		tmp[1] = col[0] ^ x2time(col[1]) ^ x3time(col[2]) ^ col[3];	//1 2 3 1
		tmp[2] = col[0] ^ col[1] ^ x2time(col[2]) ^ x3time(col[3]);	//1 1 2 3
		tmp[3] = x3time(col[0]) ^ col[1] ^ col[2] ^ x2time(col[3]);	//3 1 1 2
		//修改后的值 直接在原矩阵上修改
		col[0] = tmp[0];
		col[1] = tmp[1];
		col[2] = tmp[2];
		col[3] = tmp[3];
	}
}
//逆向列混淆
static void Contrary_MixColumns(unsigned char* col)
{
	unsigned char tmp[4];
	unsigned char xt2[4];//colx2
	unsigned char xt4[4];//colx4
	unsigned char xt8[4];//colx8
	int x;
	for (x = 0; x < 4; x++, col += 4)
	{
		/*
		xt2[0]=x2time(col[0]);
		xt2[1]=x2time(col[1]);
		xt2[2]=x2time(col[2]);
		xt2[3]=x2time(col[3]);
		xt4[0]=x2time(xt2[0]);
		xt4[1]=x2time(xt2[1]);
		xt4[2]=x2time(xt2[2]);
		xt4[3]=x2time(xt2[3]);
		xt8[0]=x2time(xt4[0]);
		xt8[1]=x2time(xt4[1]);
		xt8[2]=x2time(xt4[2]);
		xt8[3]=x2time(xt4[3]);
		tmp[0]=xt8[0]^xt4[0]^xt2[0]^xt8[1]^xt2[1]^col[1]^xt8[2]^xt4[2]^col[2]^xt8[3]^col[3];
		tmp[1]=xt8[0]^col[0]^xt8[1]^xt4[1]^xt2[1]^xt8[2]^xt2[2]^col[2]^xt8[3]^xt4[3]^col[3];
		tmp[2]=xt8[0]^xt4[0]^col[0]^xt8[1]^col[1]^xt8[2]^xt4[2]^xt2[2]^xt8[3]^xt2[3]^col[3];
		tmp[3]=xt8[0]^xt2[0]^col[0]^xt8[1]^xt4[1]^col[1]^xt8[2]^col[2]^xt8[3]^xt4[3]^xt2[3];
		*/
		tmp[0] = xEtime(col[0]) ^ xBtime(col[1]) ^ xDtime(col[2]) ^ x9time(col[3]);
		tmp[1] = x9time(col[0]) ^ xEtime(col[1]) ^ xBtime(col[2]) ^ xDtime(col[3]);
		tmp[2] = xDtime(col[0]) ^ x9time(col[1]) ^ xEtime(col[2]) ^ xBtime(col[3]);
		tmp[3] = xBtime(col[0]) ^ xDtime(col[1]) ^ x9time(col[2]) ^ xEtime(col[3]);
		col[0] = tmp[0];
		col[1] = tmp[1];
		col[2] = tmp[2];
		col[3] = tmp[3];
	}
}
/*第二类操作：行移位：行左循环移位 ShiftRows:Shifts the entire block*/
static void ShiftRows(unsigned char* col)//正向行移位
{
	/*
		1 5  9 13				5  9 13 1
		2 6 10 14				10 14 2 6
		3 7 11 15				15 3 7 11
		4 8 12 16				16 4 8 12
	*/
	unsigned char t;
	/*1nd row*///左移1位
	t = col[1]; col[1] = col[5]; col[5] = col[9]; col[9] = col[13]; col[13] = t;
	/*2rd row*///左移2位，交换2次数字来实现
	t = col[2]; col[2] = col[10]; col[10] = t;
	t = col[6]; col[6] = col[14]; col[14] = t;
	/*3th row*///左移3位，相当于右移1次
	t = col[15]; col[15] = col[11]; col[11] = col[7]; col[7] = col[3]; col[3] = t;
	/*4th row*/	//第4行不移位
}
//逆向行移位
static void Contrary_ShiftRows(unsigned char* col)
{
	unsigned char t;
	/*1nd row*/
	t = col[13]; col[13] = col[9]; col[9] = col[5]; col[5] = col[1]; col[1] = t;
	/*2rd row*/
	t = col[2]; col[2] = col[10]; col[10] = t;
	t = col[6]; col[6] = col[14]; col[14] = t;
	/*3th row*/
	t = col[3]; col[3] = col[7]; col[7] = col[11]; col[11] = col[15]; col[15] = t;
	/*4th row*/	//第4行不移位
}
/*第一类操作：s盒字节代换替换 SubBytes*/
static void SubBytes(unsigned char* col)//字节代换
{
	int x;
	for (x = 0; x < 16; x++)
	{
		col[x] = sbox[col[x]];
	}
}
//逆向字节代换
static void Contrary_SubBytes(unsigned char* col)
{
	int x;
	for (x = 0; x < 16; x++)
	{
		col[x] = contrary_sbox[col[x]];
	}
}
/*第四类操作：轮密钥加 AddRoundKey*/
static void AddRoundKey(unsigned char* col, unsigned char* expansionkey, int round)//密匙加
{
	//扩展密钥：44*32bit =11*4* 4*8 =  16字节*11轮，每轮用16字节密钥
	//第0轮，只进行一次轮密钥加
	//第1-10轮，轮密钥加
	int x;
	for (x = 0; x < 16; x++)	//每1轮操作：4*32bit密钥 = 16个字节密钥
	{
		col[x] ^= expansionkey[(round << 4) + x];
	}
}
/* AES加密总函数 10轮4类操作 Encrypt a single block with Nr Rounds(10,12,14)*/
void AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr)//加密一个区块
{
	//输入blk原文，直接在上面修改，输出blk密文
	//输入skey：
	//输入Nr = 10轮
	int round;
	//第1轮之前：轮密钥加
	AddRoundKey(blk, expansionkey, 0);
	//第1-9轮：4类操作：字节代换、行移位、列混合、轮密钥加
	for (round = 1; round <= (Nr - 1); round++)
	{
		SubBytes(blk);		//输入16字节数组，直接在原数组上修改
		ShiftRows(blk);		//输入16字节数组，直接在原数组上修改
		MixColumns(blk);	//输入16字节数组，直接在原数组上修改
		AddRoundKey(blk, expansionkey, round);
	}
	//第10轮：不进行列混合
	SubBytes(blk);
	ShiftRows(blk);
	AddRoundKey(blk, expansionkey, Nr);
}
//AES 解密总函数
void Contrary_AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr)
{
	int x;
	/* unsigned char *contrary_key=key;
	for(x=0;x<11;x++,key+=16)
	Contrary_MixColumns(key);*/
	AddRoundKey(blk, expansionkey, Nr);
	Contrary_ShiftRows(blk);
	Contrary_SubBytes(blk);
	for (x = (Nr - 1); x >= 1; x--)
	{
		AddRoundKey(blk, expansionkey, x);
		Contrary_MixColumns(blk);
		Contrary_ShiftRows(blk);
		Contrary_SubBytes(blk);
	}
	AddRoundKey(blk, expansionkey, 0);
}
/*//密钥编排，16字节--->44列32bit密钥生成--> 11组16字节:分别用于11轮 轮密钥加运算
Schedule a secret key for use.
*outkey[] must be 16*15 bytes in size
*Nk==number of 32 bit words in the key,e.g.,4,6,8
*Nr==number of rounds,e.g.,10,12,14
*/
void ScheduleKey(unsigned char* inkey, unsigned char* outkey, int Nk, int Nr)//安排一个保密密钥使用
{
	//inkey:初始16字节密钥key
	//outkey：11组*16字节扩展密钥expansionkey
	//Nk：4列
	//Nr：10轮round
	unsigned char temp[4], t;
	int x, i;
	/*copy the key*/
	//第0组：[0-3]直接拷贝
	for (i = 0; i < (4 * Nk); i++)
	{
		outkey[i] = inkey[i];
	}
	//第1-10组：[4-43]
	i = Nk;
	while (i < (4 * (Nr + 1))) //i=4~43 WORD 32bit的首字节地址，每一个4字节
	{//1次循环生成1个字节扩展密钥，4次循环生成一个WORD
		//temp：4字节数组：代表一个WORD密钥
		/*temp=w[i-1]*/
		//i不是4的倍数的时候
		//每个temp = 每个outkey32bit = 4字节
		for (x = 0; x < 4; x++)
			temp[x] = outkey[(4 * (i - 1)) + x];	//i：32bit的首字节地址
		//i是4的倍数的时候
		if (i % Nk == 0)
		{
			/*字循环：循环左移1字节 RotWord()*/
			t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
			/*字节代换：SubWord()*/
			for (x = 0; x < 4; x++)
			{
				temp[x] = sbox[temp[x]];
			}
			/*轮常量异或：Rcon[j]*/
			temp[0] ^= Rcon[(i / Nk) - 1];
		}
		//else if(Nk>6 && (i%Nk)==4)	//Nk>6的算法不同，暂时用不到
		//{
		//	/*SubWord*/
		//	for(x=0;x<4;x++)
		//	{
		//		temp[x]=sbox[temp[x]];
		//	}
		//}

		/*w[i] = w[i-4]^w[i-1]*/
		for (x = 0; x < 4; x++)
		{
			outkey[(4 * i) + x] = outkey[(4 * (i - Nk)) + x] ^ temp[x];
		}
		++i;
	}
}
unsigned char pt[40], key[17] = { 0x24, 0x7e, 0x1, 0x11, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf, 0x15, 0x88, 0x09, 0xf, 0x4f, 0x3c };
unsigned char cipher[40] = { 100, 157, 209, 60, 122, 17, 59, 225, 6, 80, 66, 132, 163, 17, 176, 183, 53, 0, 74, 24, 63, 49, 32, 79, 18, 189, 112, 50, 87, 30, 220, 78 };

unsigned char expansionkey[15 * 16];
int main(void) {
	//memset(pt, 0, sizeof(pt));
	int i;
	int j;
	printf("input flag:\n");
	scanf("%s", pt);
	if (strlen((char*)pt) > 32) return 0;
	/*加密*/
	ScheduleKey(key, expansionkey, 4, 10);	//1、密钥扩展生成
	AesEncrypt(pt, expansionkey, 10);		//2、AES 加密
	AesEncrypt(pt+16, expansionkey, 10);
	for (i = 0; i < 32; i++)
	{
		if (pt[i] != cipher[i]) {
			printf("wrong %d\n",pt[i]);
			//return 0;
		}
		
	}
	printf("right");
	printf("\n");
}
