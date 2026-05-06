#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <immintrin.h>

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

// Static Salt from .rdata:00000001800A6788
const uint8_t GEN4_SALT_DATA[16] = {
    0xEF, 0x72, 0x06, 0x66, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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

#pragma pack(push, 1)
typedef struct _LICENSE_CONTEXT
{
    uint8_t  header[0x18];     // 0x00

    uint32_t RawSeedBits;      // 0x18
    uint32_t GroupID;          // 0x1C
    uint32_t Serial;           // 0x20
    uint32_t ProductID;        // 0x24

    uint64_t Security;         // 0x28

    uint32_t InternalFlag;     // 0x30
    uint32_t State;            // 0x34

    uint8_t  xmm_block[0x10];  // 0x38 (THIS is var_60+SIMD region)

    uint8_t  tail[0x10];       // 0x48 (matches xmm spill region)
} LICENSE_CONTEXT;
#pragma pack(pop)

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

        struct _LICENSE_CONTEXT
        {
            uint8_t  header[0x18];     // 0x00–0x17

            uint32_t RawSeedBits;      // 0x18
            uint32_t GroupID;          // 0x1C
            uint32_t Sequence;         // 0x20
            uint32_t ProductID;        // 0x24

            uint64_t KeyType;          // 0x28

            uint32_t InternalFlag;     // 0x30
            uint32_t State;            // 0x34

            uint8_t  TempWorkspace[0x40]; // SIMD / scratch overlap region
        } ctx;
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
    uint8_t data[16];
    uint8_t blockA[16] = {};
    uint8_t blockB[16] = {};

    memcpy(data, in, sizeof(data));

    // Extract header flags (upper bits of payload metadata)
    uint8_t flags = (uint8_t)(in->Half.High >> 48) & 0xF7;

    // Assembly-accurate Checksum Reconstruction
    uint8_t temp_lo = (data[12] >> 7) | (data[13] << 1);
    uint8_t temp_hi = (flags << 1) | (data[13] >> 7);
    uint16_t expected = ((uint16_t)temp_hi << 8) | temp_lo;

    // Remove checksum bits before hashing
    data[12] &= 0x7F;
    data[13] = 0;
    data[14] = flags & ~1;

    // Compute CRC-10
    uint32_t crc = 0xFFFFFFFF;
    for (int i = 0; i < 16; i++)
    {
        uint8_t idx = (uint8_t)((crc >> 24) ^ data[i]);
        crc = (crc << 8) ^ g_hash_table[idx];
    }

    uint16_t actual = (uint16_t)(~crc) & 0x3FF;

    if (expected != actual)
        return E_INVALIDARG;

    if (outHash)
        *outHash = actual;

    // --- Unswizzle ---

    // Copy first word directly
// 1. Recover blockA (ID / Group / Sequence)
    blockA[0] = data[0];
    blockA[1] = data[1];
    blockA[2] = data[2] & 0x0F; // Preserve lower nibble

    // Repack Loop (Matches loc_180008CDF)
    for (int i = 0; i < 3; i++) {
        blockA[i + 4] = (data[i + 2] >> 4) | (data[i + 3] << 4);
    }

    // Special mixing for Byte 7 (Matches 0x180008D12)
    blockA[7] = ((data[6] << 4) | (data[5] >> 4)) & 0x3F;

    // Flag injection (Matches 0x180008D61)
    blockA[8] = data[8] ^ (flags & 1);

    // 2. Recover blockB (Security / KeyType) - THE MISSING PART
    // Matches loc_180008D24 loop
    for (int i = 0; i < 6; i++) {
        blockB[i] = (data[i + 6] >> 2) | (data[i + 7] << 6);
    }
    // Tail of blockB (Matches 0x180008D5A)
    blockB[6] = (data[12] >> 2) & 0x1F;

    // 3. Output
    memcpy(out32, blockA, 16);
    memcpy(out32 + 16, blockB, 16);

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
    // Work buffer for the 13-byte reconstructed license stream
    uint8_t bitStream[16];
    memset(bitStream, 0, sizeof(bitStream));

    // 1. Initial State: Check existence of extended data at offset 8
    bitStream[0] = (*(uint32_t*)(pDecodedKey + 8) != 0) ? 1 : 0;

    // 2. Unpack Block A: 1-bit Left Shift
    // Merges pDecodedKey[4-6] into bitStream[0-3]
    for (int i = 0; i < 3; ++i) {
        uint8_t sourceByte = pDecodedKey[i + 4];
        bitStream[i] = (bitStream[i] & 0x01) | (sourceByte << 1);
        bitStream[i + 1] = (sourceByte >> 7);
    }

    // 3. Patching Block A: Insert bits from Key[7] into Stream[3]
    // Preserves bits 0 and 7 of Stream[3], overwrites bits 1-6
    uint8_t bridgeBits = pDecodedKey[7] << 1;
    bitStream[3] = (bitStream[3] & 0x81) | (bridgeBits & 0x7E);

    // 4. Unpack Block B: 7-bit Left Shift
    // Merges pDecodedKey[3-4] into bitStream[3-5]
    for (int i = 0; i < 2; ++i) {
        uint8_t sourceByte = pDecodedKey[i + 3];
        bitStream[i + 3] = (bitStream[i + 3] & 0x7F) | (sourceByte << 7);
        bitStream[i + 4] = (bitStream[i + 4] & 0x80) | (sourceByte >> 1);
    }

    // 5. Unpack Metadata: Align bits from Key[2]
    uint8_t metaByte = pDecodedKey[2];
    bitStream[5] = (bitStream[5] & 0x7F) | (metaByte << 7);        // High bit
    bitStream[6] = (bitStream[6] & 0xF8) | ((metaByte >> 1) & 0x07); // Low 3 bits

    // 6. Unpack Block C: 3-bit Left Shift
    // Merges pDecodedKey[16-21] into bitStream[6-12]
    for (int i = 0; i < 6; ++i) {
        uint8_t sourceByte = pDecodedKey[i + 16];
        bitStream[i + 6] = (bitStream[i + 6] & 0x07) | (sourceByte << 3);
        bitStream[i + 7] = (bitStream[i + 7] & 0xF8) | (sourceByte >> 5);
    }

    // --- FINAL PACKING TO CONTEXT ---

    // Write primary 8-byte QWORD (Offsets 0.0 - 0.7)
    *(uint64_t*)(pContext + 0x00) = *(uint64_t*)&bitStream[0];

    // Write secondary 4-byte DWORD (Offsets 0.8 - 0.B)
    *(uint32_t*)(pContext + 0x08) = *(uint32_t*)&bitStream[8];

    // Write final byte (Offset 0.C)
    // Combines Key[22] with the remainder of our bitStream
    uint8_t finalKeyByte = pDecodedKey[22];
    uint8_t streamRemnant = bitStream[12] & 0x07;
    pContext[0x0C] = (finalKeyByte << 3) | streamRemnant;

    return 0; // Success
}

