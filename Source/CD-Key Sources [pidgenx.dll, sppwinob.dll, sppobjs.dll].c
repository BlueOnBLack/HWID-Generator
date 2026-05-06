
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~ Encode Binary Key ~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

 /**
 * Origin: Sppobjs.dll
 * __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryEncode(__m128i *a1)
 * Takes the String (input) and packs it into Binary (storage).
 */
 
 __int64 __fastcall sub_18008A114(int a1)
{
  __int64 result; // rax

  result = (unsigned int)dword_1801A47C4;
  if ( dword_1801A47C4 && a1 < 0 && (dword_1801A47C4 == a1 || dword_1801A47C4 == -1) )
    __debugbreak();
  return result;
}
 __int64 __fastcall sub_1800943F0(__int64 a1, unsigned __int64 a2, _QWORD *a3)
{
  __int64 result; // rax

  if ( a1 && a2 <= 0x7FFFFFFF )
    result = sub_180094468(a1, a2);
  else
    result = 2147942487i64;
  if ( (int)result < 0 )
  {
    if ( a3 )
      *a3 = 0i64;
  }
  return result;
}
 __int64 __fastcall sub_180094468(_WORD *a1, __int64 a2, _QWORD *a3)
{
  __int64 i; // r9
  __int64 result; // rax

  for ( i = a2; a2; --a2 )
  {
    if ( !*a1 )
      break;
    ++a1;
  }
  result = a2 == 0 ? 0x80070057 : 0;
  if ( a3 )
  {
    if ( a2 )
      *a3 = i - a2;
    else
      *a3 = 0i64;
  }
  return result;
}
 _BOOL8 __fastcall sub_18013CD18(__int16 a1)
{
  return ((a1 - 78) & 0xFFDF) == 0;
}
__int64 __fastcall sub_18013CFC4(unsigned __int16 a1)
{
  int v2; // edx
  int v3; // edx

  if ( a1 <= 0x56u )
  {
    if ( a1 != 86 )
    {
      if ( a1 <= 0x44u )
      {
        if ( a1 != 68 )
        {
          if ( a1 <= 0x37u )
          {
            switch ( a1 )
            {
              case '7':
                return 21i64;
              case '-':
                return 24i64;
              case '2':
                return 17i64;
              case '3':
                return 18i64;
              case '4':
                return 19i64;
              case '6':
                return 20i64;
            }
            return 25i64;
          }
          if ( a1 == 56 )
            return 22i64;
          v2 = a1 - 57;
          if ( a1 == 57 )
            return 23i64;
LABEL_20:
          v3 = v2 - 9;
          if ( !v3 )
            return 0i64;
          if ( v3 == 1 )
            return 1i64;
          return 25i64;
        }
        return 2i64;
      }
      if ( a1 <= 0x4Du )
      {
        if ( a1 != 77 )
        {
          if ( a1 != 70 )
          {
            if ( a1 != 71 )
            {
              if ( a1 != 72 )
              {
                if ( a1 != 74 )
                {
                  if ( a1 == 75 )
                    return 7i64;
                  return 25i64;
                }
                return 6i64;
              }
              return 5i64;
            }
            return 4i64;
          }
          return 3i64;
        }
        return 8i64;
      }
      if ( a1 != 80 )
      {
        if ( a1 != 81 )
        {
          if ( a1 != 82 )
          {
            if ( a1 != 84 )
              return 25i64;
            return 12i64;
          }
          return 11i64;
        }
        return 10i64;
      }
      return 9i64;
    }
    return 13i64;
  }
  if ( a1 <= 0x6Bu )
  {
    if ( a1 == 107 )
      return 7i64;
    if ( a1 > 0x64u )
    {
      if ( a1 != 102 )
      {
        if ( a1 != 103 )
        {
          if ( a1 != 104 )
          {
            if ( a1 != 106 )
              return 25i64;
            return 6i64;
          }
          return 5i64;
        }
        return 4i64;
      }
      return 3i64;
    }
    if ( a1 == 100 )
      return 2i64;
    if ( a1 != 87 )
    {
      if ( a1 != 88 )
      {
        v2 = a1 - 89;
        if ( a1 != 89 )
          goto LABEL_20;
        return 16i64;
      }
      return 15i64;
    }
    return 14i64;
  }
  if ( a1 != 109 )
  {
    switch ( a1 )
    {
      case 'p':
        return 9i64;
      case 'q':
        return 10i64;
      case 'r':
        return 11i64;
      case 't':
        return 12i64;
      case 'v':
        return 13i64;
    }
    if ( a1 != 119 )
    {
      if ( a1 != 120 )
      {
        if ( a1 != 121 )
          return 25i64;
        return 16i64;
      }
      return 15i64;
    }
    return 14i64;
  }
  return 8i64;
}
char __fastcall sub_18013D1B4(_BYTE *a1, __int64 a2)
{
  char result; // al

  result = (*(_BYTE *)(a2 + 14) ^ (8 * *a1)) & 8;
  *(_BYTE *)(a2 + 14) ^= result;
  return result;
}
__int64 __fastcall sub_1800943F0(_WORD *a1, unsigned __int64 a2, _QWORD *a3)
{
  __int64 result; // rax

  if ( a1 && a2 <= 0x7FFFFFFF )
    result = sub_180094468(a1, a2, a3);
  else
    result = 2147942487i64;
  if ( (int)result < 0 )
  {
    if ( a3 )
      *a3 = 0i64;
  }
  return result;
}
__int64 __fastcall sub_18013CC20(unsigned __int16 *a1, __int64 a2, _BYTE *a3, __int64 a4, int *a5)
{
  unsigned int v5; // ebx
  int v7; // r14d
  unsigned int v8; // ebp
  unsigned int v9; // esi
  unsigned int v10; // edi
  unsigned int v12; // eax
  int v13; // eax

  v5 = 0;
  v7 = 0;
  v8 = 0;
  v9 = 0;
  v10 = 0;
  while ( v10 < 0x19 )
  {
    if ( (unsigned int)sub_18013CD18(*a1) )
    {
      if ( v7 || v10 >= 0x18 )
        break;
      v7 = 1;
      memmove(a3 + 1, a3, v10);
      *a3 = v10;
    }
    else
    {
      v12 = sub_18013CFC4(*a1);
      if ( v12 >= 0x18 )
      {
        if ( v12 != 24 )
          break;
        if ( v9 > 0x17 )
          break;
        v13 = 8521760;
        if ( !_bittest(&v13, v9) || v8 >= 4 )
          break;
        ++v8;
        goto LABEL_15;
      }
      a3[v10] = v12;
    }
    ++v10;
LABEL_15:
    ++v9;
    ++a1;
    if ( v9 >= 0x1D )
    {
      if ( v8 == 4 && v10 == 25 )
      {
        *a5 = v7;
        goto LABEL_20;
      }
      break;
    }
  }
  v5 = -2147217327;
  sub_18008A114(-2147217327);
LABEL_20:
  sub_18008D904(v5);
  return v5;
}

