
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��9��8��
//��˵����GIF�ļ��ṹ��
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

//GIF�ļ�ͷ 6�ֽ�,GIF87a��GIF89a
typedef struct gifHeader_t
{
	char Signature[3];
	char VersionNum[3];
}GIF_HEADER;

//��ɫ�ṹ�� 3�ֽ�
typedef struct colot_t 
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}GIF_COLOR;	

//�߼���Ļ��ʶ�� 7�ֽ�
typedef struct logicScreenDescriptor_t
{
	unsigned short ScreenWidth;
	unsigned short ScreenHeight;

	//012: Pixelλ, ȫ�ֵ�ɫ�嵥������ʹ�õı���λ��,��Χ0~7, 
	//     Pixel+1�õ���ɫ��������������ܵ���ɫ����,2��Pixel+1�η�,���256ɫ��
	//  3: sλ,  ��λ��ʾȫ�ֵ�ɫ����ɫ����ʹ���ʽ��дӸߵ�������
	//456: crλ, ��ɫ��ȣ�cr+1ȷ��ͼ����ɫ���
	//  7: mλ,  ��λʱ��ʾ��ȫ�ֵ�ɫ�壬Pixelλ�����塣
	unsigned char PackedFields;
	unsigned char Background;		// ����ɫ����
	unsigned char PixelAspectRatio;	// ���ؿ�߱�

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
	unsigned char BlockSize;		// ���С: �̶�4

	//    0: ͸��ɫ��ʶ
	//    1: �û������ʶ
	//  2-4: ���÷��� 
	////	0-�����д���
	////	1-������ͼ�Σ���ͼ�δӵ�ǰλ����ȥ
	////	2-�ָ�������ɫ
	////	3-�ָ�����ǰ״̬
	////	4567-�Զ���
	//  5-7: ����
	unsigned char PackedFields;		
	unsigned short Delay;			// ��ʱ,��λ1/100��
	unsigned char Transparent;		// ͸��ɫ����ֵ
	unsigned char BlockTerminator;  // �̶�Ϊ 0
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

//ͼ������
typedef struct imageDescriptor 
{	
    unsigned char ImageSeparator; // image separator byte (0x2C)
	unsigned short XPos;			// ͼ��X����
	unsigned short YPos;			// ͼ��Y����
	unsigned short Width;			// ͼ����
	unsigned short Height;			// ͼ��߶�

	//  0-2: Pixelλ, �ֲ���ɫ�嵥������ʹ�õı���λ��,��Χ0~7
	//  3-4: rλ, ����,����Ϊ0
	//    5: sλ,  ��λ��ʾ�ֲ���ɫ����ɫ����ʹ���ʽ��дӸߵ�������
	//    6: �����ʶ,��λʱͼ�����ݽ������У�����˳������
	//    7: mλ,  �ֲ���ɫ���ʶ����λʱ��ʶ������ͼ���ʶ��֮����һ���ֲ���ɫ��
	//       �������������һ��ͼ��ʹ�ã��÷�ʱʹ��ȫ�ֵ�ɫ�壬����Pixelֵ
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
// 	GIF_GRAPHIC_CTRLEX GraphicCtrlEx;   //������չ
// 	GIF_IMAGEDESC ImageDesc;			//ͼ������
// 
// 	int Width, Height;      // ���ر�ʾ�ĳߴ�
// 	int BitPerPixel;        // �������ص�Bits
 	GIF_COLOR* Palette;     // ��ɫ��
 	unsigned char * Raster; // ��դ���ݵ�Bits (Byte����)
// 	int BytesPerRow;        // �����ֽڶ������ڵĿ�� (���ֽ�Ϊ��λ)
// 	int Transparent;        // ͸��ɫ���� (-1 ��û��)

// 	// ��������չ��Ա:
 	int Delay;              // ������ʱ.

	~imageElement() 
	{ 
 		if(Raster)
 			delete [] Raster;
		if(Palette)
			delete [] Palette;
	}

}GIF_IMAGELEMENT;

#pragma pack(pop)
