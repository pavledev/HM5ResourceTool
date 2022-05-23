#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <fcntl.h>
#include "zlib/zlib.h"
#include <limits.h>
#include <cmath>
#include "Scaleform/PNG.hpp"

int PNG::ReadChunk(char(*head)[4], int* destlen, unsigned char** destdata, FILE* fi)
{
	unsigned int len;
	unsigned char blen[4];
	if (destlen) *destlen = 0;
	if (destdata) *destdata = 0;
	if (!fread(&blen, 4, 1, fi))
	{
		return 0;
	}
	if (!fread(head, 4, 1, fi))
	{
		return 0;
	}
	len = blen[0] << 24 | blen[1] << 16 | blen[2] << 8 | blen[3];
	if (destlen) *destlen = len;
	if (destdata)
	{
		if (!len)
		{
			*destdata = 0;
		}
		else
		{
			*destdata = (unsigned char*)malloc(len);
			if (!fread(*destdata, len, 1, fi))
			{
				*destdata = 0;
				if (destlen) *destlen = 0;
				return 0;
			}
		}
		fseek(fi, 4, SEEK_CUR);

	}
	else
	{
		fseek(fi, len + 4, SEEK_CUR);
	}
	return 1;
}

unsigned int PNG::GetDword(FILE* fi)
{
	unsigned char b[4];
	fread(&b, 4, 1, fi);
	return b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
}

int PNG::ReadHeader(FILE* fi, struct PNGHeader* header)
{
	char id[4];
	int len;
	int ok = 0;
	unsigned char head[8] = { 137,80,78,71,13,10,26,10 };
	unsigned char head2[8];
	unsigned char* data = nullptr;
	fread(head2, 8, 1, fi);
	if (strncmp((char*)head, (char*)head2, 4))
		return 0; // not a png file

	while (ReadChunk(&id, &len, &data, fi))
	{
		//printf("Chunk: %c%c%c%c (len:%d)\n", id[0],id[1],id[2],id[3], len);
		if (!strncmp(id, "IHDR", 4))
		{
			char a, b, c, f, i;
			if (len < 8) exit(1);
			header->width = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
			header->height = data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7];
			a = data[8];      // should be 8
			b = data[9];      // should be 3(indexed) or 2(rgb)

			c = data[10];     // compression mode (0)
			f = data[11];     // filter mode (0)
			i = data[12];     // interlace mode (0)

			if (b != 0 && b != 4 && b != 2 && b != 3 && b != 6)
			{
				fprintf(stderr, "Image mode %d not supported!\n", b);
				return 0;
			}
			if (a != 8 && (b == 2 || b == 6))
			{
				printf("Bpp %d in mode %d not supported!\n", b, a);
				return 0;
			}
			if (c != 0)
			{
				printf("Compression mode %d not supported!\n", c);
				return 0;
			}
			if (f != 0)
			{
				printf("Filter mode %d not supported!\n", f);
				return 0;
			}
			if (i != 0)
			{
				printf("Interlace mode %d not supported!\n", i);
				return 0;
			}
			//printf("%dx%d bpp:%d mode:%d comp:%d filter:%d interlace:%d\n",header->width, header->height, a,b,c,f,i);
			header->bpp = a;
			header->mode = b;
			ok = 1;
		}

		free(data);
	}
	return ok;
}

unsigned char PNG::PaethPredictor(unsigned char a, unsigned char b, unsigned char c)
{
	// a = left, b = above, c = upper left
	int p = a + b - c;        // initial estimate
	int pa = abs(p - a);      // distances to a, b, c
	int pb = abs(p - b);
	int pc = abs(p - c);
	// return nearest of a,b,c,
	// breaking ties in order a,b,c.
	if (pa <= pb && pa <= pc)
		return a;
	else if (pb <= pc)
		return b;
	else return c;
}

void PNG::ApplyFilter1(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width)
{
	unsigned char last = 0;
	unsigned char upperlast = 0;

	if (mode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	else if (mode == 1)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			*dest = *src + last;
			last = *dest;
			dest++;
			src++;
		}
	}
	else if (mode == 2)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			*dest = *src + *old;
			dest++;
			old++;
			src++;
		}
	}
	else if (mode == 3)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			*dest = *src + (*old + last) / 2;
			last = *dest;
			dest++;
			old++;
			src++;
		}
	}
	else if (mode == 4)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			*dest = *src + PaethPredictor(last, *old, upperlast);
			last = *dest;
			upperlast = *old;
			dest++;
			old++;
			src++;
		}
	}

}

void PNG::ApplyFilter2(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width)
{
	unsigned char lasta = 0;
	unsigned char lastb = 0;
	unsigned char upperlasta = 0;
	unsigned char upperlastb = 0;

	if (mode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[0];
			dest[1] = src[1];
			dest += 2;
			src += 2;
		}
	}
	else if (mode == 1)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[0] + lasta;
			dest[1] = src[1] + lastb;
			lasta = dest[0];
			lastb = dest[1];
			dest += 2;
			src += 2;
		}
	}
	else if (mode == 2)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[0] + old[0];
			dest[1] = src[1] + old[1];
			dest += 2;
			old += 2;
			src += 2;
		}
	}
	else if (mode == 3)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[0] + (old[0] + lasta) / 2;
			dest[1] = src[1] + (old[1] + lastb) / 2;
			lasta = dest[0];
			lastb = dest[1];
			dest += 2;
			old += 2;
			src += 2;
		}
	}
	else if (mode == 4)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[0] + PaethPredictor(lasta, old[0], upperlasta);
			dest[1] = src[1] + PaethPredictor(lastb, old[1], upperlastb);
			lasta = dest[0];
			lastb = dest[1];
			upperlasta = old[0];
			upperlastb = old[1];
			dest += 2;
			old += 2;
			src += 2;
		}
	}
}


