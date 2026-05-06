
	//////////////////
	// ~~ *Info* ~~ //
	//////////////////

PS HKLM:\> Decode-Key QPM6N-7J2WJ-P88HH-P3YRH-YY74H

Key      : QPM6N-7J2WJ-P88HH-P3YRH-YY74H
Integer  : 5912361771358333156207981563482381
Group    : 4365
Serial   : 0
Security : 34969266479148
Checksum : 583
Upgrade  : 0
Extra    : 0

	/////////////////
	// ~ PidgenX ~ //
	/////////////////

// A1 == QPM6N-7J2WJ-P88HH-P3YRH-YY74H
__int64 __fastcall PidGenX2(
        wchar_t *Str,
        __int64 a2,
        __int64 a3,
        _WORD *a4,
        __int64 a5,
        __int64 a6,
        unsigned __int8 *a7,
        _DWORD *a8)
{
  v25 = (*(__int64 (__fastcall **)(__int64, _QWORD, wchar_t *, __int64, __int64, LPVOID *))(*(_QWORD *)v54 + 64i64))(
          v54,
          0i64,
          Str,
          a5,
          v49,
          &lpMem);
}

.rdata:00000001800A02D0 off_1800A02D0   dq offset sub_180012AE0 ; DATA XREF: sub_180015270+BB↑o
.rdata:00000001800A02D8                 dq offset sub_18000AC90
.rdata:00000001800A02E0                 dq offset sub_180012DA0
.rdata:00000001800A02E8                 dq offset sub_18000C6E0
.rdata:00000001800A02F0                 dq offset sub_18000C6D0
.rdata:00000001800A02F8                 dq offset sub_18000C970
.rdata:00000001800A0300                 dq offset sub_18000C960
.rdata:00000001800A0308                 dq offset sub_18000EF70
.rdata:00000001800A0310                 dq offset sub_18000D160

// A3 == QPM6N-7J2WJ-P88HH-P3YRH-YY74H
__int64 __fastcall sub_18000D160(int a1, int a2, int a3, int a4, int a5, __int64 a6)
{
  return sub_180009A24(a1 + 16, a2, a3, a4, a5, a6);
}

// a3 == QPM6N-7J2WJ-P88HH-P3YRH-YY74H
__int64 __fastcall sub_180009A24(__int64 a1, __int64 a2, __int64 a3, __int64 a4, int a5, __int64 a6)
{
    v33 = a3;
    v21 = sub_180009338(a1, a2, v33, v31, v9, v8, a5, a6);
}

	/////////////////
	// MAIN CALL ! //
	/////////////////

.rdata:00000001800A6788 unk_1800A6788   db 0EFh                 ; DATA XREF: sub_1800090B0+D4↑r
.rdata:00000001800A6788                                         ; sub_18000D828+9A↑r ...
.rdata:00000001800A6789                 db  72h ; r
.rdata:00000001800A678A                 db    6
.rdata:00000001800A678B                 db  66h ; f

