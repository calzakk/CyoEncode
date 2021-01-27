/*
 * test2.cpp - part of the CyoEncode library
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

#include "CyoEncode.hpp"
#include "CyoDecode.hpp"
#include "testdata.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

/*****************************************************************************/

#define TEST_BASExx(base,str,expected) \
    if (!failedTests) { \
        std::cout << "TEST_BASE" << #base << "('" << str << "')='" << expected << "'"; \
        required = CyoEncode::Base##base::GetLength(std::strlen(str)); \
        encoded.resize(required); \
        CyoEncode::Base##base::Encode(&encoded.front(), (const byte_t*)str, std::strlen(str)); \
        encodedStr = std::string(&encoded.front()); \
        if (encodedStr == expected) { \
            valid = CyoDecode::Base##base::Validate(encodedStr.c_str(), encodedStr.size()); \
            if (valid == 0) { \
                std::cout << " [passed]" << std::endl; \
            } \
            else { \
                std::cout << "\n*** ERROR: Unable to validate encoding (error " << valid << ") ***" << std::endl; \
                failedTests = true; \
            } \
        } \
        else { \
            std::cout << "\n*** ERROR: Encoding failure ***" << std::endl; \
            failedTests = true; \
        } \
    }

#define TEST_BASE16(str,expected) TEST_BASExx(16,str,expected)
#define TEST_BASE32(str,expected) TEST_BASExx(32,str,expected)
#define TEST_BASE64(str,expected) TEST_BASExx(64,str,expected)
#define TEST_BASE85(str,expected) TEST_BASExx(85,str,expected)

/*****************************************************************************/

#define TEST_BASExx_BLOCK(base,str,expected,numChars,cast) \
    if (!failedTests) { \
        std::cout << "TEST_BASE" << #base << "_BLOCK('" << str << "')='" << expected << "'"; \
        encoded.resize(numChars); \
        CyoEncode::Base##base::EncodeBlock(&encoded[0], cast str); \
        if (memcmp(&encoded[0], expected, numChars) == 0) { \
            valid = CyoDecode::Base##base::Validate(&encoded[0], numChars); \
            if (valid == 0) { \
                std::cout << " [passed]" << std::endl; \
            } \
            else { \
                std::cout << "\n*** ERROR: Unable to validate encoding (error " << valid << ") ***" << std::endl; \
                failedTests = true; \
            } \
        } \
        else { \
            std::cout << "\n*** ERROR: Encoding failure ***" << std::endl; \
            for (int i = 0; i < numChars; ++i) \
                std::cout << (int)encoded[i] << " = " << (32 <= encoded[i] && encoded[i] <= 126 ? encoded[i] : ' ') << std::endl; \
            failedTests = true; \
        } \
    }

#define TEST_BASE16_BLOCK(str,expected) TEST_BASExx_BLOCK(16,str,expected,2,*(const byte_t*))
#define TEST_BASE32_BLOCK(str,expected) TEST_BASExx_BLOCK(32,str,expected,8,(const byte_t*))
#define TEST_BASE64_BLOCK(str,expected) TEST_BASExx_BLOCK(64,str,expected,4,(const byte_t*))
#define TEST_BASE85_BLOCK(str,expected) TEST_BASExx_BLOCK(85,str,expected,5,(const byte_t*))

/*****************************************************************************/

#define CHECK_INVALID_BASExx(base,str,res) \
    if (!failedTests) { \
        std::cout << "CHECK_INVALID_BASE" << #base << "('" << str << "')=" << res; \
        valid = cyoBase##base##Validate(str, std::strlen(str)); \
        if (valid != 0) { \
            if (valid == res) \
                std::cout << " [passed]" << std::endl; \
            else { \
                std::cout << "\n*** ERROR: Expected a different return code! (" << valid << ") ***" << std::endl; \
                failedTests = true; \
            } \
        } \
        else { \
            std::cout << "\n*** ERROR: This is a valid encoding! ***" << std::endl; \
            failedTests = true; \
        } \
    }