/* also performs 24 bit conversion! */
void PNG::ApplyFilter3(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width)
{
	unsigned char lastr = 0;
	unsigned char lastg = 0;
	unsigned char lastb = 0;
	unsigned char upperlastr = 0;
	unsigned char upperlastg = 0;
	unsigned char upperlastb = 0;

	if (mode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = 255;
			dest[1] = src[0];
			dest[2] = src[1];
			dest[3] = src[2];
			dest += 4;
			src += 3;
		}
	}
	else if (mode == 1)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = 255;
			dest[1] = src[0] + lastr;
			dest[2] = src[1] + lastg;
			dest[3] = src[2] + lastb;
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			dest += 4;
			src += 3;
		}
	}
	else if (mode == 2)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = 255;
			dest[1] = src[0] + old[1];
			dest[2] = src[1] + old[2];
			dest[3] = src[2] + old[3];
			dest += 4;
			old += 4;
			src += 3;
		}
	}
	else if (mode == 3)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = 255;
			dest[1] = src[0] + (old[1] + lastr) / 2;
			dest[2] = src[1] + (old[2] + lastg) / 2;
			dest[3] = src[2] + (old[3] + lastb) / 2;
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			dest += 4;
			old += 4;
			src += 3;
		}
	}
	else if (mode == 4)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = 255;
			dest[1] = src[0] + PaethPredictor(lastr, old[1], upperlastr);
			dest[2] = src[1] + PaethPredictor(lastg, old[2], upperlastg);
			dest[3] = src[2] + PaethPredictor(lastb, old[3], upperlastb);
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			upperlastr = old[1];
			upperlastg = old[2];
			upperlastb = old[3];
			dest += 4;
			old += 4;
			src += 3;
		}
	}
}

void PNG::InverseFilter32(int mode, unsigned char* src, unsigned char* old, unsigned char* dest, unsigned width)
{
	unsigned char lastr = 0;
	unsigned char lastg = 0;
	unsigned char lastb = 0;
	unsigned char lasta = 0;
	unsigned char upperlastr = 0;
	unsigned char upperlastg = 0;
	unsigned char upperlastb = 0;
	unsigned char upperlasta = 0;

	if (mode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[3];
			dest[1] = src[0];
			dest[2] = src[1];
			dest[3] = src[2];
			dest += 4;
			src += 4;
		}
	}
	else if (mode == 1)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[3] + lasta;
			dest[1] = src[0] + lastr;
			dest[2] = src[1] + lastg;
			dest[3] = src[2] + lastb;
			lasta = dest[0];
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			dest += 4;
			src += 4;
		}
	}
	else if (mode == 2)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[3] + old[0];
			dest[1] = src[0] + old[1];
			dest[2] = src[1] + old[2];
			dest[3] = src[2] + old[3];
			dest += 4;
			old += 4;
			src += 4;
		}
	}
	else if (mode == 3)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[3] + (old[0] + lasta) / 2;
			dest[1] = src[0] + (old[1] + lastr) / 2;
			dest[2] = src[1] + (old[2] + lastg) / 2;
			dest[3] = src[2] + (old[3] + lastb) / 2;
			lasta = dest[0];
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			dest += 4;
			old += 4;
			src += 4;
		}
	}
	else if (mode == 4)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[0] = src[3] + PaethPredictor(lasta, old[0], upperlasta);
			dest[1] = src[0] + PaethPredictor(lastr, old[1], upperlastr);
			dest[2] = src[1] + PaethPredictor(lastg, old[2], upperlastg);
			dest[3] = src[2] + PaethPredictor(lastb, old[3], upperlastb);
			lasta = dest[0];
			lastr = dest[1];
			lastg = dest[2];
			lastb = dest[3];
			upperlasta = old[0];
			upperlastr = old[1];
			upperlastg = old[2];
			upperlastb = old[3];
			dest += 4;
			old += 4;
			src += 4;
		}
	}
}

int PNG::GetDimensions(const char* sname, unsigned* destwidth, unsigned* destheight)
{
	FILE* fi = nullptr;
	PNGHeader header;
	if ((fopen_s(&fi, sname, "rb")) == NULL)
	{
		fprintf(stderr, "Couldn't open %s\n", sname);
		return 0;
	}
	if (!ReadHeader(fi, &header))
	{
		return 0;
	}

	*destwidth = header.width;
	*destheight = header.height;
	fclose(fi);
	return 1;
}