.rdata:00000001800A67F0 dword_1800A67F0 dd 0, 4C11DB7h, 9823B6Eh, 0D4326D9h, 130476DCh, 17C56B6Bh
.rdata:00000001800A67F0                                         ; DATA XREF: sub_180008BCC+C0↑o
.rdata:00000001800A67F0                 dd 1A864DB2h, 1E475005h, 2608EDB8h, 22C9F00Fh, 2F8AD6D6h
.rdata:00000001800A67F0                 dd 2B4BCB61h, 350C9B64h, 31CD86D3h, 3C8EA00Ah, 384FBDBDh
.rdata:00000001800A67F0                 dd 4C11DB70h, 48D0C6C7h, 4593E01Eh, 4152FDA9h, 5F15ADACh
.rdata:00000001800A67F0                 dd 5BD4B01Bh, 569796C2h, 52568B75h, 6A1936C8h, 6ED82B7Fh
.rdata:00000001800A67F0                 dd 639B0DA6h, 675A1011h, 791D4014h, 7DDC5DA3h, 709F7B7Ah
.rdata:00000001800A67F0                 dd 745E66CDh, 9823B6E0h, 9CE2AB57h, 91A18D8Eh, 95609039h
.rdata:00000001800A67F0                 dd 8B27C03Ch, 8FE6DD8Bh, 82A5FB52h, 8664E6E5h, 0BE2B5B58h
.rdata:00000001800A67F0                 dd 0BAEA46EFh, 0B7A96036h, 0B3687D81h, 0AD2F2D84h, 0A9EE3033h
.rdata:00000001800A67F0                 dd 0A4AD16EAh, 0A06C0B5Dh, 0D4326D90h, 0D0F37027h, 0DDB056FEh
.rdata:00000001800A67F0                 dd 0D9714B49h, 0C7361B4Ch, 0C3F706FBh, 0CEB42022h, 0CA753D95h
.rdata:00000001800A67F0                 dd 0F23A8028h, 0F6FB9D9Fh, 0FBB8BB46h, 0FF79A6F1h, 0E13EF6F4h
.rdata:00000001800A67F0                 dd 0E5FFEB43h, 0E8BCCD9Ah, 0EC7DD02Dh, 34867077h, 30476DC0h
.rdata:00000001800A67F0                 dd 3D044B19h, 39C556AEh, 278206ABh, 23431B1Ch, 2E003DC5h
.rdata:00000001800A67F0                 dd 2AC12072h, 128E9DCFh, 164F8078h, 1B0CA6A1h, 1FCDBB16h
.rdata:00000001800A67F0                 dd 18AEB13h, 54BF6A4h, 808D07Dh, 0CC9CDCAh, 7897AB07h
.rdata:00000001800A67F0                 dd 7C56B6B0h, 71159069h, 75D48DDEh, 6B93DDDBh, 6F52C06Ch
.rdata:00000001800A67F0                 dd 6211E6B5h, 66D0FB02h, 5E9F46BFh, 5A5E5B08h, 571D7DD1h
.rdata:00000001800A67F0                 dd 53DC6066h, 4D9B3063h, 495A2DD4h, 44190B0Dh, 40D816BAh
.rdata:00000001800A67F0                 dd 0ACA5C697h, 0A864DB20h, 0A527FDF9h, 0A1E6E04Eh, 0BFA1B04Bh
.rdata:00000001800A67F0                 dd 0BB60ADFCh, 0B6238B25h, 0B2E29692h, 8AAD2B2Fh, 8E6C3698h
.rdata:00000001800A67F0                 dd 832F1041h, 87EE0DF6h, 99A95DF3h, 9D684044h, 902B669Dh
.rdata:00000001800A67F0                 dd 94EA7B2Ah, 0E0B41DE7h, 0E4750050h, 0E9362689h, 0EDF73B3Eh
.rdata:00000001800A67F0                 dd 0F3B06B3Bh, 0F771768Ch, 0FA325055h, 0FEF34DE2h, 0C6BCF05Fh
.rdata:00000001800A67F0                 dd 0C27DEDE8h, 0CF3ECB31h, 0CBFFD686h, 0D5B88683h, 0D1799B34h
.rdata:00000001800A67F0                 dd 0DC3ABDEDh, 0D8FBA05Ah, 690CE0EEh, 6DCDFD59h, 608EDB80h
.rdata:00000001800A67F0                 dd 644FC637h, 7A089632h, 7EC98B85h, 738AAD5Ch, 774BB0EBh
.rdata:00000001800A67F0                 dd 4F040D56h, 4BC510E1h, 46863638h, 42472B8Fh, 5C007B8Ah
.rdata:00000001800A67F0                 dd 58C1663Dh, 558240E4h, 51435D53h, 251D3B9Eh, 21DC2629h
.rdata:00000001800A67F0                 dd 2C9F00F0h, 285E1D47h, 36194D42h, 32D850F5h, 3F9B762Ch
.rdata:00000001800A67F0                 dd 3B5A6B9Bh, 315D626h, 7D4CB91h, 0A97ED48h, 0E56F0FFh
.rdata:00000001800A67F0                 dd 1011A0FAh, 14D0BD4Dh, 19939B94h, 1D528623h, 0F12F560Eh
.rdata:00000001800A67F0                 dd 0F5EE4BB9h, 0F8AD6D60h, 0FC6C70D7h, 0E22B20D2h, 0E6EA3D65h
.rdata:00000001800A67F0                 dd 0EBA91BBCh, 0EF68060Bh, 0D727BBB6h, 0D3E6A601h, 0DEA580D8h
.rdata:00000001800A67F0                 dd 0DA649D6Fh, 0C423CD6Ah, 0C0E2D0DDh, 0CDA1F604h, 0C960EBB3h
.rdata:00000001800A67F0                 dd 0BD3E8D7Eh, 0B9FF90C9h, 0B4BCB610h, 0B07DABA7h, 0AE3AFBA2h
.rdata:00000001800A67F0                 dd 0AAFBE615h, 0A7B8C0CCh, 0A379DD7Bh, 9B3660C6h, 9FF77D71h
.rdata:00000001800A67F0                 dd 92B45BA8h, 9675461Fh, 8832161Ah, 8CF30BADh, 81B02D74h
.rdata:00000001800A67F0                 dd 857130C3h, 5D8A9099h, 594B8D2Eh, 5408ABF7h, 50C9B640h
.rdata:00000001800A67F0                 dd 4E8EE645h, 4A4FFBF2h, 470CDD2Bh, 43CDC09Ch, 7B827D21h
.rdata:00000001800A67F0                 dd 7F436096h, 7200464Fh, 76C15BF8h, 68860BFDh, 6C47164Ah
.rdata:00000001800A67F0                 dd 61043093h, 65C52D24h, 119B4BE9h, 155A565Eh, 18197087h
.rdata:00000001800A67F0                 dd 1CD86D30h, 29F3D35h, 65E2082h, 0B1D065Bh, 0FDC1BECh
.rdata:00000001800A67F0                 dd 3793A651h, 3352BBE6h, 3E119D3Fh, 3AD08088h, 2497D08Dh
.rdata:00000001800A67F0                 dd 2056CD3Ah, 2D15EBE3h, 29D4F654h, 0C5A92679h, 0C1683BCEh
.rdata:00000001800A67F0                 dd 0CC2B1D17h, 0C8EA00A0h, 0D6AD50A5h, 0D26C4D12h, 0DF2F6BCBh
.rdata:00000001800A67F0                 dd 0DBEE767Ch, 0E3A1CBC1h, 0E760D676h, 0EA23F0AFh, 0EEE2ED18h
.rdata:00000001800A67F0                 dd 0F0A5BD1Dh, 0F464A0AAh, 0F9278673h, 0FDE69BC4h, 89B8FD09h
.rdata:00000001800A67F0                 dd 8D79E0BEh, 803AC667h, 84FBDBD0h, 9ABC8BD5h, 9E7D9662h
.rdata:00000001800A67F0                 dd 933EB0BBh, 97FFAD0Ch, 0AFB010B1h, 0AB710D06h, 0A6322BDFh
.rdata:00000001800A67F0                 dd 0A2F33668h, 0BCB4666Dh, 0B8757BDAh, 0B5365D03h, 0B1F740B4h
.rdata:00000001800A6BF0 ; const wchar_t aMsftRmAlgorith_0

