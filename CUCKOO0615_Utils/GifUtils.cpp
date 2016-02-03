#include "StdAfx.h"
#include "GifUtils.h"
#include <fstream>
#include <sys/stat.h>
using namespace std;


GifUtils::GifUtils(void)
{
    m_pGifRawData = NULL;
    m_pReadPointer = NULL;

    m_pGifHeader = NULL;   
    m_pGifLsd = NULL;

	//memset(&m_gifHeader, 0x00, sizeof(GIF_HEADER));
	m_pGlobalPalette = NULL;
}


GifUtils::~GifUtils(void)
{
    if(m_pGifRawData)
    {
        delete [] m_pGifRawData;
        m_pGifRawData = NULL;
    }

	if(m_pGlobalPalette)
		delete [] m_pGlobalPalette;
}

bool GifUtils::LoadGifFile( const char* szFileName )
{
	//打开文件
#pragma region 打开文件
	ifstream fGifFile(szFileName, ios::in|ios::_Nocreate|ios::binary);
	if (!fGifFile.is_open()) 
	{
        fGifFile.close();
		SetErrMsg("File not found");
		return false;
	}

    struct _stat gifFileStat;
    int nRet = _stat(szFileName, &gifFileStat);
    m_nFileSize = ( 0 == nRet )? gifFileStat.st_size : 0;
    
    if(0 == m_nFileSize)
    {
        fGifFile.close();
        SetErrMsg("File size is 0");
        return false;
    }

    if(m_pGifRawData)
        delete [] m_pGifRawData;
    m_pGifRawData = (unsigned char*)new char[m_nFileSize];
    memset(m_pGifRawData, 0x00, m_nFileSize);
    fGifFile.read((char*)m_pGifRawData, m_nFileSize);
    fGifFile.close();
    m_pReadPointer = m_pGifRawData;
#pragma endregion 打开文件

    //1 文件头
#pragma region 文件头
    m_pGifHeader = (GIF_HEADER*)m_pReadPointer;
    if ( memcmp(m_pGifHeader->Signature,"GIF", 3) != 0)
    { 
        SetErrMsg("Not a GIF File");
        return 0; 
    }
    m_pReadPointer += sizeof(GIF_HEADER);
#pragma endregion 文件头

	//2 逻辑屏幕描述符
#pragma region 逻辑屏幕描述符
    m_pGifLsd = (GIF_LSD*)m_pReadPointer;
    m_pReadPointer += sizeof(GIF_LSD);
#pragma endregion 逻辑屏幕描述符

	//3 全局调色板
#pragma region 全局调色板
	if(m_pGlobalPalette)
    {
		delete [] m_pGlobalPalette;
        m_pGlobalPalette = NULL;
    }

    m_bUseGlobalPalette = (m_pGifLsd->GetPackedValue(LSD_PACKED_GLOBALCT) == 1);
	if(m_bUseGlobalPalette)
	{
        m_nGlobalPaletteSize = static_cast<int>(1<<(m_pGifLsd->GetPackedValue(LSD_PACKED_GLOBALCTSIZE)+1));
        m_pGlobalPalette = new GIF_COLOR[m_nGlobalPaletteSize];
        memset(m_pGlobalPalette, 0x00, m_nGlobalPaletteSize*sizeof(GIF_COLOR));
        memcpy(m_pGlobalPalette, m_pReadPointer, m_nGlobalPaletteSize*sizeof(GIF_COLOR));
	}
	else   //无全局调色板则默认生成一个
	{
        m_nGlobalPaletteSize = 0;
		m_pGlobalPalette = new GIF_COLOR[MAX_GIFCOLORNUM];
		for (int n=0; n!=MAX_GIFCOLORNUM; ++n)
		{
			m_pGlobalPalette[n].R = n;
			m_pGlobalPalette[n].G = n;
			m_pGlobalPalette[n].B = n;
		}
	}
    m_clrBackGround = m_pGlobalPalette[0];
    m_pReadPointer += (m_nGlobalPaletteSize*sizeof(GIF_COLOR));
#pragma endregion 全局调色板

    m_nPicWidth = m_pGifLsd->ScreenWidth;
    m_nPicHeight = m_pGifLsd->ScreenHeight;

    GIF_GRAPHIC_CTRLEX * pGraphCtrlEx = NULL;
    bool bCtrlExtenExist = false;
    do 
    {
        GIFBlockTypes gifBlockType = GetNextBlockType();
        if(gifBlockType != BLOCK_CONTROLEXT 
            && gifBlockType != BLOCK_IMAGE )
        {
            if(SkipNextBlock())
                continue;
            else
                break;
        }

        if(gifBlockType == BLOCK_CONTROLEXT)
        {
            pGraphCtrlEx = (GIF_GRAPHIC_CTRLEX*)m_pReadPointer;
            m_pReadPointer += sizeof(GIF_GRAPHIC_CTRLEX);
            bCtrlExtenExist = true;
            continue;
        }
        else
        {
            GIF_IMAGELEMENT* pGifImageElem = new GIF_IMAGELEMENT;
            if(bCtrlExtenExist)
                pGifImageElem->Delay = pGraphCtrlEx->Delay;

            GIF_IMAGEDESC* pGifImageDesc = (GIF_IMAGEDESC*)m_pReadPointer;
            m_pReadPointer += sizeof(GIF_IMAGEDESC);
            int BitPerPixel = 0;

            if(pGifImageDesc->GetPackedValue(ID_PACKED_LOCALCT) == 1)
            {
                BitPerPixel = pGifImageDesc->BitPerPixel();
                pGifImageElem->Palette = new GIF_COLOR[1<<BitPerPixel];
                m_pReadPointer += ((1<<BitPerPixel)*sizeof(GIF_COLOR));
            }
            else
            {
                BitPerPixel = m_pGifLsd->BitPerPixel();
                pGifImageElem->Palette = new GIF_COLOR[1<<BitPerPixel];
                memcpy(pGifImageElem->Palette, m_pGlobalPalette, 1<<BitPerPixel);
            }

            short firstbyte = m_pReadPointer[0];
            m_pReadPointer++;
            int nBytesPerRow = pGifImageDesc->Width;
            nBytesPerRow += (sizeof(int)-pGifImageDesc->Width%sizeof(int))%sizeof(int);
            pGifImageElem->Raster = (unsigned char*)new char[nBytesPerRow * pGifImageDesc->Height];
            
            int nRet = LZWDecoder( (char*)m_pReadPointer, (char*)pGifImageElem->Raster,
                firstbyte, nBytesPerRow,//NextImage->AlignedWidth,
                pGifImageDesc->Width, pGifImageDesc->Height,
                ((pGifImageDesc->PackedFields&0x40))        //Interlaced?
                );

            if (nRet)
                m_arrImageElems.push_back(pGifImageElem);
            else
                delete pGifImageElem;

            m_pReadPointer--;
            if(pGifImageDesc->GetPackedValue(ID_PACKED_LOCALCT) == 1)
                m_pReadPointer -= ((1<<BitPerPixel)*sizeof(GIF_COLOR));
            m_pReadPointer -= sizeof(GIF_IMAGEDESC);
            SkipNextBlock();
            bCtrlExtenExist = false;
        }

    } 
    while ((m_pReadPointer - m_pGifRawData) < m_nFileSize);

/*
	//4 遍历读取图像
	do
	{
		//IMAGE (0x2c 图像分隔符)
		if (charGot == 0x2c) 
		{
			int k = fGifFile.tellg();

			fGifFile.read((char*)&pGifImageElem->ImageDesc, sizeof(GIF_IMAGEDESC));

			int p = sizeof(GIF_IMAGEDESC);
			int m = fGifFile.tellg();

			pGifImageElem->BitPerPixel = pGifImageElem->ImageDesc.UseLocalPalette()?
				pGifImageElem->ImageDesc.BitPerPixel():m_gifLsd.BitPerPixel();

			// Fill NextImage Data
			if (bCtrlExtenExist)
			{
				//gifImageElem->Transparent = (gifCtrlEx.PackedFields&0x01) ? gifCtrlEx.Transparent : -1;
				//NextImage->Transparency = (gifCtrlEx.PackedFields&0x1c)>1 ? 1 : 0;
				//NextImage->Delay = gifgce.Delay*10;
			}

			//前提是先设置BitPerPixel
			pGifImageElem->Init(pGifImageElem->ImageDesc.Width, pGifImageElem->ImageDesc.Height);

			//拷贝调色板
			if (pGifImageElem->ImageDesc.UseLocalPalette())
				fGifFile.read((char*)pGifImageElem->Palette, sizeof(GIF_COLOR)*(1<<pGifImageElem->BitPerPixel));
			else
				memcpy (pGifImageElem->Palette, m_pGlobalPalette, sizeof(GIF_COLOR)*(1<<pGifImageElem->BitPerPixel));

			// 1st byte of img block (CodeSize)
			short firstbyte = fGifFile.get();

			// Calculate compressed image block size
			// to fix: this allocates an extra byte per block
			long ImgStart = fGifFile.tellg();
			long ImgEnd   = ImgStart;

			int n = 0;
			while (n=fGifFile.get()) 
				fGifFile.seekg (ImgEnd+=n+1);
			fGifFile.seekg (ImgStart);

			// Allocate Space for Compressed Image
			char * pCompressedImage = new char [ImgEnd-ImgStart+4];
			memset(pCompressedImage, 0x00, ImgEnd-ImgStart+4);

			// Read and store Compressed Image
			char * pTemp = pCompressedImage;
			while (int nBlockLength = fGifFile.get())
			{
				fGifFile.read(pTemp,nBlockLength);
				pTemp += nBlockLength;
			}

			// Call LZW/GIF decompressor
			int nRet = LZWDecoder(
				(char*) pCompressedImage,
				(char*) pGifImageElem->Raster,
				firstbyte, pGifImageElem->BytesPerRow,//NextImage->AlignedWidth,
				pGifImageElem->ImageDesc.Width, pGifImageElem->ImageDesc.Height,
				((pGifImageElem->ImageDesc.PackedFields&0x40))        //Interlaced?
				);

			if (nRet)
				m_arrImageElems.push_back(pGifImageElem);
			else
				delete pGifImageElem;

			// Some cleanup
			delete[] pCompressedImage;
		}

		// *C* TRAILER: End of GIF Info
		if (charGot == 0x3b) 
			break; 

	} while (fGifFile.good());*/

	return true;
}

