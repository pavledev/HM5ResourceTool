#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <format>
#include <winerror.h>
#include <numbers>
#include "zlib/zlib.h"
#include "SwfFileReader.hpp"
#include "Matrix.hpp"
#include "SwfPlaceObject.hpp"
#include "RGBA.hpp"
#include "Swf.hpp"
#include "RenderBuffer.hpp"
#include "Shape.hpp"
#include "Shape2.hpp"
#include "Gradient.hpp"
#include "SwfFont.hpp"
#include "Character.hpp"
#include "SPoint.hpp"
#include "RenderPoint.hpp"
#include "State.hpp"
#include "Bitmap.hpp"
#include "RenderLine.hpp"
#include "Font.hpp"
#include "SwfGlyph.hpp"
#include "FillStyle.hpp"
#include "LineStyle.hpp"
#include "ShapeLine.hpp"
#include "SwfLayout.hpp"
#include "SwfKerning.hpp"
#include "AlignZone.hpp"
#include "FontUsage.hpp"
#include "SwfGlyphPair.hpp"
#include "TextCallbackBlock.hpp"
#include "Layer.hpp"

#define ST_END                  0
#define ST_SHOWFRAME            1
#define ST_DEFINESHAPE          2
#define ST_FREECHARACTER        3
#define ST_PLACEOBJECT          4
#define ST_REMOVEOBJECT         5
#define ST_DEFINEBITS           6
#define ST_DEFINEBITSJPEG       6
#define ST_DEFINEBUTTON         7
#define ST_JPEGTABLES           8
#define ST_SETBACKGROUNDCOLOR   9
#define ST_DEFINEFONT           10
#define ST_DEFINETEXT           11
#define ST_DOACTION             12
#define ST_DEFINEFONTINFO       13
#define ST_DEFINESOUND          14 /* Event sound tags. */
#define ST_STARTSOUND           15
#define ST_DEFINEBUTTONSOUND    17
#define ST_SOUNDSTREAMHEAD      18
#define ST_SOUNDSTREAMBLOCK     19
#define ST_DEFINEBITSLOSSLESS   20 /* A bitmap using lossless zlib compression. */
#define ST_DEFINEBITSJPEG2      21 /* A bitmap using an internal JPEG compression table. */
#define ST_DEFINESHAPE2         22
#define ST_DEFINEBUTTONCXFORM   23
#define ST_PROTECT              24 /* This file should not be importable for editing. */
#define ST_PLACEOBJECT2         26 /* The new style place w/ alpha color transform and name. */
#define ST_REMOVEOBJECT2        28 /* A more compact remove object that omits the character tag (just depth). */
#define ST_FREEALL              31 /* ? */
#define ST_DEFINESHAPE3         32 /* A shape V3 includes alpha values. */
#define ST_DEFINETEXT2          33 /* A text V2 includes alpha values. */
#define ST_DEFINEBUTTON2        34 /* A button V2 includes color transform, alpha and multiple actions */
#define ST_DEFINEBITSJPEG3      35 /* A JPEG bitmap with alpha info. */
#define ST_DEFINEBITSLOSSLESS2  36 /* A lossless bitmap with alpha info. */
#define ST_DEFINEEDITTEXT       37
#define ST_DEFINEMOVIE          38
#define ST_DEFINESPRITE         39 /* Define a sequence of tags that describe the behavior of a sprite. */
#define ST_NAMECHARACTER        40 /* Name a character definition, character id and a string, (used for buttons, bitmaps, sprites and sounds). */
#define ST_SERIALNUMBER         41
#define ST_GENERATORTEXT        42 /* contains an id */
#define ST_FRAMELABEL           43 /* A string label for the current frame. */
#define ST_SOUNDSTREAMHEAD2     45 /* For lossless streaming sound, should not have needed this... */
#define ST_DEFINEMORPHSHAPE     46 /* A morph shape definition */
#define ST_DEFINEFONT2          48
#define ST_TEMPLATECOMMAND      49
#define ST_GENERATOR3           51
#define ST_EXTERNALFONT         52
#define ST_EXPORTASSETS		56
#define ST_IMPORTASSETS		57
#define ST_ENABLEDEBUGGER	58
#define ST_DOINITACTION         59
#define ST_DEFINEVIDEOSTREAM    60
#define ST_VIDEOFRAME           61
#define ST_DEFINEFONTINFO2	62
#define ST_MX4			63 /*(?) */
#define ST_ENABLEDEBUGGER2      64 /* version 8 */
#define ST_SCRIPTLIMITS		65 /* version 7- u16 maxrecursedepth, u16 scripttimeoutseconds */
#define ST_SETTABINDEX		66 /* version 7- u16 depth(!), u16 tab order value */
#define ST_FILEATTRIBUTES	69 /* version 8 (required)- */
#define ST_PLACEOBJECT3		70 /* version 8 */
#define ST_IMPORTASSETS2	71 /* version 8 */
#define ST_RAWABC               72 /* version 9, used by flex */
#define ST_DEFINEFONTALIGNZONES 73 /* version 8 */
#define ST_CSMTEXTSETTINGS	74 /* version 8 */
#define ST_DEFINEFONT3		75 /* version 8 */
#define ST_SYMBOLCLASS		76 /* version 9 */
#define ST_METADATA		77 /* version 8 */
#define ST_DEFINESCALINGGRID    78 /* version 8 */
#define ST_DOABC		82 /* version 9 */
#define ST_DEFINESHAPE4		83 /* version 8 */
#define ST_DEFINEMORPHSHAPE2    84 /* version 8 */
#define ST_SCENEDESCRIPTION	86 /* version 9 */
#define ST_DEFINEBINARY		87 /* version 9 */
#define ST_DEFINEFONTNAME	88 /* version 9 */