// KeyInfo. BOB [MDL]
bool GetInfo_Gen4(const uint8_t* key3, LICENSE_WORKSPACE* pContext) {
    if (!key3 || !pContext) return false;

    // Bob's GroupId extraction (Bits 0-19)
    uint32_t groupid = 0;
    groupid |= key3[0];
    groupid |= (uint32_t)key3[1] << 8;
    groupid |= (uint32_t)(key3[2] & 0x0F) << 16;
    pContext->ctx.GroupID = groupid;

    // Bob's KeyId extraction (Bits 20-49)
    uint32_t keyid = 0;
    keyid |= (key3[2] >> 4);
    keyid |= (uint32_t)key3[3] << 4;
    keyid |= (uint32_t)key3[4] << 12;
    keyid |= (uint32_t)key3[5] << 20;
    keyid |= (uint32_t)(key3[6] & 0x3F) << 28;
    pContext->ctx.Sequence = keyid;

    // Bob's Secret extraction (Bits 50-113)
    uint64_t secret = 0;
    secret |= (key3[6] >> 2);
    secret |= (uint64_t)key3[7] << 6;
    secret |= (uint64_t)key3[8] << 14;
    secret |= (uint64_t)key3[9] << 22;
    secret |= (uint64_t)key3[10] << 30;
    secret |= (uint64_t)key3[11] << 38;
    secret |= (uint64_t)(key3[12] & 0x1F) << 46;
    // Mapping Secret to KeyType for your struct
    pContext->ctx.KeyType = secret;

    return true;
}

