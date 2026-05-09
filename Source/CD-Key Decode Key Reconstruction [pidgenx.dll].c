#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <immintrin.h>
#pragma comment(lib, "Rpcrt4.lib")

// Custom HRESULTs from the assembly mapping
#define E_INVALID_KEY_FORMAT  ((HRESULT)0x80070057L) // E_INVALIDARG
#define E_LICENSE_TAMPERED    ((HRESULT)0x80041051L) // Custom License Error
#define BYTE_PTR(x) ((uint8_t*)(x))

// License Status Constants
#define LICENSE_VALID 1

/* --- IDA Pro Style Primitives --- */
typedef unsigned char      _BYTE;
typedef unsigned short     _WORD;
typedef unsigned int       _DWORD;
typedef unsigned __int64   _QWORD;

// Mapping __int128 for MSVC/GCC compatibility
#ifdef _MSC_VER
#include <intrin.h>
typedef __m128i _OWORD; // Using SIMD type for 128-bit storage in MSVC
#else
typedef __int128 _OWORD; // standard GCC/Clang extension
#endif

/* --- CONSTANTS & TABLES --- */

// .text:000000018000A050
// Data1: 660672EFh 
// Data2: 7809h 
// Data3: 4CFDh 
// Data4: 8D 54 41 B7 FB 73 89 88
const GUID CLSID_IProductKeyAlgorithm2009 = {
    0x660672EF,
    0x7809,
    0x4CFD,
    { 0x8D, 0x54, 0x41, 0xB7, 0xFB, 0x73, 0x89, 0x88 }
};

// .rdata:00000001800A67F0 - CRC32 Table
const _DWORD g_hash_table[256] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

/* --- STRUCTURES --- */

#pragma pack(push, 1)
typedef union _DECODED_DATA {
    struct {
        _QWORD Low;         // 8 bytes (First half)
        _QWORD High;        // 8 bytes (Second half)
    } Half;                 // 2 x 8 bytes
    _BYTE Bytes[16];        // 16 x 1 byte (Array access)
} DECODED_DATA;
#pragma pack(pop)

// Size: 0x58 (88 bytes)
typedef struct _MSFT_PKEY_DATA {
    uint64_t Header;            // 0x00
    GUID     Algorithm;         // 0x08
    uint32_t GroupID;           // 0x18
    uint32_t ChannelID;         // 0x1C
    uint32_t Serial;            // 0x20
    uint32_t Sequence;          // 0x24
    uint64_t SecurityID;        // 0x28
    uint32_t LicenseAttributes; // 0x30
    uint32_t KeyType;           // 0x34
    uint64_t IssuanceTimestamp; // 0x38
    uint32_t ActivationFlags;   // 0x40
    uint32_t ReservedPadding;   // 0x44
    uint8_t  HardwareID[16];    // 0x48
} MSFT_PKEY_DATA;

#pragma pack(push, 1)
typedef struct _LICENSE_WORKSPACE
{
    // =========================
    // Stage 0: compressed input
    // =========================
    DECODED_DATA encoded;        // buffer[64] input to pipeline

    // =========================
    // Stage 1: unswizzle output
    // =========================
    uint8_t unswizzled[32];      // temporary decode result

    // =========================
    // Stage 2: unpack output
    // =========================
    uint8_t unpack[24];          // overlaps final derivation inputs

    // =========================
    // Stage 3: FINAL context (THIS is LICENSE_CONTEXT layout)
    // =========================
    union
    {
        uint8_t raw[96];
        MSFT_PKEY_DATA ctx;
    };

    // =========================
    // control
    // =========================
    int isKeyValid;
    uint32_t hash;

    // =========================
    // scratch (stack xmm reuse zone)
    // =========================
    uint8_t scratch[0x80];

} LICENSE_WORKSPACE;
#pragma pack(pop)