// Sppobjs.dll, sub_18013C8DC
// take the 25-character product key string and compress it into a raw 114-bit integer
__int64 __fastcall sub_18013C8DC(__int64 a1, _OWORD *a2, __int64 a3, _DWORD *a4)
{
  __int64 v6; // rdx
  __int64 v7; // r11
  int v8; // ecx
  unsigned int v9; // ebx
  int v10; // eax
  __int128 *v11; // r9
  char *v12; // r10
  __int64 v13; // rdx
  unsigned int i; // r11d
  __int64 v15; // r8
  __int64 v16; // rdi
  unsigned int v17; // r8d
  int v18; // eax
  __int64 v20; // [rsp+30h] [rbp-40h] BYREF
  __int128 v21; // [rsp+38h] [rbp-38h] BYREF
  char v22[32]; // [rsp+48h] [rbp-28h] BYREF

  if ( (int)sub_1800943F0(a1, 30i64, &v20) < 0 || v20 != 29 )
  {
LABEL_2:
    v8 = -2147217327;
    v9 = -2147217327;
LABEL_3:
    sub_18008A114(v8);
    goto LABEL_19;
  }
  v10 = sub_18013CC20(v7, v6, v22);
  v9 = v10;
  if ( v10 < 0 )
  {
    v8 = v10;
    goto LABEL_3;
  }
  v12 = v22;
  v13 = 0i64;
  v21 = 0i64;
  for ( i = 0; i < 0x19; ++i )
  {
    v15 = (unsigned __int8)*v12;
    if ( (_DWORD)v13 )
    {
      v11 = &v21;
      v16 = (unsigned int)v13;
      do
      {
        v17 = v15 + 24 * *(unsigned __int8 *)v11;
        *(_BYTE *)v11 = v17;
        v11 = (__int128 *)((char *)v11 + 1);
        v15 = v17 >> 8;
        --v16;
      }
      while ( v16 );
    }
    if ( (_DWORD)v15 )
    {
      if ( (unsigned int)v13 >= 0x10 )
        goto LABEL_2;
      v22[v13 - 16] = v15;
      v13 = (unsigned int)(v13 + 1);
    }
    ++v12;
  }
  v18 = v20;
  if ( (_DWORD)v20 )
  {
    LODWORD(v20) = 1;
    sub_18013D1B4(&v20, &v21, v15, v11);
    v18 = v20;
  }
  *a2 = v21;
  if ( a4 )
    *a4 = v18;
LABEL_19:
  sub_18008D904(v9);
  return v9;
}
__int64 __fastcall sub_18013CC20(unsigned __int16 *a1, __int64 a2, _BYTE *a3, __int64 a4, int *a5)
{
  unsigned int v5; // ebx
  int v7; // r14d
  unsigned int v8; // ebp
  unsigned int v9; // esi
  unsigned int v10; // edi
  unsigned int v12; // eax
  int v13; // eax

  v5 = 0;
  v7 = 0;
  v8 = 0;
  v9 = 0;
  v10 = 0;
  while ( v10 < 0x19 )
  {
    if ( (unsigned int)sub_18013CD18(*a1) )
    {
      if ( v7 || v10 >= 0x18 )
        break;
      v7 = 1;
      memmove(a3 + 1, a3, v10);
      *a3 = v10;
    }
    else
    {
      v12 = sub_18013CFC4(*a1);
      if ( v12 >= 0x18 )
      {
        if ( v12 != 24 )
          break;
        if ( v9 > 0x17 )
          break;
        v13 = 8521760;
        if ( !_bittest(&v13, v9) || v8 >= 4 )
          break;
        ++v8;
        goto LABEL_15;
      }
      a3[v10] = v12;
    }
    ++v10;
LABEL_15:
    ++v9;
    ++a1;
    if ( v9 >= 0x1D )
    {
      if ( v8 == 4 && v10 == 25 )
      {
        *a5 = v7;
        goto LABEL_20;
      }
      break;
    }
  }
  v5 = -2147217327;
  sub_18008A114(-2147217327);
LABEL_20:
  sub_18008D904(v5);
  return v5;
}
char __fastcall sub_18013D1B4(_BYTE *a1, __int64 a2)
{
  char result; // al

  result = (*(_BYTE *)(a2 + 14) ^ (8 * *a1)) & 8;
  *(_BYTE *)(a2 + 14) ^= result;
  return result;
}
_BOOL8 __fastcall sub_18013CD18(__int16 a1)
{
  return ((a1 - 78) & 0xFFDF) == 0;
}
__int64 __fastcall sub_18008A114(int a1)
{
  __int64 result; // rax

  result = (unsigned int)dword_1801A47C4;
  if ( dword_1801A47C4 && a1 < 0 && (dword_1801A47C4 == a1 || dword_1801A47C4 == -1) )
    __debugbreak();
  return result;
}
__int64 __fastcall sub_1800943F0(__int64 a1, unsigned __int64 a2, _QWORD *a3)
{
  __int64 result; // rax

  if ( a1 && a2 <= 0x7FFFFFFF )
    result = sub_180094468(a1, a2);
  else
    result = 2147942487i64;
  if ( (int)result < 0 )
  {
    if ( a3 )
      *a3 = 0i64;
  }
  return result;
}
__int64 __fastcall sub_180094468(_WORD *a1, __int64 a2, _QWORD *a3)
{
  __int64 i; // r9
  __int64 result; // rax

  for ( i = a2; a2; --a2 )
  {
    if ( !*a1 )
      break;
    ++a1;
  }
  result = a2 == 0 ? 0x80070057 : 0;
  if ( a3 )
  {
    if ( a2 )
      *a3 = i - a2;
    else
      *a3 = 0i64;
  }
  return result;
}
__int64 __fastcall sub_18013CFC4(unsigned __int16 a1)
{
  int v2; // edx
  int v3; // edx

  if ( a1 <= 0x56u )
  {
    if ( a1 != 86 )
    {
      if ( a1 <= 0x44u )
      {
        if ( a1 != 68 )
        {
          if ( a1 <= 0x37u )
          {
            switch ( a1 )
            {
              case '7':
                return 21i64;
              case '-':
                return 24i64;
              case '2':
                return 17i64;
              case '3':
                return 18i64;
              case '4':
                return 19i64;
              case '6':
                return 20i64;
            }
            return 25i64;
          }
          if ( a1 == 56 )
            return 22i64;
          v2 = a1 - 57;
          if ( a1 == 57 )
            return 23i64;
LABEL_20:
          v3 = v2 - 9;
          if ( !v3 )
            return 0i64;
          if ( v3 == 1 )
            return 1i64;
          return 25i64;
        }
        return 2i64;
      }
      if ( a1 <= 0x4Du )
      {
        if ( a1 != 77 )
        {
          if ( a1 != 70 )
          {
            if ( a1 != 71 )
            {
              if ( a1 != 72 )
              {
                if ( a1 != 74 )
                {
                  if ( a1 == 75 )
                    return 7i64;
                  return 25i64;
                }
                return 6i64;
              }
              return 5i64;
            }
            return 4i64;
          }
          return 3i64;
        }
        return 8i64;
      }
      if ( a1 != 80 )
      {
        if ( a1 != 81 )
        {
          if ( a1 != 82 )
          {
            if ( a1 != 84 )
              return 25i64;
            return 12i64;
          }
          return 11i64;
        }
        return 10i64;
      }
      return 9i64;
    }
    return 13i64;
  }
  if ( a1 <= 0x6Bu )
  {
    if ( a1 == 107 )
      return 7i64;
    if ( a1 > 0x64u )
    {
      if ( a1 != 102 )
      {
        if ( a1 != 103 )
        {
          if ( a1 != 104 )
          {
            if ( a1 != 106 )
              return 25i64;
            return 6i64;
          }
          return 5i64;
        }
        return 4i64;
      }
      return 3i64;
    }
    if ( a1 == 100 )
      return 2i64;
    if ( a1 != 87 )
    {
      if ( a1 != 88 )
      {
        v2 = a1 - 89;
        if ( a1 != 89 )
          goto LABEL_20;
        return 16i64;
      }
      return 15i64;
    }
    return 14i64;
  }
  if ( a1 != 109 )
  {
    switch ( a1 )
    {
      case 'p':
        return 9i64;
      case 'q':
        return 10i64;
      case 'r':
        return 11i64;
      case 't':
        return 12i64;
      case 'v':
        return 13i64;
    }
    if ( a1 != 119 )
    {
      if ( a1 != 120 )
      {
        if ( a1 != 121 )
          return 25i64;
        return 16i64;
      }
      return 15i64;
    }
    return 14i64;
  }
  return 8i64;
}