bool GifUtils::PlayGif( bool b )
{
	return true;
}


// 
// void imageElement::Init(int iWidth, int iHeight) 
// {
// 	if (Raster){delete[]Raster;Raster=0;}
// 	//if (pbmi){delete[]pbmi;pbmi=0;}
// 
// 	// Standard members setup
// 	Transparent = -1;
// 	Width = iWidth;
// 	BytesPerRow = iWidth; 
// 	Height = iHeight; 
// 
// 	// Animation Extra members setup:
// 	Delay = 0;
// 
// 	Palette = new GIF_COLOR[(1<<BitPerPixel)];
// 
// 	BytesPerRow += (sizeof(int)-Width%sizeof(int));        // Align BytesPerRow
// 	Raster = new char[BytesPerRow*Height];
// 
// 	BitMapInfoHeader.biSize=sizeof(BITMAPINFOHEADER);
// 	BitMapInfoHeader.biWidth=Width;
// 	BitMapInfoHeader.biHeight=-Height;                        // negative means up-to-bottom 
// 	BitMapInfoHeader.biPlanes=1;
// 	BitMapInfoHeader.biBitCount=8;
// 	BitMapInfoHeader.biCompression=BI_RGB;
// 	BitMapInfoHeader.biSizeImage=0;
// 	BitMapInfoHeader.biXPelsPerMeter=11811;
// 	BitMapInfoHeader.biYPelsPerMeter=11811;
// 	BitMapInfoHeader.biClrUsed=0;
// 	BitMapInfoHeader.biClrImportant=0;
// }