// sub_180008DB4
__int64 __fastcall DecodeBase24(const wchar_t* szKey, __int64 a2, _BYTE* pOutBytes, __int64 a4, int* pOutSpecial) {
    unsigned int decodedCount = 0;
    unsigned int totalProcessed = 0;
    unsigned int hyphenCount = 0;
    int hasSpecialN = 0;

    while (decodedCount < 25) {
        wchar_t c = szKey[totalProcessed];

        // 1. Special 'N' Handling (ASCII 78)
        if (((c - 78) & 0xFFDF) == 0) {
            if (hasSpecialN || decodedCount >= 24) break;
            hasSpecialN = 1;
            memmove(pOutBytes + 1, pOutBytes, decodedCount);
            pOutBytes[0] = (_BYTE)decodedCount;
            goto NEXT_ITER;
        }

        // 2. Hyphen Validation
        if (c == L'-') {
            // 1. Declare as long (LONG)
            long mask = 8521760;

            // 2. Cast the index to long if totalProcessed is an unsigned int
            if (!_bittest(&mask, (long)totalProcessed) || hyphenCount >= 4) break;
            hyphenCount++;
            totalProcessed++;
            continue;
        }

        // 3. Alphabet Mapping (Verified via decompiler BST branches)
        _BYTE val;
        switch (c) {
        case L'B': val = 0;  break; case L'C': val = 1;  break;
        case L'D': val = 2;  break; case L'F': val = 3;  break;
        case L'G': val = 4;  break; case L'H': val = 5;  break;
        case L'J': val = 6;  break; case L'K': val = 7;  break;
        case L'M': val = 8;  break; case L'P': val = 9;  break;
        case L'Q': val = 10; break; case L'R': val = 11; break;
        case L'T': val = 12; break; case L'V': val = 13; break;
        case L'W': val = 14; break; case L'X': val = 15; break;
        case L'Y': val = 16; break; case L'2': val = 17; break;
        case L'3': val = 18; break; case L'4': val = 19; break;
        case L'6': val = 20; break; case L'7': val = 21; break;
        case L'8': val = 22; break; case L'9': val = 23; break;
        default: return 0x80040011; // Character not in Base24 set
        }

        pOutBytes[decodedCount] = val;

    NEXT_ITER:
        decodedCount++;
        totalProcessed++;

        if (totalProcessed >= 29) {
            if (hyphenCount == 4 && decodedCount == 25) {
                *pOutSpecial = hasSpecialN;
                return 0; // SUCCESS
            }
            break;
        }
    }

    return 0x80040011; // E_FAIL / INVALID_KEY
}

// sub_180008BCC
HRESULT ValidateAndUnswizzle(DECODED_DATA* in, uint8_t* out32, uint32_t* outHash)
{
    uint8_t raw[16];
    memcpy(raw, in, 16);

    uint8_t v23[16] = {};
    uint8_t v24[16] = {};

    uint8_t flags = raw[14] & 0xF7;
    uint8_t v6 = flags & 0xFE;

    //
    // expected CRC
    //
    uint16_t expected =
        (((((flags << 1) | (raw[13] >> 7)) & 3) << 8)) |
        ((raw[12] >> 7) | (raw[13] << 1));

    //
    // CRC normalize
    //
    raw[12] &= 0x7F;
    raw[13] = 0;
    raw[14] = v6;
    raw[15] = 0;

    uint32_t crc = 0xFFFFFFFF;

    for (int i = 0; i < 16; i++)
    {
        crc =
            g_hash_table[((crc >> 24) ^ raw[i]) & 0xFF]
            ^ (crc << 8);
    }

    uint16_t actual = (~crc) & 0x3FF;

    if (expected != actual)
        return 0x8007000D;

    if (outHash)
        *outHash = actual;

    //
    // v23
    //
    v23[0] = raw[0];
    v23[1] = raw[1];
    v23[2] = raw[2] & 0x0F;

    for (int i = 0; i < 3; i++)
    {
        v23[4 + i] =
            (raw[2 + i] >> 4) |
            (raw[3 + i] << 4);
    }

    v23[7] =
        ((raw[5] | (raw[6] << 8)) >> 4) & 0x3F;

    v23[8] = raw[8] ^ ((v6 >> 1) & 1);

    //
    // v24
    //
    for (int i = 0; i < 6; i++)
    {
        v24[i] =
            (raw[7 + i] << 6) |
            (raw[6 + i] >> 2);
    }

    v24[6] =
        ((raw[12] & 0x7F) >> 2) & 0x1F;

    memcpy(out32, v23, 16);
    memcpy(out32 + 16, v24, 16);

    return S_OK;
}

