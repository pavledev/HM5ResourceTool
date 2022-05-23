#include "pch.h"
#include "Scaleform/SWFRender.hpp"
#include "Scaleform/RenderBufferInternal.hpp"
#include "Scaleform/UsageTmp.hpp"

void Dummycallback1(Tag* tag, int x, void* y)
{
}

int ComparePlacements(const void* v1, const void* v2)
{
    const SwfPlaceObject* p1 = static_cast<const SwfPlaceObject*>(v1);
    const SwfPlaceObject* p2 = static_cast<const SwfPlaceObject*>(v2);

    if (p1->depth != p2->depth)
    {
        return (int)p1->depth - (int)p2->depth;
    }
    else
    {
        if (p2->clipdepth)
        {
            return 1; //Do the clip first
        }
        else
        {
            return -1;
        }
    }
}

void SWFRender::ReadSWF(Swf* swf, const std::string& filePath)
{
    SwfLib::SwfFileReader swfFileReader = SwfLib::SwfFileReader(filePath);

    ReadSWF(swf, swfFileReader);
}

void SWFRender::ReadSWF(Swf* swf, const void* data, const unsigned int dataSize)
{
    SwfLib::SwfFileReader swfFileReader = SwfLib::SwfFileReader(data, dataSize);

    ReadSWF(swf, swfFileReader);
}

void SWFRender::ReadSWF(Swf* swf, SwfLib::SwfFileReader& swfFileReader)
{
    Tag t1;
    char* chars = swfFileReader.ReadChars(3);
    std::string magic = std::string(chars, 3);

    delete[] chars;

    swf->fileVersion = swfFileReader.ReadByte();
    swf->compressed = (magic[0] == 'C') ? 1 : 0;
    swf->fileSize = swfFileReader.ReadInt32();

    std::vector<unsigned char> target;

    if (swf->compressed)
    {
        DecompressZlib(swfFileReader, target, swf->fileSize);

        swfFileReader = SwfLib::SwfFileReader(target.data(), target.size());
    }

    swf->compressed = 0; //Derive from version number from now on
    swf->movieSize = SwfLib::SwfFileReader::ReadRect(&swfFileReader);
    swf->frameRate = swfFileReader.ReadUInt16();
    swf->frameCount = swfFileReader.ReadUInt16();

    //Read tags and connect to list
    t1.next = 0;

    Tag* t = &t1;

    while (t)
    {
        t = ReadTag(swfFileReader, t);

        if (t && t->id == ST_FILEATTRIBUTES)
        {
            swf->fileAttributes = GetU32(t);

            ResetReadBits(t);
        }
    }

    swf->firstTag = t1.next;

    if (t1.next)
    {
        t1.next->prev = nullptr;
    }
}

void SWFRender::DecompressZlib(SwfLib::SwfFileReader& swfFileReader, std::vector<unsigned char>& target, unsigned int fileLength)
{
    void* buffer = operator new(fileLength - 8);

    swfFileReader.Seek(8, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

    z_stream strm = { 0 };

    strm.total_in = strm.avail_in = fileLength - 8;
    strm.total_out = strm.avail_out = fileLength - 8;
    strm.next_in = static_cast<Bytef*>(swfFileReader.GetCurrentPointer());
    strm.next_out = static_cast<Bytef*>(buffer);

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int result = -1;
    int ret = -1;

    result = inflateInit2(&strm, (MAX_WBITS + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib

    if (result == Z_OK)
    {
        result = inflate(&strm, Z_FINISH);

        if (result == Z_STREAM_END)
        {
            ret = strm.total_out;
        }
        else
        {
            inflateEnd(&strm);

            throw std::invalid_argument("Error.");
        }
    }
    else
    {
        inflateEnd(&strm);

        throw std::invalid_argument("Error.");
    }

    unsigned char* bytes = static_cast<unsigned char*>(buffer);

    target = std::vector<unsigned char>(bytes, bytes + fileLength - 8);
}

Tag* SWFRender::ReadTag(SwfLib::SwfFileReader& swfFileReader, Tag* prev)
{
    if (swfFileReader.Position() == swfFileReader.Length())
    {
        return nullptr;
    }

    unsigned short raw = swfFileReader.ReadUInt16();
    unsigned int len = raw & 0x3f;
    int id = raw >> 6;

    if (len == 0x3f)
    {
        len = swfFileReader.ReadUInt32();
    }

    if (id == ST_DEFINESPRITE)
    {
        len = 2 * sizeof(unsigned short);
    }
    // Sprite handling fix: Flatten sprite tree

    Tag* t = static_cast<Tag*>(calloc(1, sizeof(Tag)));

    t->len = len;
    t->id = id;

    if (t->len)
    {
        t->data = static_cast<unsigned char*>(calloc(1, t->len));
        t->memsize = t->len;

        swfFileReader.ReadBytes(t->data, t->len);
    }

    if (prev)
    {
        t->prev = prev;
        prev->next = t;
    }

    return t;
}

void SWFRender::Init(RenderBuffer* buf, int posx, int posy, int width, int height, int antialize, int multiply)
{
    memset(buf, 0, sizeof(RenderBuffer));

    buf->width = width * multiply;
    buf->height = height * multiply;
    buf->posx = posx;
    buf->posy = posy;
    buf->internal = static_cast<RenderBufferInternal*>(calloc(1, sizeof(RenderBufferInternal)));

    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(buf->internal);

    if (antialize < 1)
    {
        antialize = 1;
    }

    i->antialize = antialize;
    i->multiply = multiply * antialize;
    i->height2 = antialize * buf->height;
    i->width2 = antialize * buf->width;
    i->lines = (RenderLine*)calloc(1, i->height2 * sizeof(RenderLine));

    for (int y = 0; y < i->height2; y++)
    {
        memset(&i->lines[y], 0, sizeof(RenderLine));

        i->lines[y].points = InsertTag(0, 0);
        i->lines[y].num = 0;
    }

    i->zbuf = static_cast<int*>(calloc(1, sizeof(int) * i->width2 * i->height2));
    i->img = static_cast<RGBA*>(calloc(1, sizeof(RGBA) * i->width2 * i->height2));
    i->shapes = 0;
    i->ymin = 0x7fffffff;
    i->ymax = 0 - 0x80000000;
}

Tag* SWFRender::InsertTag(Tag* after, unsigned short id)
{
    Tag* t = static_cast<Tag*>(calloc(1, sizeof(Tag)));

    t->id = id;

    if (after)
    {
        t->prev = after;
        t->next = after->next;
        after->next = t;

        if (t->next)
        {
            t->next->prev = t;
        }
    }

    return t;
}

void SWFRender::RenderSWF(RenderBuffer* buf, Swf* swf)
{
    OptimizeTagOrder(swf);
    FoldAll(swf);

    Character* idtable = static_cast<Character*>(calloc(1, sizeof(Character) * 65536)); //ID to character mapping

    //Set background color
    RGBA color = GetSWFBackgroundColor(swf);

    SetBackgroundColor(buf, color);

    //Parse definitions
    Tag* tag = swf->firstTag;

    while (tag)
    {
        if (IsDefiningTag(tag))
        {
            int id = GetDefineID(tag);

            idtable[id].tag = tag;
            idtable[id].bbox = static_cast<SRect*>(calloc(1, sizeof(SRect)));
            *idtable[id].bbox = GetDefineBBox(tag);

            if (IsShapeTag(tag))
            {
                Shape2* shape = static_cast<Shape2*>(calloc(1, sizeof(Shape2)));

                ParseDefineShape(tag, shape);

                idtable[id].type = CharacterType::shapeType;
                idtable[id].obj.shape = shape;
            }
            else if (IsImageTag(tag))
            {
                int width, height;
                RGBA* data = ExtractImage(tag, &width, &height);

                idtable[id].type = CharacterType::imageType;

                AddImage(buf, id, data, width, height);
                free(data);
            }
            else if (tag->id == ST_DEFINEFONT || tag->id == ST_DEFINEFONT2 || tag->id == ST_DEFINEFONT3)
            {
                SwfFont* swffont = nullptr;
                Font* font = static_cast<Font*>(calloc(1, sizeof(Font)));

                idtable[id].obj.font = font;

                FontExtract(swf, id, &swffont);

                font->numberOfChars = swffont->numberOfChars;
                font->glyphs = static_cast<Shape2**>(calloc(1, sizeof(Shape2*) * font->numberOfChars));

                for (int t = 0; t < font->numberOfChars; t++)
                {
                    if (!swffont->glyph[t].shape->fillStyle.n)
                    {
                        /* the actual fill color will be overwritten while Render */
                        ShapeAddSolidFillStyle(swffont->glyph[t].shape, &colorWhite);
                    }

                    font->glyphs[t] = ShapeToShape2(swffont->glyph[t].shape);
                }

                FontFree(swffont);
                idtable[id].type = CharacterType::fontType;

            }
            else if (tag->id == ST_DEFINEFONTINFO || tag->id == ST_DEFINEFONTINFO2)
            {
                idtable[id].type = CharacterType::fontType;
            }
            else if (tag->id == ST_DEFINETEXT || tag->id == ST_DEFINETEXT2)
            {
                idtable[id].type = CharacterType::textType;
            }
            else if (tag->id == ST_DEFINESPRITE)
            {
                idtable[id].type = CharacterType::spriteType;
            }
            else if (tag->id == ST_DEFINEEDITTEXT)
            {
                idtable[id].type = CharacterType::editTextType;
            }
        }

        tag = tag->next;
    }

    Matrix m;

    GetMatrix(0, &m);
    RenderFromTag(buf, idtable, swf->firstTag, &m);

    //Free id and depth tables again
    for (int t = 0; t < 65536; t++)
    {
        if (idtable[t].bbox)
        {
            free(idtable[t].bbox);

            idtable[t].bbox = 0;
        }
        if (idtable[t].type == CharacterType::shapeType)
        {
            Shape2* shape = idtable[t].obj.shape;

            if (shape)
            {
                Shape2Free(shape); // FIXME
                free(idtable[t].obj.shape); idtable[t].obj.shape = 0;
            }
        }
        else if (idtable[t].type == CharacterType::fontType)
        {
            Font* font = idtable[t].obj.font;

            if (font)
            {
                if (font->glyphs)
                {
                    for (int t = 0; t < font->numberOfChars; t++)
                    {
                        Shape2Free(font->glyphs[t]);
                        free(font->glyphs[t]);

                        font->glyphs[t] = nullptr;
                    }

                    free(font->glyphs);

                    font->glyphs = nullptr;
                }

                free(idtable[t].obj.font);

                idtable[t].obj.font = nullptr;
                font = nullptr;
            }
        }
    }

    free(idtable);
}

RGBA* SWFRender::Render(RenderBuffer* dest)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);
    RGBA* img = static_cast<RGBA*>(calloc(1, sizeof(RGBA) * dest->width * dest->height));
    int antialize = i->antialize;

    if (antialize <= 1) //No antializing
    {
        for (int y = 0; y < i->height2; y++)
        {
            RGBA* line = &i->img[y * i->width2];

            memcpy(&img[y * dest->width], line, sizeof(RGBA) * dest->width);
        }
    }
    else
    {
        RGBA** lines = (RGBA**)calloc(1, sizeof(RGBA*) * antialize);
        int q = antialize * antialize;
        int ypos = 0;

        for (int y = 0; y < i->height2; y++)
        {
            ypos = y % antialize;
            lines[ypos] = &i->img[y * i->width2];

            if (ypos == antialize - 1)
            {
                RGBA* out = &img[(y / antialize) * dest->width];

                for (int x = 0; x < dest->width; x++)
                {
                    int xpos = x * antialize;
                    int yp;
                    unsigned int r = 0, g = 0, b = 0, a = 0;

                    for (yp = 0; yp < antialize; yp++)
                    {
                        RGBA* lp = &lines[yp][xpos];

                        for (int xp = 0; xp < antialize; xp++)
                        {
                            RGBA* p = &lp[xp];
                            r += p->r;
                            g += p->g;
                            b += p->b;
                            a += p->a;
                        }
                    }

                    out[x].r = r / q;
                    out[x].g = g / q;
                    out[x].b = b / q;
                    out[x].a = a / q;
                }
            }
        }

        free(lines);
    }

    return img;
}

void SWFRender::OptimizeTagOrder(Swf* swf)
{
    Tag* tag;
    Tag* next;
    Tag* level0;
    int level;
    int changes;

    UnFoldAll(swf);
    /*
    At the moment, we don't actually do optimizing,
    only fixing of non-spec-conformant things like
    sprite tags
    */

    do
    {
        changes = 0;
        level = 0;
        level0 = 0;
        tag = swf->firstTag;

        while (tag)
        {
            next = tag->next;

            if (tag->id == ST_DEFINESPRITE)
            {
                if (tag->len > 4)
                {
                    /* ??? all sprites are supposed to be unfolded */
                    throw std::invalid_argument("Internal error in OptimizeTagOrder/UnfoldAll");
                }

                level++;

                if (level == 1)
                {
                    level0 = tag;
                    tag = next;

                    continue;
                }
            }

            if (level >= 1)
            {
                //Move non-sprite tags out of sprite
                if (!IsAllowedSpriteTag(tag) || level >= 2)
                {
                    //Remove tag from current position
                    tag->prev->next = tag->next;

                    if (tag->next)
                    {
                        tag->next->prev = tag->prev;
                    }

                    //Insert before tag level0
                    tag->next = level0;
                    tag->prev = level0->prev;
                    level0->prev = tag;

                    if (tag->prev)
                    {
                        tag->prev->next = tag;
                    }
                    else
                    {
                        swf->firstTag = tag;
                    }

                    changes = 1;
                }
            }

            if (tag->id == ST_END)
            {
                level--;
            }

            tag = next;
        }
    }
    while (changes);
}

void SWFRender::FoldAll(Swf* swf)
{
    Tag* tag = swf->firstTag;

    while (tag)
    {
        if (tag->id == ST_DEFINESPRITE)
        {
            FoldSprite(tag);
        }

        tag = NextTag(tag);
    }
}

void SWFRender::UnFoldAll(Swf* swf)
{
    Tag* tag = swf->firstTag;

    while (tag)
    {
        if (tag->id == ST_DEFINESPRITE)
        {
            UnFoldSprite(tag);
        }

        tag = tag->next;
    }
}

void SWFRender::FoldSprite(Tag* t)
{
    Tag* sprtag = t, * tmp;
    unsigned short id, frames;
    int level;

    if (t->id != ST_DEFINESPRITE)
    {
        return;
    }

    if (!t->len)
    {
        throw std::invalid_argument("Error: Sprite has no ID!");
    }

    if (t->len > 4)
    {
        //Sprite is already folded */
        return;
    }

    t->pos = 0;
    id = GetU16(t);

    free(t->data);

    t->len = t->pos = t->memsize = 0;
    t->data = 0;

    frames = 0;

    t = NextTag(sprtag);
    level = 1;

    do
    {
        if (t->id == ST_SHOWFRAME)
        {
            frames++;
        }

        if (t->id == ST_DEFINESPRITE && t->len <= 4)
        {
            level++;
        }

        if (t->id == ST_END)
        {
            level--;
        }

        t = NextTag(t);
    }
    while (t && level);

    if (level)
    {
        throw std::invalid_argument("Sprite doesn't end(1)");
    }

    SetU16(sprtag, id);
    SetU16(sprtag, frames);

    t = NextTag(sprtag);
    level = 1;

    do
    {
        if (t->len < 0x3f &&
            (t->id != ST_DEFINEBITSLOSSLESS && t->id != ST_DEFINEBITSLOSSLESS2 && t->id != ST_SOUNDSTREAMBLOCK &&
                t->id != ST_DEFINEBITSJPEG && t->id != ST_DEFINEBITSJPEG2 && t->id != ST_DEFINEBITSJPEG3)
            )
        {
            SetU16(sprtag, t->len | (t->id << 6));
        }
        else
        {
            SetU16(sprtag, 0x3f | (t->id << 6));
            SetU32(sprtag, t->len);
        }

        if (t->len)
        {
            SetBlock(sprtag, t->data, t->len);
        }

        tmp = t;

        if (t->id == ST_DEFINESPRITE && t->len <= 4)
        {
            level++;
        }

        if (t->id == ST_END)
        {
            level--;
        }

        t = NextTag(t);
        DeleteTag(0, tmp);
    }
    while (t && level);

    if (level)
    {
        throw std::invalid_argument("Sprite doesn't end(2)\n");
    }
}

void SWFRender::UnFoldSprite(Tag* t)
{
    unsigned short id, tmp;
    unsigned int len;
    Tag* next = t;
    unsigned short spriteid, spriteframes;
    int level;

    if (t->id != ST_DEFINESPRITE)
    {
        return;
    }

    if (t->len <= 4) //Not folded
    {
        return;
    }

    SetTagPos(t, 0);

    spriteid = GetU16(t); //id
    spriteframes = GetU16(t); //frames

    level = 1;

    while (1)
    {
        Tag* it = nullptr;

        tmp = GetU16(t);
        len = tmp & 0x3f;
        id = tmp >> 6;

        if (id == ST_END)
        {
            level--;
        }

        if (id == ST_DEFINESPRITE && len <= 4)
        {
            level++;
        }

        if (len == 0x3f)
        {
            len = GetU32(t);
        }

        it = InsertTag(next, id);
        next = it;
        it->len = len;
        it->id = id;

        if (it->len)
        {
            it->data = static_cast<unsigned char*>(calloc(1, it->len));
            it->memsize = it->len;

            GetBlock(t, it->data, it->len);
        }

        if (!level)
        {
            break;
        }
    }

    free(t->data); t->data = 0;

    t->memsize = t->len = t->pos = 0;

    SetU16(t, spriteid);
    SetU16(t, spriteframes);
}

unsigned char SWFRender::GetU8(Tag* t)
{
    ResetReadBits(t);

    if (static_cast<int>(t->pos) >= static_cast<int>(t->len))
    {
        throw std::invalid_argument(std::format("GetU8() out of bounds: TagID = %i\n", t->id));
    }

    return t->data[t->pos++];
}

short SWFRender::GetS16(Tag* tag)
{
    return static_cast<short>(GetU16(tag));
}

unsigned short SWFRender::GetU16(Tag* t)
{
    unsigned short res;

    ResetReadBits(t);

    if (static_cast<int>(t->pos) > (static_cast<int>(t->len) - 2))
    {
        throw std::invalid_argument(std::format("GetU16() out of bounds: TagID = {}\n", t->id));
    }

    res = t->data[t->pos] | (t->data[t->pos + 1] << 8);
    t->pos += 2;

    return res;
}

unsigned int SWFRender::GetU32(Tag* t)
{
    unsigned int res;

    ResetReadBits(t);

    if (static_cast<int>(t->pos) > (static_cast<int>(t->len) - 4))
    {
        throw std::invalid_argument(std::format("GetU32() out of bounds: TagID = {}\n", t->id));
    }

    res = t->data[t->pos] | (t->data[t->pos + 1] << 8) |
        (t->data[t->pos + 2] << 16) | (t->data[t->pos + 3] << 24);

    t->pos += 4;

    return res;
}

int SWFRender::SetU8(Tag* t, unsigned char v)
{
    ResetWriteBits(t);

    if ((t->len + 1) > t->memsize)
    {
        return (SetBlock(t, &v, 1) == 1) ? 0 : -1;
    }

    t->data[t->len++] = v;

    return 0;
}

int SWFRender::SetU16(Tag* t, unsigned short v)
{
    unsigned char a[2];

    a[0] = v & 0xff;
    a[1] = v >> 8;

    ResetWriteBits(t);

    if ((t->len + 2) > t->memsize)
    {
        return (SetBlock(t, a, 2) == 2) ? 0 : -1;
    }

    t->data[t->len++] = a[0];
    t->data[t->len++] = a[1];

    return 0;
}

int SWFRender::SetU32(Tag* t, unsigned int v)
{
    unsigned char a[4];

    a[0] = v & 0xff; //To ensure correct handling of non-intel byteorder
    a[1] = (v >> 8) & 0xff;
    a[2] = (v >> 16) & 0xff;
    a[3] = (v >> 24) & 0xff;

    ResetWriteBits(t);

    if ((t->len + 4) > t->memsize)
    {
        return (SetBlock(t, a, 4) == 4) ? 0 : -1;
    }

    t->data[t->len++] = a[0];
    t->data[t->len++] = a[1];
    t->data[t->len++] = a[2];
    t->data[t->len++] = a[3];

    return 0;
}

int SWFRender::GetBlock(Tag* t, unsigned char* b, int l)
// returns number of bytes written (<=l)
// b = nullptr -> skip data
{
    ResetReadBits(t);

    if ((t->len - t->pos) < l)
    {
        l = t->len - t->pos;
    }

    if (b && l)
    {
        memcpy(b, &t->data[t->pos], l);
    }

    t->pos += l;

    return l;
}

int SWFRender::SetBlock(Tag* t, const unsigned char* b, int l)
// Appends Block to the end of Tagdata, returns size
{
    unsigned int newlen = t->len + l;

    ResetWriteBits(t);

    if (newlen > t->memsize)
    {
        unsigned int  newmem = MemSize(newlen);
        unsigned char* newdata = (unsigned char*)(realloc(t->data, newmem));
        t->memsize = newmem;
        t->data = newdata;
    }

    if (b)
    {
        memcpy(&t->data[t->len], b, l);
    }
    else
    {
        memset(&t->data[t->len], 0x00, l);
    }

    t->len += l;

    return l;
}

void SWFRender::ResetReadBits(Tag* tag)
{
    if (tag->readBit)
    {
        tag->pos++;
        tag->readBit = 0;
    }
}

void SWFRender::ResetWriteBits(Tag* tag)
{
    if (tag->writeBit)
    {
        tag->writeBit = 0;
    }
}

unsigned int SWFRender::MemSize(unsigned int length)
{
    static unsigned int mallocSize = 128;

    return ((length / mallocSize) + 1) * mallocSize;
}

unsigned char SWFRender::IsAllowedSpriteTag(Tag* tag)
{
    int id = tag->id;
    int t = 0;

    while (swf_spritetagids[t] >= 0)
    {
        if (swf_spritetagids[t] == id)
        {
            return 1;
        }

        t++;
    }

    return 0;
}

RGBA SWFRender::GetSWFBackgroundColor(Swf* swf)
{
    Tag* t = swf->firstTag;
    RGBA color;

    color.r = color.b = color.g = 0;
    color.a = 255;

    while (t)
    {
        if (t->id == ST_SETBACKGROUNDCOLOR)
        {
            SetTagPos(t, 0);

            color.r = GetU8(t);
            color.g = GetU8(t);
            color.b = GetU8(t);

            break;
        }

        t = t->next;
    }

    return color;
}

void SWFRender::SetBackground(RenderBuffer* buf, RGBA* img, int width, int height)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(buf->internal);
    int x, xx, y, yy;
    int xstep = width * 65536 / i->width2;
    int ystep = height * 65536 / i->height2;

    if (i->shapes)
    {
        fprintf(stderr, "rfxswf: Warning: swf_Render_SetBackground() called after drawing shapes\n");
    }

    for (y = 0, yy = 0; y < i->height2; y++, yy += ystep)
    {
        RGBA* src = &img[(yy >> 16) * width];
        RGBA* line = &i->img[y * i->width2];
        for (x = 0, xx = 0; x < i->width2; x++, xx += xstep)
        {
            line[x] = src[xx >> 16];
        }
    }
}