/* custom tags- only valid for swftools */
#define ST_REFLEX              777 /* to identify generator software */
#define ST_GLYPHNAMES          778

static int swf_spritetagids[] =
{
    ST_SHOWFRAME,
    ST_PLACEOBJECT,
    ST_PLACEOBJECT2,
    ST_PLACEOBJECT3,
    ST_REMOVEOBJECT,
    ST_REMOVEOBJECT2,
    ST_DOACTION,
    ST_DOABC,
    ST_STARTSOUND,
    ST_FRAMELABEL,
    ST_SOUNDSTREAMHEAD,
    ST_SOUNDSTREAMHEAD2,
    ST_SOUNDSTREAMBLOCK,
    ST_END,
    -1
};

static int swf_definingtagids[] =
{
    ST_DEFINESHAPE,
    ST_DEFINESHAPE2,
    ST_DEFINESHAPE3,
    ST_DEFINESHAPE4,
    ST_DEFINEMORPHSHAPE,
    ST_DEFINEMORPHSHAPE2,
    ST_DEFINEFONT,
    ST_DEFINEFONT2,
    ST_DEFINEFONT3,
    ST_DEFINETEXT,
    ST_DEFINETEXT2,
    ST_DEFINEEDITTEXT,
    ST_DEFINEBITS,
    ST_DEFINEBITSJPEG2,
    ST_DEFINEBITSJPEG3,
    ST_DEFINEBITSLOSSLESS,
    ST_DEFINEBITSLOSSLESS2,
    ST_DEFINEMOVIE,
    ST_DEFINESPRITE,
    ST_DEFINEBUTTON,
    ST_DEFINEBUTTON2,
    ST_DEFINESOUND,
    ST_DEFINEVIDEOSTREAM,
    ST_DEFINEBINARY,
    -1
};

#define SF_MOVETO       0x01
#define SF_FILL0        0x02
#define SF_FILL1        0x04
#define SF_LINE         0x08
#define SF_NEWSTYLE     0x10

#define FEDTJ_PRINT  0x01
#define FEDTJ_MODIFY 0x02
#define FEDTJ_CALLBACK 0x04

#define TF_TEXTCONTROL  0x80
#define TF_HASFONT      0x08
#define TF_HASCOLOR     0x04
#define TF_HASYOFFSET   0x02
#define TF_HASXOFFSET   0x01