int PNG::Load(const char* sname, unsigned* destwidth, unsigned* destheight, unsigned char** destdata)
{
	char tagid[4];
	int len;
	unsigned char* data;
	unsigned char* imagedata;
	unsigned char* zimagedata = 0;
	unsigned long int zimagedatalen = 0;
	unsigned char* palette = 0;
	int palettelen = 0;
	unsigned char* alphapalette = nullptr;
	int alphapalettelen = 0;
	PNGHeader header;
	int bypp;
	unsigned char* data2 = nullptr;
	unsigned char alphacolor[3];
	int hasalphacolor = 0;

	FILE* fi = nullptr;
	unsigned char* scanline = nullptr;

	if ((fopen_s(&fi, sname, "rb")) == NULL)
	{
		printf("Couldn't open %s\n", sname);
		return 0;
	}

	if (!ReadHeader(fi, &header))
	{
		fclose(fi);
		return 0;
	}

	if (header.mode == 3 || header.mode == 0) bypp = 1;
	else if (header.mode == 4) bypp = 2;
	else if (header.mode == 2) bypp = 3;
	else if (header.mode == 6) bypp = 4;
	else
	{
		printf("ERROR: mode:%d\n", header.mode);
		return 0;
	}

	unsigned long long imagedatalen_64 = ((unsigned long long)header.width + 1) * header.height * bypp;
	if (imagedatalen_64 > 0xffffffff)
		return 0;
	unsigned long imagedatalen = (unsigned long)imagedatalen_64;
	imagedata = (unsigned char*)malloc(imagedatalen);

	fseek(fi, 8, SEEK_SET);
	while (!feof(fi))
	{
		if (!ReadChunk(&tagid, &len, &data, fi))
			break;
		if (!strncmp(tagid, "IEND", 4))
		{
			break;
		}
		if (!strncmp(tagid, "PLTE", 4))
		{
			palette = data;
			palettelen = len / 3;
			data = 0; //don't free data
			//printf("%d colors in palette\n", palettelen);
		}
		if (!strncmp(tagid, "tRNS", 4))
		{
			if (header.mode == 3)
			{
				alphapalette = data;
				alphapalettelen = len;
				data = 0; //don't free data
				//printf("found %d alpha colors\n", alphapalettelen);
			}
			else if (header.mode == 0 || header.mode == 2)
			{
				if (header.mode == 2)
				{
					alphacolor[0] = data[1];
					alphacolor[1] = data[3];
					alphacolor[2] = data[5];
				}
				else
				{
					alphacolor[0] = alphacolor[1] = alphacolor[2] = data[1];
				}
				hasalphacolor = 1;
			}
		}
		if (!strncmp(tagid, "IDAT", 4))
		{
			if (!zimagedata)
			{
				zimagedatalen = len;
				zimagedata = (unsigned char*)malloc(len);
				memcpy(zimagedata, data, len);
			}
			else
			{
				zimagedata = (unsigned char*)realloc(zimagedata, zimagedatalen + len);
				memcpy(&zimagedata[zimagedatalen], data, len);
				zimagedatalen += len;
			}
		}
		if (!strncmp(tagid, "tEXt", 4))
		{
			/*int t;
			printf("Image Text: ");
			for(t=0;t<len;t++) {
			if(data[t]>=32 && data[t]<128)
				printf("%c", data[t]);
			else
				printf("?");
			}
			printf("\n");*/
		}
		if (data)
		{
			free(data); data = 0;
		}
	}

	fclose(fi);
	if (!zimagedata || uncompress(imagedata, &imagedatalen, zimagedata, zimagedatalen) != Z_OK)
	{
		printf("Couldn't uncompress %s!\n", sname);
		if (zimagedata)
			free(zimagedata);
		return 0;
	}
	free(zimagedata);

	*destwidth = header.width;
	*destheight = header.height;

	data2 = (unsigned char*)malloc(header.width * header.height * 4);

	if (header.mode == 4)
	{
		int s = 0;
		int pos = 0;
		unsigned char* old = (unsigned char*)malloc(header.width * 2);
		memset(old, 0, header.width * 2);
		*destdata = data2;
		for (unsigned int y = 0; y < header.height; y++)
		{
			int mode = imagedata[pos++]; //filter mode
			unsigned char* src;
			unsigned char* dest;
			int x;
			dest = &data2[(y * header.width) * 4];

			if (header.bpp == 8)
			{
				/* one unsigned char per pixel */
				src = &imagedata[pos];
				pos += header.width * 2;
			}
			else
			{
				/* not implemented yet */
				fprintf(stderr, "ERROR: mode=4 bpp:%d\n", header.bpp);
				free(data2);
				return 0;
			}

			ApplyFilter2(mode, src, old, dest, header.width);
			memcpy(old, dest, header.width * 2);

			for (x = header.width - 1; x >= 0; x--)
			{
				unsigned char gray = dest[x * 2 + 0];
				unsigned char alpha = dest[x * 2 + 1];
				dest[x * 4 + 0] = alpha;
				dest[x * 4 + 1] = gray;
				dest[x * 4 + 2] = gray;
				dest[x * 4 + 3] = gray;
			}
		}
		free(old);
		free(imagedata);
	}
	else if (header.mode == 6 || header.mode == 2)
	{
		int s = 0;
		int pos = 0;
		*destdata = data2;

		unsigned char* firstline = (unsigned char*)malloc(header.width * 4);
		memset(firstline, 0, header.width * 4);
		for (unsigned int y = 0; y < header.height; y++)
		{
			int mode = imagedata[pos++]; //filter mode
			unsigned char* src;
			unsigned char* dest;
			unsigned char* old;
			dest = &data2[(y * header.width) * 4];

			if (header.bpp == 8)
			{
				/* one unsigned char per pixel */
				src = &imagedata[pos];
				pos += header.width * (header.mode == 6 ? 4 : 3);
			}
			else
			{
				/* not implemented yet */
				fprintf(stderr, "ERROR: bpp:%d\n", header.bpp);
				free(data2);
				return 0;
			}

			if (!y)
			{
				old = firstline;
			}
			else
			{
				old = &data2[(y - 1) * header.width * 4];
			}
			if (header.mode == 6)
			{
				InverseFilter32(mode, src, old, dest, header.width);
			}
			else
			{ // header.mode = 2
				ApplyFilter3(mode, src, old, dest, header.width);
				/* replace alpha color */
				if (hasalphacolor)
				{
					for (unsigned int x = 0; x < header.width; x++)
					{
						if (dest[x * 4 + 1] == alphacolor[0] &&
							dest[x * 4 + 2] == alphacolor[1] &&
							dest[x * 4 + 3] == alphacolor[2])
						{
							*(unsigned int*)&dest[x * 4] = 0;
						}
					}
				}
			}
		}
		free(firstline);
		free(imagedata);
	}
	else if (header.mode == 0 || header.mode == 3)
	{
		COL* rgba = 0;
		unsigned char* tmpline = (unsigned char*)malloc(header.width + 1);
		unsigned char* destline = (unsigned char*)malloc(header.width + 1);
		int pos = 0;

		*destdata = data2;

		if (header.mode == 0)
		{ // grayscale palette
			int mult = (0x1ff >> header.bpp);
			palettelen = 1 << header.bpp;
			rgba = (COL*)malloc(palettelen * sizeof(COL));
			for (int i = 0; i < palettelen; i++)
			{
				rgba[i].a = 255;
				rgba[i].r = i * mult;
				rgba[i].g = i * mult;
				rgba[i].b = i * mult;
				if (hasalphacolor)
				{
					if (rgba[i].r == alphacolor[0])
						rgba[i].a = 0;
				}
			}
		}
		else
		{
			if (!palette)
			{
				fprintf(stderr, "Error: No palette found!\n");
				exit(1);
			}
			rgba = (COL*)malloc(palettelen * 4);
			/* 24->32 bit conversion */
			for (int i = 0; i < palettelen; i++)
			{
				rgba[i].r = palette[i * 3 + 0];
				rgba[i].g = palette[i * 3 + 1];
				rgba[i].b = palette[i * 3 + 2];
				if (alphapalette && i < alphapalettelen)
				{
					rgba[i].a = alphapalette[i];
					/*rgba[i].r = ((int)rgba[i].r*rgba[i].a)/255;
					rgba[i].g = ((int)rgba[i].g*rgba[i].a)/255;
					rgba[i].b = ((int)rgba[i].b*rgba[i].a)/255;*/
				}
				else
				{
					rgba[i].a = 255;
				}
				if (hasalphacolor)
				{
					if (rgba[i].r == alphacolor[0] &&
						rgba[i].g == alphacolor[1] &&
						rgba[i].b == alphacolor[2])
						rgba[i].a = 0;
				}
			}
		}

		for (unsigned int y = 0; y < header.height; y++)
		{
			int mode = imagedata[pos++]; //filter mode
			unsigned char* old;
			unsigned char* src;
			src = &imagedata[pos];
			if (header.bpp == 8)
			{
				pos += header.width;
			}
			else
			{
				int s = 0;
				int bitpos = 0;
				unsigned int v = (1 << header.bpp) - 1;
				for (unsigned int x = 0; x < header.width; x++)
				{
					unsigned int r = src[s / 8] << 8 |
						src[s / 8 + 1];

					tmpline[x] = (r >> (16 - header.bpp - (s & 7))) & v;
					s += header.bpp;
				}
				src = tmpline;
				pos += (header.width * header.bpp + 7) / 8;
			}

			if (!y)
			{
				memset(destline, 0, header.width);
				old = &destline[y * header.width];
			}
			else
			{
				old = tmpline;
			}
			ApplyFilter1(mode, src, old, destline, header.width);
			memcpy(tmpline, destline, header.width);
			for (unsigned int x = 0; x < header.width; x++)
			{
				*(COL*)&data2[y * header.width * 4 + x * 4 + 0] = rgba[destline[x]];
			}
		}
		free(tmpline);
		free(destline);
		free(rgba);
		free(imagedata);
	}
	else
	{
		printf("expected PNG mode to be 2, 3 or 6 (is:%d)\n", header.mode);
		return 0;
	}

	return 1;
}

