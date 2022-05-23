#pragma once

class PNG
{
public:
	struct COL
	{
		unsigned char a, r, g, b;
	};

	struct PNGHeader
	{
		unsigned width;
		unsigned height;
		int bpp;
		int mode;
	};

	struct colornum_t
	{
		unsigned int num;
		unsigned int color;
	};

	static int ApplyFilter(unsigned char* dest, unsigned char* src, unsigned width, int y, int bpp);
	static int ApplyFilter32(unsigned char* dest, unsigned char* src, unsigned width, int y);
	static void ApplyFilter1(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width);
	static void ApplyFilter2(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width);
	static void ApplyFilter3(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width);
	static int ApplyFilter8(unsigned char* dest, unsigned char* src, unsigned width, int y);
	static int ApplySpecificFilter8(int filtermode, unsigned char* dest, unsigned char* src, unsigned width);
	static int ApplySpecificFilter32(int filtermode, unsigned char* dest, unsigned char* src, unsigned width);
	static void InverseFilter32(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width);
	static int Load(const char* sname, unsigned* destwidth, unsigned* destheight, unsigned char** destdata);
	static int GetDimensions(const char* sname, unsigned* destwidth, unsigned* destheight);
	static long StartChunk(FILE* fi, char* type, int len);
	static void EndChunk(FILE* fi);
	static int ReadChunk(char(*head)[4], int* destlen, unsigned char** destdata, FILE* fi);
	static int ReadHeader(FILE* fi, struct PNGHeader* header);
	static void WritePaletteBased(const char* filename, unsigned char* data, unsigned width, unsigned height, int numcolors);
	static void Write(const char* filename, unsigned char* data, unsigned width, unsigned height);
	static void WriteQuick(const char* filename, unsigned char* data, unsigned width, unsigned height);
	static void WritePaletteBased2(const char* filename, unsigned char* data, unsigned width, unsigned height);
	static void WritePaletteBased2(const char* filename, unsigned char* data, unsigned width, unsigned height, int numcolors, int compression);
	static void WriteByte(FILE* fi, unsigned char byte);
	static void WriteBytes(FILE* fi, unsigned char* bytes, int len);
	static void WriteDword(FILE* fi, unsigned int dword);
	static unsigned int GetDword(FILE* fi);
	static unsigned char PaethPredictor(unsigned char a, unsigned char b, unsigned char c);
	static char HasAlpha(unsigned char* _image, int size);
	static int CompareColors(const void* _c1, const void* _c2);
	static colornum_t* GetColors(COL* image, int size, int* num);
	static void GetOptimalPalette(COL* image, int size, int palettesize, COL* palette);
	static void QuantizeImage(unsigned char* _image, int size, int numcolors, unsigned char** newimage, COL* palette);
	static void MakeCRC32Table();
	static void PatchLen(FILE* fi, int pos, int len);
	static long CompressLine(z_stream* zs, Bytef* line, int len, FILE* fi);
	static int TestLine(z_stream* zs_orig, Bytef* line, int linelen);
	static int FinishZlib(z_stream* zs, FILE* fi);
	static unsigned int ColorHash(COL* col);
	static int GetNumberOfPaletteEntries(COL* img, unsigned width, unsigned height, COL* palette, char* has_alpha);
	static void MapToPalette(COL* src, unsigned char* dest, int size, COL* palette, int palette_size);
	static void MakeNumBitsTable();
	static int FindBestFilter(unsigned char* src, unsigned width, int bpp, int y);
};