#define FF_WIDECODES    0x01
#define FF_BOLD         0x02
#define FF_ITALIC       0x04
#define FF_ANSI         0x08
#define FF_SHIFTJIS     0x10
#define FF_UNICODE      0x20

#define FF2_BOLD         0x01
#define FF2_ITALIC       0x02
#define FF2_WIDECODES    0x04
#define FF2_WIDEOFFSETS  0x08
#define FF2_ANSI         0x10
#define FF2_UNICODE      0x20
#define FF2_SHIFTJIS     0x40
#define FF2_LAYOUT	 0x80

#define FONT_STYLE_BOLD 1
#define FONT_STYLE_ITALIC 2
#define FONT_ENCODING_UNICODE 1
#define FONT_ENCODING_ANSI 2
#define FONT_ENCODING_SHIFTJIS 4

#define FONTALIGN_THIN 0 
#define FONTALIGN_MEDIUM 1
#define FONTALIGN_THICK 2

#define SCOORD_MAX 0x7fffffffl
#define SCOORD_MIN 0 - 0x80000000l

#define FILL_SOLID      0x00
#define FILL_LINEAR     0x10  // Gradient
#define FILL_RADIAL     0x12
#define FILL_TILED      0x40  // Bitmap
#define FILL_CLIPPED    0x41

#define PF_MOVE         0x01
#define PF_CHAR         0x02
#define PF_MATRIX       0x04
#define PF_CXFORM       0x08
#define PF_RATIO        0x10
#define PF_NAME         0x20
#define PF_CLIPDEPTH    0x40
#define PF_ACTIONEVENT  0x80

#define PF2_FILTERS      0x01
#define PF2_BLENDMODE    0x02
#define PF2_ASBITMAP     0x04

/* set this to 0.777777 or something if the "both fillStyles set while not inside shape"
   problem appears to often */
#define CUT 0.77887789

#define INT(x) ((int)((x)+16)-16)

#define ET_HASTEXT 32768
#define ET_WORDWRAP 16384
#define ET_MULTILINE 8192
#define ET_PASSWORD 4096
#define ET_READONLY 2048
#define ET_HASTEXTCOLOR 1024
#define ET_HASMAXLENGTH 512
#define ET_HASFONT 256
#define ET_X3 128
#define ET_AUTOSIZE 64 /* MX */
#define ET_HASLAYOUT 32
#define ET_NOSELECT 16
#define ET_BORDER 8
#define ET_X1 4
#define ET_HTML 2 /* MX? */
#define ET_USEOUTLINES 1

#define ET_ALIGN_LEFT 0
#define ET_ALIGN_RIGHT  1
#define ET_ALIGN_CENTER 2
#define ET_ALIGN_JUSTIFY 3

class SWFRender
{
public:
    inline static RGBA colorRed = RGBA(255, 255, 0, 0);
    inline static RGBA colorWhite = RGBA(255, 255, 255, 255);
    inline static RGBA colorBlack = RGBA(255, 0, 0, 0);