// sub_18000890C
HRESULT __fastcall CompressKey(const wchar_t* szKey, DECODED_DATA* pOutRaw, int* pOutFlag)
{
    // local variables to match ASM stack
    int specialNFlag = 0;                // var_68
    uint8_t decodedBytes[32] = { 0 };    // var_50 (v22 in pseudocode)
    uint8_t accum[16] = { 0 };           // var_60
    uint32_t ebx_status;

    if (!szKey) return 0x80041051;

    // 1. Length Check (Exactly 29 characters)
    size_t len = 0;
    while (len < 30 && szKey[len] != 0) len++;
    if (len != 29) return 0x80041051;

    // 2. Base24 Decoding 
    // Matching ASM: call sub_180008DB4(rcx: szKey, rdx: 29i64, r8: decodedBytes)
    // Note: The assembly passes '29' as the second argument.
    // Adjust this call to match your DecodeBase24 signature.
    ebx_status = (uint32_t)DecodeBase24(szKey, 29, decodedBytes, 0, &specialNFlag);

    if ((int)ebx_status < 0) {
        return ebx_status;
    }

    // 3. BigInt Conversion (Base24 -> Bytes)
    uint32_t currentByteCount = 0;

    for (int i = 0; i < 25; ++i) {
        uint32_t carry = decodedBytes[i];

        if (currentByteCount > 0) {
            for (uint32_t j = 0; j < currentByteCount; ++j) {
                uint32_t temp = (uint32_t)accum[j] * 24 + carry;
                accum[j] = (uint8_t)temp;
                carry = temp >> 8;
            }
        }

        if (carry != 0) {
            if (currentByteCount >= 16) return 0x80041051;
            accum[currentByteCount] = (uint8_t)carry;
            currentByteCount++;
        }
    }

    // 4. Handle the "N" Flag logic
    // Assembly: mov eax, [rsp+98h+var_68] -> test eax, eax
    // This flag is usually set inside sub_180008DB4 if it's a specific key type
    int finalFlagValue = 0;
    if (specialNFlag != 0) {
        accum[14] |= 0x08; // Set the special bit in Byte 14
        finalFlagValue = 1;
    }

    // 5. Final Move
    memcpy(pOutRaw, accum, 16);

    if (pOutFlag) {
        *pOutFlag = finalFlagValue;
    }

    return ebx_status;
}