// a3 == QPM6N-7J2WJ-P88HH-P3YRH-YY74H
__int64 __fastcall sub_180009338(
        __int64 a1,
        const wchar_t *a2,
        __int64 a3,
        unsigned __int16 *a4,
        _DWORD *a5,
        unsigned int a6,
        int a7,
        __int64 a8)
{
  _QWORD v68[12]; // [rsp+70h] [rbp-89h] BYREF
  memset(v68, 0, 0x58ui64);
  memset(&v68[1], 0, 40);
  v15 = sub_1800090B0(a3, v68);
}

// a1 == QPM6N-7J2WJ-P88HH-P3YRH-YY74H
// Take CD-Key ANd make it Struct,
// $group    = [Marshal]::ReadInt32(a2, 0x18)
// $serial   = [Marshal]::ReadInt32(a2, 0x20)
// $security = [Marshal]::ReadInt64(a2, 0x28)
__int64 __fastcall sub_1800090B0(__int64 a1, __int64 a2)
{
  __int64 v4; // r8
  unsigned int v5; // ebx
  int v6; // ecx
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
        v7 = sub_180008A58(&v14, (char *)&v16[4] + 8);
        v5 = v7;
        if ( v7 < 0 )
        {
LABEL_7:
          v6 = v7;
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
  sub_180003B30(0);
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
    sub_1800038E8(-2147024883);
  }
  sub_180003B30(v3);
  return v3;
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
  sub_180003B30(0);
  return 0i64;
}
__int64 __fastcall sub_180008DB4(_WORD *a1, __int64 a2, _BYTE *a3, __int64 a4, int *a5)
{
  unsigned int v5; // ebx
  int v7; // r14d
  unsigned int v8; // esi
  unsigned int v9; // ebp
  unsigned int v10; // edi
  unsigned int v12; // ecx
  unsigned int v13; // ecx
  unsigned int v14; // ecx
  unsigned int v15; // ecx
  unsigned int v16; // ecx
  unsigned int v17; // ecx
  int v18; // eax
  unsigned int v19; // ecx
  unsigned int v20; // ecx
  unsigned int v21; // ecx
  unsigned int v22; // ecx
  unsigned int v23; // ecx
  unsigned int v24; // ecx
  unsigned int v25; // ecx
  unsigned int v26; // ecx
  unsigned int v27; // ecx
  unsigned int v28; // ecx
  unsigned int v29; // ecx
  unsigned int v30; // ecx
  unsigned int v31; // ecx
  unsigned int v32; // ecx
  unsigned int v33; // ecx
  unsigned int v34; // ecx
  unsigned int v35; // ecx
  unsigned int v36; // ecx
  unsigned int v37; // ecx
  unsigned int v38; // ecx
  unsigned int v39; // ecx
  unsigned int v40; // ecx

  v5 = 0;
  v7 = 0;
  v8 = 0;
  v9 = 0;
  v10 = 0;
  while ( v10 < 0x19 )
  {
    if ( ((*a1 - 78) & 0xFFDF) == 0 )
    {
      if ( v7 || v10 >= 0x18 )
        break;
      v7 = 1;
      memmove(a3 + 1, a3, v10);
      *a3 = v10;
      goto LABEL_82;
    }
    v12 = (unsigned __int16)*a1;
    if ( v12 > 0x56 )
    {
      if ( v12 > 0x6B )
      {
        v33 = v12 - 109;
        if ( !v33 )
        {
LABEL_77:
          LOBYTE(v17) = 8;
          goto LABEL_78;
        }
        v34 = v33 - 3;
        if ( !v34 )
        {
LABEL_76:
          LOBYTE(v17) = 9;
          goto LABEL_78;
        }
        v35 = v34 - 1;
        if ( !v35 )
        {
LABEL_75:
          LOBYTE(v17) = 10;
          goto LABEL_78;
        }
        v36 = v35 - 1;
        if ( !v36 )
        {
LABEL_74:
          LOBYTE(v17) = 11;
          goto LABEL_78;
        }
        v37 = v36 - 2;
        if ( v37 )
        {
          v38 = v37 - 2;
          if ( !v38 )
          {
LABEL_73:
            LOBYTE(v17) = 13;
            goto LABEL_78;
          }
          v39 = v38 - 1;
          if ( !v39 )
          {
LABEL_72:
            LOBYTE(v17) = 14;
            goto LABEL_78;
          }
          v40 = v39 - 1;
          if ( !v40 )
          {
LABEL_71:
            LOBYTE(v17) = 15;
            goto LABEL_78;
          }
          if ( v40 != 1 )
            break;
          goto LABEL_70;
        }
        goto LABEL_40;
      }
      if ( v12 != 107 )
      {
        if ( v12 > 0x64 )
        {
          v30 = v12 - 102;
          if ( !v30 )
            goto LABEL_60;
          v31 = v30 - 1;
          if ( !v31 )
            goto LABEL_59;
          v32 = v31 - 1;
          if ( !v32 )
            goto LABEL_58;
          if ( v32 != 2 )
            break;
LABEL_57:
          LOBYTE(v17) = 6;
          goto LABEL_78;
        }
        if ( v12 != 100 )
        {
          v28 = v12 - 87;
          if ( !v28 )
            goto LABEL_72;
          v29 = v28 - 1;
          if ( !v29 )
            goto LABEL_71;
          v20 = v29 - 1;
          if ( v20 )
          {
LABEL_48:
            v17 = v20 - 9;
            if ( v17 )
            {
              if ( v17 != 1 )
                break;
            }
            else
            {
              LOBYTE(v17) = 0;
            }
            goto LABEL_78;
          }
LABEL_70:
          LOBYTE(v17) = 16;
          goto LABEL_78;
        }
LABEL_52:
        LOBYTE(v17) = 2;
        goto LABEL_78;
      }
      goto LABEL_35;
    }
    if ( v12 == 86 )
      goto LABEL_73;
    if ( v12 > 0x44 )
    {
      if ( v12 > 0x4D )
      {
        v25 = v12 - 80;
        if ( !v25 )
          goto LABEL_76;
        v26 = v25 - 1;
        if ( !v26 )
          goto LABEL_75;
        v27 = v26 - 1;
        if ( !v27 )
          goto LABEL_74;
        if ( v27 != 2 )
          break;
LABEL_40:
        LOBYTE(v17) = 12;
        goto LABEL_78;
      }
      if ( v12 == 77 )
        goto LABEL_77;
      v21 = v12 - 70;
      if ( !v21 )
      {
LABEL_60:
        LOBYTE(v17) = 3;
        goto LABEL_78;
      }
      v22 = v21 - 1;
      if ( !v22 )
      {
LABEL_59:
        LOBYTE(v17) = 4;
        goto LABEL_78;
      }
      v23 = v22 - 1;
      if ( !v23 )
      {
LABEL_58:
        LOBYTE(v17) = 5;
        goto LABEL_78;
      }
      v24 = v23 - 2;
      if ( !v24 )
        goto LABEL_57;
      if ( v24 != 1 )
        break;
LABEL_35:
      LOBYTE(v17) = 7;
      goto LABEL_78;
    }
    if ( v12 == 68 )
      goto LABEL_52;
    if ( v12 <= 0x37 )
    {
      if ( v12 == 55 )
      {
        LOBYTE(v17) = 21;
      }
      else
      {
        v13 = v12 - 45;
        if ( !v13 )
        {
          if ( v9 > 0x17 )
            break;
          v18 = 8521760;
          if ( !_bittest(&v18, v9) || v8 >= 4 )
            break;
          ++v8;
          goto LABEL_83;
        }
        v14 = v13 - 5;
        if ( v14 )
        {
          v15 = v14 - 1;
          if ( v15 )
          {
            v16 = v15 - 1;
            if ( v16 )
            {
              if ( v16 != 2 )
                break;
              LOBYTE(v17) = 20;
            }
            else
            {
              LOBYTE(v17) = 19;
            }
          }
          else
          {
            LOBYTE(v17) = 18;
          }
        }
        else
        {
          LOBYTE(v17) = 17;
        }
      }
      goto LABEL_78;
    }
    v19 = v12 - 56;
    if ( v19 )
    {
      v20 = v19 - 1;
      if ( v20 )
        goto LABEL_48;
      LOBYTE(v17) = 23;
    }
    else
    {
      LOBYTE(v17) = 22;
    }
LABEL_78:
    a3[v10] = v17;
LABEL_82:
    ++v10;
LABEL_83:
    ++v9;
    ++a1;
    if ( v9 >= 0x1D )
    {
      if ( v8 == 4 && v10 == 25 )
      {
        *a5 = v7;
        goto LABEL_88;
      }
      break;
    }
  }
  v5 = -2147217327;
  sub_1800038E8(-2147217327);
LABEL_88:
  sub_180003B30(v5);
  return v5;
}