void SWFRender::SetBackgroundColor(RenderBuffer* buf, RGBA color)
{
    SetBackground(buf, &color, 1, 1);
}

unsigned char SWFRender::IsDefiningTag(Tag* tag)
{
    int id = tag->id;
    int t = 0;

    while (swf_definingtagids[t] >= 0)
    {
        if (swf_definingtagids[t] == id)
        {
            return 1;
        }

        t++;
    }

    return 0;
}

unsigned int SWFRender::GetTagPos(Tag* t)
{
    return t->pos;
}

void SWFRender::SetTagPos(Tag* t, unsigned int pos)
{
    ResetReadBits(t);

    if (pos <= t->len)
    {
        t->pos = pos;
    }
    else
    {
        throw std::invalid_argument(std::format("SetTagPos({}) out of bounds: TagID = {}", pos, t->id));
    }
}

unsigned short SWFRender::GetTagID(Tag* t)
{
    return t->id;
}

Tag* SWFRender::NextTag(Tag* t)
{
    return t->next;
}

Tag* SWFRender::DeleteTag(Swf* swf, Tag* t)
{
    Tag* next = t->next;

    if (swf && swf->firstTag == t)
    {
        swf->firstTag = t->next;
    }

    if (t->prev)
    {
        t->prev->next = t->next;
    }

    if (t->next)
    {
        t->next->prev = t->prev;
    }

    if (t->data)
    {
        free(t->data);
    }

    free(t);

    return next;
}

const char* SWFRender::TagGetName(Tag* tag)
{
    switch (tag->id)
    {
    case ST_END:
        return "END";
    case ST_SHOWFRAME:
        return "SHOWFRAME";
    case ST_DEFINESHAPE:
        return "DEFINESHAPE";
    case ST_FREECHARACTER:
        return "FREECHARACTER";
    case ST_PLACEOBJECT:
        return "PLACEOBJECT";
    case ST_REMOVEOBJECT:
        return "REMOVEOBJECT";
    case ST_DEFINEBITSJPEG:
        return "DEFINEBITSJPEG";
    case ST_DEFINEBUTTON:
        return "DEFINEBUTTON";
    case ST_JPEGTABLES:
        return "JPEGTABLES";
    case ST_SETBACKGROUNDCOLOR:
        return "SETBACKGROUNDCOLOR";
    case ST_DEFINEFONT:
        return "DEFINEFONT";
    case ST_DEFINETEXT:
        return "DEFINETEXT";
    case ST_DEFINEEDITTEXT:
        return "DEFINEEDITTEXT";
    case ST_DOACTION:
        return "DOACTION";
    case ST_DOABC:
        return "DOABC";
    case ST_RAWABC:
        return "RAWABC";
    case ST_DEFINEFONTINFO:
        return "DEFINEFONTINFO";
    case ST_DEFINESOUND:
        return "DEFINESOUND";
    case ST_STARTSOUND:
        return "STARTSOUND";
    case ST_DEFINEBUTTONSOUND:
        return "DEFINEBUTTONSOUND";
    case ST_SOUNDSTREAMHEAD:
        return "SOUNDSTREAMHEAD";
    case ST_SOUNDSTREAMBLOCK:
        return "SOUNDSTREAMBLOCK";
    case ST_DEFINEBITSLOSSLESS:
        return "DEFINEBITSLOSSLESS";
    case ST_DEFINEBITSJPEG2:
        return "DEFINEBITSJPEG2";
    case ST_DEFINESHAPE2:
        return "DEFINESHAPE2";
    case ST_DEFINEBUTTONCXFORM:
        return "DEFINEBUTTONCXFORM";
    case ST_PROTECT:
        return "PROTECT";
    case ST_PLACEOBJECT2:
        return "PLACEOBJECT2";
    case ST_REMOVEOBJECT2:
        return "REMOVEOBJECT2";
    case ST_DEFINESHAPE3:
        return "DEFINESHAPE3";
    case ST_DEFINETEXT2:
        return "DEFINETEXT2";
    case ST_DEFINEBUTTON2:
        return "DEFINEBUTTON2";
    case ST_DEFINEBITSJPEG3:
        return "DEFINEBITSJPEG3";
    case ST_DEFINEBITSLOSSLESS2:
        return "DEFINEBITSLOSSLESS2";
    case ST_DEFINESPRITE:
        return "DEFINESPRITE";
    case ST_NAMECHARACTER:
        return "NAMECHARACTER";
    case ST_SERIALNUMBER:
        return "SERIALNUMBER";
    case ST_GENERATORTEXT:
        return "GENERATORTEXT";
    case ST_FRAMELABEL:
        return "FRAMELABEL";
    case ST_SOUNDSTREAMHEAD2:
        return "SOUNDSTREAMHEAD2";
    case ST_DEFINEMORPHSHAPE:
        return "DEFINEMORPHSHAPE";
    case ST_DEFINEMORPHSHAPE2:
        return "DEFINEMORPHSHAPE2";
    case ST_DEFINEFONT2:
        return "DEFINEFONT2";
    case ST_TEMPLATECOMMAND:
        return "TEMPLATECOMMAND";
    case ST_GENERATOR3:
        return "GENERATOR3";
    case ST_EXTERNALFONT:
        return "EXTERNALFONT";
    case ST_EXPORTASSETS:
        return "EXPORTASSETS";
    case ST_SYMBOLCLASS:
        return "SYMBOLCLASS";
    case ST_DEFINEBINARY:
        return "DEFINEBINARY";
    case ST_IMPORTASSETS:
        return "IMPORTASSETS";
    case ST_ENABLEDEBUGGER:
        return "ENABLEDEBUGGER";
    case ST_DOINITACTION:
        return "DOINITACTION";
    case ST_DEFINEMOVIE:
        return "DEFINEMOVIE";
    case ST_DEFINEVIDEOSTREAM:
        return "DEFINEVIDEOSTREAM";
    case ST_VIDEOFRAME:
        return "VIDEOFRAME";
    case ST_DEFINEFONTINFO2:
        return "DEFINEFONTINFO2";
    case ST_FILEATTRIBUTES:
        return "FILEATTRIBUTES";
    case ST_IMPORTASSETS2:
        return "IMPORTASSETS2";
    case ST_DEFINESCALINGGRID:
        return "DEFINESCALINGGRID";
    case ST_DEFINESHAPE4:
        return "DEFINESHAPE4";
    case ST_DEFINEFONT3:
        return "DEFINEFONT3";
    case ST_DEFINEFONTALIGNZONES:
        return "DEFINEFONTALIGNZONES";
    case ST_DEFINEFONTNAME:
        return "DEFINEFONTNAME";
    case ST_CSMTEXTSETTINGS:
        return "CSMTEXTSETTINGS";
    case ST_FREEALL:
        return "FREEALL";
    case ST_MX4:
        return "MX4";
    case ST_SCRIPTLIMITS:
        return "SCRIPTLIMITS";
    case ST_SETTABINDEX:
        return "SETTABINDEX";
    case ST_ENABLEDEBUGGER2:
        return "ENABLEDEBUGGER2";
    case ST_PLACEOBJECT3:
        return "PLACEOBJECT3";
    case ST_METADATA:
        return "METADATA";
    case ST_SCENEDESCRIPTION:
        return "SCENEDESCRIPTION";

    case ST_REFLEX:
        return "REFLEX";
    case ST_GLYPHNAMES:
        return "GLYPHNAMES";
    }

    return 0;
}

unsigned short SWFRender::GetDefineID(Tag* t)
// up to SWF 4.0
{
    unsigned short id = 0;
    unsigned int oldTagPos = GetTagPos(t);

    SetTagPos(t, 0);

    switch (GetTagID(t))
    {
    case ST_DEFINESHAPE:
    case ST_DEFINESHAPE2:
    case ST_DEFINESHAPE3:
    case ST_DEFINESHAPE4:
    case ST_DEFINEMORPHSHAPE:
    case ST_DEFINEMORPHSHAPE2:
    case ST_DEFINEEDITTEXT:
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINESCALINGGRID: //pseudodefine
    case ST_DEFINEBUTTON:
    case ST_DEFINEBUTTON2:
    case ST_DEFINEBUTTONCXFORM: //pseudodefine
    case ST_DEFINEBUTTONSOUND: //pseudodefine
    case ST_CSMTEXTSETTINGS: //pseudodefine
    case ST_DEFINEFONT:
    case ST_DEFINEFONT2:
    case ST_DEFINEFONT3:
    case ST_DEFINEFONTINFO: //pseudodefine
    case ST_DEFINEFONTINFO2: //pseudodefine
    case ST_DEFINEFONTALIGNZONES: //pseudodefine
    case ST_DEFINEFONTNAME: //pseudodefine
    case ST_DEFINETEXT:
    case ST_DEFINEBINARY:
    case ST_DEFINETEXT2:
    case ST_DEFINESOUND:
    case ST_DEFINESPRITE:
    case ST_DEFINEMOVIE:
    case ST_DEFINEVIDEOSTREAM:
    case ST_GLYPHNAMES: //pseudodefine
    case ST_VIDEOFRAME: //pseudodefine
    case ST_NAMECHARACTER: //pseudodefine
    case ST_DOINITACTION: //pseudodefine
        id = GetU16(t);
        break;
    default:
        throw std::invalid_argument(std::format("Error: tag {} ({}) has no id\n", t->id, TagGetName(t)));
    }

    SetTagPos(t, oldTagPos);

    return id;
}