    static void ReadSWF(Swf* swf, const std::string& filePath);
    static void ReadSWF(Swf* swf, const void* data, const unsigned int dataSize);
    static void ReadSWF(Swf* swf, SwfLib::SwfFileReader& swfFileReader);
    static void DecompressZlib(SwfLib::SwfFileReader& swfFileReader, std::vector<unsigned char>& target, unsigned int fileLength);
    static Tag* ReadTag(SwfLib::SwfFileReader& swfFileReader, Tag* prev);
    static void Init(RenderBuffer* buf, int posx, int posy, int width, int height, int antialize, int multiply);
    static Tag* InsertTag(Tag* after, unsigned short id);
    static void RenderSWF(RenderBuffer* buf, Swf* swf);
    static RGBA* Render(RenderBuffer* dest);
    static void OptimizeTagOrder(Swf* swf);
    static void FoldAll(Swf* swf);
    static void UnFoldAll(Swf* swf);
    static void FoldSprite(Tag* t);
    static void UnFoldSprite(Tag* t);
    static unsigned char GetU8(Tag* t);
    static short GetS16(Tag* tag);
    static unsigned short GetU16(Tag* t);
    static unsigned int GetU32(Tag* t);
    static int GetBlock(Tag* t, unsigned char* b, int l);
    static int SetU8(Tag* t, unsigned char v);
    static int SetU16(Tag* t, unsigned short v);
    static int SetU32(Tag* t, unsigned int v);
    static int SetBlock(Tag* t, const unsigned char* b, int l);
    static void ResetReadBits(Tag* tag);
    static void ResetWriteBits(Tag* tag);
    static unsigned int MemSize(unsigned int length);
    static unsigned char IsAllowedSpriteTag(Tag* tag);
    static RGBA GetSWFBackgroundColor(Swf* swf);
    static void SetBackground(RenderBuffer* buf, RGBA* img, int width, int height);
    static void SetBackgroundColor(RenderBuffer* buf, RGBA color);
    static unsigned char IsDefiningTag(Tag* tag);
    static unsigned int GetTagPos(Tag* t);
    static void SetTagPos(Tag* t, unsigned int pos);
    static unsigned short GetTagID(Tag* t);
    static Tag* NextTag(Tag* t);
    static Tag* DeleteTag(Swf* swf, Tag* t);
    static const char* TagGetName(Tag* tag);
    static unsigned short GetDefineID(Tag* t);
    static SRect GetDefineBBox(Tag* t);
    static int GetRect(Tag* t, SRect* r);
    static unsigned int GetBits(Tag* t, int nbits);
    static int GetSBits(Tag* t, int nbits);
    static void ExpandRect2(SRect* src, SRect* add);
    static unsigned char IsShapeTag(Tag* tag);
    static unsigned char IsImageTag(Tag* tag);
    static void ParseDefineShape(Tag* tag, Shape2* shape);
    static int ParseFillStyleArray(Tag* tag, Shape2* shape);
    static void ParseFillStyle(FillStyle* dest, Tag* tag, int num);
    static void GetRGB(Tag* t, RGBA* col);
    static void GetRGBA(Tag* t, RGBA* col);
    static int GetMatrix(Tag* t, Matrix* m);
    static void GetGradient(Tag* tag, Gradient* gradient, char alpha);
    static ShapeLine* ParseShapeData(unsigned char* data, int bits, int fillbits, int linebits, int version, Shape2* shape2);
    static void EnumerateUsedIDsOfStyles(Tag* tag, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph);
    static void EnumerateUsedIDsOfFillStyle(Tag* tag, int t, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph);
    static void EnumerateUsedIDsOfLineStyle(Tag* tag, int t, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph);
    static void GetMorphGradient(Tag* tag, Gradient* gradient1, Gradient* gradient2);
    static RGBA* ExtractImage(Tag* tag, int* dwidth, int* dheight);
    static void AddImage(RenderBuffer* buf, unsigned short id, RGBA* img, int width, int height);
    static int FontExtract(Swf* swf, int id, SwfFont** font);
    static int DefineFont(int id, SwfFont* f, Tag* t);
    static int DefineFontInfo(int id, SwfFont* f, Tag* t);
    static int DefineFont2(int id, SwfFont* font, Tag* tag);
    static int DefineFontAlignZones(int id, SwfFont* font, Tag* tag);
    static int DefineText(int id, SwfFont* f, Tag* t, int jobs);
    static int DefineTextCallback(int id, SwfFont* f, Tag* t, int jobs,
        void (*callback) (void* self, int* chars, int* xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color),
        void* self);
    static int GlyphNames(int id, SwfFont* f, Tag* tag);
    static void FontUpdateUsage(SwfFont* f, Tag* tag);
    static int GetSimpleShape(Tag* t, Shape** s);
    static Shape2* ShapeToShape2(Shape* shape);
    static SRect GetShapeBoundingBox(Shape2* shape2);
    static void Shape2Free(Shape2* s);
    static char* GetString(Tag* t);
    static int ShapeAddSolidFillStyle(Shape* s, RGBA* color);
    static int ShapeAddFillStyle(Shape* s, unsigned char type, Matrix* m, RGBA* color, unsigned short idBitmap, Gradient* gradient);
    static int ParseDefineText(Tag* tag,
        void (*callback) (void* self, int* chars, int* xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color),
        void* self);
    static void UpdateUsage(void* self, int* chars, int* xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color);
    static int FontInitUsage(SwfFont* f);
    static int FontUseGlyph(SwfFont* f, int glyph, unsigned short size);
    static char ShapeIsEmpty(Shape* s);
    static void FontUsePair(SwfFont* f, int char1, int char2);
    static void Hashadd(FontUsage* u, int char1, int char2, int nr);
    static unsigned Hash2(int char1, int char2);
    static int FontUseGetPair(SwfFont* f, int char1, int char2);
    static void FontFree(SwfFont* f);
    static void FreeGlyphNames(SwfFont* f);
    static void FreeUsage(SwfFont* f);
    static void FreeLayout(SwfFont* f);
    static void FontFreeName(SwfFont* f);
    static void FontFreeAlignZones(SwfFont* f);
    static void LayoutFree(SwfLayout* l);
    static int ShapeNew(Shape** s);
    static void RenderFromTag(RenderBuffer* buf, Character* idtable, Tag* firstTag, Matrix* m);
    static unsigned char IsPlaceTag(Tag* tag);
    static void GetPlaceObject(Tag* tag, SwfPlaceObject* obj);
    static void PlaceObjectFree(SwfPlaceObject* obj);
    static int GetCXForm(Tag* t, CXForm* cx, unsigned char alpha);
    static Matrix* MatrixJoin(Matrix* d, Matrix* s1, Matrix* s2);
    static int RFXSWF_SP(int a1, int a2, int b1, int b2);
    static void RenderShape(RenderBuffer* dest, Shape2* shape, Matrix* m, CXForm* c, unsigned short _depth, unsigned short _clipdepth);
    static double MatrixSize(Matrix* m);
    static Shape2* Shape2Clone(Shape2* s);
    static Shape2* LineStyle2FillStyle(Shape2* shape);
    static void TransformPoint(Matrix* m, int x, int y, int* dx, int* dy);
    static SPoint TurnPoint(SPoint p, Matrix* m);
    static void AddSolidLine(RenderBuffer* buf, double x1, double y1, double x2, double y2, double width, RenderPoint* p);
    static void AddLine(RenderBuffer* buf, double x1, double y1, double x2, double y2, RenderPoint* p);
    static void AddPixel(RenderBuffer* dest, float x, int y, RenderPoint* p);
    static void Process(RenderBuffer* dest, unsigned int clipdepth);
    static void ChangeState(int y, State* state, RenderPoint* p);
    static void SearchLayer(State* state, int depth, Layer** before, Layer** self, Layer** after);
    static void DeleteLayer(State* state, Layer* todel);
    static void AddLayer(State* state, Layer* before, Layer* toadd);
    static void FreeLayers(State* state);
    static void Fill(RenderBuffer* dest, RGBA* line, int* zline, int y, int x1, int x2, State* fillstate, unsigned int clipdepth);
    static void FillClip(RGBA* line, int* z, int y, int x1, int x2, unsigned int depth);
    static void FillSolid(RGBA* line, int* z, int y, int x1, int x2, RGBA col, unsigned int depth);
    static void FillBitmap(RGBA* line, int* z, int y, int x1, int x2, Matrix* m, Bitmap* b, int clipbitmap, unsigned int depth, double fmultiply);
    static void FillGradient(RGBA* line, int* z, int y, int x1, int x2, Matrix* m, Gradient* g, int type, unsigned int depth, double fmultiply);
    static int Clamp(int v);
    static void ClearTag(Tag* t);
    static void ShapeFree(Shape* s);
    static void TextCallback(void* self, int* chars, int* xpos, int nr, int fontid, int fontsize,
        int xstart, int ystart, RGBA* color);
    static int CompareRenderPoints(const void* _a, const void* _b);
    static void Delete(RenderBuffer* dest);
};