// Pidgenx.dll, Retail Version, sub_180020C5C
// Takes the String (input) and packs it into Binary (storage).

__int64 __fastcall sub_180020C5C(_WORD *a1, _OWORD *a2, unsigned int a3, int *a4)
{
  unsigned int v7; // ebx
  int v8; // ecx
  _WORD *v9; // rax
  __int64 v10; // r8
  __int64 v11; // rdx
  __int64 v12; // rcx
  int v13; // eax
  char *v14; // r9
  unsigned int v15; // edx
  unsigned int i; // r10d
  unsigned int v17; // r8d
  __int128 *v18; // r11
  __int64 v19; // rdi
  unsigned int v20; // r8d
  __int64 v21; // rax
  int v22; // eax
  __int64 v24; // [rsp+30h] [rbp-68h]
  __int128 v25; // [rsp+38h] [rbp-60h] BYREF
  char v26[32]; // [rsp+48h] [rbp-50h] BYREF

  if ( a3 < 0x10 )
  {
    v7 = -2147024809;
    v8 = -2147024809;
LABEL_3:
    sub_180002E08(v8);
    goto LABEL_29;
  }
  if ( a1 )
  {
    v9 = a1;
    v10 = 30i64;
    do
    {
      if ( !*v9 )
        break;
      ++v9;
      --v10;
    }
    while ( v10 );
    v11 = v10 == 0 ? 0x80070057 : 0;
    if ( v10 )
      v12 = 30 - v10;
    else
      v12 = 0i64;
  }
  else
  {
    v12 = v24;
    v11 = 2147942487i64;
  }
  if ( (int)v11 < 0 || v12 != 29 )
  {
LABEL_13:
    v8 = -2147217327;
    v7 = -2147217327;
    goto LABEL_3;
  }
  v13 = sub_180020DE8(a1, v11, v26);
  v7 = v13;
  if ( v13 < 0 )
  {
    v8 = v13;
    goto LABEL_3;
  }
  v14 = v26;
  v25 = 0i64;
  v15 = 0;
  for ( i = 0; i < 0x19; ++i )
  {
    v17 = (unsigned __int8)*v14;
    if ( v15 )
    {
      v18 = &v25;
      v19 = v15;
      do
      {
        v20 = v17 + 24 * *(unsigned __int8 *)v18;
        *(_BYTE *)v18 = v20;
        v18 = (__int128 *)((char *)v18 + 1);
        v17 = v20 >> 8;
        --v19;
      }
      while ( v19 );
    }
    if ( v17 )
    {
      if ( v15 >= 0x10 )
        goto LABEL_13;
      v21 = v15++;
      v26[v21 - 16] = v17;
    }
    ++v14;
  }
  v22 = v24;
  if ( (_DWORD)v24 )
  {
    BYTE14(v25) |= 8u;
    v22 = 1;
  }
  *a2 = v25;
  if ( a4 )
    *a4 = v22;
LABEL_29:
  sub_180002E28(v7);
  return v7;
}
__int64 __fastcall sub_180020DE8(_WORD *a1, __int64 a2, _BYTE *a3, __int64 a4, int *a5)
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
LABEL_80:
          LOBYTE(v17) = 8;
          goto LABEL_81;
        }
        v34 = v33 - 3;
        if ( !v34 )
        {
LABEL_79:
          LOBYTE(v17) = 9;
          goto LABEL_81;
        }
        v35 = v34 - 1;
        if ( !v35 )
        {
LABEL_78:
          LOBYTE(v17) = 10;
          goto LABEL_81;
        }
        v36 = v35 - 1;
        if ( !v36 )
        {
LABEL_77:
          LOBYTE(v17) = 11;
          goto LABEL_81;
        }
        v37 = v36 - 2;
        if ( v37 )
        {
          v38 = v37 - 2;
          if ( !v38 )
          {
LABEL_76:
            LOBYTE(v17) = 13;
            goto LABEL_81;
          }
          v39 = v38 - 1;
          if ( !v39 )
          {
LABEL_75:
            LOBYTE(v17) = 14;
            goto LABEL_81;
          }
          v40 = v39 - 1;
          if ( !v40 )
          {
LABEL_74:
            LOBYTE(v17) = 15;
            goto LABEL_81;
          }
          if ( v40 != 1 )
            break;
          goto LABEL_73;
        }
        goto LABEL_43;
      }
      if ( v12 != 107 )
      {
        if ( v12 > 0x64 )
        {
          v30 = v12 - 102;
          if ( !v30 )
            goto LABEL_63;
          v31 = v30 - 1;
          if ( !v31 )
            goto LABEL_62;
          v32 = v31 - 1;
          if ( !v32 )
            goto LABEL_61;
          if ( v32 != 2 )
            break;
LABEL_60:
          LOBYTE(v17) = 6;
          goto LABEL_81;
        }
        if ( v12 != 100 )
        {
          v28 = v12 - 87;
          if ( !v28 )
            goto LABEL_75;
          v29 = v28 - 1;
          if ( !v29 )
            goto LABEL_74;
          v20 = v29 - 1;
          if ( v20 )
          {
LABEL_51:
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
            goto LABEL_81;
          }
LABEL_73:
          LOBYTE(v17) = 16;
          goto LABEL_81;
        }
LABEL_55:
        LOBYTE(v17) = 2;
        goto LABEL_81;
      }
      goto LABEL_38;
    }
    if ( v12 == 86 )
      goto LABEL_76;
    if ( v12 > 0x44 )
    {
      if ( v12 > 0x4D )
      {
        v25 = v12 - 80;
        if ( !v25 )
          goto LABEL_79;
        v26 = v25 - 1;
        if ( !v26 )
          goto LABEL_78;
        v27 = v26 - 1;
        if ( !v27 )
          goto LABEL_77;
        if ( v27 != 2 )
          break;
LABEL_43:
        LOBYTE(v17) = 12;
        goto LABEL_81;
      }
      if ( v12 == 77 )
        goto LABEL_80;
      v21 = v12 - 70;
      if ( !v21 )
      {
LABEL_63:
        LOBYTE(v17) = 3;
        goto LABEL_81;
      }
      v22 = v21 - 1;
      if ( !v22 )
      {
LABEL_62:
        LOBYTE(v17) = 4;
        goto LABEL_81;
      }
      v23 = v22 - 1;
      if ( !v23 )
      {
LABEL_61:
        LOBYTE(v17) = 5;
        goto LABEL_81;
      }
      v24 = v23 - 2;
      if ( !v24 )
        goto LABEL_60;
      if ( v24 != 1 )
        break;
LABEL_38:
      LOBYTE(v17) = 7;
      goto LABEL_81;
    }
    if ( v12 == 68 )
      goto LABEL_55;
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
      goto LABEL_81;
    }
    v19 = v12 - 56;
    if ( v19 )
    {
      v20 = v19 - 1;
      if ( v20 )
        goto LABEL_51;
      LOBYTE(v17) = 23;
    }
    else
    {
      LOBYTE(v17) = 22;
    }
