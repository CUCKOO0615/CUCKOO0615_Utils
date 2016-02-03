//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：数字
//*************************************************

namespace DigitUtils
{
	inline unsigned int CountDecimalDigit32(unsigned int n)
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





}
