// v16 = v15(a3, v65);
// in Unprotected file ... 

// Size: 0x58 (88 bytes) // v12
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
	
Full Context Hex Dump:
0000: 00 00 00 00 00 00 00 00 EF 72 06 66 09 78 FD 4C
0010: 8D 54 41 B7 FB 73 89 88 0D 11 00 00 00 00 00 00
0020: 00 00 00 00 00 00 00 00 2C E0 B4 EA CD 1F 00 00
0030: 01 00 00 00 01 00 00 00 0D 11 00 00 00 00 B0 80
0040: D3 AA 37 7F 80 23 09 00 00 00 00 80 86 08 60 01
0050: A7 55 6F FE 00 00 00 00 00 00 00 00 00 00 00 00

.rdata:00000001800A6788 unk_1800A6788   db 0EFh                 ; DATA XREF: sub_1800090B0+D4↑r
.rdata:00000001800A6788                                         ; sub_18000D828+9A↑r ...
.rdata:00000001800A6789                 db  72h ; r
.rdata:00000001800A678A                 db    6
.rdata:00000001800A678B                 db  66h ; f

__int64 __fastcall sub_1800090B0(__int64 a1, __int64 a2)
{
  __int64 v4; // r8
  unsigned int v5; // ebx
  __int64 v6; // rcx
  int v7; // eax
  int v8; // eax
  __int128 v9; // xmm1
  __int128 v10; // xmm0
  __int128 v11; // xmm1
  __int128 v12; // xmm0
  __int128 v14; // [rsp+28h] [rbp-29h] BYREF
  __int128 v15; // [rsp+38h] [rbp-19h]
  __int128 v16[6]; // [rsp+48h] [rbp-9h] BYREF
  int v17; // [rsp+C0h] [rbp+6Fh] BYREF

  v17 = 0;
  v14 = 0i64;
  v15 = 0i64;
  memset(v16, 0, 0x58ui64);
  v5 = 0;
  *((_QWORD *)&v16[2] + 1) = 0i64;
  *(__int128 *)((char *)v16 + 8) = 0i64;
  *(__int128 *)((char *)&v16[1] + 8) = 0i64;
  if ( a2 )
  {
    if ( a1 )
    {
      v7 = sub_18000890C(a1, (char *)&v16[3] + 8, v4, &v17);
      v5 = v7;
      if ( v7 < 0 )
        goto LABEL_7;
      LODWORD(v16[3]) = 1;
      if ( v17 )
      {
        v8 = sub_180008BCC((char *)&v16[3] + 8, &v14);
        v5 = v8;
        if ( v8 == -2147024883 )
        {
          v5 = -2147217327;
          goto LABEL_3;
        }
        if ( v8 < 0 )
          goto LABEL_3;
        v7 = sub_180008A58((__int64)&v14, (__int64)&v16[4] + 8);
        v5 = v7;
        if ( v7 < 0 )
        {
LABEL_7:
          v6 = (unsigned int)v7;
          goto LABEL_4;
        }
        *((_QWORD *)&v16[2] + 1) = v15;
        DWORD1(v16[2]) = DWORD2(v14);
        *(__int128 *)((char *)v16 + 8) = unk_1800A6788;
        DWORD1(v16[3]) = 1;
        HIDWORD(v16[1]) = DWORD1(v14) / 0xF4240;
        LODWORD(v16[2]) = DWORD1(v14) % 0xF4240;
        DWORD2(v16[1]) = v14;
      }
    }
    v9 = v16[1];
    *(_OWORD *)a2 = v16[0];
    v10 = v16[2];
    *(_OWORD *)(a2 + 16) = v9;
    v11 = v16[3];
    *(_OWORD *)(a2 + 32) = v10;
    v12 = v16[4];
    *(_OWORD *)(a2 + 48) = v11;
    *(_QWORD *)&v11 = *(_QWORD *)&v16[5];
    *(_OWORD *)(a2 + 64) = v12;
    *(_QWORD *)(a2 + 80) = v11;
    goto LABEL_15;
  }
  v5 = -2147024809;
LABEL_3:
  v6 = v5;
LABEL_4:
  sub_1800038E8(v6);
LABEL_15:
  sub_180003B30(v5);
  return v5;
}
__int64 __fastcall sub_180008A58(__int64 a1, __int64 a2)
{
  __int64 *v2; // r8
  bool v3; // zf
  _BYTE *v4; // r9
  __int64 v6; // rdx
  __int64 v8; // r11
  unsigned __int8 v9; // cl
  _BYTE *v10; // rdx
  _BYTE *v11; // r8
  __int64 v12; // r11
  unsigned __int8 v13; // cl
  __int64 v14; // r11
  _BYTE *v15; // r8
  unsigned __int8 v16; // cl
  __int64 v17; // r9
  _BYTE *v18; // rdx
  __int64 v19; // r10
  unsigned __int8 v20; // cl
  char v21; // cl
  char v22; // al
  __int64 v24[2]; // [rsp+20h] [rbp-20h] BYREF

  v2 = v24;
  v3 = *(_DWORD *)(a1 + 8) == 0;
  v4 = (char *)v24 + 1;
  memset(&v24[1], 0, 5);
  v24[0] = !v3;
  v6 = a1 + 4 - (_QWORD)v24;
  v8 = 3i64;
  do
  {
    v9 = *((_BYTE *)v2 + v6);
    *(_BYTE *)v2 &= 1u;
    *v4 &= ~1u;
    *(_BYTE *)v2 |= 2 * v9;
    v2 = (__int64 *)((char *)v2 + 1);
    *v4++ |= v9 >> 7;
    --v8;
  }
  while ( v8 );
  v10 = (char *)v24 + 3;
  v11 = (char *)v24 + 4;
  v12 = 2i64;
  BYTE3(v24[0]) ^= (BYTE3(v24[0]) ^ (2 * *(_BYTE *)(a1 + 7))) & 0x7E;
  do
  {
    v13 = v10[a1 - ((_QWORD)v24 + 3)];
    *v10 &= ~0x80u;
    *v11 &= 0x80u;
    *v10++ |= v13 << 7;
    *v11++ |= v13 >> 1;
    --v12;
  }
  while ( v12 );
  v14 = a1 + 16;
  v15 = (char *)v24 + 7;
  v16 = *(_BYTE *)(a1 + 2);
  v17 = 6i64;
  BYTE5(v24[0]) = BYTE5(v24[0]) & 0x7F | (v16 << 7);
  v18 = (char *)v24 + 6;
  BYTE6(v24[0]) ^= (BYTE6(v24[0]) ^ (v16 >> 1)) & 7;
  v19 = a1 + 16 - ((_QWORD)v24 + 6);
  do
  {
    v20 = v18[v19];
    *v18 &= 7u;
    *v15 &= 0xF8u;
    *v18++ |= 8 * v20;
    *v15++ |= v20 >> 5;
    --v17;
  }
  while ( v17 );
  v21 = *(_BYTE *)(v14 + 6);
  v22 = BYTE4(v24[1]) & 7;
  *(_QWORD *)a2 = v24[0];
  *(_DWORD *)(a2 + 8) = v24[1];
  *(_BYTE *)(a2 + 12) = v22 | (8 * v21);
  sub_180003B30(0i64);
  return 0i64;
}
__int64 __fastcall sub_180008BCC(__int64 a1, _OWORD *a2)
{
  unsigned int v3; // edi
  __int64 v4; // r8
  __int64 v5; // r15
  unsigned __int8 v6; // si
  char v7; // cl
  __int128 *v8; // rdx
  int v9; // r8d
  unsigned __int8 v10; // r14
  unsigned int v11; // ebx
  __int64 v12; // rax
  char *v13; // r8
  __int64 v14; // rdx
  char v15; // al
  char *v16; // r8
  __int64 v17; // r9
  __int64 v18; // rdx
  char v19; // al
  __int128 v20; // xmm1
  int v22; // [rsp+20h] [rbp-50h]
  __int128 v23; // [rsp+30h] [rbp-40h]
  __int128 v24; // [rsp+40h] [rbp-30h]
  __int128 v25; // [rsp+50h] [rbp-20h] BYREF

  v3 = 0;
  v4 = HIWORD(*(_QWORD *)(a1 + 8));
  v25 = *(_OWORD *)a1;
  LOBYTE(v4) = v4 & 0xF7;
  v5 = 3i64;
  v22 = (unsigned __int8)(SWORD6(v25) >> 7);
  v6 = v4 ^ v4 & 1;
  BYTE14(v25) = v6;
  v7 = (2 * v4) | (BYTE13(v25) >> 7);
  v8 = &v25;
  v9 = 16;
  v10 = BYTE12(v25) & 0x7F;
  BYTE1(v22) = v7 & 3;
  v23 = 0i64;
  v11 = -1;
  v24 = 0i64;
  WORD6(v25) = BYTE12(v25) & 0x7F;
  do
  {
    v12 = *(unsigned __int8 *)v8;
    v8 = (__int128 *)((char *)v8 + 1);
    v11 = dword_1800A67F0[v12 ^ ((unsigned __int64)v11 >> 24)] ^ (v11 << 8);
    --v9;
  }
  while ( v9 );
  sub_180003B30(0i64);
  if ( v22 == (~(_WORD)v11 & 0x3FF) )
  {
    v13 = (char *)&v25 + 3;
    v14 = 0i64;
    LOWORD(v23) = v25;
    BYTE2(v23) ^= (BYTE2(v23) ^ BYTE2(v25)) & 0xF;
    do
    {
      v15 = *v13++;
      *((_BYTE *)&v23 + v14 + 4) = (16 * v15) | (*((_BYTE *)&v25 + v14 + 2) >> 4);
      ++v14;
      --v5;
    }
    while ( v5 );
    v16 = (char *)&v25 + 7;
    v17 = 6i64;
    v18 = 0i64;
    BYTE7(v23) = BYTE7(v23) & 0xC0 ^ (*(__int16 *)((char *)&v25 + 5) >> 4) & 0x3F;
    do
    {
      v19 = *v16++;
      *((_BYTE *)&v24 + v18) = (v19 << 6) | (*((_BYTE *)&v25 + v18 + 6) >> 2);
      ++v18;
      --v17;
    }
    while ( v17 );
    BYTE6(v24) ^= (BYTE6(v24) ^ (v10 >> 2)) & 0x1F;
    v20 = v24;
    BYTE8(v23) ^= (BYTE8(v23) ^ (v6 >> 1)) & 1;
    *a2 = v23;
    a2[1] = v20;
  }
  else
  {
    v3 = -2147024883;
    sub_1800038E8(2147942413i64);
  }
  sub_180003B30(v3);
  return v3;
}
__int64 __fastcall sub_18000890C(_WORD *a1, _OWORD *a2, __int64 a3, _DWORD *a4)
{
  _WORD *v7; // rax
  __int64 v8; // rcx
  int v9; // eax
  unsigned int v10; // ebx
  __int64 v11; // rcx
  char *v12; // r9
  unsigned int v13; // edx
  unsigned int i; // r10d
  unsigned int v15; // r8d
  __int128 *v16; // r11
  __int64 v17; // rdi
  unsigned int v18; // r8d
  __int64 v19; // rax
  __int128 v21; // [rsp+38h] [rbp-60h] BYREF
  char v22[32]; // [rsp+48h] [rbp-50h] BYREF

  if ( !a1 )
    goto LABEL_19;
  v7 = a1;
  v8 = 30i64;
  do
  {
    if ( !*v7 )
      break;
    ++v7;
    --v8;
  }
  while ( v8 );
  if ( !v8 || ((30 - v8) & ((unsigned __int128)-(__int128)(unsigned __int64)v8 >> 64)) != 29 )
  {
LABEL_19:
    v11 = 2147749969i64;
    v10 = -2147217327;
    goto LABEL_20;
  }
  v9 = sub_180008DB4(a1, 29i64, v22);
  v10 = v9;
  if ( v9 < 0 )
  {
    v11 = (unsigned int)v9;
LABEL_20:
    sub_1800038E8(v11);
    goto LABEL_21;
  }
  v12 = v22;
  v21 = 0i64;
  v13 = 0;
  for ( i = 0; i < 0x19; ++i )
  {
    v15 = (unsigned __int8)*v12;
    if ( v13 )
    {
      v16 = &v21;
      v17 = v13;
      do
      {
        v18 = v15 + 24 * *(unsigned __int8 *)v16;
        *(_BYTE *)v16 = v18;
        v16 = (__int128 *)((char *)v16 + 1);
        v15 = v18 >> 8;
        --v17;
      }
      while ( v17 );
    }
    if ( v15 )
    {
      if ( v13 >= 0x10 )
        goto LABEL_19;
      v19 = v13++;
      v22[v19 - 16] = v15;
    }
    ++v12;
  }
  *a2 = v21;
  if ( a4 )
    *a4 = 0;
LABEL_21:
  sub_180003B30(v10);
  return v10;
}