// sub_180008A58
__int32 __fastcall UnpackLicenseContext(unsigned char* pDecodedKey, byte* pContext)
{
    // The assembly uses a 16-byte stack buffer (v24[2] as __int64)
    uint8_t v24[16];
    memset(v24, 0, sizeof(v24));

    // 1. Initial Boolean (v24[0] = !v3)
    v24[0] = (*(uint32_t*)(pDecodedKey + 8) != 0) ? 1 : 0;

    // 2. Unpack Block A (The "do-while" v8=3 loop)
    // Matches: *(_BYTE *)v2 |= 2 * v9; *v4++ |= v9 >> 7;
    for (int i = 0; i < 3; ++i) {
        uint8_t v9 = pDecodedKey[i + 4];
        v24[i] &= 0x01;
        v24[i + 1] &= ~0x01; // v4 points to v2+1
        v24[i] |= (2 * v9);
        v24[i + 1] |= (v9 >> 7);
    }

    // 3. The 0x7E Patch (Bridge bits)
    // ASM: BYTE3(v24[0]) ^= (BYTE3(v24[0]) ^ (2 * *(_BYTE *)(a1 + 7))) & 0x7E;
    v24[3] ^= (v24[3] ^ (2 * pDecodedKey[7])) & 0x7E;

    // 4. Unpack Block B (The "do-while" v12=2 loop)
    // Matches: *v10++ |= v13 << 7; *v11++ |= v13 >> 1;
    for (int i = 0; i < 2; ++i) {
        uint8_t v13 = pDecodedKey[i + 3];
        v24[i + 3] &= ~0x80;
        v24[i + 4] &= 0x80;
        v24[i + 3] |= (v13 << 7);
        v24[i + 4] |= (v13 >> 1);
    }

    // 5. Unpack Metadata (The v16 block)
    // ASM: BYTE5(v24[0]) = BYTE5(v24[0]) & 0x7F | (v16 << 7);
    // ASM: BYTE6(v24[0]) ^= (BYTE6(v24[0]) ^ (v16 >> 1)) & 7;
    uint8_t v16_val = pDecodedKey[2];
    v24[5] = (v24[5] & 0x7F) | (v16_val << 7);
    v24[6] ^= (v24[6] ^ (v16_val >> 1)) & 0x07;

    // 6. Unpack Block C (The "do-while" v17=6 loop)
    // Matches: *v18++ |= 8 * v20; *v15++ |= v20 >> 5;
    for (int i = 0; i < 6; ++i) {
        uint8_t v20 = pDecodedKey[i + 16];
        v24[i + 6] &= 0x07;
        v24[i + 7] &= 0xF8;
        v24[i + 6] |= (8 * v20);
        v24[i + 7] |= (v20 >> 5);
    }

    // 7. Final Byte Construction (Offset 0x0C)
    // ASM: v21 = *(_BYTE *)(v14 + 6); (Key[16+6] -> Key[22])
    // ASM: v22 = BYTE4(v24[1]) & 7; (v24[12] & 7)
    uint8_t v21_val = pDecodedKey[22];
    uint8_t v22_val = v24[12] & 0x07;

    // Output mapping
    *(uint64_t*)(pContext + 0x00) = *(uint64_t*)&v24[0];
    *(uint32_t*)(pContext + 0x08) = *(uint32_t*)&v24[8];
    pContext[0x0C] = v22_val | (8 * v21_val);

    return 0;
}

