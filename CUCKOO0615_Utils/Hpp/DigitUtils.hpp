//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：数字
//*************************************************
#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace DigitUtils
{
	/*
	** 计算无符号int的数字长度
	** @Ret: 长度范围 1-9
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
	** 将整数转换为汉字大写
	** @Param llNum: 要转换的整数, 最大值 ULLONG_MAX:9223372036854775807*2+1
	** @Param wstrChinese: 转换后的汉字大写
	*/
	static wchar_t arrChineseNums[] = 
	{ L'零', L'壹', L'贰', L'叁', L'肆', L'伍', L'陆', L'柒', L'捌', L'玖' };
	static wchar_t wszNumTemplete[] = 
	{ 0x00, L'仟', 0x00, L'佰', 0x00, L'拾', 0x00, 0x00 };
	static wchar_t wszChineseUnit[] = 	{ L'亿', L'万' };
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
				if (L'零' == wstr[i])
					wstr[++i] = L'零';
			}

			wstr.erase(std::unique(wstr.begin(), wstr.end()), wstr.end());
			if (L'零' == *(wstr.end() - 1))	wstr.erase(wstr.end() - 1);

			vecTmp.push_back(wstr);
			llNum = llNum / 10000;
		}
		if (!vecTmp.size())	return;

		wstrChinese = vecTmp[0];
		for (int i = 1; i != vecTmp.size(); ++i)
			wstrChinese = vecTmp[i] + wszChineseUnit[i % 2] + wstrChinese;

		for (int i = 0; i != wstrChinese.length() - 1; ++i)
		{
			if (L'亿' == wstrChinese[i] && L'万' == wstrChinese[i + 1])
				wstrChinese[i + 1] = L'零';
		}

		wstrChinese.erase(std::unique(wstrChinese.begin(), wstrChinese.end()), wstrChinese.end());
		if (L'零' == *(wstrChinese.end() - 1)) wstrChinese.erase(wstrChinese.end() - 1);
		if (L'零' == *(wstrChinese.begin())) wstrChinese.erase(wstrChinese.begin());

		return;
	}

}
