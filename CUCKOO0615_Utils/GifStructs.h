
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年9月8日
//◇说明：GIF文件结构体
//*************************************************
#pragma once
#include <windows.h>

#pragma pack(push, 1)

enum GIFBlockTypes
{
    BLOCK_UNKNOWN,
    BLOCK_APPEXT,
    BLOCK_COMMEXT,
    BLOCK_CONTROLEXT,
    BLOCK_PLAINTEXT,
    BLOCK_IMAGE,
    BLOCK_TRAILER
};

// graphic control extension packed field values
enum ControlExtValues 
{
    GCX_PACKED_DISPOSAL,  // disposal method
    GCX_PACKED_USERINPUT,
    GCX_PACKED_TRANSPCOLOR
};

enum LSDPackedValues 
{
    LSD_PACKED_GLOBALCT,
    LSD_PACKED_CRESOLUTION,
    LSD_PACKED_SORT,
    LSD_PACKED_GLOBALCTSIZE
};

enum IDPackedValues   
{
    ID_PACKED_LOCALCT,
    ID_PACKED_INTERLACE,
    ID_PACKED_SORT,
    ID_PACKED_LOCALCTSIZE
};

//GIF文件头 6字节,GIF87a或GIF89a
typedef struct gifHeader_t
{
	char Signature[3];
	char VersionNum[3];
}GIF_HEADER;

//颜色结构体 3字节
typedef struct colot_t 
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}GIF_COLOR;	

//逻辑屏幕标识符 7字节
typedef struct logicScreenDescriptor_t
{
	unsigned short ScreenWidth;
	unsigned short ScreenHeight;

	//012: Pixel位, 全局调色板单个像素使用的比特位数,范围0~7, 
	//     Pixel+1得到调色板的索引数（即总的颜色数量,2的Pixel+1次方,最大256色）
	//  3: s位,  置位表示全局调色板颜色按照使用率进行从高到底排列
	//456: cr位, 颜色深度，cr+1确定图像颜色深度
	//  7: m位,  置位时表示有全局调色板，Pixel位有意义。
	unsigned char PackedFields;
	unsigned char Background;		// 背景色索引
	unsigned char PixelAspectRatio;	// 像素宽高比

    inline int BitPerPixel() { return(PackedFields&0x07) + 1; }
    inline int GetPackedValue(enum LSDPackedValues Value)
    {
        int nRet = (int)PackedFields;
        switch (Value)
        {
        case LSD_PACKED_GLOBALCT:
            nRet = nRet >> 7;
            break;

        case LSD_PACKED_CRESOLUTION:
            nRet = ((nRet & 0x70) >> 4) + 1;
            break;

        case LSD_PACKED_SORT:
            nRet = (nRet & 8) >> 3;
            break;

        case LSD_PACKED_GLOBALCTSIZE:
            nRet &= 7;
            break;
        };
        return nRet;
    };
}GIF_LSD;

// application extension block
struct TGIFAppExtension 
{
    unsigned char ExtIntroducer; // extension introducer (0x21)
    unsigned char ExtLabel; // app. extension label (0xFF)
    unsigned char BlockSize; // fixed value of 11
    char AppIdentifier[8];   // application identifier
    char AppAuth[3];  // application authentication code
};

// GRAPHIC CONTROL EXTENSION
typedef struct graphicControlExtension_t 
{				
    unsigned char ExtIntroducer; // extension introducer (0x21)
    unsigned char ControlLabel;  // control extension label (0xF9)
	unsigned char BlockSize;		// 块大小: 固定4

	//    0: 透明色标识
	//    1: 用户输入标识
	//  2-4: 处置方法 
	////	0-不进行处置
	////	1-不处置图形，把图形从当前位置移去
	////	2-恢复到背景色
	////	3-恢复到先前状态
	////	4567-自定义
	//  5-7: 保留
	unsigned char PackedFields;		
	unsigned short Delay;			// 延时,单位1/100秒
	unsigned char Transparent;		// 透明色索引值
	unsigned char BlockTerminator;  // 固定为 0
public:
    inline int GetPackedValue(enum ControlExtValues Value)
    {
        int nRet = (int)PackedFields;
        switch (Value)
        {
        case GCX_PACKED_DISPOSAL:
            nRet = (nRet & 28) >> 2;
            break;

        case GCX_PACKED_USERINPUT:
            nRet = (nRet & 2) >> 1;
            break;

        case GCX_PACKED_TRANSPCOLOR:
            nRet &= 1;
            break;
        };
        return nRet;
    };
}GIF_GRAPHIC_CTRLEX;

