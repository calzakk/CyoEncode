/*
 * testdata.h - part of the CyoEncode library
 * 
 * Copyright (c) 2009-2017, Graham Bull.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TESTDATA_H
#define __TESTDATA_H

#include <stdlib.h>

/*****************************************************************************/

struct TestData
{
    const char* data;
    const char* encoded;
};

struct TestInvalid
{
    const char* data;
    int result;
};

/*************************************/
/* Test vectors from RFC 4648 for Base16-64 */

const struct TestData TestDataBase16[] =
{
    { "", "" },
    { "f", "66" },
    { "fo", "666F" },
    { "foo", "666F6F" },
    { "foob", "666F6F62" },
    { "fooba", "666F6F6261" },
    { "foobar", "666F6F626172" },
    { NULL }
};

const struct TestData TestDataBase32[] =
{
    { "", "" },
    { "f", "MY======" },
    { "fo", "MZXQ====" },
    { "foo", "MZXW6===" },
    { "foob", "MZXW6YQ=" },
    { "fooba", "MZXW6YTB" },
    { "foobar", "MZXW6YTBOI======" },
    { NULL }
};

const struct TestData TestDataBase64[] =
{
    { "", "" },
    { "f", "Zg==" },
    { "fo", "Zm8=" },
    { "foo", "Zm9v" },
    { "foob", "Zm9vYg==" },
    { "fooba", "Zm9vYmE=" },
    { "foobar", "Zm9vYmFy" },
    { NULL }
};

const struct TestData TestDataBase85[] =
{
    { "", "" },
    { "f", "Ac" },
    { "fo", "Ao@" },
    { "foo", "AoDS" },
    { "foob", "AoDTs" },
    { "fooba", "AoDTs@/" },
    { "foobar", "AoDTs@<)" },
    { "Man", "9jqo" },
    { ".", "/c" },
    { NULL }
};

/*************************************/
/* Blocks */

const struct TestData TestBlocksBase16[] =
{
    { "\0", "00" },
    { "1", "31" },
    { "A", "41" },
    { "\xFF", "FF" },
    { NULL }
};

const struct TestData TestBlocksBase32[] =
{
    { "\0\0\0\0\0", "AAAAAAAA" },
    { "12345", "GEZDGNBV" },
    { "ABCDE", "IFBEGRCF" },
    { "\xFF\xFF\xFF\xFF\xFF", "77777777" },
    { NULL }
};

const struct TestData TestBlocksBase64[] =
{
    { "\0\0\0", "AAAA" },
    { "123", "MTIz" },
    { "ABC", "QUJD" },
    { "\xFF\xFF\xFF", "////" },
    { NULL }
};

const struct TestData TestBlocksBase85[] =
{
    { "foob", "AoDTs" },
    { "Man ", "9jqo^" },
    { NULL }
};

/*************************************/
/* Invalid */

const struct TestInvalid TestInvalidBase16[] =
{
    { "1", -1 }, //must be multiple of 2 chars
    { "123", -1 }, //must be multiple of 2 chars
    { "12345", -1 }, //must be multiple of 2 chars
    { "1G", -2 }, //invalid character (G)
    { "1Z", -2 }, //invalid character (Z)
    { NULL }
};
const struct TestInvalid TestInvalidBase32[] =
{
    { "A", -1 }, //must be multiple of 8 chars
    { "AB", -1 }, //must be multiple of 8 chars
    { "ABC", -1 }, //must be multiple of 8 chars
    { "ABCD", -1 }, //must be multiple of 8 chars
    { "ABCDE", -1 }, //must be multiple of 8 chars
    { "ABCDEF", -1 }, //must be multiple of 8 chars
    { "ABCDEFG", -1 }, //must be multiple of 8 chars
    { "ABCDEFGHI", -1 }, //must be multiple of 8 chars
    { "A=======", -2 }, //too much padding
    { "A0======", -2 }, //invalid character (0)
    { "A1======", -2 }, //invalid character (1)
    { "A8======", -2 }, //invalid character (8)
    { "A9======", -2 }, //invalid character (9)
    { "ABCDEF=H", -2 }, //non-trailing padding
    { "ABCDE=GH", -2 }, //non-trailing padding
    { "ABCD=FGH", -2 }, //non-trailing padding
    { "ABC=EFGH", -2 }, //non-trailing padding
    { "AB=DEFGH", -2 }, //non-trailing padding
    { "A=CDEFGH", -2 }, //non-trailing padding
    { "=BCDEFGH", -2 }, //non-trailing padding
    { NULL }
};
const struct TestInvalid TestInvalidBase64[] =
{
    { "A", -1 }, //must be multiple of 4 chars
    { "AB", -1 }, //must be multiple of 4 chars
    { "ABC", -1 }, //must be multiple of 4 chars
    { "ABCDE", -1 }, //must be multiple of 4 chars
    { "A===", -2 }, //too much padding
    { "A ==", -2 }, //invalid character ( )
    { "A!==", -2 }, //invalid character (!)
    { "A&==", -2 }, //invalid character (&)
    { "A@==", -2 }, //invalid character (@)
    { "A{==", -2 }, //invalid character ({)
    { "AB=D", -2 }, //non-trailing padding
    { "A=CD", -2 }, //non-trailing padding
    { "=BCD", -2 }, //non-trailing padding
    { NULL }
};
const struct TestInvalid TestInvalidBase85[] =
{
    //invalid Base85 characters...
    { "v", -2 },
    { "w", -2 },
    { "x", -2 },
    { "y", -2 },
    { "{", -2 },
    { "|", -2 },
    { "}", -2 },
    { "~", -2 },
    { NULL }
};

/*****************************************************************************/

#endif /*__TESTDATA_H*/