#define CHECK_INVALID_BASE16(enc,res) CHECK_INVALID_BASExx(16,enc,res)
#define CHECK_INVALID_BASE32(enc,res) CHECK_INVALID_BASExx(32,enc,res)
#define CHECK_INVALID_BASE64(enc,res) CHECK_INVALID_BASExx(64,enc,res)
#define CHECK_INVALID_BASE85(enc,res) CHECK_INVALID_BASExx(85,enc,res)

/*****************************************************************************/

int run_cpp_tests()
{
    using CyoEncode::byte_t;

    const std::string original = "A wise man speaks when he has something to say";
    size_t required = 0;
    std::vector<char> encoded;
    std::vector<byte_t> decoded;
    int valid = 0;
    bool failedTests = false;

    std::cout << "Running C++ tests..." << std::endl;

    /* Encode using Base64 */

    std::cout << "Original = '" << original << "'" << std::endl;
    required = CyoEncode::Base64::GetLength(original.size());
    encoded.resize(required); //includes space for a terminator
    CyoEncode::Base64::Encode(&encoded.front(), (byte_t*)original.c_str(), original.size());
    std::string encodedStr(&encoded.front());

    /* Validate encoding */

    valid = CyoDecode::Base64::Validate(encodedStr.c_str(), encodedStr.size());
    if (valid < 0)
    {
        std::cout << "*** ERROR: Encoding failure (error " << valid << ") ***" << std::endl;
        return 1;
    }
    std::cout << "Encoded = '" << encodedStr << "'" << std::endl;

    /* Decode using Base64 */

    required = CyoDecode::Base64::GetLength(encodedStr.size());
    decoded.resize(required); //includes space for a terminator
    CyoDecode::Base64::Decode((byte_t*)&decoded.front(), encodedStr.c_str(), encodedStr.size());
    std::string decodedStr((char*)&decoded.front());

    /* Validate decoding */

    if (original != decodedStr)
    {
        std::cout << "*** ERROR: Decoding failure ***" << std::endl;
        return 1;
    }
    std::cout << "Decoded = '" << decodedStr << "'" << std::endl;

    /* Tests */

    for (const TestData* base16 = TestDataBase16; base16->data != NULL; ++base16) {
        TEST_BASE16(base16->data, base16->encoded);
    }
    for (const TestData* base32 = TestDataBase32; base32->data != NULL; ++base32) {
        TEST_BASE32(base32->data, base32->encoded);
    }
    for (const TestData* base64 = TestDataBase64; base64->data != NULL; ++base64) {
        TEST_BASE64(base64->data, base64->encoded);
    }
    for (const TestData* base85 = TestDataBase85; base85->data != NULL; ++base85) {
        TEST_BASE85(base85->data, base85->encoded);
    }

    /* Blocks */

    for (const TestData* base16 = TestBlocksBase16; base16->data != NULL; ++base16) {
        TEST_BASE16_BLOCK(base16->data, base16->encoded);
    }
    for (const TestData* base32 = TestBlocksBase32; base32->data != NULL; ++base32) {
        TEST_BASE32_BLOCK(base32->data, base32->encoded);
    }
    for (const TestData* base64 = TestBlocksBase64; base64->data != NULL; ++base64) {
        TEST_BASE64_BLOCK(base64->data, base64->encoded);
    }
    for (const TestData* base85 = TestBlocksBase85; base85->data != NULL; ++base85) {
        TEST_BASE85_BLOCK(base85->data, base85->encoded);
    }

    /* Invalid */

    for (const TestInvalid* base16 = TestInvalidBase16; base16->data != NULL; ++base16) {
        CHECK_INVALID_BASE16(base16->data, base16->result);
    }
    for (const TestInvalid* base32 = TestInvalidBase32; base32->data != NULL; ++base32) {
        CHECK_INVALID_BASE32(base32->data, base32->result);
    }
    for (const TestInvalid* base64 = TestInvalidBase64; base64->data != NULL; ++base64) {
        CHECK_INVALID_BASE64(base64->data, base64->result);
    }
    for (const TestInvalid* base85 = TestInvalidBase85; base85->data != NULL; ++base85) {
        CHECK_INVALID_BASE85(base85->data, base85->result);
    }

    if (!failedTests)
        std::cout << "*** All C++ tests passed ***" << std::endl;

    return 0;
}