LABEL_81:
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
  sub_180002E08(-2147217327);
LABEL_88:
  sub_180002E28(v5);
  return v5;
}

// Pidgenx.dll, Insider Version, sub_18000890C
// take the 25-character product key string and compress it into a raw 114-bit integer
__int64 __fastcall sub_18000890C(_WORD *a1, _OWORD *a2, __int64 a3, _DWORD *a4)
{
  _WORD *v7; // rax
  __int64 v8; // rcx
  int v9; // eax
  unsigned int v10; // ebx
  int v11; // ecx
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
    v11 = -2147217327;
    v10 = -2147217327;
    goto LABEL_20;
  }
  v9 = sub_180008DB4(a1, 29i64, v22);
  v10 = v9;
  if ( v9 < 0 )
  {
    v11 = v9;
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
  sub_1800038E8(2147749969i64);
LABEL_88:
  sub_180003B30(v5, a2, a3, a4);
  return v5;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~ Decode Binary Key ~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * Origin: Sppwinob.dll <Windws 8 Beta 7850>
 * __int64 __fastcall ProductKeyHelpers_BinaryDecode(__int128 *a1, unsigned __int16 **a2)
 * new Version, sub_18004046C [966 from 1023, near the end]
 * int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__int128 *a1, unsigned __int16 **a2)
 * Takes the String (input) and packs it into Binary (storage).
 
 SppComApi.dll,
 __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__m128i *a1, __int64 a2, unsigned __int16 **a3)
 
 LicensingWinRT.dll
 __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__m128i *a1, __int64 a2, unsigned __int16 **a3)
 
 LicensingDiagSpp.dll
 __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__m128i *a1, __int64 a2, unsigned __int16 **a3)
 */