char PNG::HasAlpha(unsigned char* _image, int size)
{
	COL* image = (COL*)_image;
	int t;
	for (t = 0; t < size; t++)
	{
		if (image[t].a != 255)
			return 1;
	}
	return 0;
}

int PNG::CompareColors(const void* _c1, const void* _c2)
{
	colornum_t* c1 = (colornum_t*)_c1;
	colornum_t* c2 = (colornum_t*)_c2;
	return c2->num - c1->num;
}

PNG::colornum_t* PNG::GetColors(COL* image, int size, int* num)
{
	unsigned char* colexists = (unsigned char*)malloc((256 * 256 * 256) / 8);
	memset(colexists, 0, (256 * 256 * 256) / 8);
	int t;
	int count = 0;

	/* find all different colors in the image */
	for (t = 0; t < size; t++)
	{
		int index = (image[t].r) | (image[t].g) << 8 | (image[t].b) << 16;
		if (!(colexists[index / 8] & (1 << (index & 7))))
		{
			count++;
			colexists[index / 8] |= (1 << (index & 7));
		}
	}

	/* now store them in an array */
	colornum_t* colors = (colornum_t*)malloc(sizeof(colornum_t) * count);
	int pos = 0;
	for (t = 0; t < 256 * 256 * 256; t++)
	{
		if (colexists[t / 8] & (1 << (t & 7)))
		{
			colors[pos].color = t;
			colors[pos].num = 0;
			pos++;
		}
	}

	/* next, count how often each color occurs */
	for (t = 0; t < size; t++)
	{
		unsigned int col = (image[t].r) | (image[t].g) << 8 | (image[t].b) << 16;
		int min, max, i, l;
		for (min = 0, max = count, i = count / 2, l = count; i != l; l = i, i = (min + max) / 2)
		{
			// binary search
			if (colors[i].color >= col) max = i;
			else min = i + 1;
		}
		assert(colors[i].color == col);
		colors[i].num++;
	}
	free(colexists);
	*num = count;
	return colors;
}

void PNG::GetOptimalPalette(COL* image, int size, int palettesize, COL* palette)
{
	int num;
	memset(palette, 0, sizeof(COL) * 256);
	colornum_t* colors = GetColors(image, size, &num);

	assert(palettesize <= 256);

	qsort(colors, num, sizeof(colornum_t), CompareColors);

	if (num <= palettesize)
	{
		/* if there are not more than palettesize different colors in
		   the image anyway, we are done */
		int t;
		for (t = 0; t < num; t++)
		{
			palette[t].r = colors[t].color;
			palette[t].g = colors[t].color >> 8;
			palette[t].b = colors[t].color >> 16;
			palette[t].a = 255;
		}
		return;
	}

	if (num > 2048)
	{
		/* if there are too many different colors, pick the ones that
		   occur most often */
		num = 2048;
	}

	colornum_t* centers = (colornum_t*)malloc(sizeof(colornum_t) * palettesize);
	int t;
	for (t = 0; t < palettesize; t++)
	{
		centers[t].color = colors[t].color;
	}
	unsigned char* belongsto = (unsigned char*)malloc(num);
	memset(belongsto, 0, num);
	/* do a k-means clustering on the colors */
	char change = 1;
	int tries = 0;
	while (change)
	{
		if (tries++ >= (palettesize + num) * 2)
		{
			fprintf(stderr, "Warning: didn't find optimal palette\n");
			break;
		}
		change = 0;
		int s, t;
		for (s = 0; s < palettesize; s++)
		{
			centers[s].num = 0;
		}
		for (t = 0; t < num; t++)
		{
			int best = 0x7fffffff;
			int bestpos = 0;
			for (s = 0; s < palettesize; s++)
			{
				int distance = 0;
				distance += abs((int)(centers[s].color >> 0 & 0xff) - (int)(colors[t].color >> 0 & 0xff));
				distance += abs((int)(centers[s].color >> 8 & 0xff) - (int)(colors[t].color >> 8 & 0xff));
				distance += abs((int)(centers[s].color >> 16 & 0xff) - (int)(colors[t].color >> 16 & 0xff));
				distance *= colors[t].num;
				if (distance < best)
				{
					best = distance;
					bestpos = s;
				}
			}
			if (bestpos != belongsto[t])
				change = 1;
			belongsto[t] = bestpos;
		}
		for (s = 0; s < palettesize; s++)
		{
			int r = 0;
			int g = 0;
			int b = 0;
			int count = 0;
			for (t = 0; t < num; t++)
			{
				if (belongsto[t] == s)
				{
					r += ((colors[t].color >> 0) & 0xff) * colors[t].num;
					g += ((colors[t].color >> 8) & 0xff) * colors[t].num;
					b += ((colors[t].color >> 16) & 0xff) * colors[t].num;
					count += colors[t].num;
				}
			}
			if (!count)
			{
				int random = rand() % num;
				centers[s].color = colors[random].color;
				centers[s].num = 0;
				change = 1;
			}
			else
			{
				r /= count;
				g /= count;
				b /= count;
				centers[s].color = r | g << 8 | b << 16;
				centers[s].num = count;
			}
		}
	}
	free(belongsto);
	free(colors);
	for (t = 0; t < palettesize; t++)
	{
		palette[t].r = centers[t].color;
		palette[t].g = centers[t].color >> 8;
		palette[t].b = centers[t].color >> 16;
		palette[t].a = 255;
	}
	free(centers);
}