// ****************************************************************************
// * LZWDecoder (C/C++)                                                       *
// * Codec to perform LZW (GIF Variant) decompression.                        *
// *                         (c) Nov2000, Juan Soulie <jsoulie@cplusplus.com> *
// ****************************************************************************
//
// Parameter description:
//  - bufIn: Input buffer containing a "de-blocked" GIF/LZW compressed image.
//  - bufOut: Output buffer where result will be stored.
//  - InitCodeSize: Initial CodeSize to be Used
//    (GIF files include this as the first byte in a picture block)
//  - AlignedWidth : Width of a row in memory (including alignment if needed)
//  - Width, Height: Physical dimensions of image.
//  - Interlace: 1 for Interlaced GIFs.

int GifUtils::LZWDecoder (char* parBufIn, char* parBufOut, 
	short InitCodeSize, 
	int AlignedWidth, 
	int Width, int Height, 
	bool bInterlace)
{
	int row=0, col=0;                      // used to point output if Interlaced
	int nPixels = 0;
	int maxPixels = Width*Height;          // Output pixel counter

	short CodeSize = InitCodeSize + 1;     // Current CodeSize (size in bits of codes)
	short ClearCode = (1 << InitCodeSize); // Clear code : resets decompressor
	short EndCode = ClearCode + 1;         // End code : marks end of information

	long whichBit = 0;                     // Index of next bit in bufIn
	long LongCode;                         // Temp. var. from which Code is retrieved
	short Code;                            // Code extracted
	short PrevCode;                        // Previous Code
	short OutCode;                         // Code to output

	// Translation Table:
	const int nArrMaxSize = 4096;

	short Prefix[nArrMaxSize] = {0};              // Prefix: index of another Code
	unsigned char Suffix[nArrMaxSize] = {0};      // Suffix: terminating character

	short FirstEntry = ClearCode + 2;      // Index of first free entry in table
	short NextEntry = FirstEntry;          // Index of next free entry in table

	unsigned char OutStack[nArrMaxSize + 1] = {0};    // Output buffer
	int OutIndex;                          // Characters in OutStack

	int RowOffset = 0;                     // Offset in output buffer for current row

	while (nPixels<maxPixels) 
	{
		OutIndex = 0;                 // Reset Output Stack

		// GET NEXT CODE FROM bufIn:
		// LZW compression uses code items longer than a single byte.
		// For GIF Files, code sizes are variable between 9 and 12 bits 
		// That's why we must read data (Code) this way:
		LongCode = *((long*)(parBufIn + whichBit/8));   // Get some bytes from bufIn
		LongCode >>= (whichBit&7);                      // Discard too low bits
		Code = (LongCode & ((1<<CodeSize) - 1));        // Discard too high bits
		whichBit += CodeSize;                           // Increase Bit Offset

		// SWITCH, DIFFERENT POSIBILITIES FOR CODE:
		if (Code == EndCode)              // END CODE
			break;                        // Exit LZW Decompression loop

		if (Code == ClearCode)            // CLEAR CODE:
		{                    
			CodeSize = InitCodeSize + 1;  // Reset CodeSize
			NextEntry = FirstEntry;       // Reset Translation Table
			PrevCode = Code;              // Prevent next to be added to table.
			continue;                     // restart, to get another code
		}
		if (Code < NextEntry)             // CODE IS IN TABLE
			OutCode = Code;               // Set code to output.
		else                              // CODE IS NOT IN TABLE:
		{                                 
			OutIndex++;                   // Keep "first" character of previous output.
			OutCode = PrevCode;           // Set PrevCode to be output
		}

		// EXPAND OutCode IN OutStack
		// - Elements up to FirstEntry are Raw-Codes and are not expanded
		// - Table Prefices contain indexes to other codes
		// - Table Suffices contain the raw codes to be output
		while (OutCode >= FirstEntry) 
		{
			if (OutIndex > nArrMaxSize) 
				return 0;
			OutStack[OutIndex++] = Suffix[OutCode];        // Add suffix to Output Stack
			OutCode = Prefix[OutCode];                     // Loop with preffix
		}

		// NOW OutCode IS A RAW CODE, ADD IT TO OUTPUT STACK.
		if (OutIndex > nArrMaxSize) 
			return 0;
		OutStack[OutIndex++] = (unsigned char)OutCode;

		// ADD NEW ENTRY TO TABLE (PrevCode + OutCode)
		// (EXCEPT IF PREVIOUS CODE WAS A CLEARCODE)
		if (PrevCode!=ClearCode) 
		{
			Prefix[NextEntry] = PrevCode;
			Suffix[NextEntry] = (unsigned char)OutCode;
			NextEntry++;

			// Prevent Translation table overflow:
			if (NextEntry >= nArrMaxSize) 
				return 0;

			// INCREASE CodeSize IF NextEntry IS INVALID WITH CURRENT CodeSize
			if (NextEntry >= (1<<CodeSize)) 
			{
				if (CodeSize < 12) 
					CodeSize++;
				//else                                // Do nothing. Maybe next is Clear Code.
			}
		}

		PrevCode = Code;

		// Avoid the possibility of overflow on 'bufOut'.
		if ((nPixels + OutIndex) > maxPixels)
			OutIndex = maxPixels - nPixels;

		// OUTPUT OutStack (LAST-IN FIRST-OUT ORDER)
		for (int n = OutIndex-1; n >= 0; n--) 
		{
			if (col == Width)     // Check if new row.
			{
				if (bInterlace)    // 交错模式
				{                                
					if ((row&7)==0) 
					{
						row += 8; 
						if (row >= Height) 
							row=4;
					}
					else if ((row&3)==0) 
					{
						row += 8; 
						if (row >= Height) 
							row=2;
					}
					else if ((row&1)==0) 
					{
						row+=4; 
						if (row>=Height) 
							row=1;
					}
					else 
						row+=2;
				}
				else           //顺序模式
				{
					row++;
				}

				RowOffset = row*AlignedWidth;   // Set new row offset
				col=0;
			}

			parBufOut[RowOffset+col]=OutStack[n]; // Write output

			col++;         // Increase counters.
			nPixels++;             
		}
	}        // while (main decompressor loop)

	return whichBit;
}

