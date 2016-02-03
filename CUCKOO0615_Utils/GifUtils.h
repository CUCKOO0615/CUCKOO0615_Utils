
#pragma once
#include <string.h>

//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵����GIFͼƬ
//*************************************************

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#pragma pack(4)

#define MAX_GIFCOLORNUM 256

#include "GifStructs.h"
#include <vector>
using namespace std;

class GifUtils
{
public:
    unsigned char* m_pGifRawData;
    unsigned char* m_pReadPointer;
    _off_t m_nFileSize;

	GIF_HEADER * m_pGifHeader;    //Gif�ļ�ͷ
	GIF_LSD    * m_pGifLsd;       //�߼���Ļ��ʶ��
	GIF_COLOR* m_pGlobalPalette;  //ȫ�ֵ�ɫ��
    unsigned int m_nGlobalPaletteSize;
    bool m_bUseGlobalPalette;

    GIF_COLOR m_clrBackGround;

    unsigned int m_nPicWidth;
    unsigned int m_nPicHeight;

    unsigned int m_nFrameCount;


    bool SkipNextGraphicBlock();
    int GetNextBlockLen() const;
    GIFBlockTypes GetNextBlockType() const;
    bool SkipNextBlock();
    unsigned int GetSubBlocksLen(unsigned int nStartingOffset) const;

    //�ļ���ָ�����õ�ȫ�ֵ�ɫ�����ݿ�ĩβ
    void ResetReadPointer();

	int LZWDecoder (char* parBufIn, char* parBufOut, 
		short InitCodeSize, 
		int AlignedWidth, 
		int Width, int Height, 
		bool bInterlace);

	vector<GIF_IMAGELEMENT*> m_arrImageElems;
public:
	GifUtils(void);
	~GifUtils(void);

	bool LoadGifFile(const char* szFileName);
	bool PlayGif(bool b);

// 	int SaveFrame(GIF_IMAGELEMENT * parImageElem, const char* szFilePath);
// 	int SaveFrames();

	//////////////////////////////////////////////////////////////////////////
private:
	char m_szErrMsg[ERRMSG_LENGTH];
	void SetErrMsg(const char* szErrMsg) { strcpy(m_szErrMsg, szErrMsg); }

public:
	const char* GetErrMsg() { return m_szErrMsg; }


// public:
// 	int FrameWidth, FrameHeight;	// Dimensions of ImageSet in pixels.
// 	int nLoops;						// Number of Loops (0 = infinite)
// 
// 	C_Image ** img;				// Images' Vector.
// 	int nImages;					// Number of images (vector size)
// 
// 	void AddImage (C_Image*);		// Append new image to vector (push_back)
// 
// 	// constructor and destructor:
// 	C_ImageSet() {img=0; nImages=0; nLoops=0;}
// 	~C_ImageSet() {for (int n=0;n<nImages;n++) delete img[n]; delete[] img;}
// 
// 	// File Formats:
// 	int LoadGIF (char* szFile);
// 	int SaveGIF (char* szFile) {return 0;};	// NOT IMPLEMENTED
	
};
