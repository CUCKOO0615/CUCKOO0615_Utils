//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵��������
//*************************************************
#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace DigitUtils
{
	/*
	** �����޷���int�����ֳ���
	** @Ret: ���ȷ�Χ 1-9
	*/
	unsigned int CountDecimalDigit32(unsigned int n)
	{
		if (n < 100000)
		{
			if (n < 10)return 1;
			if (n < 100)return 2;
			if (n < 1000)return 3;
			if (n < 10000)return 4;
			return 5;
		}
		else
		{
			if (n < 1000000)return 6;
			if (n < 10000000)return 7;
			if (n < 100000000)return 8;
			if (n < 1000000000)return 9;
			return 10;
		}
	}
	
	/*
	** ������ת��Ϊ���ִ�д
	** @Param llNum: Ҫת��������, ���ֵ ULLONG_MAX:9223372036854775807*2+1
	** @Param wstrChinese: ת����ĺ��ִ�д
	*/
	static wchar_t arrChineseNums[] = 
	{ L'��', L'Ҽ', L'��', L'��', L'��', L'��', L'½', L'��', L'��', L'��' };
	static wchar_t wszNumTemplete[] = 
	{ 0x00, L'Ǫ', 0x00, L'��', 0x00, L'ʰ', 0x00, 0x00 };
	static wchar_t wszChineseUnit[] = 	{ L'��', L'��' };
	void Num2Chinese(unsigned __int64 llNum, std::wstring& wstrChinese)
	{
		std::vector<std::wstring> vecTmp;
		while (true)
		{
			int n = llNum % 10000;
			if (!n && llNum < 10000) break;

			for (int i = 6; i > -1; i -= 2, n /= 10)
				wszNumTemplete[i] = arrChineseNums[n % 10];

			std::wstring wstr(wszNumTemplete);
			for (int i = 0; i != 6; ++i)
			{
				if (L'��' == wstr[i])
					wstr[++i] = L'��';
			}

			wstr.erase(std::unique(wstr.begin(), wstr.end()), wstr.end());
			if (L'��' == *(wstr.end() - 1))	wstr.erase(wstr.end() - 1);

			vecTmp.push_back(wstr);
			llNum = llNum / 10000;
		}
		if (!vecTmp.size())	return;

		wstrChinese = vecTmp[0];
		for (int i = 1; i != vecTmp.size(); ++i)
			wstrChinese = vecTmp[i] + wszChineseUnit[i % 2] + wstrChinese;

		for (int i = 0; i != wstrChinese.length() - 1; ++i)
		{
			if (L'��' == wstrChinese[i] && L'��' == wstrChinese[i + 1])
				wstrChinese[i + 1] = L'��';
		}

		wstrChinese.erase(std::unique(wstrChinese.begin(), wstrChinese.end()), wstrChinese.end());
		if (L'��' == *(wstrChinese.end() - 1)) wstrChinese.erase(wstrChinese.end() - 1);
		if (L'��' == *(wstrChinese.begin())) wstrChinese.erase(wstrChinese.begin());

		return;
	}

}