/*

int GifUtils::SaveFrame(GIF_IMAGELEMENT * parImageElem, const char* szFilePath)
{
	int n;
	// Create file.
	ofstream bmpfile (szFilePath , ios::out|ios::binary|ios::trunc);
	if (!bmpfile.is_open()) 
	{ 
		SetErrMsg("Error creating file");
		return 0;
	}

	// determine BPP for file:
	int SaveBPP = 8; //parImageElem->BitPerPixel;

	// *1* SAVE BITMAP FILE HEADER
	struct BITMAPFILEHEADER {
		unsigned short	bfType; 
		unsigned long	bfSize; 
		unsigned short	bfReserved1; 
		unsigned short	bfReserved2; 
		unsigned long	bfOffBits; 
	} bmfh;

	bmfh.bfType = 'MB';
	bmfh.bfSize = 0;	// TO DO
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = 54 + (1<<SaveBPP)*4;
	bmpfile.write((char*)&bmfh,sizeof (bmfh));

	// *2* SAVE BITMAP INFO HEADER
	struct BITMAPINFOHEADER {
		unsigned long  biSize;
		long  biWidth;
		long  biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned long  biCompression;
		unsigned long  biSizeImage;
		long  biXPelsPerMeter;
		long  biYPelsPerMeter;
		unsigned long  biClrUsed;
		unsigned long  biClrImportant;
	} bmih;

	bmih.biSize = sizeof(bmih);
	bmih.biWidth = parImageElem->ImageDesc.Width;
	bmih.biHeight = parImageElem->ImageDesc.Height;	// down-top
	bmih.biPlanes = 1;
	bmih.biBitCount = SaveBPP;
	bmih.biCompression= 0; // BI_RGB?
	bmih.biSizeImage = (parImageElem->ImageDesc.Width * SaveBPP)/8;
	bmih.biSizeImage += (4- (bmih.biSizeImage)%4)%4;
	bmih.biXPelsPerMeter=11811;
	bmih.biYPelsPerMeter=11811;
	bmih.biClrUsed=0;
	bmih.biClrImportant=0;

	bmpfile.write ((char*)&bmih,sizeof (bmih));

	// *3* IF BPP AREN'T 24, SAVE PALETTE.
	for (n=0;n< 1<<SaveBPP;n++)
	{
		bmpfile.put(parImageElem->Palette[n].B);
		bmpfile.put(parImageElem->Palette[n].G);
		bmpfile.put(parImageElem->Palette[n].R);
		bmpfile.put((char)0);
	}
	for (;n < 1<<SaveBPP; n++)	// in case SaveBPP is higher than BPP
		bmpfile.write((char*)'\0\0\0\0',4);

	// *4* SAVE RASTER DATA

	int PixelsPerByte = 8/SaveBPP;	//used only if BPP are less than 8
	int BitMask = (1<<SaveBPP)-1;	//used only if BPP are less than 8

	// Raster Data Rows are 32bit aligned in BMP files.
	int RowAlignmentInFile = ((4- ((parImageElem->ImageDesc.Width*SaveBPP+7)/8)%4)%4); // (bytes)
	for (int row=0; row<parImageElem->ImageDesc.Height; row++)
	{
		char * pPixel;
		// If height is positive the bmp is bottom-up, set adequate row info:
		pPixel= (char*)parImageElem->Raster + parImageElem->BytesPerRow *((bmih.biHeight>0)? parImageElem->ImageDesc.Height-row-1:row );

		if (SaveBPP >= 8)	// 8 or more BPP: Save as block.
			bmpfile.write (pPixel, parImageElem->ImageDesc.Width*SaveBPP/8);
		else				// Less than 8BPP: Save packing bytes.
		{
			unsigned char charToPut;
			for (int col=0; col < parImageElem->ImageDesc.Width; col+=PixelsPerByte)
			{
				charToPut=0;
				for (int bit=8 ; bit >0 ; bit -= SaveBPP)	// high to low
					charToPut |= (*pPixel++ & BitMask) << (bit - SaveBPP);
				bmpfile.put(charToPut);
			}
		}
		// Ignore aligment bytes of file:
		for (int m=0; m<RowAlignmentInFile; m++) 
			bmpfile.put ((char)0);
	}
	bmpfile.close();
	return 1;
}


int GifUtils::SaveFrames()
{
	const char* szFolder = "D:\\123\\%d.bmp";

	for(int i=0;i!=m_arrImageElems.size();++i)
	{
		char szBuf[256] = {0};
		sprintf(szBuf, szFolder, i);
		SaveFrame(m_arrImageElems[i], szBuf);
	}
	return 0;
}*/
/*

bool GifUtils::SkipNextGraphicBlock()
{
    //if (!m_pRawData) return FALSE;

    // GIF header + LSDescriptor [+ GCT] [+ Control block] + Data

    enum GIFBlockTypes nBlock;
    nBlock = GetNextBlock();

    while ((nBlock != BLOCK_CONTROLEXT) && (nBlock != BLOCK_IMAGE) &&
        (nBlock != BLOCK_PLAINTEXT) && (nBlock != BLOCK_UNKNOWN) &&
        (nBlock != BLOCK_TRAILER) )
    {
        if (!SkipNextBlock()) 
            return NULL;
        nBlock = GetNextBlock();
    };

    if ((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
        return false;

    // it's either a control ext.block, an image or a plain text
    if (GetNextBlockLen() <= 0) 
        return false;

    if (nBlock == BLOCK_CONTROLEXT)
    {
        if (!SkipNextBlock()) 
            return false;
        nBlock = GetNextBlock();

        // skip everything until we meet an image block or a plain-text block
        while ((nBlock != BLOCK_IMAGE) && (nBlock != BLOCK_PLAINTEXT) &&
            (nBlock != BLOCK_UNKNOWN) && (nBlock != BLOCK_TRAILER) )
        {
            if (!SkipNextBlock()) return NULL;
            nBlock = GetNextBlock();
        };

        if ((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
            return false;
    };

    // skip the found data block (image or plain-text)
    if (!SkipNextBlock()) 
        return false;

    return true;
}
*/

