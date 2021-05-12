
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DMCRC_H_INCLUDE__
#define __DMCRC_H_INCLUDE__

#include <stddef.h>
#include "dmos.h"

class CDMCRC;
class CDMCRCTableInit;

static const UINT32 kCRCPoly = 0xEDB88320;

class CDMCRCTableInit
{
public:
    CDMCRCTableInit() { InitTable(); }

    static inline UINT32* GetTable()
    {
        static UINT32 sTable[256];

        return (UINT32*)&sTable;
    }

    static inline void InitTable()
    {
        for (UINT32 i = 0; i < 256; i++)
        {
            UINT32 r = i;
            for (int j = 0; j < 8; j++)
                if (r & 1)
                    r = (r >> 1) ^ kCRCPoly;
                else
                    r >>= 1;
            GetTable()[i] = r;
        }
    }
};

class CDMCRC
{
private:
    UINT32 _value;
public:
    static inline CDMCRCTableInit& CRCTableInit()
    {
        static CDMCRCTableInit sCRCTableInit;
        return sCRCTableInit;
    }

    CDMCRC():  _value(0xFFFFFFFF){ CRCTableInit(); };

    inline void Init() { _value = 0xFFFFFFFF; }

    inline void UpdateUINT8(UINT8 v)
    {
        _value = CDMCRCTableInit::GetTable()[((UINT8)(_value)) ^ v] ^ (_value >> 8);
    }

    inline void UpdateUINT16(UINT16 v)
    {
        UpdateUINT8(UINT8(v));
        UpdateUINT8(UINT8(v >> 8));
    }

    inline void UpdateUINT32(UINT32 v)
    {
        for (int i = 0; i < 4; i++)
            UpdateUINT8((UINT8)(v >> (8 * i)));
    }

    inline void UpdateUInt64(UINT64 v)
    {
        for (int i = 0; i < 8; i++)
            UpdateUINT8((UINT8)(v >> (8 * i)));
    }

    inline void Update(const void *data, size_t size)
    {
        UINT32 v = _value;
        const UINT8 *p = (const UINT8 *)data;
        for (; size > 0 ; size--, p++)
            v = CDMCRCTableInit::GetTable()[((UINT8)(v)) ^ *p] ^ (v >> 8);
        _value = v;
    }

    inline UINT32 GetDigest() const { return _value ^ 0xFFFFFFFF; }

    static  inline UINT32 CalculateDigest(const void *data, size_t size)
    {
        CDMCRC crc;
        crc.Update(data, size);
        return crc.GetDigest();
    }
    static inline bool VerifyDigest(UINT32 digest, const void *data, size_t size)
    {
        return (CalculateDigest(data, size) == digest);
    }
};

#endif // __DMCRC_H_INCLUDE__