SRect SWFRender::GetDefineBBox(Tag* t)
{
    unsigned short id = 0;
    SRect b1, b2;

    memset(&b1, 0, sizeof(b1));

    unsigned int oldTagPos = GetTagPos(t);

    SetTagPos(t, 0);
    GetRect(0, &b1);

    switch (GetTagID(t))
    {
    case ST_DEFINESHAPE:
    case ST_DEFINESHAPE2:
    case ST_DEFINESHAPE3:
    case ST_DEFINESHAPE4:
    case ST_DEFINEEDITTEXT:
    case ST_DEFINETEXT:
    case ST_DEFINETEXT2:
    case ST_DEFINEVIDEOSTREAM:
        id = GetU16(t);
        GetRect(t, &b1);
        break;
    case ST_DEFINEMORPHSHAPE:
        id = GetU16(t);
        GetRect(t, &b1);
        GetRect(t, &b2);
        ExpandRect2(&b1, &b2);
        break;
    case ST_DEFINEBITSLOSSLESS:
    case ST_DEFINEBITSLOSSLESS2:
    case ST_DEFINEBITS:
    case ST_DEFINEBITSJPEG2:
    case ST_DEFINEBITSJPEG3:
        // FIXME
        break;
    }

    SetTagPos(t, oldTagPos);

    return b1;
}

int SWFRender::GetRect(Tag* t, SRect* r)
{
    int nbits;
    SRect dummy;

    if (!t)
    {
        r->xmin = r->xmax = r->ymin = r->ymax = 0; return 0;
    }

    if (!r)
    {
        r = &dummy;
    }

    nbits = static_cast<int>(GetBits(t, 5));
    r->xmin = GetSBits(t, nbits);
    r->xmax = GetSBits(t, nbits);
    r->ymin = GetSBits(t, nbits);
    r->ymax = GetSBits(t, nbits);

    return 0;
}

unsigned int SWFRender::GetBits(Tag* t, int nbits)
{
    unsigned int res = 0;

    if (!nbits)
    {
        return 0;
    }

    if (!t->readBit)
    {
        t->readBit = 0x80;
    }

    while (nbits)
    {
        res <<= 1;

        if (t->pos >= t->len)
        {
            std::string message;

            message += std::format("GetBits() out of bounds: TagID = {}, pos={}, len={}\n", t->id, t->pos, t->len);

            int i, m = t->len > 10 ? 10 : t->len;

            for (i = -1; i < m; i++)
            {
                message += std::format("({}){} ", i, t->data[i]);
            }

            throw std::invalid_argument(message);
        }

        if (t->data[t->pos] & t->readBit)
        {
            res |= 1;
        }

        t->readBit >>= 1;
        nbits--;

        if (!t->readBit)
        {
            if (nbits)
            {
                t->readBit = 0x80;
            }

            t->pos++;
        }
    }

    return res;
}

int SWFRender::GetSBits(Tag* t, int nbits)
{
    unsigned int res = GetBits(t, nbits);

    if (res & (1 << (nbits - 1)))
    {
        res |= (0xffffffff << nbits);
    }

    return static_cast<int>(res);
}

void SWFRender::ExpandRect2(SRect* src, SRect* add)
{
    if ((add->xmin | add->ymin | add->xmax | add->ymax) == 0)
    {
        return;
    }

    if ((src->xmin | src->ymin | src->xmax | src->ymax) == 0)
    {
        *src = *add;
    }

    if (add->xmin < src->xmin)
    {
        src->xmin = add->xmin;
    }

    if (add->ymin < src->ymin)
    {
        src->ymin = add->ymin;
    }

    if (add->xmax > src->xmax)
    {
        src->xmax = add->xmax;
    }

    if (add->ymax > src->ymax)
    {
        src->ymax = add->ymax;
    }
}

unsigned char SWFRender::IsShapeTag(Tag* tag)
{
    if (tag->id == ST_DEFINESHAPE ||
        tag->id == ST_DEFINESHAPE2 ||
        tag->id == ST_DEFINESHAPE3 ||
        tag->id == ST_DEFINESHAPE4)
    {
        return 1;
    }

    return 0;
}

unsigned char SWFRender::IsImageTag(Tag* tag)
{
    if (tag->id == ST_DEFINEBITSJPEG ||
        tag->id == ST_DEFINEBITSJPEG2 ||
        tag->id == ST_DEFINEBITSJPEG3 ||
        tag->id == ST_DEFINEBITSLOSSLESS ||
        tag->id == ST_DEFINEBITSLOSSLESS2)
    {
        return 1;
    }

    return 0;
}

void SWFRender::ParseDefineShape(Tag* tag, Shape2* shape)
{
    int num = 0, id;
    unsigned short fill, line;
    ShapeLine* l;

    if (tag->id == ST_DEFINESHAPE)
    {
        num = 1;
    }
    else if (tag->id == ST_DEFINESHAPE2)
    {
        num = 2;
    }
    else if (tag->id == ST_DEFINESHAPE3)
    {
        num = 3;
    }
    else if (tag->id == ST_DEFINESHAPE4)
    {
        num = 4;
    }
    else
    {
        throw std::invalid_argument("ParseDefineShape must be called with a shape tag.");
    }

    SetTagPos(tag, 0);

    id = GetU16(tag); //id
    memset(shape, 0, sizeof(Shape2));
    shape->bbox = static_cast<SRect*>(calloc(1, sizeof(SRect)));
    GetRect(tag, shape->bbox);

    if (num >= 4)
    {
        SRect r2;

        ResetReadBits(tag);
        GetRect(tag, &r2); // edge bounds

        unsigned char flags = GetU8(tag); // flags, &1: contains scaling stroke, &2: contains non-scaling stroke
    }

    if (!ParseFillStyleArray(tag, shape))
    {
        return;
    }

    ResetReadBits(tag);

    fill = (unsigned short)GetBits(tag, 4);
    line = (unsigned short)GetBits(tag, 4);

    if (!fill && !line)
    {
        throw std::invalid_argument("Fill/Line bits are both zero.");
    }

    shape->lines = ParseShapeData(&tag->data[tag->pos], (tag->len - tag->pos) * 8, fill, line, num, shape);
    l = shape->lines;
}

int SWFRender::ParseFillStyleArray(Tag* tag, Shape2* shape)
{
    unsigned short count;
    int t;
    int num = 0;
    int fillstylestart = shape->numberOfFillstyles;
    int linestylestart = shape->numberOfLineStyles;

    if (tag->id == ST_DEFINESHAPE)
    {
        num = 1;
    }
    else if (tag->id == ST_DEFINESHAPE2)
    {
        num = 2;
    }
    else if (tag->id == ST_DEFINESHAPE3)
    {
        num = 3;
    }
    else if (tag->id == ST_DEFINESHAPE4)
    {
        num = 4;
    }

    count = GetU8(tag);

    if (count == 0xff && num > 1) //Defineshape2,3 only
    {
        count = GetU16(tag);
    }

    shape->numberOfFillstyles += count;

    if (shape->numberOfFillstyles)
    {
        shape->fillStyles = static_cast<FillStyle*>(realloc(shape->fillStyles, sizeof(FillStyle) * shape->numberOfFillstyles));

        for (t = fillstylestart; t < shape->numberOfFillstyles; t++)
        {
            ParseFillStyle(&shape->fillStyles[t], tag, num);
        }
    }

    ResetReadBits(tag);
    count = GetU8(tag); // line style array

    if (count == 0xff)
    {
        count = GetU16(tag);
    }

    shape->numberOfLineStyles += count;

    if (count)
    {
        shape->lineStyles = static_cast<LineStyle*>(realloc(shape->lineStyles, sizeof(LineStyle) * shape->numberOfLineStyles));
        /* TODO: should we start with 1 and insert a correct definition of the
           "built in" lineStyle 0? */
        for (t = linestylestart; t < shape->numberOfLineStyles; t++)
        {
            char fill = 0;

            shape->lineStyles[t].width = GetU16(tag);

            if (num >= 4)
            {
                unsigned short flags = GetU16(tag);

                if ((flags & 0x30) == 0x20)
                {
                    GetU16(tag); //Miter limit
                }

                if (flags & 0x08)
                {
                    fprintf(stderr, "Warning: Filled strokes parsing not yet fully supported.");

                    fill = 1;
                }
            }

            if (fill)
            {
                FillStyle f;

                ParseFillStyle(&f, tag, num);

                shape->lineStyles[t].color = f.color;
            }
            else
            {
                if (num >= 3)
                {
                    GetRGBA(tag, &shape->lineStyles[t].color);
                }
                else
                {
                    GetRGB(tag, &shape->lineStyles[t].color);
                }
            }
        }
    }

    return 1;
}

void SWFRender::ParseFillStyle(FillStyle* dest, Tag* tag, int num)
{
    int type = GetU8(tag); //type

    dest->type = type;

    if (type == 0)
    {
        /* plain color */
        if (num >= 3)
        {
            GetRGBA(tag, &dest->color);
        }
        else
        {
            GetRGB(tag, &dest->color);
        }
    }
    else if (type == 0x10 || type == 0x11 || type == 0x12 || type == 0x13)
    {
        /* linear/radial gradient fill */
        ResetReadBits(tag);
        GetMatrix(tag, &dest->m);
        ResetReadBits(tag);
        GetGradient(tag, &dest->gradient, num >= 3 ? 1 : 0);

        if (type == 0x13)
        {
            GetU16(tag);
        }
    }
    else if (type == 0x40 || type == 0x41 || type == 0x42 || type == 0x43)
    {
        /* bitmap fill */
        ResetReadBits(tag);
        dest->idBitmap = GetU16(tag); //id
        ResetReadBits(tag); //?
        GetMatrix(tag, &dest->m);
    }
    else
    {
        fprintf(stderr, "Unknown fillStyle:0x%02x in tag %02d\n", type, tag->id);
    }
}

void SWFRender::GetRGB(Tag* t, RGBA* col)
{
    RGBA dummy;

    if (!col)
    {
        col = &dummy;
    }

    col->r = GetU8(t);
    col->g = GetU8(t);
    col->b = GetU8(t);
    col->a = 255;
}

void SWFRender::GetRGBA(Tag* t, RGBA* col)
{
    RGBA dummy;

    if (!col)
    {
        col = &dummy;
    }

    col->r = GetU8(t);
    col->g = GetU8(t);
    col->b = GetU8(t);
    col->a = GetU8(t);
}

int SWFRender::GetMatrix(Tag* t, Matrix* m)
{
    Matrix dummy;
    int nbits;

    if (!m)
    {
        m = &dummy;
    }

    if (!t)
    {
        m->sx = m->sy = 0x10000;
        m->r0 = m->r1 = 0;
        m->tx = m->ty = 0;

        return -1;
    }

    ResetReadBits(t);

    if (GetBits(t, 1))
    {
        nbits = GetBits(t, 5);
        m->sx = GetSBits(t, nbits);
        m->sy = GetSBits(t, nbits);
    }
    else
    {
        m->sx = m->sy = 0x10000;
    }

    if (GetBits(t, 1))
    {
        nbits = GetBits(t, 5);
        m->r0 = GetSBits(t, nbits);
        m->r1 = GetSBits(t, nbits);
    }
    else
    {
        m->r0 = m->r1 = 0x0;
    }

    nbits = GetBits(t, 5);
    m->tx = GetSBits(t, nbits);
    m->ty = GetSBits(t, nbits);

    return 0;
}

void SWFRender::GetGradient(Tag* tag, Gradient* gradient, char alpha)
{
    if (!tag)
    {
        memset(gradient, 0, sizeof(Gradient));

        return;
    }

    unsigned char num = GetU8(tag) & 15;

    if (gradient)
    {
        gradient->num = num;
        gradient->rgba = static_cast<RGBA*>(calloc(1, sizeof(RGBA) * gradient->num));
        gradient->ratios = static_cast<unsigned char*>(calloc(1, sizeof(gradient->ratios[0]) * gradient->num));
    }

    for (int t = 0; t < num; t++)
    {
        unsigned char ratio = GetU8(tag);
        RGBA color;

        if (!alpha)
        {
            GetRGB(tag, &color);
        }
        else
        {
            GetRGBA(tag, &color);
        }

        if (gradient)
        {
            gradient->ratios[t] = ratio;
            gradient->rgba[t] = color;
        }
    }
}

/* todo: merge this with GetSimpleShape */
ShapeLine* SWFRender::ParseShapeData(unsigned char* data, int bits, int fillbits, int linebits, int version, Shape2* shape2)
{
    ShapeLine _lines;
    ShapeLine* lines = &_lines;

    Tag _tag;
    Tag* tag = &_tag;
    int fill0 = 0;
    int fill1 = 0;
    int line = 0;
    int x = 0, y = 0;
    int linestyleadd = 0;
    int fillstyleadd = 0;

    memset(tag, 0, sizeof(Tag));

    tag->data = data;
    tag->len = tag->memsize = (bits + 7) / 8;
    tag->pos = 0;
    tag->id = version == 1 ? ST_DEFINESHAPE : (version == 2 ? ST_DEFINESHAPE2 : (version == 3 ? ST_DEFINESHAPE3 : ST_DEFINESHAPE4));

    lines->next = 0;

    while (1)
    {
        int flags;

        flags = GetBits(tag, 1);

        if (!flags)
        { //style change
            flags = GetBits(tag, 5);

            if (!flags)
            {
                break;
            }

            if (flags & 1)
            { //move
                int n = GetBits(tag, 5);

                x = GetSBits(tag, n); //x
                y = GetSBits(tag, n); //y
            }

            if (flags & 2)
            {
                fill0 = GetBits(tag, fillbits) + fillstyleadd;
            }

            if (flags & 4)
            {
                fill1 = GetBits(tag, fillbits) + fillstyleadd;
            }

            if (flags & 8)
            {
                line = GetBits(tag, linebits) + linestyleadd;
            }

            if (flags & 16)
            {
                if (!shape2)
                {
                    fprintf(stderr, "rfxswf: Error: Additional fillStyles not supported\n"); fflush(stderr);
                    EnumerateUsedIDsOfStyles(tag, Dummycallback1, 0, version, 0);
                }
                else
                {
                    linestyleadd = shape2->numberOfLineStyles;
                    fillstyleadd = shape2->numberOfFillstyles;

                    if (!ParseFillStyleArray(tag, shape2))
                    {
                        return 0;
                    }
                }

                fillbits = GetBits(tag, 4);
                linebits = GetBits(tag, 4);
            }
            if (flags & 1)
            { //move
                lines->next = static_cast<ShapeLine*>(calloc(1, sizeof(ShapeLine)));
                lines = lines->next;
                lines->type = ShapeLineType::moveTo;
                lines->x = x;
                lines->y = y;
                lines->sx = lines->sy = 0;
                lines->fillstyle0 = fill0;
                lines->fillstyle1 = fill1;
                lines->lineStyle = line;
                lines->next = 0;
            }
        }
        else
        {
            flags = GetBits(tag, 1);
            if (flags)
            { //straight edge
                int n = GetBits(tag, 4) + 2;
                if (GetBits(tag, 1))
                { //line flag
                    x += GetSBits(tag, n); //delta x
                    y += GetSBits(tag, n); //delta y
                }
                else
                {
                    int v = GetBits(tag, 1);
                    int d;
                    d = GetSBits(tag, n); //vert/horz
                    if (v) y += d;
                    else  x += d;
                }
                lines->next = static_cast<ShapeLine*>(calloc(1, sizeof(ShapeLine)));
                lines = lines->next;
                lines->type = ShapeLineType::lineTo;
                lines->x = x;
                lines->y = y;
                lines->sx = lines->sy = 0;
                lines->fillstyle0 = fill0;
                lines->fillstyle1 = fill1;
                lines->lineStyle = line;
                lines->next = 0;
            }
            else
            { //curved edge
                int n = GetBits(tag, 4) + 2;
                int x1, y1;
                x += GetSBits(tag, n);
                y += GetSBits(tag, n);
                x1 = x;
                y1 = y;
                x += GetSBits(tag, n);
                y += GetSBits(tag, n);

                lines->next = static_cast<ShapeLine*>(calloc(1, sizeof(ShapeLine)));
                lines = lines->next;
                lines->type = ShapeLineType::splineTo;
                lines->sx = x1;
                lines->sy = y1;
                lines->x = x;
                lines->y = y;
                lines->fillstyle0 = fill0;
                lines->fillstyle1 = fill1;
                lines->lineStyle = line;
                lines->next = 0;
            }
        }
    }

    return _lines.next;
}

