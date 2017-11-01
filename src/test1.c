/*
 * test1.c - part of the CyoEncode library
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

#include "CyoEncode.h"
#include "CyoDecode.h"
#include "testdata.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*****************************************************************************/

#define TEST_BASExx(base,str,expected) \
    printf( "TEST_BASE%s('%s')='%s'", #base, str, expected ); \
    required = cyoBase##base##EncodeGetLength( strlen( str )); \
    encoded = (char*)malloc( required ); \
    if (encoded == NULL) { \
        printf( "\n*** ERROR: Unable to allocate buffer for encoding ***\n" ); \
        goto exit; \
    } \
    cyoBase##base##Encode( encoded, str, strlen( str )); \
    if (strcmp( encoded, expected ) != 0) { \
        printf( "\n*** ERROR: Encoding failure ***\n" ); \
        goto exit; \
    } \
    valid = cyoBase##base##Validate( encoded, strlen( encoded )); \
    if (valid < 0) { \
        printf( "\n*** ERROR: Unable to validate encoding (error %d) ***\n", valid ); \
        goto exit; \
    } \
    printf( " [passed]\n" ); \
    free( encoded ); encoded = NULL;

#define TEST_BASE16(str,expected) TEST_BASExx(16,str,expected)
#define TEST_BASE32(str,expected) TEST_BASExx(32,str,expected)
#define TEST_BASE64(str,expected) TEST_BASExx(64,str,expected)
#define TEST_BASE85(str,expected) TEST_BASExx(85,str,expected)

/*****************************************************************************/

#define TEST_BASExx_BLOCK(base,str,expected,output) \
    printf( "TEST_BASE%s_BLOCK('%s')='%s'", #base, str, expected ); \
    encoded = (char*)malloc( output ); \
    if (encoded == NULL) { \
        printf( "\n*** ERROR: Unable to allocate buffer for encoding ***\n" ); \
        goto exit; \
    } \
    cyoBase##base##EncodeBlock( encoded, str ); \
    if (memcmp( encoded, expected, output ) != 0) { \
        printf( "\n*** ERROR: Encoding failure ***\n" ); \
        goto exit; \
    } \
    valid = cyoBase##base##Validate( encoded, output ); \
    if (valid < 0) { \
        printf( "\n*** ERROR: Unable to validate encoding (error %d) ***\n", valid ); \
        goto exit; \
    } \
    printf( " [passed]\n" ); \
    free( encoded ); encoded = NULL;

#define TEST_BASE16_BLOCK(str,expected) TEST_BASExx_BLOCK(16,str,expected,2)
#define TEST_BASE32_BLOCK(str,expected) TEST_BASExx_BLOCK(32,str,expected,8)
#define TEST_BASE64_BLOCK(str,expected) TEST_BASExx_BLOCK(64,str,expected,4)
#define TEST_BASE85_BLOCK(str,expected) TEST_BASExx_BLOCK(85,str,expected,5)

/*****************************************************************************/

#define CHECK_INVALID_BASExx(base,str,res) \
    printf( "CHECK_INVALID_BASE%s('%s')=%d", #base, str, res ); \
    valid = cyoBase##base##Validate( str, strlen( str )); \
    if (valid == 0) { \
        printf( "\n*** ERROR: This is a valid encoding! ***\n" ); \
        goto exit; \
    } \
    if (valid != res) { \
        printf( "\n*** ERROR: Expected a different return code! (%d) ***\n", valid ); \
        goto exit; \
    } \
    required = cyoBase##base##DecodeGetLength( strlen( str )); \
    decoded = (char*)malloc(required); \
    size = cyoBase##base##Decode( decoded, str, strlen( str )); \
    if (size != 0) { \
        printf( "\n*** ERROR: This is a valid encoding! ***\n" ); \
        goto exit; \
    } \
    printf( " [passed]\n" ); \

#define CHECK_INVALID_BASE16(enc,res) CHECK_INVALID_BASExx(16,enc,res)
#define CHECK_INVALID_BASE32(enc,res) CHECK_INVALID_BASExx(32,enc,res)
#define CHECK_INVALID_BASE64(enc,res) CHECK_INVALID_BASExx(64,enc,res)
#define CHECK_INVALID_BASE85(enc,res) CHECK_INVALID_BASExx(85,enc,res)

/*****************************************************************************/

int run_c_tests(void)
{
    const char* const original = "A wise man speaks when he has something to say";
    size_t required = 0;
    size_t size = 0;
    char* encoded = NULL;
    char* decoded = NULL;
    int valid = 0;
    int retcode = 1;

    printf("Running C tests...\n");

    /* Encode using Base64 */

    printf("Original = '%s'\n", original);
    required = cyoBase64EncodeGetLength(strlen(original));
    encoded = (char*)malloc(required);
    if (encoded == NULL)
    {
        printf("*** ERROR: Unable to allocate buffer for encoding ***\n");
        goto exit;
    }
    cyoBase64Encode(encoded, original, strlen(original));

    /* Validate encoding */

    valid = cyoBase64Validate(encoded, strlen(encoded));
    if (valid < 0)
    {
        printf("*** ERROR: Encoding failure (error %d) ***\n", valid);
        goto exit;
    }
    printf("Encoded = '%s'\n", encoded);

    /* Decode using Base64 */

    required = cyoBase64DecodeGetLength(strlen(encoded));
    decoded = (char*)malloc(required);
    if (decoded == NULL)
    {
        printf("*** ERROR: Unable to allocate buffer for decoding ***\n");
        goto exit;
    }
    cyoBase64Decode(decoded, encoded, strlen(encoded));

    /* Validate decoding */

    if (strcmp(original, decoded) != 0)
    {
        printf("*** ERROR: Encoding/decoding failure ***\n");
        goto exit;
    }
    printf("Decoded = '%s'\n", decoded);

    free(encoded);
    encoded = NULL;
    free(decoded);
    decoded = NULL;

    /* Tests */

    for (const struct TestData* base16 = TestDataBase16; base16->data != NULL; ++base16) {
        TEST_BASE16(base16->data, base16->encoded);
    }
    for (const struct TestData* base32 = TestDataBase32; base32->data != NULL; ++base32) {
        TEST_BASE32(base32->data, base32->encoded);
    }
    for (const struct TestData* base64 = TestDataBase64; base64->data != NULL; ++base64) {
        TEST_BASE64(base64->data, base64->encoded);
    }
    for (const struct TestData* base85 = TestDataBase85; base85->data != NULL; ++base85) {
        TEST_BASE85(base85->data, base85->encoded);
    }

    /* Blocks */

    for (const struct TestData* base16 = TestBlocksBase16; base16->data != NULL; ++base16) {
        TEST_BASE16_BLOCK(base16->data, base16->encoded);
    }
    for (const struct TestData* base32 = TestBlocksBase32; base32->data != NULL; ++base32) {
        TEST_BASE32_BLOCK(base32->data, base32->encoded);
    }
    for (const struct TestData* base64 = TestBlocksBase64; base64->data != NULL; ++base64) {
        TEST_BASE64_BLOCK(base64->data, base64->encoded);
    }
    for (const struct TestData* base85 = TestBlocksBase85; base85->data != NULL; ++base85) {
        TEST_BASE85_BLOCK(base85->data, base85->encoded);
    }

    /* Invalid */

    for (const struct TestInvalid* base16 = TestInvalidBase16; base16->data != NULL; ++base16) {
        CHECK_INVALID_BASE16(base16->data, base16->result);
    }
    for (const struct TestInvalid* base32 = TestInvalidBase32; base32->data != NULL; ++base32) {
        CHECK_INVALID_BASE32(base32->data, base32->result);
    }
    for (const struct TestInvalid* base64 = TestInvalidBase64; base64->data != NULL; ++base64) {
        CHECK_INVALID_BASE64(base64->data, base64->result);
    }
    for (const struct TestInvalid* base85 = TestInvalidBase85; base85->data != NULL; ++base85) {
        CHECK_INVALID_BASE85(base85->data, base85->result);
    }

    printf("*** All C tests passed ***\n");
    retcode = 0;

exit:
    if (encoded != NULL)
        free(encoded);
    if (decoded != NULL)
        free(decoded);

    return retcode;
}