// comment extension block
struct TGIFCommentExt  
{
    unsigned char ExtIntroducer; // extension introducer (0x21)
    unsigned char CommentLabel;  // comment extension label (0xFE)
};

// plain text extension block
struct TGIFPlainTextExt 
{
    unsigned char ExtIntroducer;  // extension introducer (0x21)
    unsigned char PlainTextLabel; // text extension label (0x01)
    unsigned char BlockSize; // fixed value of 12
    unsigned short LeftPos;    // text grid left position
    unsigned short TopPos;     // text grid top position
    unsigned short GridWidth;  // text grid width
    unsigned short GridHeight; // text grid height
    unsigned char CellWidth;  // character cell width
    unsigned char CellHeight; // character cell height
    unsigned char FgColor; // text foreground color index
    unsigned char BkColor; // text background color index
};

//图像描述
typedef struct imageDescriptor 
{	
    unsigned char ImageSeparator; // image separator byte (0x2C)
	unsigned short XPos;			// 图像X坐标
	unsigned short YPos;			// 图像Y坐标
	unsigned short Width;			// 图像宽度
	unsigned short Height;			// 图像高度

	//  0-2: Pixel位, 局部调色板单个像素使用的比特位数,范围0~7
	//  3-4: r位, 保留,必须为0
	//    5: s位,  置位表示局部调色板颜色按照使用率进行从高到底排列
	//    6: 交叉标识,置位时图像数据交叉排列，否则顺序排列
	//    7: m位,  局部调色板标识，置位时标识紧跟在图像标识符之后有一个局部调色板
	//       仅供跟在它后的一副图像使用；置否时使用全局调色板，忽略Pixel值
	unsigned char PackedFields;

    inline int BitPerPixel() { return (PackedFields&0x07)+ 1; }
	inline int GetPackedValue(enum IDPackedValues Value)
    {
        int nRet = (int)PackedFields;
        switch (Value)
        {
        case ID_PACKED_LOCALCT:
            nRet >>= 7;
            break;

        case ID_PACKED_INTERLACE:
            nRet = ((nRet & 0x40) >> 6);
            break;

        case ID_PACKED_SORT:
            nRet = (nRet & 0x20) >> 5;
            break;

        case ID_PACKED_LOCALCTSIZE:
            nRet &= 7;
            break;
        };
        return nRet;
    };
} GIF_IMAGEDESC;

typedef struct imageElement
{
// 	GIF_GRAPHIC_CTRLEX GraphicCtrlEx;   //控制扩展
// 	GIF_IMAGEDESC ImageDesc;			//图像描述
// 
// 	int Width, Height;      // 像素表示的尺寸
// 	int BitPerPixel;        // 单个像素的Bits
 	GIF_COLOR* Palette;     // 调色板
 	unsigned char * Raster; // 光栅数据的Bits (Byte对齐)
// 	int BytesPerRow;        // 包含字节对齐在内的宽度 (以字节为单位)
// 	int Transparent;        // 透明色索引 (-1 或没有)

// 	// 动画的扩展成员:
 	int Delay;              // 毫秒延时.

	~imageElement() 
	{ 
 		if(Raster)
 			delete [] Raster;
		if(Palette)
			delete [] Palette;
	}

}GIF_IMAGELEMENT;

#pragma pack(pop)