void SWFRender::EnumerateUsedIDsOfStyles(Tag* tag, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph)
{
    unsigned short count = GetU8(tag);

    if (count == 0xff && num > 1) //Defineshape2,3,4 only
    {
        count = GetU16(tag);
    }

    for (int t = 0; t < count; t++)
    {
        EnumerateUsedIDsOfFillStyle(tag, t, callback, callback_data, num, morph);
    }

    ResetReadBits(tag);
    count = GetU8(tag); // line style array

    if (count == 0xff)
    {
        count = GetU16(tag);
    }

    for (int t = 0; t < count; t++)
    {
        EnumerateUsedIDsOfLineStyle(tag, t, callback, callback_data, num, morph);
    }
}

void SWFRender::EnumerateUsedIDsOfFillStyle(Tag* tag, int t, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph)
{
    int type;
    type = GetU8(tag); //type

    if (type == 0)
    {
        RGBA color;

        if (num >= 3)
        {
            GetRGBA(tag, &color);

            if (morph)
            {
                GetRGBA(tag, nullptr);
            }
        }
        else
        {
            GetRGB(tag, &color);

            if (morph)
            {
                GetRGB(tag, nullptr);
            }
        }
    }
    else if (type == 0x10 || type == 0x12 || type == 0x13)
    {
        ResetReadBits(tag);

        Matrix m;

        GetMatrix(tag, &m);

        if (morph)
        {
            GetMatrix(tag, &m);
        }

        ResetReadBits(tag);

        if (morph)
        {
            GetMorphGradient(tag, nullptr, nullptr);

            if (type == 0x13)
            {
                GetU16(tag);
                GetU16(tag);
            }
        }
        else
        {
            Gradient g;

            GetGradient(tag, &g, /*alpha*/ num >= 3 ? 1 : 0);

            if (type == 0x13)
            {
                GetU16(tag);
            }
        }
    }
    else if (type == 0x40 || type == 0x41 || type == 0x42 || type == 0x43)
    {
        ResetReadBits(tag);

        if (tag->data[tag->pos] != 0xff ||
            tag->data[tag->pos + 1] != 0xff)
        {
            (callback)(tag, tag->pos, callback_data);
        }

        GetU16(tag);
        ResetReadBits(tag);
        GetMatrix(tag, nullptr);

        if (morph)
        {
            GetMatrix(tag, nullptr);
        }
    }
    else
    {
        fprintf(stderr, "Unknown fillStyle:0x%02x in tag %02d\n", type, tag->id);
    }
}

void SWFRender::EnumerateUsedIDsOfLineStyle(Tag* tag, int t, void (*callback)(Tag*, int, void*), void* callback_data, int num, int morph)
{
    RGBA color;
    unsigned short width = GetU16(tag);
    char fill = 0;

    if (morph)
    {
        GetU16(tag);
    }

    if (num >= 4)
    {
        unsigned short flags = GetU16(tag);

        if ((flags & 0x30) == 0x20)
        {
            unsigned short miter = GetU16(tag); // miter limit
        }

        if (flags & 0x08)
        {
            fill = 1;
        }
    }

    if (!fill)
    {
        if (num >= 3)
        {
            GetRGBA(tag, &color); if (morph) GetRGBA(tag, nullptr);
        }
        else
        {
            GetRGB(tag, &color); if (morph) GetRGB(tag, nullptr);
        }
    }
    else
    {
        EnumerateUsedIDsOfFillStyle(tag, t, callback, callback_data, num, morph);
    }
}

void SWFRender::GetMorphGradient(Tag* tag, Gradient* gradient1, Gradient* gradient2)
{
    int num = GetU8(tag) & 15;

    if (gradient1)
    {
        gradient1->num = num;
    }

    if (gradient2)
    {
        gradient2->num = num;
    }

    if (gradient1)
    {
        gradient1->num = num;
        gradient1->rgba = (RGBA*)calloc(1, sizeof(RGBA) * gradient1->num);
        gradient1->ratios = (unsigned char*)calloc(1, sizeof(gradient1->ratios[0]) * gradient1->num);
    }

    if (gradient2)
    {
        gradient2->num = num;
        gradient2->rgba = (RGBA*)calloc(1, sizeof(RGBA) * gradient2->num);
        gradient2->ratios = (unsigned char*)calloc(1, sizeof(gradient2->ratios[0]) * gradient2->num);
    }

    for (int t = 0; t < num; t++)
    {
        unsigned char ratio;
        RGBA color;

        ratio = GetU8(tag);
        GetRGBA(tag, &color);

        if (gradient1)
        {
            gradient1->ratios[t] = ratio;
            gradient1->rgba[t] = color;
        }

        ratio = GetU8(tag);
        GetRGBA(tag, &color);

        if (gradient2)
        {
            gradient2->ratios[t] = ratio;
            gradient2->rgba[t] = color;
        }
    }
}

RGBA* SWFRender::ExtractImage(Tag* tag, int* dwidth, int* dheight)
{
    RGBA* img;

    SetTagPos(tag, 2); // id is 2 bytes

    if (tag->id == ST_DEFINEBITSJPEG ||
        tag->id == ST_DEFINEBITSJPEG2 || tag->id == ST_DEFINEBITSJPEG3)
    {
#ifdef HAVE_JPEGLIB
        return JPEG2TagToImage(tag, dwidth, dheight);
#else
        fprintf(stderr, "rfxswf: Error: No JPEG library compiled in");

        return 0;
#endif
    }
    if (tag->id == ST_DEFINEBITSLOSSLESS ||
        tag->id == ST_DEFINEBITSLOSSLESS2)
    {
#ifdef HAVE_ZLIB
        return DefineLosslessBitsTagToImage(tag, dwidth, dheight);
#else
        fprintf(stderr, "rfxswf: Error: No JPEG library compiled in");

        return 0;
#endif
    }

    fprintf(stderr, "rfxswf: Error: Invalid tag (%d, %s)", tag->id, TagGetName(tag));

    return 0;
}

void SWFRender::AddImage(RenderBuffer* buf, unsigned short id, RGBA* img, int width, int height)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(buf->internal);
    Bitmap* bm = static_cast<Bitmap*>(calloc(1, sizeof(Bitmap)));

    bm->id = id;
    bm->width = width;
    bm->height = height;
    bm->data = static_cast<RGBA*>(calloc(1, width * height * 4));

    memcpy(bm->data, img, width * height * 4);

    bm->next = i->bitmaps;
    i->bitmaps = bm;
}

int SWFRender::FontExtract(Swf* swf, int id, SwfFont** font)
{
    if (!swf || !font)
    {
        return -1;
    }

    SwfFont* f = (SwfFont*)calloc(1, sizeof(SwfFont));
    Tag* t = swf->firstTag;

    while (t)
    {
        int nid = 0;
        switch (GetTagID(t))
        {
        case ST_DEFINEFONT:
            nid = DefineFont(id, f, t);
            break;

        case ST_DEFINEFONT2:
        case ST_DEFINEFONT3:
            nid = DefineFont2(id, f, t);
            break;

        case ST_DEFINEFONTALIGNZONES:
            nid = DefineFontAlignZones(id, f, t);
            break;

        case ST_DEFINEFONTINFO:
        case ST_DEFINEFONTINFO2:
            nid = DefineFontInfo(id, f, t);
            break;

        case ST_DEFINETEXT:
        case ST_DEFINETEXT2:
            if (!f->layout)
            {
                nid = DefineText(id, f, t, FEDTJ_MODIFY);
            }

            if (f->version >= 3 && f->layout)
            {
                FontUpdateUsage(f, t);
            }

            break;

        case ST_GLYPHNAMES:
            nid = GlyphNames(id, f, t);
            break;
        }

        if (nid > 0)
        {
            id = nid;
        }

        t = NextTag(t);
    }

    if (f->id != id)
    {
        free(f);

        f = 0;
    }

    font[0] = f;

    return 0;
}

int SWFRender::DefineFont(int id, SwfFont* f, Tag* t)
{
    unsigned short fid;
    SetTagPos(t, 0);

    fid = GetU16(t);

    if (!id || id == fid)
    {
        unsigned short of;
        int n, i;

        id = fid;
        f->version = 1;
        f->id = fid;

        of = GetU16(t);
        n = of / 2;
        f->numberOfChars = n;
        f->glyph = (SwfGlyph*)calloc(1, sizeof(SwfGlyph) * n);

        for (i = 1; i < n; i++)
        {
            GetU16(t);
        }

        for (i = 0; i < n; i++)
        {
            GetSimpleShape(t, &f->glyph[i].shape);
        }
    }

    return id;
}

int SWFRender::DefineFontInfo(int id, SwfFont* f, Tag* t)
{
    unsigned short fid;
    unsigned short maxcode;
    unsigned char flags;

    SetTagPos(t, 0);

    fid = GetU16(t);

    if (fid == id)
    {
        unsigned char l = GetU8(t);

        if (f->version > 1)
        {
            /* Especially with Flash MX, DefineFont2 may have FontInfo fields,
               too. However, they only add little information to what's already
               inside the DefineFont2 tag */
            return id;
        }

        if (f->name)
        {
            free(f->name);
        }

        f->name = static_cast<unsigned char*>(calloc(1, l + 1));
        GetBlock(t, f->name, l);
        f->name[l] = 0;

        flags = GetU8(t);

        if (flags & 2)
        {
            f->style |= FONT_STYLE_BOLD;
        }

        if (flags & 4)
        {
            f->style |= FONT_STYLE_ITALIC;
        }

        if (flags & 8)
        {
            f->encoding |= FONT_ENCODING_ANSI;
        }

        if (flags & 16)
        {
            f->encoding |= FONT_ENCODING_SHIFTJIS;
        }

        if (flags & 32)
        {
            f->encoding |= FONT_ENCODING_UNICODE;
        }

        if (t->id == ST_DEFINEFONTINFO2)
        {
            f->language = GetU8(t);
        }

        f->glyph2Ascii = static_cast<unsigned short*>(calloc(1, sizeof(unsigned short) * f->numberOfChars));
        maxcode = 0;

        for (int i = 0; i < f->numberOfChars; i++)
        {
            f->glyph2Ascii[i] = ((flags & FF_WIDECODES) ? GetU16(t) : GetU8(t));

            if (f->glyph2Ascii[i] > maxcode)
            {
                maxcode = f->glyph2Ascii[i];
            }
        }

        maxcode++;

        if (maxcode < 256)
        {
            maxcode = 256;
        }

        f->maxAscii = maxcode;
        f->ascii2Glyph = static_cast<int*>(calloc(1, sizeof(int) * maxcode));

        memset(f->ascii2Glyph, -1, sizeof(int) * maxcode);

        for (int i = 0; i < f->numberOfChars; i++)
        {
            f->ascii2Glyph[f->glyph2Ascii[i]] = i;
        }
    }

    return id;
}

int SWFRender::DefineFont2(int id, SwfFont* font, Tag* tag)
{
    int t, glyphcount;
    int maxcode;
    int fid;
    unsigned int offset_start;
    unsigned int* offset;
    unsigned char flags1, langcode, namelen;

    SetTagPos(tag, 0);

    font->version = tag->id == ST_DEFINEFONT3 ? 3 : 2;
    fid = GetU16(tag);

    if (id && id != fid)
    {
        return id;
    }

    font->id = fid;
    flags1 = GetU8(tag);
    langcode = GetU8(tag);	//reserved flags

    if (flags1 & 1)
    {
        font->style |= FONT_STYLE_BOLD;
    }

    if (flags1 & 2)
    {
        font->style |= FONT_STYLE_ITALIC;
    }

    if (flags1 & 16)
    {
        font->encoding |= FONT_ENCODING_ANSI;
    }

    if (flags1 & 32)
    {
        font->encoding |= FONT_ENCODING_UNICODE;
    }

    if (flags1 & 64)
    {
        font->encoding |= FONT_ENCODING_SHIFTJIS;
    }

    namelen = GetU8(tag);
    font->name = (unsigned char*)calloc(1, namelen + 1);
    font->name[namelen] = 0;

    GetBlock(tag, font->name, namelen);

    glyphcount = GetU16(tag);
    font->numberOfChars = glyphcount;

    font->glyph = static_cast<SwfGlyph*>(calloc(1, sizeof(SwfGlyph) * glyphcount));
    font->glyph2Ascii = static_cast<unsigned short*>(calloc(1, sizeof(unsigned short) * glyphcount));

    offset = static_cast<unsigned int*>(calloc(1, sizeof(unsigned int) * (glyphcount + 1)));
    offset_start = tag->pos;

    if (flags1 & 8)
    {		// wide offsets
        for (t = 0; t < glyphcount; t++)
        {
            offset[t] = GetU32(tag);	//offset[t]
        }

        if (glyphcount) //This _if_ is not in the specs
        {
            offset[glyphcount] = GetU32(tag); // fontcodeoffset
        }
        else
        {
            offset[glyphcount] = tag->pos;
        }
    }
    else
    {
        for (t = 0; t < glyphcount; t++)
            offset[t] = GetU16(tag);	//offset[t]

        if (glyphcount)		/* this _if_ is not in the specs */
            offset[glyphcount] = GetU16(tag);	// fontcodeoffset
        else
            offset[glyphcount] = tag->pos;
    }
    for (t = 0; t < glyphcount; t++)
    {
        SetTagPos(tag, offset[t] + offset_start);
        GetSimpleShape(tag, &(font->glyph[t].shape));
    }

    if (glyphcount)
        SetTagPos(tag, offset[glyphcount] + offset_start);

    free(offset);

    maxcode = 0;
    for (t = 0; t < glyphcount; t++)
    {
        int code;
        if (flags1 & 4)		// wide codes (always on for definefont3)
            code = GetU16(tag);
        else
            code = GetU8(tag);
        font->glyph2Ascii[t] = code;
        if (code > maxcode)
            maxcode = code;
    }
    maxcode++;
    if (maxcode < 256)
        maxcode = 256;
    font->maxAscii = maxcode;
    font->ascii2Glyph = static_cast<int*>(calloc(1, sizeof(int) * maxcode));
    memset(font->ascii2Glyph, -1, sizeof(int) * maxcode);
    for (t = 0; t < glyphcount; t++)
    {
        font->ascii2Glyph[font->glyph2Ascii[t]] = t;
    }

    if (flags1 & 128)
    {		// has layout
        unsigned short kerningCount;
        font->layout = static_cast<SwfLayout*>(calloc(1, sizeof(SwfLayout)));
        font->layout->ascent = GetU16(tag);
        font->layout->descent = GetU16(tag);
        font->layout->leading = GetU16(tag);
        for (t = 0; t < glyphcount; t++)
        {
            short advance = GetS16(tag);
            font->glyph[t].advance = advance;
        }
        font->layout->bounds = static_cast<SRect*>(calloc(1, glyphcount * sizeof(SRect)));
        for (t = 0; t < glyphcount; t++)
        {
            ResetReadBits(tag);
            GetRect(tag, &font->layout->bounds[t]);
            SRect b = font->layout->bounds[t];
            if ((b.xmin | b.xmax | b.ymin | b.ymax) == 0)
            {
                // recalculate bounding box
                Shape2* shape2 = ShapeToShape2(font->glyph[t].shape);
                font->layout->bounds[t] = GetShapeBoundingBox(shape2);
                Shape2Free(shape2); free(shape2);
            }
        }

        kerningCount = GetU16(tag);
        font->layout->kerningCount = kerningCount;

        font->layout->kerning = static_cast<SwfKerning*>(calloc(1, sizeof(SwfKerning) * kerningCount));
        if (kerningCount)
        {
            font->layout->kerning = static_cast<SwfKerning*>(calloc(1, sizeof(*font->layout->kerning) * kerningCount));
            for (t = 0; t < kerningCount; t++)
            {
                if (flags1 & 4)
                {	// wide codes
                    font->layout->kerning[t].char1 = GetU16(tag);
                    font->layout->kerning[t].char2 = GetU16(tag);
                }
                else
                {
                    font->layout->kerning[t].char1 = GetU8(tag);
                    font->layout->kerning[t].char2 = GetU8(tag);
                }
                font->layout->kerning[t].adjustment = GetS16(tag);
            }
        }
    }
    return font->id;
}