// Pidgenx.dll, Retail Version, sub_180020A1C
// Take Binary Key ANd make it Struct,
// $group    = [Marshal]::ReadInt32(a2, 0x00)
// $serial   = [Marshal]::ReadInt32(a2, 0x04)
// $security = [Marshal]::ReadInt32(a2, 0x10)
__int64 __fastcall sub_180020A1C(__m128i* a1, _OWORD* a2, BOOL* a3)
{
    void(__fastcall * v6)(__int128*); // rbx
    __m128i v7; // xmm1
    unsigned int v8; // edi
    __int64 v9; // r15
    int v10; // r8d
    unsigned __int64 v11; // rcx
    unsigned __int8 v12; // r14
    unsigned __int8 v13; // dl
    char v14; // si
    unsigned __int8 v15; // r14
    char v16; // al
    unsigned __int8 v17; // si
    char v18; // al
    __int128* v19; // rdx
    unsigned int v20; // ebx
    __int64 v21; // rax
    __int8* v22; // r8
    __int64 v23; // rdx
    char v24; // al
    __int8* v25; // r8
    __int64 v26; // r9
    __int64 v27; // rdx
    char v28; // al
    __int128 v29; // xmm1
    int v31; // [rsp+20h] [rbp-50h]
    BOOL v32; // [rsp+24h] [rbp-4Ch]
    __int128 v33; // [rsp+30h] [rbp-40h] BYREF
    __int128 v34; // [rsp+40h] [rbp-30h]
    __m128i v35; // [rsp+50h] [rbp-20h] BYREF

    v6 = (void(__fastcall*)(__int128*))sub_18000F2AC(&unk_1800DDF42);
    v6(&v33);
    sub_1800071F0((__int64)v6);
    v7 = *a1;
    v8 = 0;
    v35 = v7;
    v9 = 3i64;
    v10 = 16;
    v11 = HIWORD(_mm_srli_si128(v7, 8).m128i_u64[0]);
    v12 = _mm_cvtsi128_si32(_mm_srli_si128(v7, 12));
    v13 = _mm_cvtsi128_si32(_mm_srli_si128(v7, 13));
    v32 = (v11 & 8) != 0;
    HIWORD(v31) = 0;
    v14 = v11 ^ (v11 ^ (4 * ((v11 & 8) != 0))) & 8;
    LOBYTE(v11) = v12 >> 7;
    v15 = v12 & 0x7F;
    v35.m128i_i16[6] = v15;
    LOBYTE(v31) = (2 * v13) | v11;
    v16 = 2 * v14;
    v17 = v14 & 0xFE;
    v35.m128i_i8[14] = v17;
    v18 = v16 & 3 | (v13 >> 7);
    v19 = (__int128*)&v35;
    BYTE1(v31) = v18;
    v20 = -1;
    do
    {
        v21 = *(unsigned __int8*)v19;
        v19 = (__int128*)((char*)v19 + 1);
        v20 = (v20 << 8) ^ dword_1800CAD90[v21 ^ ((unsigned __int64)v20 >> 24)];
        --v10;
    } while (v10);
    sub_180002E28(0i64);
    if (v31 == (~(_WORD)v20 & 0x3FF))
    {
        v22 = &v35.m128i_i8[3];
        v23 = 0i64;
        LOWORD(v33) = v35.m128i_i16[0];
        BYTE2(v33) ^= (BYTE2(v33) ^ v35.m128i_i8[2]) & 0xF;
        do
        {
            v24 = *v22++;
            *((_BYTE*)&v33 + v23 + 4) = (16 * v24) | ((unsigned __int8)v35.m128i_i8[v23 + 2] >> 4);
            ++v23;
            --v9;
        } while (v9);
        v25 = &v35.m128i_i8[7];
        v26 = 6i64;
        v27 = 0i64;
        BYTE7(v33) ^= (BYTE7(v33) ^ (*(__int16*)((char*)&v35.m128i_i16[2] + 1) >> 4)) & 0x3F;
        do
        {
            v28 = *v25++;
            *((_BYTE*)&v34 + v27) = (v28 << 6) | ((unsigned __int8)v35.m128i_i8[v27 + 6] >> 2);
            ++v27;
            --v26;
        } while (v26);
        BYTE8(v33) ^= (BYTE8(v33) ^ (v17 >> 1)) & 1;
        BYTE6(v34) ^= (BYTE6(v34) ^ (v15 >> 2)) & 0x1F;
        v29 = v34;
        *a2 = v33;
        a2[1] = v29;
        if (a3)
            *a3 = v32;
    }
    else
    {
        v8 = -2147024883;
        sub_180002E08(-2147024883);
    }
    sub_180002E28(v8);
    return v8;
}