// sub_1800090B0
HRESULT DecodeLicenseContext(
    const wchar_t* ProductKey,
    MSFT_PKEY_DATA* ctx,
    uint32_t* pHash)
{
    if (!ctx || !ProductKey) return E_INVALIDARG;

    LICENSE_WORKSPACE workspace = { 0 };
    MSFT_PKEY_DATA* pOut = &workspace.ctx;

    // 1. Base24 -> BigInt
    HRESULT hr = CompressKey(ProductKey, &workspace.encoded, &workspace.isKeyValid);
    if (FAILED(hr)) return hr;

    // 2. Unswizzle (CRC + Bit Reorder)
    // Result v14/v15 goes into workspace.unswizzled
    hr = ValidateAndUnswizzle(&workspace.encoded, workspace.unswizzled, pHash);
    if (FAILED(hr)) return hr;

    // 3. Unpack HWID (The 3-bit Shift Logic)
    // Destination: Offset 0x48

    hr = UnpackLicenseContext(workspace.unswizzled, pOut->HardwareID);
    if (FAILED(hr)) return hr;

    std::cout << "-----------------------------------------------" << std::endl;
    printf("UnpackLicenseContext Results:\n");
    unsigned char* pRaw = (unsigned char*)&workspace.unswizzled;
    for (int i = 0; i < 32; ++i) {
        printf("%02X ", pRaw[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    std::cout << "-----------------------------------------------" << std::endl;

    // --- 4. MANUAL MAPPING (Matching ASM sub_1800090B0) ---

    // Algorithm GUID (Offset 0x08)
    pOut->Algorithm = CLSID_IProductKeyAlgorithm2009;

    // GroupID (Offset 0x18) - DWORD 0 of v14
    pOut->GroupID = *(uint32_t*)&workspace.unswizzled[0];

    uint32_t channelData =
        *(uint32_t*)&workspace.unswizzled[4];

    pOut->ChannelID = channelData / 1000000;
    pOut->Serial = channelData % 1000000;

    // THIS OFFSET IS WRONG CURRENTLY
    pOut->SecurityID =
        *(uint64_t*)&workspace.unswizzled[16];

    // License Attr & Key Type (Offset 0x30 & 0x34)
    pOut->LicenseAttributes = 1;
    pOut->KeyType = 1;
    pOut->IssuanceTimestamp = 0;
    pOut->ActivationFlags = 0;
    pOut->ReservedPadding = 0;

    // 5. Final Copy to Output
    memcpy(ctx, pOut, 0x58);

    return S_OK;
}

int main() {


    MSFT_PKEY_DATA ctx = { 0 };
    const wchar_t* myKey = L"K8KNG-MGG4H-KX82M-M8QYW-DGRFH";

    std::cout << "--- License Decoder Tool ---" << std::endl;
    uint32_t pHash = 0;
    HRESULT hr = DecodeLicenseContext(myKey, &ctx, &pHash);

    if (SUCCEEDED(hr)) {
        std::wcout << L"Key:            " << myKey << std::endl;
        std::cout << "Result:         VALID" << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;

        // Use the 'ctx' variable directly, not a pointer to it
        printf("Algorithm ID:   {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\n",
            ctx.Algorithm.Data1, ctx.Algorithm.Data2, ctx.Algorithm.Data3,
            ctx.Algorithm.Data4[0], ctx.Algorithm.Data4[1], ctx.Algorithm.Data4[2],
            ctx.Algorithm.Data4[3], ctx.Algorithm.Data4[4], ctx.Algorithm.Data4[5],
            ctx.Algorithm.Data4[6], ctx.Algorithm.Data4[7]);

        // 2. Print Numeric IDs
        std::cout << "Group ID:       " << std::dec << ctx.GroupID << std::endl;
        std::cout << "Channel ID:     " << ctx.ChannelID << std::endl;
        std::cout << "Serial:         " << ctx.Serial << std::endl;
        std::cout << "Sequence:       " << ctx.Sequence << std::endl;
        std::cout << "Security ID:    0x" << std::hex << std::uppercase << ctx.SecurityID << std::endl;

        // 3. Print Flags and Attributes
        std::cout << std::dec << std::nouppercase;
        std::cout << "Key Type:       " << ctx.KeyType << std::endl;
        std::cout << "License Attr:   0x" << std::hex << ctx.LicenseAttributes << std::endl;
        std::cout << "Activation Flg: 0x" << ctx.ActivationFlags << std::dec << std::endl;

        // 4. Print Hardware ID (HWID) as Hex String
        std::cout << "Hardware ID:    ";
        for (int i = 0; i < 16; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (int)ctx.HardwareID[i] << (i < 15 ? ":" : "");
        }
        std::cout << std::dec << std::setfill(' ') << std::endl;

        // 5. Print internal Hash/CRC
        std::cout << "Internal Hash:  " << pHash << std::endl;

        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "End Results:  " << std::endl;
        unsigned char* pRaw = (unsigned char*)&ctx;
        for (int i = 0; i < 88; ++i) {
            printf("%02X ", pRaw[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        std::cout << "\n-----------------------------------------------" << std::endl;
        std::cout << "Real Api Call Memory Dump!" << std::endl;
        std::cout << "00 00 00 00 00 00 00 00 EF 72 06 66 09 78 FD 4C" << std::endl;
        std::cout << "8D 54 41 B7 FB 73 89 88 0D 11 00 00 00 00 00 00" << std::endl;
        std::cout << "0B 00 00 00 00 00 00 00 0C 00 00 00 00 00 00 00" << std::endl;
        std::cout << "01 00 00 00 01 00 00 00 0D 11 B0 00 00 00 30 00" << std::endl;
        std::cout << "00 00 00 00 00 DB 08 00 16 00 00 80 86 08 60 00" << std::endl;
        std::cout << "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
    }
    else {
        std::cerr << "Result:  INVALID (HRESULT: 0x" << std::hex << hr << ")" << std::endl;
        if (hr == E_LICENSE_TAMPERED) {
            std::cerr << "Reason:  Checksum mismatch (CRC failed)." << std::endl;
        }
        else if (hr == 0x80040011) {
            std::cerr << "Reason:  Invalid characters or length." << std::endl;
        }
    }

    std::cout << "\n";
    system("pause");
    return 0;
}