int SWFRender::DefineFontAlignZones(int id, SwfFont* font, Tag* tag)
{
    unsigned short fid;
    SetTagPos(tag, 0);
    fid = GetU16(tag);

    if (fid == id)
    {
        font->alignzoneFlags = GetU8(tag);
        font->alignZones = static_cast<AlignZone*>(calloc(1, sizeof(AlignZone) * font->numberOfChars));
        int i = 0;
        while (tag->pos < tag->len)
        {
            if (i >= font->numberOfChars)
                break;
            int nr = GetU8(tag); // should be 2
            if (nr != 1 && nr != 2)
            {
                fprintf(stderr, "rfxswf: Can't parse alignzone tags with %d zones", nr);
                break;
            }
            unsigned short x = GetU16(tag);
            unsigned short y = GetU16(tag);
            unsigned short dx = (nr == 2) ? GetU16(tag) : 0xffff;
            unsigned short dy = (nr == 2) ? GetU16(tag) : 0xffff;
            unsigned char xy = GetU8(tag);

#ifdef DEBUG_RFXSWF
            if ((!(xy & 1) && (x != 0 || (dx != 0 && dx != 0xffff))) ||
                (!(xy & 2) && (y != 0 || (dy != 0 && dy != 0xffff))))
            {
                fprintf(stderr, "Warning: weird combination of alignzone bits and values (%d x:%04x-%04x y:%04x-%04x)\n", xy,
                    x, dx, y, dy);
            }
#endif
            if (!(xy & 1))
            {
                x = 0xffff;
                dx = 0xffff;
            }
            else if (!(xy & 2))
            {
                y = 0xffff;
                dy = 0xffff;
            }
            font->alignZones[i].x = x;
            font->alignZones[i].y = y;
            font->alignZones[i].dx = dx;
            font->alignZones[i].dy = dy;
            i++;
        }
    }
    return id;
}

int SWFRender::DefineText(int id, SwfFont* f, Tag* t, int jobs)
{
    return DefineTextCallback(id, f, t, jobs, 0, 0);
}

int SWFRender::DefineTextCallback(int id, SwfFont* f, Tag* t, int jobs,
    void (*callback) (void* self, int* chars, int* xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color),
    void* self)
{
    unsigned short cid;
    SRect r;
    Matrix m;
    unsigned char gbits, abits;
    int fid = -1;
    RGBA color;
    int x = 0, y = 0;
    int fontsize = 0;

    memset(&color, 0, sizeof(color));

    SetTagPos(t, 0);

    cid = GetU16(t);
    GetRect(t, &r);
    GetMatrix(t, &m);
    gbits = GetU8(t);
    abits = GetU8(t);

    while (1)
    {
        int flags, num;
        flags = GetU8(t);
        if (!flags)
            break;

        if (flags & TF_TEXTCONTROL)
        {
            if (flags & TF_HASFONT)
                fid = GetU16(t);
            if (flags & TF_HASCOLOR)
            {
                color.r = GetU8(t);	// rgb
                color.g = GetU8(t);
                color.b = GetU8(t);
                if (GetTagID(t) == ST_DEFINETEXT2)
                    color.a = GetU8(t);
                else
                    color.a = 255;
            }
            if (flags & TF_HASXOFFSET)
                x = GetS16(t);
            if (flags & TF_HASYOFFSET)
                y = GetS16(t);
            if (flags & TF_HASFONT)
                fontsize = GetU16(t);
        }

        num = GetU8(t);
        if (!num)
            break;

        {
            int i;
            int buf[256];
            int advance[256];
            int xpos = 0;
            for (i = 0; i < num; i++)
            {
                int glyph;
                int adv = 0;
                advance[i] = xpos;
                glyph = GetBits(t, gbits);
                adv = GetBits(t, abits);
                xpos += adv;

                if (id == fid)
                {
                    if (jobs & FEDTJ_PRINT)
                    {
                        int code = f->glyph2Ascii[glyph];
                        printf("%lc", code);
                    }
                    if (jobs & FEDTJ_MODIFY)
                        f->glyph[glyph].advance = adv * 20;	//?
                }

                buf[i] = glyph;
            }
            if ((id == fid) && (jobs & FEDTJ_PRINT))
                printf("\n");
            if (jobs & FEDTJ_CALLBACK)
                callback(self, buf, advance, num, fid, fontsize, x, y, &color);
            x += xpos;
        }
    }

    return id;
}

int SWFRender::GlyphNames(int id, SwfFont* f, Tag* tag)
{
    unsigned short fid;
    SetTagPos(tag, 0);

    fid = GetU16(tag);

    if (fid == id)
    {
        int num = GetU16(tag);
        int t;
        f->glyphNames = static_cast<char**>(calloc(1, sizeof(char*) * num));
        for (t = 0; t < num; t++)
        {
            f->glyphNames[t] = _strdup(GetString(tag));
        }
    }
    return id;
}

void SWFRender::FontUpdateUsage(SwfFont* f, Tag* tag)
{
    UsageTmp u;
    u.font = f;
    u.lastx = 0 - 0x80000000;
    u.lasty = 0 - 0x80000000;
    u.last = 0;
    ParseDefineText(tag, UpdateUsage, &u);
}

int SWFRender::GetSimpleShape(Tag* t, Shape** s) // without Linestyle/Fillstyle Record
{
    Shape* sh;
    int bitl, len;
    int end;
    unsigned int pos;

    if (FAILED(ShapeNew(s))) return -1;
    sh = s[0];

    ResetReadBits(t);
    sh->bits.fill = static_cast<unsigned short>(GetBits(t, 4));
    sh->bits.line = static_cast<unsigned short>(GetBits(t, 4));
    bitl = 0; end = 0; pos = GetTagPos(t);

    while (!end)
    {
        int edge = GetBits(t, 1); bitl += 1;
        if (edge)
        {
            bitl += 1;
            if (GetBits(t, 1))                 // Line
            {
                unsigned short nbits = GetBits(t, 4) + 2;
                bitl += 5;

                if (GetBits(t, 1))               // x/y Line
                {
                    GetBits(t, nbits);
                    GetBits(t, nbits);
                    bitl += nbits * 2;
                }
                else                            // hline/vline
                {
                    GetBits(t, nbits + 1);
                    bitl += nbits + 1;
                }
            }
            else                              // Curve
            {
                unsigned short nbits = GetBits(t, 4) + 2;
                bitl += 4;

                GetBits(t, nbits);
                GetBits(t, nbits);
                GetBits(t, nbits);
                GetBits(t, nbits);

                bitl += 4 * nbits;
            }
        }
        else
        {
            unsigned short flags = GetBits(t, 5); bitl += 5;
            if (flags)
            {
                if (flags & SF_MOVETO)
                {
                    unsigned short nbits = GetBits(t, 5); bitl += 5;
                    GetBits(t, nbits);
                    GetBits(t, nbits);
                    bitl += 2 * nbits;
                }

                if (flags & SF_FILL0)
                {
                    GetBits(t, sh->bits.fill);
                    bitl += sh->bits.fill;
                }

                if (flags & SF_FILL1)
                {
                    GetBits(t, sh->bits.fill);
                    bitl += sh->bits.fill;
                }

                if (flags & SF_LINE)
                {
                    GetBits(t, sh->bits.line);
                    bitl += sh->bits.line;
                }

                if (flags & SF_NEWSTYLE)
                {
                    fprintf(stderr, "RFXSWF: Can't process extended styles in shape.\n");
                }
            }
            else end = 1;
        }
    }
    SetTagPos(t, pos);
    len = (bitl + 7) / 8;

    if (sh->data) free(sh->data);
    sh->data = static_cast<unsigned char*>(calloc(1, len));

    if (sh->data)
    {
        sh->bitLength = bitl;
        GetBlock(t, sh->data, len);
    }
    else return -1;

    return len;
}

Shape2* SWFRender::ShapeToShape2(Shape* shape)
{

    Shape2* shape2 = static_cast<Shape2*>(calloc(1, sizeof(Shape2)));

    shape2->numberOfLineStyles = shape->lineStyle.n;
    if (shape2->numberOfLineStyles)
    {
        shape2->lineStyles = static_cast<LineStyle*>(calloc(1, sizeof(LineStyle) * shape->lineStyle.n));
        memcpy(shape2->lineStyles, shape->lineStyle.data, sizeof(LineStyle) * shape->lineStyle.n);
    }

    shape2->numberOfFillstyles = shape->fillStyle.n;
    if (shape2->numberOfFillstyles)
    {
        shape2->fillStyles = static_cast<FillStyle*>(calloc(1, sizeof(FillStyle) * shape->fillStyle.n));
        memcpy(shape2->fillStyles, shape->fillStyle.data, sizeof(FillStyle) * shape->fillStyle.n);
    }

    shape2->lines = ParseShapeData(shape->data, shape->bitLength, shape->bits.fill, shape->bits.line, 1, 0);
    shape2->bbox = 0;
    return shape2;
}

SRect SWFRender::GetShapeBoundingBox(Shape2* shape2)
{
    SRect r;
    ShapeLine* l = shape2->lines;
    int lastx = 0, lasty = 0;
    int valid = 0;
    r.xmin = r.ymin = SCOORD_MAX;
    r.xmax = r.ymax = SCOORD_MIN;

    while (l)
    {
        int t1;
        if (l->lineStyle > 0)
        {
            t1 = shape2->lineStyles[l->lineStyle - 1].width * 3 / 2;
        }
        else
        {
            t1 = 0;
        }

        if (l->type == ShapeLineType::lineTo || l->type == ShapeLineType::splineTo)
        {
            valid = 1;
            if (lastx - t1 < r.xmin) r.xmin = lastx - t1;
            if (lasty - t1 < r.ymin) r.ymin = lasty - t1;
            if (lastx + t1 > r.xmax) r.xmax = lastx + t1;
            if (lasty + t1 > r.ymax) r.ymax = lasty + t1;
            if (l->x - t1 < r.xmin) r.xmin = l->x - t1;
            if (l->y - t1 < r.ymin) r.ymin = l->y - t1;
            if (l->x + t1 > r.xmax) r.xmax = l->x + t1;
            if (l->y + t1 > r.ymax) r.ymax = l->y + t1;
            if (l->type == ShapeLineType::splineTo)
            {
                if (l->sx - t1 < r.xmin) r.xmin = l->sx - t1;
                if (l->sy - t1 < r.ymin) r.ymin = l->sy - t1;
                if (l->sx + t1 > r.xmax) r.xmax = l->sx + t1;
                if (l->sy + t1 > r.ymax) r.ymax = l->sy + t1;
            }
        }
        lastx = l->x;
        lasty = l->y;
        l = l->next;
    }
    if (!valid) memset(&r, 0, sizeof(SRect));
    return r;
}

void SWFRender::Shape2Free(Shape2* s)
{
    ShapeLine* line = s->lines;
    s->lines = 0;
    while (line)
    {
        ShapeLine* next = line->next;
        line->next = 0;
        free(line);
        line = next;
    }

    if (s->lineStyles)
    {
        free(s->lineStyles);
        s->lineStyles = 0;
    }
    if (s->fillStyles)
    {
        free(s->fillStyles);
        s->fillStyles = 0;
    }
    if (s->bbox)
    {
        free(s->bbox);
        s->bbox = 0;
    }
}

char* SWFRender::GetString(Tag* t)
{
    int pos = t->pos;
    while (t->pos < t->len && GetU8(t));
    /* make sure we always have a trailing zero byte */
    if (t->pos == t->len)
    {
        if (t->len == t->memsize)
        {
            ResetWriteBits(t);
            SetU8(t, 0);
            t->len = t->pos;
        }
        t->data[t->len] = 0;
    }
    return reinterpret_cast<char*>(&(t->data[pos]));
}

int SWFRender::ShapeAddSolidFillStyle(Shape* s, RGBA* color)
{
    return ShapeAddFillStyle(s, FILL_SOLID, nullptr, color, 0, 0);
}

int SWFRender::ShapeAddFillStyle(Shape* s, unsigned char type, Matrix* m, RGBA* color, unsigned short idBitmap, Gradient* gradient)
{
    RGBA def_c;
    Matrix def_m;
    Gradient def_g;

    // handle defaults

    if (!s) return -1;
    if (!color)
    {
        color = &def_c;
        def_c.a = 0xff;
        def_c.r = def_c.g = def_c.b = 0;
    }
    if (!m)
    {
        m = &def_m;
        GetMatrix(nullptr, m);
    }
    if (!gradient)
    {
        gradient = &def_g;
        GetGradient(nullptr, gradient, 1);
    }

    // handle memory

    if (s->fillStyle.data)
    {
        FillStyle* xnew = static_cast<FillStyle*>(realloc(s->fillStyle.data, (s->fillStyle.n + 1) * sizeof(FillStyle)));
        if (!xnew) return -1;
        s->fillStyle.data = xnew;
    }
    else
    {
        s->fillStyle.data = static_cast<FillStyle*>(calloc(1, sizeof(FillStyle)));
        s->fillStyle.n = 0;
        if (!s->fillStyle.data) return -1;
    }

    // set fillStyle

    s->fillStyle.data[s->fillStyle.n].type = type;
    s->fillStyle.data[s->fillStyle.n].idBitmap = idBitmap;
    memcpy(&s->fillStyle.data[s->fillStyle.n].m, m, sizeof(Matrix));
    memcpy(&s->fillStyle.data[s->fillStyle.n].color, color, sizeof(RGBA));
    memcpy(&s->fillStyle.data[s->fillStyle.n].gradient, gradient, sizeof(Gradient));

    return (++s->fillStyle.n);
}

int SWFRender::ParseDefineText(Tag* tag,
    void (*callback) (void* self, int* chars, int* xpos, int nr, int fontid, int fontsize, int xstart, int ystart, RGBA* color), void* self)
{
    return DefineTextCallback(-1, 0, tag, FEDTJ_CALLBACK, callback, self);
}