void PNG::QuantizeImage(unsigned char* _image, int size, int numcolors, unsigned char** newimage, COL* palette)
{
	COL* image = (COL*)_image;
	GetOptimalPalette(image, size, numcolors, palette);
	*newimage = (unsigned char*)malloc(size);
	int t;
	for (t = 0; t < size; t++)
	{
		int best = 0x7fffffff;
		int bestcol = 0;
		int s;
		for (s = 0; s < numcolors; s++)
		{
			int distance = 0;
			distance += (int)pow((palette[s].r) - (image[t].r), 2) * 5;
			distance += (int)pow((palette[s].g) - (image[t].g), 2) * 6;
			distance += (int)pow((palette[s].b) - (image[t].b), 2) * 4;
			if (distance < best)
			{
				best = distance;
				bestcol = s;
			}
		}
		(*newimage)[t] = bestcol;
	}
}

static unsigned int mycrc32;

static unsigned int* crc32_table = nullptr;
void PNG::MakeCRC32Table()
{
	int t;
	if (crc32_table)
		return;
	crc32_table = (unsigned int*)malloc(1024);

	for (t = 0; t < 256; t++)
	{
		unsigned int c = t;
		int s;
		for (s = 0; s < 8; s++)
		{
			c = (0xedb88320L * (c & 1)) ^ (c >> 1);
		}
		crc32_table[t] = c;
	}
}

void PNG::WriteByte(FILE* fi, unsigned char byte)
{
	fwrite(&byte, 1, 1, fi);
	mycrc32 = crc32_table[(mycrc32 ^ byte) & 0xff] ^ (mycrc32 >> 8);
}

long PNG::StartChunk(FILE* fi, char* type, int len)
{
	unsigned char mytype[4] = { 0,0,0,0 };
	unsigned char mylen[4];
	long filepos;
	mylen[0] = len >> 24;
	mylen[1] = len >> 16;
	mylen[2] = len >> 8;
	mylen[3] = len;
	memcpy(mytype, type, strlen(type));
	filepos = ftell(fi);
	fwrite(&mylen, 4, 1, fi);
	mycrc32 = 0xffffffff;
	WriteByte(fi, mytype[0]);
	WriteByte(fi, mytype[1]);
	WriteByte(fi, mytype[2]);
	WriteByte(fi, mytype[3]);
	return filepos;
}

void PNG::PatchLen(FILE* fi, int pos, int len)
{
	unsigned char mylen[4];

	mylen[0] = len >> 24;
	mylen[1] = len >> 16;
	mylen[2] = len >> 8;
	mylen[3] = len;
	fseek(fi, pos, SEEK_SET);
	fwrite(&mylen, 4, 1, fi);
	fseek(fi, 0, SEEK_END);
}

void PNG::WriteBytes(FILE* fi, unsigned char* bytes, int len)
{
	int t;
	for (t = 0; t < len; t++)
		WriteByte(fi, bytes[t]);
}

void PNG::WriteDword(FILE* fi, unsigned int dword)
{
	WriteByte(fi, dword >> 24);
	WriteByte(fi, dword >> 16);
	WriteByte(fi, dword >> 8);
	WriteByte(fi, dword);
}

void PNG::EndChunk(FILE* fi)
{
	unsigned int tmp = mycrc32 ^ 0xffffffff;
	unsigned char tmp2[4];
	tmp2[0] = tmp >> 24;
	tmp2[1] = tmp >> 16;
	tmp2[2] = tmp >> 8;
	tmp2[3] = tmp;
	fwrite(&tmp2, 4, 1, fi);
}

#define ZLIB_BUFFER_SIZE 16384

long PNG::CompressLine(z_stream* zs, Bytef* line, int len, FILE* fi)
{
	long size = 0;
	zs->next_in = line;
	zs->avail_in = len;

	while (1)
	{
		int ret = deflate(zs, Z_NO_FLUSH);
		if (ret != Z_OK)
		{
			fprintf(stderr, "error in deflate(): %s", zs->msg ? zs->msg : "unknown");
			return 0;
		}
		if (zs->avail_out != ZLIB_BUFFER_SIZE)
		{
			int consumed = ZLIB_BUFFER_SIZE - zs->avail_out;
			size += consumed;
			WriteBytes(fi, zs->next_out - consumed, consumed);
			zs->next_out = zs->next_out - consumed;
			zs->avail_out = ZLIB_BUFFER_SIZE;
		}
		if (!zs->avail_in)
		{
			break;
		}
	}
	return size;
}

int PNG::TestLine(z_stream* zs_orig, Bytef* line, int linelen)
{
	z_stream zs;
	int ret = deflateCopy(&zs, zs_orig);
	if (ret != Z_OK)
	{
		fprintf(stderr, "Couldn't copy stream\n");
		return 0;
	}

	zs.next_in = line;
	zs.avail_in = linelen;

	long size = 0;

	int mode = Z_SYNC_FLUSH;
	while (1)
	{
		int ret = deflate(&zs, mode);
		if (ret != Z_OK && ret != Z_STREAM_END)
		{
			fprintf(stderr, "error in deflate(): %s (mode %s, %d unsigned chars remaining)\n", zs.msg ? zs.msg : "unknown",
				mode == Z_SYNC_FLUSH ? "Z_SYNC_FLUSH" : "Z_FINISH", zs.avail_in);
			return 0;
		}
		if (zs.avail_out != ZLIB_BUFFER_SIZE)
		{
			int consumed = ZLIB_BUFFER_SIZE - zs.avail_out;
			size += consumed;
			zs.next_out = zs.next_out - consumed;
			zs.avail_out = ZLIB_BUFFER_SIZE;
		}
		if (ret == Z_STREAM_END)
		{
			break;
		}
		if (!zs.avail_in)
		{
			mode = Z_FINISH;
		}
	}
	ret = deflateEnd(&zs);
	if (ret != Z_OK)
	{
		fprintf(stderr, "error in deflateEnd(): %s\n", zs.msg ? zs.msg : "unknown");
		return 0;
	}
	return size;
}