GIFBlockTypes GifUtils::GetNextBlockType() const
{
    switch(m_pReadPointer[0])
    {
    case 0x21:
        // extension block
        switch(m_pReadPointer[1])
        {
        case 0x01:
            // plain text extension
            return BLOCK_PLAINTEXT;
            break;

        case 0xF9:
            // graphic control extension
            return BLOCK_CONTROLEXT;
            break;

        case 0xFE:
            // comment extension
            return BLOCK_COMMEXT;
            break;

        case 0xFF:
            // application extension
            return BLOCK_APPEXT;
            break;
        };
        break;

    case 0x3B:
        // trailer
        return BLOCK_TRAILER;
        break;

    case 0x2C:
        // image data
        return BLOCK_IMAGE;
        break;
    };

    return BLOCK_UNKNOWN;
}


bool GifUtils::SkipNextBlock()
{
    int nLen = GetNextBlockLen();
    if ((nLen <= 0) || ((m_pReadPointer - m_pGifRawData + nLen) > m_nFileSize))
        return false;
    m_pReadPointer += nLen;
    return true;
}

int GifUtils::GetNextBlockLen() const
{
    GIFBlockTypes nBlock = GetNextBlockType();
    int nTmp;
    switch(nBlock)
    {
    case BLOCK_UNKNOWN:
        return -1;
        break;

    case BLOCK_TRAILER:
        return 1;
        break;

    case BLOCK_APPEXT:
        nTmp = GetSubBlocksLen(m_pReadPointer - m_pGifRawData + sizeof(TGIFAppExtension));
        if (nTmp > 0)
            return sizeof(TGIFAppExtension)+nTmp;
        break;

    case BLOCK_COMMEXT:
        nTmp = GetSubBlocksLen(m_pReadPointer - m_pGifRawData + sizeof(TGIFCommentExt));
        if (nTmp > 0)
            return sizeof(TGIFCommentExt)+nTmp;
        break;

    case BLOCK_CONTROLEXT:
        return sizeof(GIF_GRAPHIC_CTRLEX);
        break;

    case BLOCK_PLAINTEXT:
        nTmp = GetSubBlocksLen(m_pReadPointer - m_pGifRawData + sizeof(TGIFPlainTextExt));
        if (nTmp > 0)
            return sizeof(TGIFPlainTextExt)+nTmp;
        break;

    case BLOCK_IMAGE:
        GIF_IMAGEDESC *pIDescr = (GIF_IMAGEDESC*)m_pReadPointer;
        //局部调色板大小
        int nLCTSize = (int)(pIDescr->GetPackedValue(ID_PACKED_LOCALCT)*3*
            (1<<(pIDescr->GetPackedValue(ID_PACKED_LOCALCTSIZE)+1)));

        int nTmp = GetSubBlocksLen(m_pReadPointer - m_pGifRawData +
            sizeof(GIF_IMAGEDESC) + nLCTSize + 1);
        if (nTmp > 0)
            return sizeof(GIF_IMAGEDESC) + nLCTSize + 1 + nTmp;
        break;
    };
    return 0;
}

unsigned int GifUtils::GetSubBlocksLen(unsigned int nStartingOffset) const
{
    unsigned int nRet = 0;
    while (m_pGifRawData[nStartingOffset] != 0)
    {
        nRet += m_pGifRawData[nStartingOffset]+1;
        nStartingOffset += m_pGifRawData[nStartingOffset]+1;
    };
    return nRet+1;
}

void GifUtils::ResetReadPointer()
{
    m_pReadPointer = m_pGifRawData + 
        sizeof(GIF_HEADER) + sizeof(GIF_LSD) + 
        m_nGlobalPaletteSize*sizeof(GIF_COLOR);
}