void SWFRender::UpdateUsage(void* self, int* chars, int* xpos, int nr,
    int fontid, int fontsize, int xstart, int ystart, RGBA* color)
{
    UsageTmp* u = static_cast<UsageTmp*>(self);
    if (!u->font->use)
    {
        FontInitUsage(u->font);
    }
    if (fontid != u->font->id)
        return;

    int t;
    for (t = 0; t < nr; t++)
    {
        int x = xpos[t];
        int y = ystart;
        int c = chars[t];
        if (c<0 || c>u->font->numberOfChars)
            continue;
        FontUseGlyph(u->font, c, fontsize);
        if (u->lasty == y && x >= u->lastx - 200 && abs(u->lastx - x) < 200 &&
            u->last != c && !ShapeIsEmpty(u->font->glyph[u->last].shape) &&
            !ShapeIsEmpty(u->font->glyph[c].shape))
        {
            FontUsePair(u->font, u->last, c);
        }
        u->lasty = y;
        /* FIXME: do we still need to divide advance by 20 for definefont3? */
        u->lastx = x + (u->font->glyph[c].advance * fontsize / 20480);
        u->last = c;
    }
}

int SWFRender::FontInitUsage(SwfFont* f)
{
    if (!f)
        return -1;
    if (f->use)
    {
        fprintf(stderr, "Usage initialized twice");
        return -1;
    }
    f->use = static_cast<FontUsage*>(calloc(1, sizeof(FontUsage)));
    f->use->smallestSize = 0xffff;
    f->use->chars = (int*)calloc(1, sizeof(f->use->chars[0]) * f->numberOfChars);
    return 0;
}

int SWFRender::FontUseGlyph(SwfFont* f, int glyph, unsigned short size)
{
    if (!f->use)
        FontInitUsage(f);
    if (glyph < 0 || glyph >= f->numberOfChars)
        return -1;
    if (!f->use->chars[glyph])
        f->use->usedGlyphs++;
    f->use->chars[glyph] = 1;
    if (size && size < f->use->smallestSize)
        f->use->smallestSize = size;
    return 0;
}

char SWFRender::ShapeIsEmpty(Shape* s)
{
    if (!s || !s->data) return 1;
    Tag _tag;
    Tag* tag = &_tag;
    memset(tag, 0, sizeof(Tag));
    tag->data = s->data;
    tag->len = tag->memsize = (s->bitLength + 7) / 8;
    tag->pos = 0;

    while (1)
    {
        if (!GetBits(tag, 1))
        {
            unsigned short flags = GetBits(tag, 5);
            if (!flags) break;
            if (flags & 1)
            { //move
                int n = GetBits(tag, 5);
                GetSBits(tag, n); //x
                GetSBits(tag, n); //y
            }
            if (flags & 2) GetBits(tag, s->bits.fill);
            if (flags & 4) GetBits(tag, s->bits.fill);
            if (flags & 8) GetBits(tag, s->bits.line);
            if (flags & 16) { return 0; }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void SWFRender::FontUsePair(SwfFont* f, int char1, int char2)
{
    if (!f->use)
        FontInitUsage(f);
    FontUsage* u = f->use;

    if (u->numberOfNeighbors * 3 >= u->neighborsHashSize * 2)
    {
        if (u->neighborsHash)
        {
            free(u->neighborsHash);
        }
        u->neighborsHashSize = u->neighborsHashSize ? u->neighborsHashSize * 2 : 1024;
        u->neighborsHash = static_cast<int*>(calloc(1, u->neighborsHashSize * sizeof(int)));
        int t;
        for (t = 0; t < u->numberOfNeighbors; t++)
        {
            Hashadd(u, u->neighbors[t].char1, u->neighbors[t].char2, t);
        }
    }

    int nr = FontUseGetPair(f, char1, char2);
    if (!nr)
    {
        if (u->numberOfNeighbors == u->neighborsSize)
        {
            u->neighborsSize += 4096;
            u->neighbors = static_cast<SwfGlyphPair*>(realloc(u->neighbors, sizeof(SwfGlyphPair) * u->neighborsSize));
        }
        u->neighbors[u->numberOfNeighbors].char1 = char1;
        u->neighbors[u->numberOfNeighbors].char2 = char2;
        u->neighbors[u->numberOfNeighbors].num = 1;
        Hashadd(u, char1, char2, u->numberOfNeighbors);
        u->numberOfNeighbors++;
    }
    else
    {
        u->neighbors[nr - 1].num++;
    }
}

void SWFRender::Hashadd(FontUsage* u, int char1, int char2, int nr)
{
    unsigned hash = Hash2(char1, char2);
    while (1)
    {
        hash = hash % u->neighborsHashSize;
        if (!u->neighborsHash[hash])
        {
            u->neighborsHash[hash] = nr + 1;
            return;
        }
        hash++;
    }
}

unsigned SWFRender::Hash2(int char1, int char2)
{
    unsigned hash = char1 ^ (char2 << 8);
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

int SWFRender::FontUseGetPair(SwfFont* f, int char1, int char2)
{
    FontUsage* u = f->use;
    if (!u || !u->neighborsHashSize)
        return 0;
    unsigned hash = Hash2(char1, char2);
    while (1)
    {
        hash = hash % u->neighborsHashSize;
        int pos = u->neighborsHash[hash];
        if (!pos)
            return 0;
        if (pos &&
            u->neighbors[pos - 1].char1 == char1 &&
            u->neighbors[pos - 1].char2 == char2)
        {
            return pos;
        }
        hash++;
    }
}

void SWFRender::FontFree(SwfFont* f)
{
    int i;
    if (!f)
        return;

    if (f->glyph)
    {
        for (i = 0; i < f->numberOfChars; i++)
            if (f->glyph[i].shape)
            {
                ShapeFree(f->glyph[i].shape);
                f->glyph[i].shape = nullptr;
            }
        free(f->glyph);
        f->glyph = nullptr;
    }
    if (f->ascii2Glyph)
    {
        free(f->ascii2Glyph);
        f->ascii2Glyph = nullptr;
    }
    if (f->glyph2Ascii)
    {
        free(f->glyph2Ascii);
        f->glyph2Ascii = nullptr;
    }
    if (f->glyph2Glyph)
    {
        free(f->glyph2Glyph);
        f->glyph2Glyph = nullptr;
    }

    FontFreeName(f);
    FreeLayout(f);
    FreeGlyphNames(f);
    FreeUsage(f);
    FontFreeAlignZones(f);

    free(f);
}

void SWFRender::FreeGlyphNames(SwfFont* f)
{
    if (f->glyphNames)
    {
        int t;
        for (t = 0; t < f->numberOfChars; t++)
        {
            if (f->glyphNames[t])
            {
                free(f->glyphNames[t]);
                f->glyphNames[t] = 0;
            }
        }
        free(f->glyphNames);
        f->glyphNames = 0;
    }
}

void SWFRender::FreeUsage(SwfFont* f)
{
    if (f->use)
    {
        if (f->use->chars)
        {
            free(f->use->chars); f->use->chars = 0;
        }
        if (f->use->neighbors)
        {
            free(f->use->neighbors); f->use->neighbors = 0;
        }
        if (f->use->neighborsHash)
        {
            free(f->use->neighborsHash); f->use->neighborsHash = 0;
        }
        free(f->use); f->use = 0;
    }
}

void SWFRender::FreeLayout(SwfFont* f)
{
    if (f->layout)
    {
        LayoutFree(f->layout);
        f->layout = 0;
    }
}

void SWFRender::FontFreeName(SwfFont* f)
{
    if (f->name)
    {
        free(f->name);
        f->name = 0;
    }
}

void SWFRender::FontFreeAlignZones(SwfFont* f)
{
    if (f->alignZones)
        free(f->alignZones);
    f->alignZones = 0;
}

void SWFRender::LayoutFree(SwfLayout* l)
{
    if (l)
    {
        if (l->kerning)
            free(l->kerning);
        l->kerning = nullptr;
        if (l->bounds)
            free(l->bounds);
        l->bounds = nullptr;
    }
    free(l);
}

int SWFRender::ShapeNew(Shape** s)
{
    Shape* sh;
    if (!s) return -1;
    sh = static_cast<Shape*>(calloc(1, sizeof(Shape)));
    *s = sh;
    return 0;
}

void SWFRender::RenderFromTag(RenderBuffer* buf, Character* idtable, Tag* firstTag, Matrix* m)
{
    Tag* tag = 0;
    int numplacements = 0;
    SwfPlaceObject* placements;

    tag = firstTag;
    numplacements = 0;
    while (tag)
    {
        if (tag->id == ST_PLACEOBJECT ||
            tag->id == ST_PLACEOBJECT2)
        {
            numplacements++;
        }
        if (tag->id == ST_SHOWFRAME || tag->id == ST_END)
            break;
        tag = tag->next;
    }

    placements = static_cast<SwfPlaceObject*>(calloc(1, sizeof(SwfPlaceObject) * numplacements));
    numplacements = 0;

    tag = firstTag;
    while (tag)
    {
        if (IsPlaceTag(tag))
        {
            SwfPlaceObject p;
            GetPlaceObject(tag, &p);
            /* TODO: add move and deletion */
            placements[numplacements++] = p;
            PlaceObjectFree(&p); //dirty! but it only frees fields we don't use
        }
        if (tag->id == ST_SHOWFRAME || tag->id == ST_END)
            break;
        tag = tag->next;
    }

    qsort(placements, numplacements, sizeof(SwfPlaceObject), ComparePlacements);

    int t;
    for (t = 0; t < numplacements; t++)
    {
        SwfPlaceObject* p = &placements[t];
        int id = p->id;
        Matrix m2;
        MatrixJoin(&m2, m, &p->Matrix);

        if (!idtable[id].tag)
        {
            fprintf(stderr, "rfxswf: Id %d is unknown\n", id);
            continue;
        }

        if (idtable[id].type == CharacterType::shapeType)
        {
            //SRect sbbox = swf_TurnRect(*idtable[id].bbox, &p->Matrix);
            RenderShape(buf, idtable[id].obj.shape, &m2, &p->cxform, p->depth, p->clipdepth);
        }
        else if (idtable[id].type == CharacterType::spriteType)
        {
            UnFoldSprite(idtable[id].tag);
            RenderFromTag(buf, idtable, idtable[id].tag->next, &m2);
            FoldSprite(idtable[id].tag);
        }
        else if (idtable[id].type == CharacterType::textType)
        {
            Tag* tag = idtable[id].tag;
            TextCallbackBlock info;
            Matrix mt;

            SetTagPos(tag, 0);
            GetU16(tag);
            GetRect(tag, 0);
            GetMatrix(tag, &mt);
            MatrixJoin(&info.m, &m2, &mt);
            /*printf("Text Matrix:\n");
            swf_DumpMatrix(stdout, &m);
            printf("Placement Matrix:\n");
            swf_DumpMatrix(stdout, &p->Matrix);
            printf("Final Matrix:\n");
            swf_DumpMatrix(stdout, &info.m);*/

            info.idtable = idtable;
            info.depth = p->depth;
            info.cxform = &p->cxform;
            info.clipdepth = p->clipdepth;
            info.buf = buf;

            ParseDefineText(tag, TextCallback, &info);
        }
        else if (idtable[id].type == CharacterType::editTextType)
        {
            Tag* tag = idtable[id].tag;
            unsigned short flags = GetBits(tag, 16);
            if (flags & ET_HASTEXT)
            {
                fprintf(stderr, "edittext not supported yet (id %d)\n", id);
            }
        }
        else
        {
            fprintf(stderr, "Unknown/Unsupported Object Type for id %d: %s\n", id, TagGetName(idtable[id].tag));
        }
    }

    free(placements);
}

unsigned char SWFRender::IsPlaceTag(Tag* tag)
{
    if (tag->id == ST_PLACEOBJECT ||
        tag->id == ST_PLACEOBJECT2 ||
        tag->id == ST_PLACEOBJECT3)
        return 1;
    return 0;
}

void SWFRender::GetPlaceObject(Tag* tag, SwfPlaceObject* obj)
{
    if (obj)
        memset(obj, 0, sizeof(SwfPlaceObject));

    if (!tag)
    {
        GetMatrix(0, &obj->Matrix);
        GetCXForm(0, &obj->cxform, 1);
        //obj->internal = PF_CHAR|PF_MATRIX|PF_CXFORM;
        return;
    }
    SetTagPos(tag, 0);

    if (tag->id == ST_PLACEOBJECT)
    {
        obj->id = GetU16(tag);
        obj->depth = GetU16(tag);
        GetMatrix(tag, &obj->Matrix);
        GetCXForm(tag, &obj->cxform, 0);
        //obj->internal = PF_CHAR|PF_MATRIX|PF_CXFORM;
    }
    else if (tag->id == ST_PLACEOBJECT2 || tag->id == ST_PLACEOBJECT3)
    {
        unsigned char flags, flags2 = 0;
        flags = GetU8(tag);
        if (tag->id == ST_PLACEOBJECT3)
            flags2 = GetU8(tag);
        memset(obj, 0, sizeof(SwfPlaceObject));

        GetMatrix(0, &obj->Matrix);
        GetCXForm(0, &obj->cxform, 1);

        obj->flags = flags;
        obj->depth = GetU16(tag);
        //obj->internal = flags;
        if (flags & PF_MOVE) obj->move = 1;
        if (flags & PF_CHAR) obj->id = GetU16(tag);
        if (flags & PF_MATRIX) GetMatrix(tag, &obj->Matrix);
        if (flags & PF_CXFORM) GetCXForm(tag, &obj->cxform, 1);
        if (flags & PF_RATIO) obj->ratio = GetU16(tag);
        /* if you modify the order of these operations, also
           modify it in ../src/swfcombine.c */
        if (flags & PF_CLIPDEPTH)
            obj->clipdepth = GetU16(tag); //clip
        if (flags & PF_NAME)
        {
            int l, t;
            unsigned char* data;
            ResetReadBits(tag);

            l = strlen((const char*)&tag->data[tag->pos]);
            t = 0;
            data = static_cast<unsigned char*>(calloc(1, l + 1));
            obj->name = reinterpret_cast<char*>(data);

            while ((data[t++] = GetU8(tag)));
        }
        if (flags2 & PF2_BLENDMODE)
        {
            obj->blendmode = GetU8(tag);
        }

        /* Actionscript ignored (for now) */
        obj->actions = 0;
    }
    else
    {
        fprintf(stderr, "rfxswf: Bad Tag: %d not a placeobject\n", tag->id);
    }
}

void SWFRender::PlaceObjectFree(SwfPlaceObject* obj)
{
    if (obj->name)
    {
        free(obj->name);
    }
}

int SWFRender::GetCXForm(Tag* t, CXForm* cx, unsigned char alpha)
{
    CXForm cxf;
    int hasadd;
    int hasmul;
    int nbits;

    if (!cx) cx = &cxf;

    cx->a0 = cx->r0 = cx->g0 = cx->b0 = 256;
    cx->a1 = cx->r1 = cx->g1 = cx->b1 = 0;

    if (!t)
    {
        return 0;
    }

    ResetReadBits(t);
    hasadd = GetBits(t, 1);
    hasmul = GetBits(t, 1);
    nbits = GetBits(t, 4);

    if (hasmul)
    {
        cx->r0 = static_cast<short>(GetSBits(t, nbits));
        cx->g0 = static_cast<short>(GetSBits(t, nbits));
        cx->b0 = static_cast<short>(GetSBits(t, nbits));
        if (alpha)
            cx->a0 = static_cast<short>(GetSBits(t, nbits));
    }

    if (hasadd)
    {
        cx->r1 = static_cast<short>(GetSBits(t, nbits));
        cx->g1 = static_cast<short>(GetSBits(t, nbits));
        cx->b1 = static_cast<short>(GetSBits(t, nbits));
        if (alpha)
            cx->a1 = static_cast<short>(GetSBits(t, nbits));
    }

    return 0;
}

Matrix* SWFRender::MatrixJoin(Matrix* d, Matrix* s1, Matrix* s2)
{
    if (!d)
    {
        return nullptr;
    }

    if (!s1)
    {
        return (s2) ? (Matrix*)memcpy(d, s2, sizeof(Matrix)) : nullptr;
    }

    if (!s2)
    {
        return (Matrix*)memcpy(d, s1, sizeof(Matrix));
    }

    d->tx = s1->tx + RFXSWF_SP(s1->sx, s1->r1, s2->tx, s2->ty);
    d->ty = s1->ty + RFXSWF_SP(s1->r0, s1->sy, s2->tx, s2->ty);

    d->sx = RFXSWF_SP(s1->sx, s1->r1, s2->sx, s2->r0);
    d->r0 = RFXSWF_SP(s1->r0, s1->sy, s2->sx, s2->r0);

    d->r1 = RFXSWF_SP(s1->sx, s1->r1, s2->r1, s2->sy);
    d->sy = RFXSWF_SP(s1->r0, s1->sy, s2->r1, s2->sy);

    //DumpMatrix(nullptr,d);

    return d;
}

int SWFRender::RFXSWF_SP(int a1, int a2, int b1, int b2)
{
    long long a = (static_cast<long long>(a1) * static_cast<long long>(b1) + static_cast<long long>(a2) * static_cast<long long>(b2)) >> 16;
    int result = static_cast<int>(a);

    if (a != result)
    {
        fprintf(stderr, "Warning: overflow in Matrix multiplication\n");
    }

    return result;
}

void SWFRender::RenderShape(RenderBuffer* dest, Shape2* shape, Matrix* m, CXForm* c, unsigned short _depth, unsigned short _clipdepth)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);

    ShapeLine* line;
    int x = 0, y = 0;
    Matrix mat = *m;
    Shape2* s2 = 0;
    Shape2* lshape = 0;
    RenderPoint p, lp;
    unsigned int clipdepth;
    double widthmultiply = MatrixSize(m);

    memset(&p, 0, sizeof(RenderPoint));
    memset(&lp, 0, sizeof(RenderPoint));

    clipdepth = _clipdepth ? _clipdepth << 16 | 0xffff : 0;
    p.depth = _depth << 16;

    mat.tx -= dest->posx * 20;
    mat.ty -= dest->posy * 20;

    s2 = Shape2Clone(shape);
    line = s2->lines;
    if (shape->numberOfFillstyles)
    {
        int t;
        p.s = s2;
        /* multiply fillStyles matrices with placement Matrix-
           important for texture and gradient fill */
        for (t = 0; t < s2->numberOfFillstyles; t++)
        {
            Matrix nm;
            MatrixJoin(&nm, &mat, &s2->fillStyles[t].m);
            /*nm.sx *= i->multiply;
            nm.sy *= i->multiply;
            nm.r0 *= i->multiply;
            nm.r1 *= i->multiply;
            nm.tx *= i->multiply;
            nm.ty *= i->multiply;*/
            s2->fillStyles[t].m = nm;
        }
    }

    if (shape->numberOfLineStyles)
    {
        lshape = LineStyle2FillStyle(shape);
        lp.s = lshape;
        lp.depth = (_depth << 16) + 1;
    }


    while (line)
    {
        int x1, y1, x2, y2, x3, y3;

        if (line->type == ShapeLineType::moveTo)
        {
        }
        else if (line->type == ShapeLineType::lineTo)
        {
            TransformPoint(&mat, x, y, &x1, &y1);
            TransformPoint(&mat, line->x, line->y, &x3, &y3);

            if (line->lineStyle && !clipdepth)
            {
                lp.shapeLine = &lshape->lines[line->lineStyle - 1];
                AddSolidLine(dest, x1, y1, x3, y3, shape->lineStyles[line->lineStyle - 1].width * widthmultiply, &lp);
                lp.depth++;
            }
            if (line->fillstyle0 || line->fillstyle1)
            {
                assert(shape->numberOfFillstyles);
                p.shapeLine = line;
                AddLine(dest, x1, y1, x3, y3, &p);
            }
        }
        else if (line->type == ShapeLineType::splineTo)
        {
            int c, t, parts, qparts;
            double xx, yy;

            TransformPoint(&mat, x, y, &x1, &y1);
            TransformPoint(&mat, line->sx, line->sy, &x2, &y2);
            TransformPoint(&mat, line->x, line->y, &x3, &y3);

            c = abs(x3 - 2 * x2 + x1) + abs(y3 - 2 * y2 + y1);
            xx = x1;
            yy = y1;

            parts = static_cast<int>(sqrt(static_cast<float>(c)) / 3);
            if (!parts) parts = 1;

            for (t = 1; t <= parts; t++)
            {
                double nx = static_cast<double>(t * t * x3 + 2 * t * (parts - t) * x2 + (parts - t) * (parts - t) * x1) / static_cast<double>(parts * parts);
                double ny = static_cast<double>(t * t * y3 + 2 * t * (parts - t) * y2 + (parts - t) * (parts - t) * y1) / static_cast<double>(parts * parts);

                if (line->lineStyle && !clipdepth)
                {
                    lp.shapeLine = &lshape->lines[line->lineStyle - 1];
                    AddSolidLine(dest, xx, yy, nx, ny, shape->lineStyles[line->lineStyle - 1].width * widthmultiply, &lp);
                    lp.depth++;
                }
                if (line->fillstyle0 || line->fillstyle1)
                {
                    assert(shape->numberOfFillstyles);
                    p.shapeLine = line;
                    AddLine(dest, xx, yy, nx, ny, &p);
                }

                xx = nx;
                yy = ny;
            }
        }
        x = line->x;
        y = line->y;
        line = line->next;
    }

    Process(dest, clipdepth);

    if (s2)
    {
        Shape2Free(s2); free(s2); s2 = 0;
    }
    if (lshape)
    {
        Shape2Free(lshape); free(lshape); lshape = 0;
    }
}

double SWFRender::MatrixSize(Matrix* m)
{
    double l1 = sqrt((m->sx / 65536.0) * (m->sx / 65536.0) + (m->r0 / 65536.0) * (m->r0 / 65536.0));
    double l2 = sqrt((m->r1 / 65536.0) * (m->r1 / 65536.0) + (m->sy / 65536.0) * (m->sy / 65536.0));

    return sqrt(l1 * l2);
}

Shape2* SWFRender::Shape2Clone(Shape2* s)
{
    ShapeLine* line = s->lines;
    ShapeLine* prev = 0;
    Shape2* s2 = static_cast<Shape2*>(calloc(1, sizeof(Shape2)));

    memcpy(s2, s, sizeof(Shape2));
    s2->lineStyles = static_cast<LineStyle*>(calloc(1, sizeof(LineStyle) * s->numberOfLineStyles));
    memcpy(s2->lineStyles, s->lineStyles, sizeof(LineStyle) * s->numberOfLineStyles);
    s2->fillStyles = static_cast<FillStyle*>(calloc(1, sizeof(FillStyle) * s->numberOfFillstyles));
    memcpy(s2->fillStyles, s->fillStyles, sizeof(FillStyle) * s->numberOfFillstyles);

    while (line)
    {
        ShapeLine* line2 = static_cast<ShapeLine*>(calloc(1, sizeof(ShapeLine)));

        memcpy(line2, line, sizeof(ShapeLine));
        line2->next = 0;

        if (prev)
        {
            prev->next = line2;
        }
        else
        {
            s2->lines = line2;
        }

        prev = line2;
        line = line->next;
    }

    if (s->bbox)
    {
        s2->bbox = static_cast<SRect*>(calloc(1, sizeof(SRect)));

        memcpy(s2->bbox, s->bbox, sizeof(SRect));
    }

    return s2;
}

Shape2* SWFRender::LineStyle2FillStyle(Shape2* shape)
{
    Shape2* s = static_cast<Shape2*>(calloc(1, sizeof(Shape2)));

    s->numberOfFillstyles = shape->numberOfLineStyles;
    s->fillStyles = static_cast<FillStyle*>(calloc(1, sizeof(FillStyle) * shape->numberOfLineStyles));
    s->lines = static_cast<ShapeLine*>(calloc(1, sizeof(ShapeLine) * shape->numberOfLineStyles));

    for (int t = 0; t < shape->numberOfLineStyles; t++)
    {
        s->lines[t].fillstyle0 = t + 1;
        s->fillStyles[t].type = FILL_SOLID;
        s->fillStyles[t].color = shape->lineStyles[t].color;
    }

    return s;
}

void SWFRender::TransformPoint(Matrix* m, int x, int y, int* dx, int* dy)
{
    SPoint p, d;

    p.x = x;
    p.y = y;

    d = TurnPoint(p, m);

    *dx = d.x;
    *dy = d.y;
}

SPoint SWFRender::TurnPoint(SPoint p, Matrix* m)
{
    SPoint r;

    r.x = (int)(m->sx * (1 / 65536.0) * p.x + m->r1 * (1 / 65536.0) * p.y + 0.5) + m->tx;
    r.y = (int)(m->r0 * (1 / 65536.0) * p.x + m->sy * (1 / 65536.0) * p.y + 0.5) + m->ty;

    return r;
}

void SWFRender::AddSolidLine(RenderBuffer* buf, double x1, double y1, double x2, double y2, double width, RenderPoint* p)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(buf->internal);

    double dx = x2 - x1;
    double dy = y2 - y1;
    double sd;
    double d;

    int t;
    int segments;
    double lastx, lasty;
    double vx, vy;
    double xx, yy;

    /* Make sure the line is always at least one pixel wide */
#ifdef LINEMODE1
    /* That's what Macromedia's Player does at least at zoom level >= 1.  */
    width += 20;
#else
    /* That's what Macromedia's Player seems to do at zoom level 0.  */
    /* TODO: needs testing */

    /* TODO: how does this interact with scaling? */
    if (width * i->multiply < 20)
    {
        width = 20 / i->multiply;
    }
#endif

    sd = static_cast<double>(dx) * static_cast<double>(dx) + static_cast<double>(dy) * static_cast<double>(dy);
    d = sqrt(sd);

    if (!dx && !dy)
    {
        vx = 1;
        vy = 0;
    }
    else
    {
        vx = (dy / d);
        vy = (-dx / d);
    }

    segments = static_cast<int>(width / 2);

    if (segments < 2)
    {
        segments = 2;
    }

    segments = 8;

    vx = vx * width * 0.5;
    vy = vy * width * 0.5;

    xx = x2 + vx;
    yy = y2 + vy;

    AddLine(buf, x1 + vx, y1 + vy, xx, yy, p);

    lastx = xx;
    lasty = yy;

    for (t = 1; t < segments; t++)
    {
        double s = sin(t * std::numbers::pi / segments);
        double c = cos(t * std::numbers::pi / segments);

        xx = (x2 + vx * c - vy * s);
        yy = (y2 + vx * s + vy * c);

        AddLine(buf, lastx, lasty, xx, yy, p);

        lastx = xx;
        lasty = yy;
    }

    xx = (x2 - vx);
    yy = (y2 - vy);

    AddLine(buf, lastx, lasty, xx, yy, p);

    lastx = xx;
    lasty = yy;
    xx = (x1 - vx);
    yy = (y1 - vy);

    AddLine(buf, lastx, lasty, xx, yy, p);

    lastx = xx;
    lasty = yy;

    for (t = 1; t < segments; t++)
    {
        double s = sin(t * std::numbers::pi / segments);
        double c = cos(t * std::numbers::pi / segments);

        xx = (x1 - vx * c + vy * s);
        yy = (y1 - vx * s - vy * c);

        AddLine(buf, lastx, lasty, xx, yy, p);

        lastx = xx;
        lasty = yy;
    }

    AddLine(buf, lastx, lasty, (x1 + vx), (y1 + vy), p);
}

void SWFRender::AddLine(RenderBuffer* buf, double x1, double y1, double x2, double y2, RenderPoint* p)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(buf->internal);
    double diffx, diffy;
    double ny1, ny2, stepx;

    assert(p->shapeLine);

    y1 = y1 * i->multiply;
    y2 = y2 * i->multiply;
    x1 = x1 * i->multiply;
    x2 = x2 * i->multiply;

    y1 = y1 / 20.0;
    y2 = y2 / 20.0;
    x1 = x1 / 20.0;
    x2 = x2 / 20.0;

    if (y2 < y1)
    {
        double x;
        double y;

        x = x1; x1 = x2; x2 = x;
        y = y1; y1 = y2; y2 = y;
    }

    diffx = x2 - x1;
    diffy = y2 - y1;

    ny1 = INT(y1) + CUT;
    ny2 = INT(y2) + CUT;

    if (ny1 < y1)
    {
        ny1 = INT(y1) + 1.0 + CUT;
    }

    if (ny2 >= y2)
    {
        ny2 = INT(y2) - 1.0 + CUT;
    }

    if (ny1 > ny2)
    {
        return;
    }

    stepx = diffx / diffy;
    x1 = x1 + (ny1 - y1) * stepx;
    x2 = x2 + (ny2 - y2) * stepx;

    int posy = INT(ny1);
    int endy = INT(ny2);
    double posx = 0;
    double startx = x1;

    while (posy <= endy)
    {
        float xx = static_cast<float>(startx + posx);

        AddPixel(buf, xx, posy, p);

        posx += stepx;
        posy++;
    }
}