int PNG::FinishZlib(z_stream* zs, FILE* fi)
{
	int size = 0;
	int ret;
	while (1)
	{
		ret = deflate(zs, Z_FINISH);
		if (ret != Z_OK &&
			ret != Z_STREAM_END)
		{
			fprintf(stderr, "error in deflate(finish): %s\n", zs->msg ? zs->msg : "unknown");
			return 0;
		}

		if (zs->avail_out != ZLIB_BUFFER_SIZE)
		{
			int consumed = ZLIB_BUFFER_SIZE - zs->avail_out;
			size += consumed;
			WriteBytes(fi, zs->next_out - consumed, consumed);
			zs->next_out = zs->next_out - consumed;
			zs->avail_out = ZLIB_BUFFER_SIZE;
		}
		if (ret == Z_STREAM_END)
		{
			break;
		}
	}
	ret = deflateEnd(zs);
	if (ret != Z_OK)
	{
		fprintf(stderr, "error in deflateEnd(): %s\n", zs->msg ? zs->msg : "unknown");
		return 0;
	}
	return size;
}

unsigned int PNG::ColorHash(COL* col)
{
	unsigned int col32 = *(unsigned int*)col;
	unsigned int hash = (col32 >> 17) ^ col32;
	hash ^= ((hash >> 8) + 1) ^ hash;
	return hash;
}

int PNG::GetNumberOfPaletteEntries(COL* img, unsigned width, unsigned height, COL* palette, char* has_alpha)
{
	int len = width * height;
	int t;
	int palsize = 0;
	int size[256];
	int palette_overflow = 0;
	unsigned int lastcol32 = 0;

	memset(size, 0, sizeof(size));

	unsigned int* pal = (unsigned int*)malloc(65536 * sizeof(unsigned int));
	int* count = (int*)malloc(65536 * sizeof(int));

	assert(sizeof(COL) == sizeof(unsigned int));
	assert(width && height);

	lastcol32 = (*(unsigned int*)&img[0]) ^ 0xffffffff; // don't match

	for (t = 0; t < len; t++)
	{
		unsigned int col32 = *(unsigned int*)&img[t];
		if (col32 == lastcol32)
			continue;

		if (img[t].a != 255)
			*has_alpha = 1;
		int i;

		unsigned int hash = ColorHash(&img[t]) & 255;

		int csize = size[hash];
		unsigned int* cpal = &pal[hash * 256];
		int* ccount = &count[hash * 256];
		for (i = 0; i < csize; i++)
		{
			if (col32 == cpal[i])
			{
				ccount[i]++;
				break;
			}
		}
		if (i == csize)
		{
			if (palsize == 256)
			{
				palette_overflow = 1;
				break;
			}
			count[size[hash]] = 1;
			cpal[size[hash]++] = col32;
			palsize++;
		}
		lastcol32 = col32;
	}
	if (palette_overflow)
	{
		free(pal);
		*has_alpha = 1;
		return width * height;
	}
	if (palette)
	{
		int i = 0;
		int occurences[256];
		for (t = 0; t < 256; t++)
		{
			int s;
			int csize = size[t];
			unsigned int* cpal = &pal[t * 256];
			int* ccount = &count[t * 256];
			for (s = 0; s < csize; s++)
			{
				occurences[i] = ccount[s];
				palette[i++] = *(COL*)(&cpal[s]);
			}
		}
		assert(i == palsize);
		int j;
		for (i = 0; i < palsize - 1; i++)
		{
			for (j = i + 1; j < palsize; j++)
			{
				if (occurences[j] < occurences[i])
				{
					int o = occurences[i];
					COL c = palette[i];
					occurences[i] = occurences[j];
					palette[i] = palette[j];
					occurences[j] = o;
					palette[j] = c;
				}
			}
		}
	}
	free(pal);
	free(count);
	return palsize;
}

void PNG::MapToPalette(COL* src, unsigned char* dest, int size, COL* palette, int palette_size)
{
	int t;
	int palette_hash[1024];
	memset(palette_hash, 0, sizeof(int) * 1024);

	for (t = 0; t < palette_size; t++)
	{
		unsigned int hash = ColorHash(&palette[t]) & 1023;
		while (palette_hash[hash])
			hash = (hash + 1) & 1023;
		palette_hash[hash] = t;
	}

	for (t = 0; t < size; t++)
	{
		unsigned int hash = ColorHash(&src[t]);
		int index = 0;
		while (1)
		{
			hash &= 1023;
			index = palette_hash[hash];
			if (!memcmp(&palette[index], &src[t], sizeof(COL)))
				break;
			hash++;
		}
		dest[t] = palette_hash[hash];
	}
}

int PNG::ApplySpecificFilter8(int filtermode, unsigned char* dest, unsigned char* src, unsigned width)
{
	int pos2 = 0;
	int pos = 0;
	int srcwidth = width;
	if (filtermode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[pos2++] = src[pos++]; //alpha
		}
	}
	else if (filtermode == 1)
	{
		/* x difference filter */
		dest[pos2++] = src[pos++];
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos] - src[pos - 1];
			pos++;
		}
	}
	else if (filtermode == 2)
	{
		/* y difference filter */
		for (unsigned int x = 0; x < width; x++)
		{
			dest[pos2++] = src[pos + 0] - src[pos - srcwidth + 0]; //alpha
			pos++;
		}
	}
	else if (filtermode == 3)
	{
		dest[pos2++] = src[pos + 0] - src[pos - srcwidth + 0] / 2;
		pos++;
		/* x+y difference filter */
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos + 0] - (src[pos - 1 + 0] + src[pos - srcwidth + 0]) / 2; //alpha
			pos++;
		}
	}
	else if (filtermode == 4)
	{
		dest[pos2++] = src[pos + 0] - PaethPredictor(0, src[pos - srcwidth + 0], 0);
		pos++;
		/* paeth difference filter */
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos + 0] - PaethPredictor(src[pos - 1 + 0], src[pos - srcwidth + 0], src[pos - 1 - srcwidth + 0]);
			pos++;
		}
	}
	return filtermode;
}