__int64 __fastcall ProductKeyHelpers_BinaryDecode(__int128 *a1, unsigned __int16 **a2)
{
  __int64 v2; // rcx
  unsigned int v3; // ebx
  int v4; // eax

  if ( a1 && a2 )
  {
    v4 = CProductKeyUtilsT<CEmptyType>::BinaryDecode(a1, a2);
    v3 = v4;
    if ( v4 >= 0 )
      return v3;
    v2 = (unsigned int)v4;
  }
  else
  {
    v2 = 2147942487i64;
    v3 = -2147024809;
  }
  CBreakOnFailureT<CEmptyType>::CheckToBreakOnFailure(v2);
  if ( WPP_GLOBAL_Control != &WPP_GLOBAL_Control && (*((_BYTE *)WPP_GLOBAL_Control + 68) & 2) != 0 )
    WARBIRD::g_IndirectBranchTargets8(
      *((_QWORD *)WPP_GLOBAL_Control + 7),
      10i64,
      WPP_40bb4e7f9098b02c05e351ec6325e929_Traceguids,
      v3);
  return v3;
}
__int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__int128 *a1, unsigned __int16 **a2)
{
  unsigned __int16 *v2; // rbx
  int v5; // edi
  __int64 v6; // r9
  BOOL v7; // r10d
  __int64 v8; // r8
  __int64 i; // rcx
  unsigned int v10; // r8d
  __int64 v11; // rbx
  int v12; // eax
  unsigned __int16 *v13; // r8
  int v14; // r9d
  __int64 v15; // r10
  int v16; // edx
  __int16 v17; // ax
  __int64 v18; // rcx
  HANDLE ProcessHeap; // rax
  unsigned __int16 *v21; // [rsp+28h] [rbp-51h] BYREF
  __int128 v22; // [rsp+30h] [rbp-49h]
  __int16 v23; // [rsp+40h] [rbp-39h] BYREF
  __int16 Src[27]; // [rsp+42h] [rbp-37h] BYREF
  __int16 v25[28]; // [rsp+78h] [rbp-1h] BYREF

  v2 = 0i64;
  v21 = 0i64;
  memcpy(v25, L"BCDFGHJKMPQRTVWXY2346789", 0x32ui64);
  v22 = *a1;
  if ( (BYTE14(v22) & 0xF0) != 0 )
    goto LABEL_2;
  v6 = 24i64;
  v7 = (BYTE14(v22) & 8) != 0;
  BYTE14(v22) = (4 * (((BYTE14(v22) & 8) != 0) & 2)) | BYTE14(v22) & 0xF7;
  do
  {
    LODWORD(v8) = 0;
    for ( i = 14i64; i >= 0; --i )
    {
      v10 = *((unsigned __int8 *)&v22 + i) + ((_DWORD)v8 << 8);
      *((_BYTE *)&v22 + i) = v10 / 0x18;
      v8 = v10 % 0x18;
    }
    Src[--v6] = v25[v8];
  }
  while ( v6 >= 0 );
  if ( (_BYTE)v22 )
  {
LABEL_2:
    v5 = -2147024883;
    CBreakOnFailureT<CEmptyType>::CheckToBreakOnFailure(2147942413i64);
LABEL_16:
    if ( WPP_GLOBAL_Control != &WPP_GLOBAL_Control && (*((_BYTE *)WPP_GLOBAL_Control + 68) & 2) != 0 )
      WARBIRD::g_IndirectBranchTargets8(
        *((_QWORD *)WPP_GLOBAL_Control + 7),
        11i64,
        WPP_33b221283fe149b69b56c7e3db1fdc6a_Traceguids,
        (unsigned int)v5);
    goto LABEL_19;
  }
  if ( v7 )
  {
    v11 = v8;
    memmove(&v23, Src, 2 * v8);
    Src[v11 - 1] = 78;
  }
  v12 = STRAPI_CreateCchBufferN(0x2Du, 0x1Eui64, &v21);
  v5 = v12;
  if ( v12 >= 0 )
  {
    v13 = v21;
    v14 = 0;
    v15 = 0i64;
    do
    {
      v16 = (unsigned __int64)(1717986919i64 * v14++) >> 32;
      v17 = Src[v15 - 1];
      v18 = v15 + (int)(((unsigned int)v16 >> 31) + (v16 >> 1));
      ++v15;
      v13[v18] = v17;
    }
    while ( v14 < 25 );
    v2 = 0i64;
    *a2 = v13;
  }
  else
  {
    CBreakOnFailureT<CEmptyType>::CheckToBreakOnFailure((unsigned int)v12);
    v2 = v21;
  }
  if ( v5 < 0 )
    goto LABEL_16;
LABEL_19:
  if ( v2 )
  {
    ProcessHeap = GetProcessHeap();
    HeapFree(ProcessHeap, 0, v2 - 2);
  }
  return (unsigned int)v5;
}