void SWFRender::AddPixel(RenderBuffer* dest, float x, int y, RenderPoint* p)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);

    if (x >= i->width2 || y >= i->height2 || y < 0)
    {
        return;
    }

    p->x = x;

    if (y < i->ymin)
    {
        i->ymin = y;
    }

    if (y > i->ymax)
    {
        i->ymax = y;
    }

    i->lines[y].num++;

    SetBlock(i->lines[y].points, reinterpret_cast<unsigned char*>(p), sizeof(RenderPoint));
}

void SWFRender::Process(RenderBuffer* dest, unsigned int clipdepth)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);
    int y;

    if (i->ymax < i->ymin)
    {
        /* shape is empty. return.
           only, if it's a clipshape, remember the clipdepth */
        if (clipdepth)
        {
            for (y = 0; y < i->height2; y++)
            {
                if (clipdepth > i->lines[y].pendingClipdepth)
                {
                    i->lines[y].pendingClipdepth = clipdepth;
                }
            }
        }

        return; //nothing (else) to do
    }

    if (clipdepth)
    {
        /* lines outside the clip shape are not filled
           immediately, only the highest clipdepth so far is
           stored there. They will be clipfilled once there's
           actually something about to happen in that line */
        for (y = 0; y < i->ymin; y++)
        {
            if (clipdepth > i->lines[y].pendingClipdepth)
            {
                i->lines[y].pendingClipdepth = clipdepth;
            }
        }

        for (y = i->ymax + 1; y < i->height2; y++)
        {
            if (clipdepth > i->lines[y].pendingClipdepth)
            {
                i->lines[y].pendingClipdepth = clipdepth;
            }
        }
    }

    for (y = i->ymin; y <= i->ymax; y++)
    {
        int n;
        Tag* tag = i->lines[y].points;
        int num = i->lines[y].num;
        RenderPoint* points = reinterpret_cast<RenderPoint*>(tag->data);
        RGBA* line = &i->img[i->width2 * y];
        int* zline = &i->zbuf[i->width2 * y];
        int lastx = 0;
        State fillstate;

        memset(&fillstate, 0, sizeof(State));
        qsort(points, num, sizeof(RenderPoint), CompareRenderPoints);
        /* resort points */
        /*if(y==884) {
            for(n=0;n<num;n++) {
            printf("%f (%d/%d) %d\n", points[n].x,
                points[n].shapeLine->fillstyle0,
                points[n].shapeLine->fillstyle1,
                points[n].shapeLine->lineStyle);
            }
        }*/

        if (i->lines[y].pendingClipdepth && !clipdepth)
        {
            FillClip(line, zline, y, 0, i->width2, i->lines[y].pendingClipdepth);
            i->lines[y].pendingClipdepth = 0;
        }

        for (n = 0; n < num; n++)
        {
            RenderPoint* p = &points[n];
            RenderPoint* next = n < num - 1 ? &points[n + 1] : 0;
            int startx = static_cast<int>(p->x);
            int endx = static_cast<int>(next ? next->x : i->width2);

            if (endx > i->width2)
            {
                endx = i->width2;
            }

            if (startx < 0)
            {
                startx = 0;
            }

            if (endx < 0)
            {
                endx = 0;
            }

            if (clipdepth)
            {
                /* for clipping, the inverse is filled
                   TODO: lastx!=startx only at the start of the loop,
                         so this might be moved up
                 */
                FillClip(line, zline, y, lastx, startx, clipdepth);
            }

            ChangeState(y, &fillstate, p);

            Fill(dest, line, zline, y, startx, endx, &fillstate, clipdepth);
            /*	    if(y == 0 && startx == 232 && endx == 418) {
                    printf("ymin=%d ymax=%d\n", i->ymin, i->ymax);
                    for(n=0;n<num;n++) {
                        RenderPoint*p = &points[n];
                        printf("x=%f depth=%08x\n", p->x, p->depth);
                    }
                    }*/

            lastx = endx;

            if (endx == i->width2)
            {
                break;
            }
        }

        if (clipdepth)
        {
            /* TODO: is lastx *ever* != i->width2 here? */
            FillClip(line, zline, y, lastx, i->width2, clipdepth);
        }

        FreeLayers(&fillstate);

        i->lines[y].num = 0;

        ClearTag(i->lines[y].points);
    }

    i->ymin = 0x7fffffff;
    i->ymax = 0 - 0x80000000;
}