int PNG::ApplySpecificFilter32(int filtermode, unsigned char* dest, unsigned char* src, unsigned width)
{
	int pos2 = 0;
	int pos = 0;
	int srcwidth = width * 4;

	if (filtermode == 0)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			dest[pos2++] = src[pos + 1];
			dest[pos2++] = src[pos + 2];
			dest[pos2++] = src[pos + 3];
			dest[pos2++] = src[pos + 0]; //alpha
			pos += 4;
		}
	}
	else if (filtermode == 1)
	{
		/* x difference filter */
		dest[pos2++] = src[pos + 1];
		dest[pos2++] = src[pos + 2];
		dest[pos2++] = src[pos + 3];
		dest[pos2++] = src[pos + 0];
		pos += 4;
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos + 1] - src[pos - 4 + 1];
			dest[pos2++] = src[pos + 2] - src[pos - 4 + 2];
			dest[pos2++] = src[pos + 3] - src[pos - 4 + 3];
			dest[pos2++] = src[pos + 0] - src[pos - 4 + 0]; //alpha
			pos += 4;
		}
	}
	else if (filtermode == 2)
	{
		/* y difference filter */
		for (unsigned int x = 0; x < width; x++)
		{
			dest[pos2++] = src[pos + 1] - src[pos - srcwidth + 1];
			dest[pos2++] = src[pos + 2] - src[pos - srcwidth + 2];
			dest[pos2++] = src[pos + 3] - src[pos - srcwidth + 3];
			dest[pos2++] = src[pos + 0] - src[pos - srcwidth + 0]; //alpha
			pos += 4;
		}
	}
	else if (filtermode == 3)
	{
		dest[pos2++] = src[pos + 1] - src[pos - srcwidth + 1] / 2;
		dest[pos2++] = src[pos + 2] - src[pos - srcwidth + 2] / 2;
		dest[pos2++] = src[pos + 3] - src[pos - srcwidth + 3] / 2;
		dest[pos2++] = src[pos + 0] - src[pos - srcwidth + 0] / 2;
		pos += 4;
		/* x+y difference filter */
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos + 1] - (src[pos - 4 + 1] + src[pos - srcwidth + 1]) / 2;
			dest[pos2++] = src[pos + 2] - (src[pos - 4 + 2] + src[pos - srcwidth + 2]) / 2;
			dest[pos2++] = src[pos + 3] - (src[pos - 4 + 3] + src[pos - srcwidth + 3]) / 2;
			dest[pos2++] = src[pos + 0] - (src[pos - 4 + 0] + src[pos - srcwidth + 0]) / 2; //alpha
			pos += 4;
		}
	}
	else if (filtermode == 4)
	{
		dest[pos2++] = src[pos + 1] - PaethPredictor(0, src[pos - srcwidth + 1], 0);
		dest[pos2++] = src[pos + 2] - PaethPredictor(0, src[pos - srcwidth + 2], 0);
		dest[pos2++] = src[pos + 3] - PaethPredictor(0, src[pos - srcwidth + 3], 0);
		dest[pos2++] = src[pos + 0] - PaethPredictor(0, src[pos - srcwidth + 0], 0);
		pos += 4;
		/* paeth difference filter */
		for (unsigned int x = 1; x < width; x++)
		{
			dest[pos2++] = src[pos + 1] - PaethPredictor(src[pos - 4 + 1], src[pos - srcwidth + 1], src[pos - 4 - srcwidth + 1]);
			dest[pos2++] = src[pos + 2] - PaethPredictor(src[pos - 4 + 2], src[pos - srcwidth + 2], src[pos - 4 - srcwidth + 2]);
			dest[pos2++] = src[pos + 3] - PaethPredictor(src[pos - 4 + 3], src[pos - srcwidth + 3], src[pos - 4 - srcwidth + 3]);
			dest[pos2++] = src[pos + 0] - PaethPredictor(src[pos - 4 + 0], src[pos - srcwidth + 0], src[pos - 4 - srcwidth + 0]);
			pos += 4;
		}
	}
	return filtermode;
}

static int* num_bits_table = 0;

void PNG::MakeNumBitsTable()
{
	if (num_bits_table) return;
	num_bits_table = (int*)malloc(sizeof(num_bits_table[0]) * 256);
	int t;
	for (t = 0; t < 256; t++)
	{
		int bits = 0;
		int v = t;
		while (v)
		{
			bits++;
			v &= v - 1;
		}
		num_bits_table[t] = bits;
	}
}

int PNG::FindBestFilter(unsigned char* src, unsigned width, int bpp, int y)
{
	MakeNumBitsTable();

	int num_filters = y > 0 ? 5 : 2; //don't apply y-direction filter in first line

	int bytes_per_pixel = bpp >> 3;
	int w = width * bytes_per_pixel;
	int back_x = bytes_per_pixel;
	int back_y = y ? width * bytes_per_pixel : 0;

	unsigned char* pairs[5];
	pairs[0] = (unsigned char*)calloc(1, 8192);
	pairs[1] = (unsigned char*)calloc(1, 8192);
	pairs[2] = (unsigned char*)calloc(1, 8192);
	pairs[3] = (unsigned char*)calloc(1, 8192);
	pairs[4] = (unsigned char*)calloc(1, 8192);

	unsigned char old[5];
	int l = bytes_per_pixel - 1;
	old[0] = src[l];
	old[1] = src[l];
	old[2] = src[l] - src[l - back_y];
	old[3] = src[l] - src[l - back_y];
	old[4] = src[l] - PaethPredictor(0, src[l - back_y], 0);

	int different_pairs[5] = { 0,0,0,0,0 };

	int x;
	for (x = bytes_per_pixel; x < w; x++)
	{
		unsigned char dest[5];
		dest[0] = src[x];
		dest[1] = src[x] - src[x - back_x];
		dest[2] = src[x] - src[x - back_y];
		dest[3] = src[x] - (src[x - back_x] + src[x - back_y]) / 2;
		dest[4] = src[x] - PaethPredictor(src[x - back_x], src[x - back_y], src[x - back_x - back_y]);

		int i;
		for (i = 0; i < 5; i++)
		{
			int v = dest[i] << 8 | old[i];
			int p = v >> 3;
			int b = 1 << (v & 7);
			if (!pairs[i][p] & b)
			{
				pairs[i][p] |= b;
				different_pairs[i]++;
			}
		}
		memcpy(old, dest, sizeof(old));
	}
	int f;
	int best_nr = 0;
	int best_energy = INT_MAX;
	for (f = 0; f < num_filters; f++)
	{
		int energy = different_pairs[f];
		if (energy < best_energy)
		{
			best_nr = f;
			best_energy = energy;
		}
	}
	free(pairs[0]);
	free(pairs[1]);
	free(pairs[2]);
	free(pairs[3]);
	free(pairs[4]);
	return best_nr;
}