// sub_1800090B0
HRESULT DecodeLicenseContext(
    const wchar_t* ProductKey,
    LICENSE_CONTEXT* ctx,
    uint32_t* pHash,
    BOOLEAN AsKeyInfo)
{

    if (!ctx) return E_POINTER;
    if (!ProductKey) return E_INVALIDARG;

    HRESULT hr = S_OK;
    LICENSE_WORKSPACE Context = { 0 };
    LICENSE_WORKSPACE* pContext = &Context;

    // =====================================================
    // 1. COMPRESS (Base24 decode -> encoded[0])
    // =====================================================
    hr = CompressKey(
        ProductKey,
        &pContext->encoded,
        &pContext->isKeyValid
    );

    if (FAILED(hr))
        return hr;

    // =====================================================
    // 2. INTERNAL FLAG (matches buffer[0x30])
    // =====================================================
    *(uint32_t*)&pContext->raw[0x30] = 1;

    // =====================================================
    // KEY INFO FAST PATH
    // =====================================================
    if (pContext->isKeyValid && AsKeyInfo)
    {
        if (GetInfo_Gen4(pContext->encoded.Bytes, pContext))
            return S_OK;

        return E_FAIL;
    }

    if (!pContext->isKeyValid)
        return S_OK;

    // =====================================================
    // 3. UNSWIZZLE
    // =====================================================
    hr = ValidateAndUnswizzle(
        &pContext->encoded,
        pContext->unswizzled,
        pHash
    );

    if (hr == (HRESULT)0x8007000D)
        return (HRESULT)0x80041111;

    if (FAILED(hr))
        return hr;

    // =====================================================
    // 4. UNPACK
    // IMPORTANT: assembly writes into SAME CONTEXT region
    // =====================================================
    hr = UnpackLicenseContext(
        pContext->unswizzled,
        pContext->unpack
    );

    if (FAILED(hr))
        return hr;

    // =====================================================
    // 5. KEYTYPE
    // =====================================================
    uint64_t keyType = *(uint64_t*)&pContext->unswizzled[16];
    *(uint64_t*)&pContext->raw[0x28] = keyType;

    // =====================================================
    // 6. SALT + STATE
    // =====================================================
    memcpy(&pContext->raw[0x08], GEN4_SALT_DATA, 16);
    *(uint32_t*)&pContext->raw[0x34] = 1;

    // =====================================================
    // 7. GROUP + SEQUENCE (Bob model)
    // =====================================================
    uint32_t sequenceData = *(uint32_t*)&pContext->unswizzled[4];
    uint32_t sequence = sequenceData % 1000000;

    uint64_t rawData = *(uint64_t*)&pContext->unswizzled[0];
    uint32_t groupID = (uint32_t)(rawData & 0x3FFFF);

    *(uint32_t*)&pContext->raw[0x1C] = groupID;
    *(uint32_t*)&pContext->raw[0x20] = sequence;

    // =====================================================
    // 8. PRODUCT + SEED
    // =====================================================
    *(uint32_t*)&pContext->raw[0x24] =
        *(uint32_t*)&pContext->unswizzled[8];

    *(uint32_t*)&pContext->raw[0x18] =
        *(uint32_t*)&pContext->unswizzled[0];

    memcpy(ctx, pContext->raw, sizeof(LICENSE_CONTEXT));
    return hr;
}

int main() {

    /*
        Key      : K8KNG-MGG4H-KX82M-M8QYW-DGRFH
        Integer  : 4441847703199715836355246925287693
        Group    : 4365
        Serial   : 11
        Security : 12
        Checksum : 438
        Upgrade  : 0
        Extra    : 0
    */

    LICENSE_CONTEXT ctx = { 0 };
    const wchar_t* myKey = L"K8KNG-MGG4H-KX82M-M8QYW-DGRFH";

    std::cout << "\n";
    std::cout << "--- License Decoder Tool ---" << std::endl;
    uint32_t pHash = 0;
    HRESULT hr = DecodeLicenseContext(myKey, &ctx, &pHash, 0);

    if (SUCCEEDED(hr)) {
        std::wcout << L"Key:        " << myKey << std::endl;
        std::cout << "Result:     VALID" << std::endl;
        std::cout << "Group ID:   " << ctx.GroupID << std::endl;
        std::cout << "Serial:     " << ctx.Serial << std::endl;
        std::cout << "Security:   0x" << std::hex << ctx.Security << std::endl;
        std::cout << "Hash:       " << std::dec << pHash << std::endl;
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