void SWFRender::ChangeState(int y, State* state, RenderPoint* p)
{
    Layer* before = 0, * self = 0, * after = 0;

    SearchLayer(state, p->depth, &before, &self, &after);

    if (self)
    {
        /* shape update */
        if (self->fillID < 0/*??*/ || !p->shapeLine->fillstyle0 || !p->shapeLine->fillstyle1)
        {
            /* filling ends */

            DeleteLayer(state, self);
        }
        else
        {
            /*both fill0 and fill1 are set- exchange the two, updating the layer */
            if (self->fillID == p->shapeLine->fillstyle0)
            {
                self->fillID = p->shapeLine->fillstyle1;
                self->p = p;
            }
            else if (self->fillID == p->shapeLine->fillstyle1)
            {
                self->fillID = p->shapeLine->fillstyle0;
                self->p = p;
            }
        }

        return;
    }
    else
    {
        Layer* n = 0;

        if (p->shapeLine && p->shapeLine->fillstyle0 && p->shapeLine->fillstyle1)
        {
            /* this is a hack- a better way would be to make sure that
               we always get (0,32), (32, 33), (33, 0) in the right order if
               they happen to fall on the same pixel.
               (not: (0,32), (33, 0), (32, 33))
           Notice: Weird fill styles appear if lineStyles are involved, too.
            */
            fprintf(stderr, "<line %d: both fillStyles set while not inside shape>\n", y);

            return;
        }

        n = static_cast<Layer*>(calloc(1, sizeof(Layer)));

        n->fillID = p->shapeLine->fillstyle0 ? p->shapeLine->fillstyle0 : p->shapeLine->fillstyle1;
        n->p = p;

        AddLayer(state, before, n);
    }
}

void SWFRender::SearchLayer(State* state, int depth, Layer** before, Layer** self, Layer** after)
{
    Layer* last = 0, * l = state->layers;

    while (l && l->p->depth < depth)
    {
        last = l;
        l = l->next;
    }

    *before = last;

    if (l && l->p->depth == depth)
    {
        *self = l;
    }
    else
    {
        *after = l;
    }
}

void SWFRender::DeleteLayer(State* state, Layer* todel)
{
    Layer* before = todel->prev;
    Layer* next = todel->next;

    free(todel);

    if (!before)
    {
        state->layers = next;

        if (next)
        {
            next->prev = 0;
        }
    }
    else
    {
        before->next = next;

        if (before->next)
        {
            before->next->prev = before;
        }
    }
}

void SWFRender::AddLayer(State* state, Layer* before, Layer* toadd)
{
    if (!before)
    {
        toadd->next = state->layers;
        toadd->prev = 0;
        state->layers = toadd;
    }
    else
    {
        toadd->next = before->next;
        toadd->prev = before;
        before->next = toadd;
    }

    if (toadd->next)
    {
        toadd->next->prev = toadd;
    }
}

void SWFRender::FreeLayers(State* state)
{
    Layer* l = state->layers;

    while (l)
    {
        Layer* next = l->next;

        free(l);

        l = next;
    }
}

void SWFRender::Fill(RenderBuffer* dest, RGBA* line, int* zline, int y, int x1, int x2, State* fillstate, unsigned int clipdepth)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);
    int clip = 1;

    Layer* l = fillstate->layers;

    if (x1 >= x2) //Zero width? nothing to do.
    {
        return;
    }

    while (l)
    {
        if (l->fillID == 0)
        {
            /* not filled. TODO: we should never add those in the first place */
        }
        else if (l->fillID > l->p->s->numberOfFillstyles)
        {
            fprintf(stderr, "Fill style out of bounds (%d>%d)", l->fillID, l->p->s->numberOfLineStyles);
        }
        else if (clipdepth)
        {
            /* filled region- not used for clipping */
            clip = 0;
        }
        else
        {
            FillStyle* f;

            f = &l->p->s->fillStyles[l->fillID - 1];

            if (f->type == FILL_SOLID)
            {
                /* plain color fill */
                FillSolid(line, zline, y, x1, x2, f->color, l->p->depth);
            }
            else if (f->type == FILL_TILED || f->type == FILL_CLIPPED || f->type == (FILL_TILED | 2) || f->type == (FILL_CLIPPED | 2))
            {
                /* TODO: optimize (do this in add_pixel()?) */
                Bitmap* b = i->bitmaps;

                while (b && b->id != f->idBitmap)
                {
                    b = b->next;
                }

                if (!b)
                {
                    fprintf(stderr, "Shape references unknown bitmap %d\n", f->idBitmap);
                    FillSolid(line, zline, y, x1, x2, colorRed, l->p->depth);
                }
                else
                {
                    FillBitmap(line, zline, y, x1, x2, &f->m, b, /*clipped?*/f->type & 1, l->p->depth, i->multiply);
                }
            }
            else if (f->type == FILL_LINEAR || f->type == FILL_RADIAL)
            {
                FillGradient(line, zline, y, x1, x2, &f->m, &f->gradient, f->type, l->p->depth, i->multiply);
            }
            else
            {
                fprintf(stderr, "Undefined fillmode: %02x\n", f->type);
            }
        }

        l = l->next;
    }

    if (clip && clipdepth)
    {
        FillClip(line, zline, y, x1, x2, clipdepth);
    }
}

void SWFRender::FillClip(RGBA* line, int* z, int y, int x1, int x2, unsigned int depth)
{
    int x = x1;

    if (x1 >= x2)
    {
        return;
    }

    do
    {
        if (depth > z[x])
        {
            z[x] = depth;
        }
    }
    while (++x < x2);
}

void SWFRender::FillSolid(RGBA* line, int* z, int y, int x1, int x2, RGBA col, unsigned int depth)
{
    int x = x1;

    if (col.a != 255)
    {
        int ainv = 255 - col.a;

        col.r = (col.r * col.a) >> 8;
        col.g = (col.g * col.a) >> 8;
        col.b = (col.b * col.a) >> 8;
        col.a = 255;

        do
        {
            if (depth >= z[x])
            {
                line[x].r = ((line[x].r * ainv) >> 8) + col.r;
                line[x].g = ((line[x].g * ainv) >> 8) + col.g;
                line[x].b = ((line[x].b * ainv) >> 8) + col.b;
                line[x].a = 255;
                z[x] = depth;
            }
        }
        while (++x < x2);
    }
    else
    {
        do
        {
            if (depth >= z[x])
            {
                line[x] = col;
                z[x] = depth;
            }
        }
        while (++x < x2);
    }
}

void SWFRender::FillBitmap(RGBA* line, int* z, int y, int x1, int x2, Matrix* m, Bitmap* b, int clipbitmap, unsigned int depth, double fmultiply)
{
    int x = x1;

    double m11 = m->sx * fmultiply / 65536.0, m21 = m->r1 * fmultiply / 65536.0;
    double m12 = m->r0 * fmultiply / 65536.0, m22 = m->sy * fmultiply / 65536.0;
    double rx = m->tx * fmultiply / 20.0;
    double ry = m->ty * fmultiply / 20.0;

    double det = m11 * m22 - m12 * m21;
    if (fabs(det) < 0.0005)
    {
        /* x direction equals y direction- the image is invisible */
        return;
    }
    det = 20.0 / det;

    if (!b->width || !b->height)
    {
        FillSolid(line, z, y, x1, x2, colorRed, depth);
        return;
    }

    do
    {
        if (depth >= z[x])
        {
            RGBA col;
            int xx = (int)(((x - rx) * m22 - (y - ry) * m21) * det);
            int yy = (int)((-(x - rx) * m12 + (y - ry) * m11) * det);
            int ainv;

            if (clipbitmap)
            {
                if (xx < 0) xx = 0;
                if (xx >= b->width) xx = b->width - 1;
                if (yy < 0) yy = 0;
                if (yy >= b->height) yy = b->height - 1;
            }
            else
            {
                xx %= b->width;
                yy %= b->height;
                if (xx < 0) xx += b->width;
                if (yy < 0) yy += b->height;
            }

            col = b->data[yy * b->width + xx];
            ainv = 255 - col.a;

            line[x].r = Clamp(((line[x].r * ainv) >> 8) + col.r);
            line[x].g = Clamp(((line[x].g * ainv) >> 8) + col.g);
            line[x].b = Clamp(((line[x].b * ainv) >> 8) + col.b);
            line[x].a = 255;

            z[x] = depth;
        }
    }
    while (++x < x2);
}

void SWFRender::FillGradient(RGBA* line, int* z, int y, int x1, int x2, Matrix* m, Gradient* g, int type, unsigned int depth, double fmultiply)
{
    int x = x1;

    double m11 = m->sx * fmultiply / 80, m21 = m->r1 * fmultiply / 80;
    double m12 = m->r0 * fmultiply / 80, m22 = m->sy * fmultiply / 80;
    double rx = m->tx * fmultiply / 20.0;
    double ry = m->ty * fmultiply / 20.0;

    double det = m11 * m22 - m12 * m21;

    if (fabs(det) < 0.0005)
    {
        /* x direction equals y direction- the image is invisible */
        return;
    }

    det = 1.0 / det;

    RGBA palette[512];
    RGBA oldcol = g->rgba[0];
    int r0 = g->ratios[0] * 2;
    int t;

    for (t = 0; t < r0; t++)
    {
        palette[t] = oldcol;
    }

    for (t = 1; t < g->num; t++)
    {
        int r1 = g->ratios[t] * 2;
        RGBA newcol = g->rgba[t];

        if (r0 == r1)
        {
            continue;
        }

        double f = 1.0 / (r1 - r0);
        double p0 = 1;
        double p1 = 0;

        for (; r0 <= r1; r0++)
        {
            palette[r0].r = oldcol.r * p0 + newcol.r * p1;
            palette[r0].g = oldcol.g * p0 + newcol.g * p1;
            palette[r0].b = oldcol.b * p0 + newcol.b * p1;
            palette[r0].a = oldcol.a * p0 + newcol.a * p1;
            p0 -= f;
            p1 += f;
        }

        oldcol = newcol;
    }

    for (t = r0; t < 512; t++)
    {
        palette[t] = oldcol;
    }

    do
    {
        if (depth >= z[x])
        {
            RGBA col;
            double xx = ((x - rx) * m22 - (y - ry) * m21) * det;
            double yy = (-(x - rx) * m12 + (y - ry) * m11) * det;

            if (type == FILL_LINEAR)
            {
                int xr = xx * 256;
                if (xr < -256)
                    xr = -256;
                if (xr > 255)
                    xr = 255;
                col = palette[xr + 256];
            }
            else
            {
                int xr = sqrt(xx * xx + yy * yy) * 511;
                if (xr < 0)
                    xr = 0;
                if (xr > 511)
                    xr = 511;
                col = palette[xr];
            }

            int ainv = 255 - col.a;

            line[x].r = Clamp(((line[x].r * ainv) >> 8) + col.r);
            line[x].g = Clamp(((line[x].g * ainv) >> 8) + col.g);
            line[x].b = Clamp(((line[x].b * ainv) >> 8) + col.b);
            line[x].a = 255;

            z[x] = depth;
        }
    }
    while (++x < x2);
}

int SWFRender::Clamp(int v)
{
    if (v > 255)
    {
        return 255;
    }
    else
    {
        return v;
    }
}

void SWFRender::ClearTag(Tag* t)
{
    if (t->data)
    {
        free(t->data);
    }

    t->data = 0;
    t->pos = 0;
    t->len = 0;
    t->readBit = 0;
    t->writeBit = 0;
    t->memsize = 0;
}

void SWFRender::ShapeFree(Shape* s)
{
    if (!s)
    {
        return;
    }

    if (s->lineStyle.data)
    {
        free(s->lineStyle.data);
    }

    s->lineStyle.data = nullptr;
    s->lineStyle.n = 0;

    if (s->fillStyle.data)
    {
        free(s->fillStyle.data);
    }

    s->fillStyle.data = nullptr;
    s->fillStyle.n = 0;

    if (s->data)
    {
        free(s->data);
    }

    s->data = nullptr;

    free(s);
}

void SWFRender::TextCallback(void* self, int* chars, int* xpos, int nr, int fontid, int fontsize,
    int xstart, int ystart, RGBA* color)
{
    TextCallbackBlock* info = static_cast<TextCallbackBlock*>(self);
    Font* font = 0;
    int t;

    if (info->idtable[fontid].type != CharacterType::fontType)
    {
        fprintf(stderr, "ID %d is not a font\n", fontid);
        return;
    }
    else if (!info->idtable[fontid].obj.font)
    {
        fprintf(stderr, "Font %d unknown\n", fontid);
        return;
    }
    else
    {
        font = info->idtable[fontid].obj.font;
    }

    for (t = 0; t < nr; t++)
    {
        int x = xstart + xpos[t];
        int y = ystart;
        Matrix m = info->m;
        SPoint p;

        p.x = x; p.y = y;
        p = TurnPoint(p, &m);

        m.sx = (m.sx * fontsize) / 1024;
        m.sy = (m.sy * fontsize) / 1024;
        m.r0 = (m.r0 * fontsize) / 1024;
        m.r1 = (m.r1 * fontsize) / 1024;
        m.tx = p.x;
        m.ty = p.y;

        if (chars[t] < 0 || chars[t] >= font->numberOfChars)
        {
            fprintf(stderr, "Character out of range: %d\n", chars[t]);
        }
        else
        {
            Shape2* shape = font->glyphs[chars[t]];
            shape->fillStyles[0].color = *color; //q&d

            RenderShape(info->buf, shape, &m, info->cxform, info->depth, info->clipdepth);
        }
    }
}

int SWFRender::CompareRenderPoints(const void* _a, const void* _b)
{
    const RenderPoint* a = static_cast<const RenderPoint*>(_a);
    const RenderPoint* b = static_cast<const RenderPoint*>(_b);

    if (a->x < b->x)
    {
        return -1;
    }

    if (a->x > b->x)
    {
        return 1;
    }

    return 0;
}

void SWFRender::Delete(RenderBuffer* dest)
{
    RenderBufferInternal* i = static_cast<RenderBufferInternal*>(dest->internal);
    Bitmap* b = i->bitmaps;

    // delete canvas
    free(i->zbuf);
    free(i->img);

    // delete line buffers
    for (int y = 0; y < i->height2; y++)
    {
        DeleteTag(0, i->lines[y].points);

        i->lines[y].points = 0;
    }

    // delete bitmaps
    while (b)
    {
        Bitmap* next = b->next;

        free(b->data); b->data = 0;
        free(b);

        b = next;
    }

    free(i->lines);
    i->lines = 0;
    free(dest->internal);
    dest->internal = 0;
}