int PNG::ApplyFilter(unsigned char* dest, unsigned char* src, unsigned width, int y, int bpp)
{
	int best_nr = FindBestFilter(src, width, bpp, y);

	if (bpp == 8)
		ApplySpecificFilter8(best_nr, dest, src, width);
	else
		ApplySpecificFilter32(best_nr, dest, src, width);
	return best_nr;
}

int PNG::ApplyFilter8(unsigned char* dest, unsigned char* src, unsigned width, int y)
{
	return ApplyFilter(dest, src, width, y, 8);
}

int PNG::ApplyFilter32(unsigned char* dest, unsigned char* src, unsigned width, int y)
{
	return ApplyFilter(dest, src, width, y, 32);
}

void PNG::WritePaletteBased2(const char* filename, unsigned char* data, unsigned width, unsigned height, int numcolors, int compression)
{
	FILE* fi;
	int t;
	unsigned char format;
	unsigned char* data2 = 0;
	unsigned char head[] = { 137,80,78,71,13,10,26,10 }; // PNG header
	int cols;
	char alpha = 1;
	int pos = 0;
	int bpp;
	int ret;
	char has_alpha = 0;
	z_stream zs;
	COL palette[256];

	MakeCRC32Table();

	if (numcolors > 256)
	{
		bpp = 32;
		cols = 0;
		format = 5;
	}
	else if (!numcolors)
	{
		int num = GetNumberOfPaletteEntries((COL*)data, width, height, palette, &has_alpha);
		if (num <= 255)
		{
			//printf("image has %d different colors (alpha=%d)\n", num, has_alpha);
			data2 = (unsigned char*)malloc(width * height);
			MapToPalette((COL*)data, data2, width * height, palette, num);
			data = data2;
			bpp = 8;
			cols = num;
			format = 3;
		}
		else
		{
			bpp = 32;
			cols = 0;
			format = 5;
		}
	}
	else
	{
		bpp = 8;
		cols = numcolors;
		format = 3;
		QuantizeImage(data, width * height, numcolors, &data, palette);
	}

	fopen_s(&fi, filename, "wb");
	if (!fi)
	{
		perror("open");
		return;
	}
	fwrite(head, sizeof(head), 1, fi);

	StartChunk(fi, (char*)"IHDR", 13);
	WriteDword(fi, width);
	WriteDword(fi, height);
	WriteByte(fi, 8);
	if (format == 3)
		WriteByte(fi, 3); //indexed
	else if (format == 5 && alpha == 0)
		WriteByte(fi, 2); //rgb
	else if (format == 5 && alpha == 1)
		WriteByte(fi, 6); //rgba
	else return;

	WriteByte(fi, 0); //compression mode
	WriteByte(fi, 0); //filter mode
	WriteByte(fi, 0); //interlace mode
	EndChunk(fi);

	if (format == 3)
	{
		StartChunk(fi, (char*)"PLTE", cols * 3);
		for (t = 0; t < cols; t++)
		{
			WriteByte(fi, palette[t].r);
			WriteByte(fi, palette[t].g);
			WriteByte(fi, palette[t].b);
		}
		EndChunk(fi);

		if (has_alpha)
		{
			StartChunk(fi, (char*)"tRNS", cols);
			for (t = 0; t < cols; t++)
			{
				WriteByte(fi, palette[t].a);
			}
			EndChunk(fi);
		}
	}

	long idatpos = StartChunk(fi, (char*)"IDAT", 0);

	memset(&zs, 0, sizeof(z_stream));
	Bytef* writebuf = (Bytef*)malloc(ZLIB_BUFFER_SIZE);
	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;
	zs.next_out = writebuf;
	zs.avail_out = ZLIB_BUFFER_SIZE;
	ret = deflateInit(&zs, compression);
	if (ret != Z_OK)
	{
		fprintf(stderr, "error in deflateInit(): %s", zs.msg ? zs.msg : "unknown");
		return;
	}

	long idatsize = 0;
	{
		int bypp = bpp / 8;
		unsigned srcwidth = width * bypp;
		unsigned linelen = 1 + srcwidth;
		if (bypp == 2)
			linelen = 1 + ((srcwidth + 1) & ~1);
		else if (bypp == 3)
			linelen = 1 + ((srcwidth + 2) / 3) * 3;
		else if (bypp == 4)
			linelen = 1 + ((srcwidth + 3) & ~3);
		unsigned char* line = (unsigned char*)malloc(linelen);
		memset(line, 0, linelen);

		for (unsigned int y = 0; y < height; y++)
		{
			if (bpp == 8)
				line[0] = ApplyFilter8(line + 1, &data[y * srcwidth], width, y);
			else
				line[0] = ApplyFilter32(line + 1, &data[y * srcwidth], width, y);

			idatsize += CompressLine(&zs, line, linelen, fi);
		}

		free(line);
	}

	idatsize += FinishZlib(&zs, fi);
	PatchLen(fi, idatpos, idatsize);
	EndChunk(fi);

	StartChunk(fi, (char*)"IEND", 0);
	EndChunk(fi);

	free(writebuf);
	if (data2)
		free(data2);
	fclose(fi);
}

void PNG::WritePaletteBased(const char* filename, unsigned char* data, unsigned width, unsigned height, int numcolors)
{
	WritePaletteBased2(filename, data, width, height, numcolors, Z_BEST_COMPRESSION);
}

void PNG::Write(const char* filename, unsigned char* data, unsigned width, unsigned height)
{
	WritePaletteBased2(filename, data, width, height, 0, Z_BEST_COMPRESSION);
}

void PNG::WriteQuick(const char* filename, unsigned char* data, unsigned width, unsigned height)
{
	WritePaletteBased2(filename, data, width, height, 257, Z_NO_COMPRESSION);
}

void PNG::WritePaletteBased2(const char* filename, unsigned char* data, unsigned width, unsigned height)
{
	WritePaletteBased2(filename, data, width, height, 256, Z_BEST_COMPRESSION);
}