// sppwinob.dll, <Windows 10, Retail>
// __int64 __fastcall sub_18004046C(__m128i *a1, __int64 a2, __int64 *a3)
__int64 __fastcall sub_18004046C(__m128i *a1, __int64 a2, __int64 *a3)
{
  int v4; // ecx
  unsigned int v5; // ebx
  __int64 i; // r9
  __int64 v7; // r8
  __int64 j; // rcx
  unsigned int v9; // r8d
  __int64 v10; // rbx
  int v11; // eax
  __int64 v12; // r9
  __int16 *v13; // r10
  int k; // r8d
  __int16 v15; // ax
  __int64 v16; // rcx
  BOOL v18; // [rsp+20h] [rbp-49h] BYREF
  __int64 v19; // [rsp+28h] [rbp-41h] BYREF
  __m128i v20; // [rsp+30h] [rbp-39h] BYREF
  __int128 v21[3]; // [rsp+40h] [rbp-29h]
  __int16 v22; // [rsp+70h] [rbp+7h]
  __int16 v23; // [rsp+78h] [rbp+Fh] BYREF
  char Src[54]; // [rsp+7Ah] [rbp+11h] BYREF

  v19 = 0i64;
  v21[0] = xmmword_18004AAA8;
  v22 = 0;
  v21[1] = xmmword_18004AAB8;
  v21[2] = xmmword_18004AAC8;
  v20 = *a1;
  if ( (_mm_srli_si128(v20, 8).m128i_u64[0] & 0xF0000000000000i64) != 0 )
    goto LABEL_2;
  v18 = 0;
  sub_180040660((__int64)&v20, &v18);
  v18 = v18;
  sub_180040650((char *)&v18, (__int64)&v20);
  for ( i = 24i64; i >= 0; *(_WORD *)&Src[2 * i-- - 2] = *((_WORD *)v21 + v7) )
  {
    LODWORD(v7) = 0;
    for ( j = 14i64; j >= 0; --j )
    {
      v9 = v20.m128i_u8[j] + ((_DWORD)v7 << 8);
      v20.m128i_i8[j] = v9 / 0x18;
      v7 = v9 % 0x18;
    }
  }
  if ( v20.m128i_i8[0] )
  {
LABEL_2:
    v4 = -2147024883;
    v5 = -2147024883;
LABEL_3:
    sub_1800097C8(v4);
    goto LABEL_16;
  }
  if ( v18 )
  {
    v10 = 2 * v7;
    memmove(&v23, Src, 2 * v7);
    *(_WORD *)&Src[v10 - 2] = 78;
  }
  v11 = sub_18000FCC0(45i64, 30i64, &v19);
  v5 = v11;
  if ( v11 < 0 )
  {
    v4 = v11;
    goto LABEL_3;
  }
  v12 = v19;
  v13 = &v23;
  for ( k = 0; k < 25; ++k )
  {
    v15 = *v13++;
    v16 = k + k / 5;
    *(_WORD *)(v12 + 2 * v16) = v15;
  }
  v19 = 0i64;
  *a3 = v12;
LABEL_16:
  sub_18000A514(v5);
  sub_18000BF30(&v19);
  return v5;
}
bool __fastcall sub_180040660(__int64 a1, _BYTE *a2)
{
  char v2; // al
  bool result; // al

  v2 = *(_BYTE *)(a1 + 14);
  *a2 &= ~1u;
  result = (v2 & 8) != 0;
  *a2 |= result;
  return result;
}
char __fastcall sub_180040650(char *a1, __int64 a2)
{
  char v2; // al
  char result; // al
  _BYTE *v4; // rdx

  v4 = (_BYTE *)(a2 + 14);
  v2 = *a1;
  *v4 &= ~8u;
  result = 8 * ((v2 & 2) != 0);
  *v4 |= result;
  return result;
}
__int64 __fastcall sub_18000FCC0(unsigned __int16 a1, __int64 a2, __int64 a3)
{
  int v5; // eax
  unsigned int v6; // ebx
  unsigned int v8; // [rsp+48h] [rbp+20h] BYREF

  v5 = sub_18000B474(a2, &v8);
  v6 = v5;
  if ( v5 < 0 || (v5 = sub_18000D95C(a1, v8, a3), v6 = v5, v5 < 0) )
    sub_1800097C8(v5);
  sub_18000A514(v6);
  return v6;
}
__int64 __fastcall sub_18000B474(__int64 a1, _DWORD *a2)
{
  int v3; // edi
  unsigned int v4; // ebx
  int v5; // ecx
  int v6; // eax

  v3 = a1;
  if ( a1 == (unsigned int)a1 )
  {
    v6 = sub_1800091D8();
    v4 = v6;
    if ( v6 >= 0 )
    {
      *a2 = v3;
      goto LABEL_7;
    }
    v5 = v6;
  }
  else
  {
    v4 = -2147024362;
    v5 = -2147024362;
  }
  sub_1800097C8(v5);
LABEL_7:
  sub_18000A514(v4);
  return v4;
}
__int64 __fastcall sub_18000D95C(unsigned __int16 a1, unsigned int a2, unsigned int **a3)
{
  unsigned int v6; // edi
  int v7; // eax
  int v8; // ecx
  unsigned int *v9; // rbx
  unsigned int *v10; // r10
  __int64 v11; // rax
  unsigned int v13; // [rsp+58h] [rbp+10h] BYREF
  unsigned int *v14; // [rsp+68h] [rbp+20h] BYREF

  v14 = 0i64;
  v6 = 0;
  if ( !a2 )
  {
    *a3 = 0i64;
    goto LABEL_13;
  }
  v7 = sub_18000AD00(a2, 2i64, &v13);
  v6 = v7;
  if ( v7 < 0 )
    goto LABEL_3;
  v7 = sub_18000AC64(v13, 4i64, &v13);
  v6 = v7;
  if ( v7 < 0 )
    goto LABEL_3;
  sub_180008FAC(v13);
  sub_18000C13C((__int64)&v14);
  v9 = v14;
  if ( !v14 )
  {
    v6 = -2147024882;
    v8 = -2147024882;
    goto LABEL_4;
  }
  v7 = sub_18000FE8C(a2, 1i64, v14);
  v6 = v7;
  if ( v7 < 0 )
  {
LABEL_3:
    v8 = v7;
LABEL_4:
    sub_1800097C8(v8);
    goto LABEL_13;
  }
  v10 = v9 + 1;
  if ( a1 )
    sub_180011658(v9 + 1, a1, *v9);
  v11 = *v9;
  v6 = 0;
  v14 = 0i64;
  *((_WORD *)v10 + v11) = 0;
  *a3 = v10;
LABEL_13:
  sub_18000A514(v6);
  sub_18000BEC8((__int64)&v14);
  return v6;
}
_WORD *__fastcall sub_180011658(_WORD *a1, __int16 a2, __int64 a3)
{
  _WORD *v4; // rdi
  __int64 i; // rcx

  if ( a3 )
  {
    v4 = a1;
    for ( i = a3; i; --i )
      *v4++ = a2;
  }
  return a1;
}
__int64 __fastcall sub_180008FAC(__int64 a1)
{
  return sub_180009098(a1);
}
__int64 __fastcall sub_18000FE8C(unsigned int a1, int a2, _DWORD *a3)
{
  unsigned int v3; // ebx

  if ( a1 - a2 > a1 )
  {
    v3 = -2147024362;
    sub_1800097C8(-2147024362);
  }
  else
  {
    *a3 = a1 - a2;
    v3 = 0;
  }
  sub_18000A514(v3);
  return v3;
}
__int64 __fastcall sub_18000C13C(__int64 a1)
{
  sub_18000C07C(a1);
  return a1;
}
__int64 __fastcall sub_18000AC64(unsigned int a1, int a2, _DWORD *a3)
{
  unsigned int v3; // ebx

  if ( a1 + a2 < a1 )
  {
    v3 = -2147024362;
    sub_1800097C8(-2147024362);
  }
  else
  {
    *a3 = a1 + a2;
    v3 = 0;
  }
  sub_18000A514(v3);
  return v3;
}
__int64 __fastcall sub_18000AD00(int a1, unsigned int a2, _DWORD *a3)
{
  unsigned int v3; // ebx

  v3 = 0;
  if ( a1 && a2 )
  {
    if ( a2 * a1 / a2 == a1 )
    {
      *a3 = a2 * a1;
    }
    else
    {
      v3 = -2147024362;
      sub_1800097C8(-2147024362);
    }
  }
  else
  {
    *a3 = 0;
  }
  sub_18000A514(v3);
  return v3;
}