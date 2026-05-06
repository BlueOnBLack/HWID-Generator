// Ps1 Call
$ret = $Global:PIDGENX::GetPKeyData(
  $key, $configPath, $Mpc, [IntPtr]::Zero, $HWID,
  [ref]$IID, [ref]$Edition, [ref]$Channel, [ref]$Partnum,
  [IntPtr]::Zero
)

>>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> 

 // GetPKeyData
 v23 = *(__int64 (__fastcall **)(__int64, _QWORD, wchar_t *, _QWORD, _DWORD, __int64 *))(*(_QWORD *)v50 + 64i64);
 _guard_check_icall_fptr(v23);
 v18 = v23(v22, 0i64, Str, 0i64, 0, &v48);
 v17 = v18;
 if ( v18 < 0 )
  goto LABEL_33;
 if ( *(_DWORD *)(*(_QWORD *)(v48 + 120) + 48i64) )
 {
  v24 = *(_QWORD *)(v48 + 128);
  v25 = *(_DWORD *)(v24 + 48);
  v26 = (unsigned int)(*(_QWORD *)(v48 + 144) / 0x3E8ui64) % (*(_DWORD *)(v24 + 52) - v25 + 1);
  *(_DWORD *)(v48 + 24) = v25 + v26;
  *(_DWORD *)(v48 + 64) = (v25 + v26) % 0xF4240;
  *(_DWORD *)(v48 + 60) = *(_DWORD *)(v48 + 24) / 0xF4240u;
 }
 ProcessHeap = GetProcessHeap();
 v12 = HeapAlloc(ProcessHeap, 0, 0x58ui64);
 if ( !v12 )
  goto LABEL_2;
 v28 = v48;
 *v12 = *(_OWORD *)(v48 + 32);
 v12[1] = *(_OWORD *)(v28 + 48);
 v12[2] = *(_OWORD *)(v28 + 64);
 v12[3] = *(_OWORD *)(v28 + 80);
 v12[4] = *(_OWORD *)(v28 + 96);
 *((_QWORD *)v12 + 10) = *(_QWORD *)(v28 + 112);
 v18 = sub_180001E80(*(_QWORD *)(*(_QWORD *)(v48 + 120) + 24i64), &v53);
 v17 = v18;
 if ( v18 < 0 )
  goto LABEL_33;
 v29 = sub_180001E80(*(_QWORD *)(*(_QWORD *)(v48 + 120) + 40i64), &lpMem);
 v17 = v29;
 if ( v29 < 0 )
 {
  sub_180002E08((unsigned int)v29);
  v14 = lpMem;
  goto LABEL_35;
 }
 v30 = sub_180001E80(*(_QWORD *)(*(_QWORD *)(v48 + 128) + 24i64), &v52);
 v17 = v30;
 if ( v30 < 0 )
 {
  sub_180002E08((unsigned int)v30);
LABEL_28:
  v14 = lpMem;
  v15 = v52;
  goto LABEL_35;
 }
 v33 = sub_18001D8B8((int)v12 + 8, v31, a5, v32, (__int64)&v56);

>>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> 

// v18 = v23(v22, 0i64, CD-KEY, 0i64, 0, &v48);
__int64 __fastcall sub_180021D80(int a1, int a2, int a3, int a4, int a5, __int64 a6)
{
  unsigned int v6; // ebx

  v6 = sub_180029F30(a1 + 16, a2, a3, a4, a5, a6);
  sub_1800064C4(&unk_1800EA414, &unk_180101790);
  return v6;
}
__int64 __fastcall sub_180029F30(__int64 a1, unsigned __int16 *a2, __int64 a3, __int64 a4, int a5, __int64 *a6)
{
  __int64 v6; // r15
  void *v7; // rbx
  unsigned int v8; // edi
  _DWORD *v9; // r14
  int v13; // ecx
  unsigned int v14; // edi
  unsigned __int16 *v15; // r8
  unsigned int v16; // r9d
  int v17; // ecx
  int v18; // edx
  int v19; // ecx
  int v20; // ecx
  int v21; // eax
  int v22; // eax
  HANDLE ProcessHeap; // rax
  void *v24; // rbx
  HANDLE v25; // rax
  LPVOID v27; // [rsp+40h] [rbp-20h] BYREF
  LPVOID lpMem; // [rsp+48h] [rbp-18h] BYREF
  unsigned __int16 *v29; // [rsp+50h] [rbp-10h]
  unsigned int v30; // [rsp+A0h] [rbp+40h] BYREF
  __int64 v31; // [rsp+B0h] [rbp+50h]
  int v32; // [rsp+B8h] [rbp+58h]

  v31 = a3;
  v27 = 0i64;
  v6 = a1 + 8;
  v7 = 0i64;
  v8 = 0;
  v29 = 0i64;
  v9 = 0i64;
  lpMem = 0i64;
  v30 = 0;
  if ( a1 != -8 )
    sub_18002BAE4(a1 + 8);
  if ( !a4 )
    goto LABEL_24;
  if ( *(_DWORD *)a4 < 0x18u )
    goto LABEL_5;
  v15 = *(unsigned __int16 **)(a4 + 8);
  if ( !v15 )
    goto LABEL_5;
  v16 = *(_DWORD *)(a4 + 16);
  if ( !v16 )
    goto LABEL_5;
  v17 = *(_DWORD *)(a4 + 4);
  v18 = (*(_DWORD *)(a4 + 20) >> 1) & 1;
  v32 = *(_DWORD *)(a4 + 20) & 1;
  if ( !v17 )
  {
    v21 = sub_18002AA3C(a1, (_DWORD)a2, (_DWORD)v15, v16, v18, (__int64)&v27, (__int64)&v30);
    goto LABEL_17;
  }
  v19 = v17 - 1;
  if ( !v19 )
  {
    v21 = sub_18002ABCC(a1, (_DWORD)a2, (_DWORD)v15, v16, v18, (__int64)&v27, (__int64)&v30);
    goto LABEL_17;
  }
  v20 = v19 - 1;
  if ( !v20 )
  {
    v21 = sub_18002AE30(a1, (__int64)a2, (__int64)v15, v16, v18, &v27, &v30);
LABEL_17:
    v14 = v21;
    if ( v21 < 0 )
    {
LABEL_25:
      v13 = v21;
      goto LABEL_26;
    }
    v9 = v27;
    v8 = v30;
    goto LABEL_19;
  }
  if ( v20 == 1 && v16 == 1 )
  {
    v29 = v15;
LABEL_19:
    if ( !v32 && *(_DWORD *)(a4 + 4) != 3 )
    {
      v22 = sub_18002A898(a1, a2, v9, v8, &lpMem, &v30);
      v14 = v22;
      if ( v22 < 0 )
      {
        sub_180002E08(v22);
        v7 = lpMem;
        goto LABEL_27;
      }
      v7 = lpMem;
      v8 = v30;
      v9 = lpMem;
    }
LABEL_24:
    v21 = Orchestrator(a1, a2, v31, v29, v9, v8, a5, a6);
    v14 = v21;
    if ( v21 >= 0 )
      goto LABEL_27;
    goto LABEL_25;
  }
LABEL_5:
  v13 = -2147217370;
  v14 = -2147217370;
LABEL_26:
  sub_180002E08(v13);
LABEL_27:
  sub_180002E28(v14);
  if ( v6 )
    sub_180029C38(v6);
  if ( v7 )
  {
    ProcessHeap = GetProcessHeap();
    HeapFree(ProcessHeap, 0, v7);
  }
  v24 = v27;
  if ( v27 )
  {
    v25 = GetProcessHeap();
    HeapFree(v25, 0, v24);
  }
  return v14;
}
__int64 __fastcall sub_18002C79C(
        __int64 a1,
        const wchar_t *a2,
        __int64 a3,
        unsigned __int16 *a4,
        _DWORD *a5,
        unsigned int a6,
        int a7,
        __int64 *a8)
{
  signed int v9; // r14d
  void (__fastcall *v13)(char *); // rbx
  __int64 *v14; // rbx
  __int64 (__fastcall *v15)(__int64, char *); // rdi
  signed int v16; // esi
  __int64 *v17; // rdi
  int v18; // ecx
  int v19; // eax
  unsigned __int16 *v20; // rax
  int v21; // edx
  int v22; // ecx
  unsigned int v23; // esi
  __int64 v24; // rcx
  __int64 v25; // rdi
  __int64 v26; // r8
  char *v27; // r13
  int v28; // edx
  int v29; // eax
  __int64 v30; // rdx
  int v31; // r8d
  int v32; // eax
  int v33; // eax
  _DWORD *v34; // r12
  __int64 v35; // rdx
  HANDLE v36; // rax
  __int64 v37; // r8
  signed int j; // edx
  __int64 v39; // r11
  char *v40; // rcx
  char *v41; // r10
  int v42; // r9d
  int v43; // eax
  unsigned int i; // eax
  __int64 v45; // r8
  unsigned __int16 *v46; // rax
  int v47; // edx
  int v48; // ecx
  unsigned int v49; // edi
  HANDLE ProcessHeap; // rax
  __int64 *v51; // rax
  unsigned int v52; // esi
  char *v53; // rax
  char *v54; // r8
  int v55; // edx
  int v56; // ecx
  __int64 v57; // rax
  HANDLE v58; // rax
  unsigned int v60; // [rsp+30h] [rbp-B9h] BYREF
  __int64 v61; // [rsp+38h] [rbp-B1h] BYREF
  __int64 *v62; // [rsp+40h] [rbp-A9h]
  __int64 v63; // [rsp+48h] [rbp-A1h]
  _QWORD *v64; // [rsp+50h] [rbp-99h] BYREF
  char v65[8]; // [rsp+60h] [rbp-89h] BYREF
  int v66; // [rsp+68h] [rbp-81h] BYREF
  int v67; // [rsp+6Ch] [rbp-7Dh]
  int v68; // [rsp+70h] [rbp-79h]
  int v69; // [rsp+74h] [rbp-75h]
  unsigned int v70; // [rsp+78h] [rbp-71h]
  UUID Uuid; // [rsp+C0h] [rbp-29h] BYREF
  __int64 v72; // [rsp+D0h] [rbp-19h] BYREF
  int v73; // [rsp+D8h] [rbp-11h]
  char v74; // [rsp+DCh] [rbp-Dh]

  v9 = a6;
  v62 = a8;
  v13 = (void (__fastcall *)(char *))sub_1800101C8((__int64)&unk_1800DF420);
  v13(v65);
  sub_1800071F0((__int64)v13);
  v64 = 0i64;
  v14 = 0i64;
  v63 = a1 + 8;
  if ( a1 != -8 )
    sub_18002BAE4(a1 + 8);
  v15 = (__int64 (__fastcall *)(__int64, char *))sub_18000B6C4((__int64)&unk_1800D7400);
  v16 = v15(a3, v65);
  sub_1800071F0((__int64)v15);
  if ( v16 == -2147217327 )
    goto LABEL_4;
  v17 = 0i64;
  if ( v16 < 0 )
    goto LABEL_6;
  if ( a2 )
  {
    if ( v66 || v67 || v68 || v69 )
    {
      v19 = sub_180021570(&v66, (const wchar_t **)&v61);
      v16 = v19;
      if ( v19 < 0 )
        goto LABEL_98;
      v20 = (unsigned __int16 *)v61;
      do
      {
        v21 = *(unsigned __int16 *)((char *)a2 + (_QWORD)v20 - v61);
        v22 = *v20 - v21;
        if ( v22 )
          break;
        ++v20;
      }
      while ( v21 );
      if ( v22 )
        goto LABEL_13;
    }
    else if ( !wcscmp(a2, L"msft:rm/algorithm/pkey/2009") )
    {
LABEL_13:
      v18 = -2147217374;
LABEL_14:
      v16 = v18;
      goto LABEL_99;
    }
  }
  if ( a4 )
  {
    v60 = 13;
    v72 = 0i64;
    v73 = 0;
    v74 = 0;
    v19 = sub_18002A618(a4, &Uuid, (__int64)&v61);
    v16 = v19;
    if ( v19 >= 0 )
    {
      v23 = v60;
      if ( !(unsigned __int8)sub_180027D88(a1 + 112, &Uuid, &v60) )
        goto LABEL_23;
      if ( !(unsigned __int8)sub_180027B94(
                               a1 + 128,
                               *(unsigned int *)(*(_QWORD *)(*(_QWORD *)(a1 + 120) + 8i64 * (int)v60) + 20i64),
                               &v60) )
      {
        v16 = -2147418113;
        goto LABEL_6;
      }
      v25 = (int)v60;
      if ( v70 )
      {
        v24 = *(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60);
        if ( v70 != *(_DWORD *)(v24 + 4) )
          goto LABEL_23;
      }
      if ( a2 )
      {
        v26 = *(_QWORD *)(a1 + 136);
        v24 = *(_QWORD *)(*(_QWORD *)(v26 + 8i64 * (int)v60) + 8i64);
        v27 = (char *)a2 - v24;
        do
        {
          v28 = *(unsigned __int16 *)&v27[v24];
          v29 = *(unsigned __int16 *)v24 - v28;
          if ( v29 )
            break;
          v24 += 2i64;
        }
        while ( v28 );
        if ( v29 )
          goto LABEL_23;
        if ( v61 )
        {
          v24 = *(_QWORD *)(*(_QWORD *)(v26 + 8i64 * (int)v60) + 8i64);
          v30 = v61 - v24;
          do
          {
            v31 = *(unsigned __int16 *)(v24 + v30);
            v32 = *(unsigned __int16 *)v24 - v31;
            if ( v32 )
              break;
            v24 += 2i64;
          }
          while ( v31 );
          if ( v32 )
          {
LABEL_23:
            v18 = -2147217375;
            goto LABEL_14;
          }
        }
      }
      v33 = sub_180021758(v24, (unsigned int *)&v72, v23, (__int64)v65);
      v16 = v33;
      if ( v33 < 0 )
        sub_180002E08(v33);
      sub_180002E28((unsigned int)v16);
      if ( v16 < 0 )
        goto LABEL_6;
      v17 = (__int64 *)(*(_QWORD *)(a1 + 136) + 8 * v25);
      goto LABEL_42;
    }
LABEL_98:
    v18 = v19;
    goto LABEL_99;
  }
  v34 = a5;
  if ( a5 )
  {
    if ( !a6 )
      goto LABEL_63;
    v35 = v70;
    if ( !v70 )
    {
      v49 = a6 & 0x1FFFFFFF;
      if ( (a6 & 0x1FFFFFFF) != a6 )
        sub_180002E08(-2147024362);
      v16 = a6 != v49 ? 0x80070216 : 0;
      sub_180002E28((unsigned int)v16);
      if ( v49 != a6 )
        goto LABEL_6;
      ProcessHeap = GetProcessHeap();
      v51 = (__int64 *)HeapAlloc(ProcessHeap, 0, 8 * a6);
      v14 = v51;
      v17 = v51;
      if ( !v51 )
        goto LABEL_53;
      v52 = 0;
      v61 = (__int64)v51;
      while ( (unsigned __int8)sub_180027B94(a1 + 128, (unsigned int)*v34, &v60) )
      {
        if ( a2 )
        {
          v53 = *(char **)(*(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60) + 8i64);
          v54 = (char *)((char *)a2 - v53);
          do
          {
            v55 = *(unsigned __int16 *)&v54[(_QWORD)v53];
            v56 = *(unsigned __int16 *)v53 - v55;
            if ( v56 )
              break;
            v53 += 2;
          }
          while ( v55 );
          if ( v56 )
            break;
        }
        ++v52;
        ++v34;
        v57 = v61;
        *(_QWORD *)v61 = *(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60);
        v61 = v57 + 8;
        if ( v52 >= a6 )
          goto LABEL_95;
      }
LABEL_63:
      v18 = -2147217370;
      goto LABEL_14;
    }
LABEL_65:
    if ( !(unsigned __int8)sub_180027B94(a1 + 128, v35, &v60) )
    {
      v16 = -2147217388;
      goto LABEL_6;
    }
    if ( !a6 )
      goto LABEL_75;
    for ( i = 0; i < a6; ++i )
    {
      if ( *v34 == v70 )
        break;
      ++v34;
    }
    if ( i < a6 )
    {
LABEL_75:
      if ( v66 || v67 || v68 || v69 )
      {
        v19 = sub_180021570(&v66, (const wchar_t **)&v61);
        v16 = v19;
        if ( v19 < 0 )
          goto LABEL_98;
        v45 = *(_QWORD *)(a1 + 136);
        v46 = (unsigned __int16 *)v61;
        do
        {
          v47 = *(unsigned __int16 *)((char *)v46 + *(_QWORD *)(*(_QWORD *)(v45 + 8i64 * (int)v60) + 8i64) - v61);
          v48 = *v46 - v47;
          if ( v48 )
            break;
          ++v46;
        }
        while ( v47 );
        if ( v48 )
          goto LABEL_13;
      }
      else
      {
        v45 = *(_QWORD *)(a1 + 136);
      }
      v17 = (__int64 *)(v45 + 8i64 * (int)v60);
LABEL_42:
      v9 = 1;
      goto LABEL_96;
    }
    goto LABEL_63;
  }
  v35 = v70;
  if ( v70 )
    goto LABEL_65;
  v9 = *(_DWORD *)(a1 + 132);
  if ( !a2 )
  {
    if ( v9 )
      v17 = *(__int64 **)(a1 + 136);
    goto LABEL_95;
  }
  v16 = 0;
  if ( v9 )
  {
    LODWORD(v17) = 8 * v9;
    if ( (v9 & 0x1FFFFFFF) != v9 )
    {
      v16 = -2147024362;
      sub_180002E08(-2147024362);
      LODWORD(v17) = v60;
    }
  }
  sub_180002E28((unsigned int)v16);
  if ( v16 < 0 )
    goto LABEL_6;
  v36 = GetProcessHeap();
  v14 = (__int64 *)HeapAlloc(v36, 0, (unsigned int)v17);
  v17 = v14;
  if ( !v14 )
  {
LABEL_53:
    v18 = -2147024882;
    goto LABEL_14;
  }
  v37 = 0i64;
  for ( j = 0; j < (unsigned int)v9; ++j )
  {
    v39 = *(_QWORD *)(a1 + 136);
    v40 = *(char **)(*(_QWORD *)(v39 + 8i64 * j) + 8i64);
    v41 = (char *)((char *)a2 - v40);
    do
    {
      v42 = *(unsigned __int16 *)&v41[(_QWORD)v40];
      v43 = *(unsigned __int16 *)v40 - v42;
      if ( v43 )
        break;
      v40 += 2;
    }
    while ( v42 );
    if ( !v43 )
    {
      v14[v37] = *(_QWORD *)(v39 + 8i64 * j);
      v37 = (unsigned int)(v37 + 1);
    }
  }
  v9 = v37;
LABEL_95:
  if ( v9 < 1 )
  {
LABEL_4:
    v16 = -2147217373;
LABEL_6:
    v18 = v16;
LABEL_99:
    sub_180002E08(v18);
    goto LABEL_100;
  }
LABEL_96:
  v19 = sub_18002CF44((__int64)v65, v17, v9, a7, &v61);
  v16 = v19;
  if ( v19 < 0 )
    goto LABEL_98;
  
  // A1, A2
  v19 = sub_18002A448(a1, (__int64)v65, v61, v62);
  
  v16 = v19;
  if ( v19 < 0 )
    goto LABEL_98;
LABEL_100:
  sub_180002E28((unsigned int)v16);
  if ( v14 )
  {
    v58 = GetProcessHeap();
    HeapFree(v58, 0, v14);
  }
  if ( v63 )
    sub_180029C38(v63);
  sub_1800024DC(&v64);
  return (unsigned int)v16;
}
__int64 __fastcall sub_18002A448(__int64 a1, __int64 a2, __int64 a3, __int64 *a4)
{
  int v4; // esi
  int v5; // r14d
  int v9; // esi
  int v10; // eax
  unsigned int v11; // edi
  __int64 v12; // rcx
  __int64 v13; // r14
  __int64 v14; // r12
  HANDLE ProcessHeap; // rax
  void *v16; // rax
  __int64 v17; // rbx
  void (__fastcall *v18)(__int64); // rdi
  __int64 v20[2]; // [rsp+30h] [rbp-10h] BYREF
  char v21; // [rsp+80h] [rbp+40h] BYREF
  __int64 v22; // [rsp+88h] [rbp+48h]
  int v23; // [rsp+90h] [rbp+50h] BYREF

  v22 = a2;
  v4 = 1000000 * *(_DWORD *)(a2 + 28);
  v20[0] = 0i64;
  v5 = a1 + 144;
  v9 = *(_DWORD *)(a2 + 32) + v4;
  v10 = sub_18002B370((int)a1 + 144, *(_DWORD *)(a3 + 4), 0, v9, (__int64)&v21, (__int64)&v23);
  v11 = v10;
  if ( v10 < 0 )
    goto LABEL_2;
  if ( v21 )
  {
    v11 = -2147217371;
    goto LABEL_6;
  }
  v10 = sub_18002B370(v5, *(_DWORD *)(a3 + 4), 1, v9, (__int64)&v21, (__int64)&v23);
  v11 = v10;
  if ( v10 >= 0 )
  {
    if ( v21 )
    {
      v13 = *(_QWORD *)(*(_QWORD *)(a1 + 152) + 8i64 * v23);
      if ( (unsigned __int8)sub_180027D88(a1 + 112, v13 + 4, &v23) )
      {
        v14 = *(_QWORD *)(*(_QWORD *)(a1 + 120) + 8i64 * v23);
        ProcessHeap = GetProcessHeap();
        v16 = HeapAlloc(ProcessHeap, 0, 0x98ui64);
        v17 = (__int64)v16;
        if ( v16 )
        {
          memset(v16, 0, 0x98ui64);
          *(_DWORD *)v17 = 1;
          v18 = (void (__fastcall *)(__int64))sub_1800101C8(&unk_1800DF420);
          v18(v17 + 32);
          sub_1800071F0(v18);
          *(_QWORD *)(v17 + 120) = 0i64;
          *(_QWORD *)(v17 + 128) = 0i64;
          *(_QWORD *)(v17 + 136) = 0i64;
        }
        else
        {
          v17 = 0i64;
        }
        sub_1800024DC(v20);
        v20[0] = v17;
        if ( v17 )
        {
          v10 = sub_18002E074(v17, v22, v14, v13, a3);
          v11 = v10;
          if ( v10 >= 0 )
          {
            v20[0] = 0i64;
            *a4 = v17;
            goto LABEL_19;
          }
          goto LABEL_2;
        }
        v11 = -2147024882;
      }
      else
      {
        v11 = -2147418113;
      }
    }
    else
    {
      v11 = -2147217372;
    }
LABEL_6:
    v12 = v11;
    goto LABEL_3;
  }
LABEL_2:
  v12 = (unsigned int)v10;
LABEL_3:
  sub_180002E08(v12);
LABEL_19:
  sub_180002E28(v11);
  sub_1800024DC(v20);
  return v11;
}
__int64 __fastcall sub_18002E074(__int64 a1, __int64 a2, __int64 a3, void *a4, const wchar_t **a5)
{
  signed int v8; // edi
  int v9; // eax
  __int64 v10; // r8
  __int64 v11; // rcx
  __int64 v12; // rcx
  int v13; // eax
  signed int LastError; // eax
  signed int v15; // eax
  struct _FILETIME v16; // rax
  __int64 v17; // r12
  __int64 v18; // rbx
  HANDLE ProcessHeap; // rax
  __int64 v20; // rax
  void *v21; // rcx
  void *v22; // rcx
  void *v23; // rbx
  unsigned __int16 *v24; // rbx
  void *v25; // rbx
  HANDLE v26; // rax
  void *v27; // rbx
  HANDLE v28; // rax
  void *v29; // rbx
  void *v30; // rbx
  RPC_WSTR StringUuid; // [rsp+20h] [rbp-60h] BYREF
  __int64 v33; // [rsp+28h] [rbp-58h] BYREF
  __int64 v34; // [rsp+30h] [rbp-50h] BYREF
  LPVOID lpMem; // [rsp+38h] [rbp-48h]
  LPVOID v36; // [rsp+40h] [rbp-40h]
  LPVOID v37; // [rsp+48h] [rbp-38h]
  struct _FILETIME FileTime; // [rsp+50h] [rbp-30h] BYREF
  void *v39; // [rsp+58h] [rbp-28h]
  RPC_WSTR String; // [rsp+60h] [rbp-20h] BYREF
  struct _SYSTEMTIME SystemTime; // [rsp+68h] [rbp-18h] BYREF

  v37 = (LPVOID)a3;
  v34 = 0i64;
  v39 = a4;
  v33 = 0i64;
  StringUuid = 0i64;
  _InterlockedIncrement((volatile signed __int32 *)a3);
  v36 = a4;
  _InterlockedIncrement((volatile signed __int32 *)a4);
  lpMem = a5;
  _InterlockedIncrement((volatile signed __int32 *)a5);
  if ( UuidToStringW((const UUID *)(a3 + 4), &StringUuid) )
  {
    sub_180005120(&unk_1800D54F2, &unk_180101F08);
    v8 = -2147217375;
    sub_180005E44(&unk_1800D447C, &unk_180101E98);
    sub_180002E08(2147749921i64);
    sub_180005120(&unk_1800DEFAD, &unk_180101A28);
    goto LABEL_40;
  }
  sub_180006B60(&unk_1800EB290, &unk_180101718);
  v9 = wcscmp(a5[1], L"msft:rm/algorithm/pkey/2009");
  v11 = a2 + 72;
  if ( v9 )
  {
    v8 = sub_18002E5C0(v11, 12i64, v10, &v33);
    sub_1800057B4(&unk_1800F3449, &unk_180101E00);
    if ( v8 < 0 )
      goto LABEL_5;
    sub_180006EAC(&unk_1800E9278, &unk_180101E70);
    sub_1800064C4(&unk_1800E6113, &unk_180101F78);
    v13 = sub_180003C18(&v34, L"msft2005:%s&%s", StringUuid, v33);
  }
  else
  {
    v8 = sub_18002E5C0(v11, 13i64, v10, &v33);
    sub_180006B60(&unk_1800DB87D, &unk_1801017EC);
    if ( v8 < 0 )
    {
LABEL_5:
      v12 = (unsigned int)v8;
LABEL_6:
      sub_180002E08(v12);
      goto LABEL_40;
    }
    v13 = sub_180003C18(&v34, L"msft2009:%s&%s", StringUuid, v33);
  }
  v8 = v13;
  if ( v13 < 0 )
  {
    v12 = (unsigned int)v13;
    goto LABEL_6;
  }
  GetLocalTime(&SystemTime);
  if ( !SystemTimeToFileTime(&SystemTime, &FileTime) )
  {
    LastError = GetLastError();
    if ( LastError )
    {
      v8 = (unsigned __int16)LastError | 0x80070000;
      if ( LastError <= 0 )
        v8 = LastError;
    }
    else
    {
      v8 = -2147467259;
    }
    goto LABEL_5;
  }
  v8 = 0;
  if ( dword_1801029A8 == 1 )
  {
    if ( !CryptGenRandom(hProv, 8u, (BYTE *)(a1 + 144)) )
    {
      v15 = GetLastError();
      if ( v15 )
      {
        v8 = (unsigned __int16)v15 | 0x80070000;
        if ( v15 <= 0 )
          v8 = v15;
        sub_180005470(&unk_1800DABC8, &unk_180101748);
      }
      else
      {
        v8 = -2147467259;
      }
      sub_180002E08((unsigned int)v8);
      sub_180005B00(&unk_1800D5D9D, &unk_180101F50);
    }
  }
  else
  {
    v8 = -2147418113;
    sub_180005B00(&unk_1800D577C, &unk_180101F18);
    sub_180002E08(2147549183i64);
    sub_180006810(&unk_1800E723C, &unk_18010208C);
  }
  sub_180002E28((unsigned int)v8);
  if ( v8 < 0 )
  {
    sub_180006810(&unk_1800F0093, &unk_180101AE4);
    goto LABEL_5;
  }
  v16 = FileTime;
  
  *(_OWORD *)(a1 + 32) = *(_OWORD *)a2;
  *(_OWORD *)(a1 + 48) = *(_OWORD *)(a2 + 16);
  *(_OWORD *)(a1 + 64) = *(_OWORD *)(a2 + 32);
  *(_OWORD *)(a1 + 80) = *(_OWORD *)(a2 + 48);
  *(_OWORD *)(a1 + 96) = *(_OWORD *)(a2 + 64);
  *(_QWORD *)(a1 + 112) = *(_QWORD *)(a2 + 80);
  *(struct _FILETIME *)(a1 + 16) = v16;
  sub_1800064C4(&unk_1800DE881, &unk_180102218);
  *(_DWORD *)(a1 + 24) = *(_DWORD *)(a2 + 32) + 1000000 * *(_DWORD *)(a2 + 28);
  sub_180006EAC(&unk_1800FCFFC, &unk_180101B28);
  v17 = v34;
  sub_180005120(&unk_1800CFD92, &unk_180101B7C);
  v34 = 0i64;
  v18 = *(_QWORD *)(a1 + 8);
  sub_180005E44(&unk_1800E809C, &unk_180102178);
  if ( v18 )
  {
    ProcessHeap = GetProcessHeap();
    HeapFree(ProcessHeap, 0, (LPVOID)(v18 - 4));
    sub_180002E28(0i64);
  }
  v20 = 0i64;
  if ( v17 )
    v20 = v17;
  v37 = 0i64;
  *(_QWORD *)(a1 + 8) = v20;
  sub_180006810(&unk_1800DEFF1, &unk_180101A2C);
  v21 = *(void **)(a1 + 120);
  if ( v21 )
    sub_180002CB0(v21);
  *(_QWORD *)(a1 + 120) = a3;
  sub_1800057B4(&unk_1800EC498, &unk_180101820);
  v22 = *(void **)(a1 + 128);
  v36 = 0i64;
  if ( v22 )
    sub_180002C18(v22);
  *(_QWORD *)(a1 + 128) = v39;
  sub_180006190(&unk_1800E01F0, &unk_180101B10);
  v23 = *(void **)(a1 + 136);
  lpMem = 0i64;
  sub_180005470(&unk_1800E2047, &unk_180101C94);
  if ( v23 )
    sub_180002B5C(v23);
  *(_QWORD *)(a1 + 136) = a5;
LABEL_40:
  sub_180002E28((unsigned int)v8);
  v24 = StringUuid;
  if ( StringUuid )
  {
    sub_180006B60(&unk_1800EF65D, &unk_180101A70);
    String = v24;
    sub_180005470(&unk_1800F1E40, &unk_180101C80);
    RpcStringFreeW(&String);
    StringUuid = 0i64;
  }
  if ( v33 )
  {
    v25 = (void *)(v33 - 4);
    v26 = GetProcessHeap();
    HeapFree(v26, 0, v25);
    sub_180002E28(0i64);
  }
  if ( v34 )
  {
    v27 = (void *)(v34 - 4);
    v28 = GetProcessHeap();
    HeapFree(v28, 0, v27);
    sub_180002E28(0i64);
  }
  if ( lpMem )
  {
    sub_180002B5C(lpMem);
    sub_180006190(&unk_1800F1E70, &unk_180101C84);
  }
  v29 = v36;
  if ( v36 )
  {
    sub_180006EAC(&unk_1800DD02B, &unk_180102200);
    sub_180002C18(v29);
  }
  v30 = v37;
  if ( v37 )
  {
    sub_1800057B4(&unk_1800EFAAD, &unk_180101AB4);
    sub_180002CB0(v30);
  }
  return (unsigned int)v8;
}

>>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> 

// v16 = v15(a3, v65);
// in Unprotected file ... 

// {v12} Psudo Struct
// Size: 0x58 (88 bytes) // v12
typedef struct _MSFT_PKEY_DATA {
	uint64_t Reserved;          // 0x00
	uint8_t  Signature[16];     // 0x08
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
        v7 = sub_180008A58(&v14, (char *)&v16[4] + 8);
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

>>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> 

PidgenX Insider
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
  int v9; // esi
  _QWORD *v13; // rbx
  unsigned int v14; // r12d
  int v15; // eax
  __int64 v16; // r8
  int v17; // edi
  __int64 v18; // rcx
  int v19; // eax
  unsigned __int16 *v20; // rax
  int v21; // edx
  int v22; // ecx
  __int64 *v23; // r14
  size_t v24; // rsi
  __int64 v25; // r9
  __int64 v26; // r12
  __int64 v27; // r9
  char *v28; // rcx
  signed __int64 v29; // r15
  int v30; // edx
  int v31; // eax
  unsigned __int16 *v32; // rcx
  __int64 v33; // rdx
  int v34; // r8d
  int v35; // eax
  __int64 v36; // rdx
  _DWORD *v37; // r15
  __int64 v38; // rdx
  size_t v39; // r14
  HANDLE v40; // rax
  unsigned int v41; // r8d
  signed int i; // edx
  __int64 v43; // rdi
  unsigned __int16 *v44; // rcx
  size_t v45; // r10
  int v46; // r9d
  int v47; // eax
  __int64 v48; // rax
  __int64 v49; // r8
  unsigned int j; // eax
  unsigned __int16 *v51; // rax
  int v52; // edx
  int v53; // ecx
  int v54; // r14d
  HANDLE ProcessHeap; // rax
  _QWORD *v56; // rax
  __int64 v57; // r9
  unsigned int v58; // r12d
  size_t v59; // r14
  _QWORD *v60; // rdi
  unsigned __int16 *v61; // rax
  size_t v62; // r8
  int v63; // edx
  int v64; // ecx
  HANDLE v65; // rax
  int v67; // [rsp+30h] [rbp-C9h] BYREF
  __int64 v68; // [rsp+38h] [rbp-C1h] BYREF
  size_t Size; // [rsp+40h] [rbp-B9h]
  int v70; // [rsp+48h] [rbp-B1h]
  __int64 v71; // [rsp+50h] [rbp-A9h] BYREF
  RPC_WSTR StringUuid; // [rsp+58h] [rbp-A1h]
  __int64 v73; // [rsp+60h] [rbp-99h]
  __int64 v74; // [rsp+68h] [rbp-91h]
  _QWORD v75[12]; // [rsp+70h] [rbp-89h] BYREF
  UUID Uuid; // [rsp+D0h] [rbp-29h] BYREF
  __int64 v77; // [rsp+E0h] [rbp-19h] BYREF
  int v78; // [rsp+E8h] [rbp-11h]
  char v79; // [rsp+ECh] [rbp-Dh]

  v9 = a6;
  Size = (size_t)a2;
  StringUuid = a4;
  v73 = a8;
  memset(v75, 0, 0x58ui64);
  v67 = 0;
  v70 = 0;
  v13 = 0i64;
  v68 = 0i64;
  v14 = 0;
  v71 = 0i64;
  v74 = a1 + 8;
  memset(&v75[1], 0, 40);
  Uuid = 0i64;
  if ( a1 != -8 )
  {
    sub_18000AA3C(a1 + 8);
    v70 = 1;
  }
  v15 = sub_1800090B0(a3, (__int64)v75);
  v17 = v15;
  if ( v15 == -2147217327 )
    goto LABEL_4;
  if ( v15 < 0 )
    goto LABEL_6;
  if ( a2 )
  {
    if ( LODWORD(v75[1]) || __PAIR64__(HIDWORD(v75[1]), 0) != LODWORD(v75[2]) || HIDWORD(v75[2]) )
    {
      v19 = sub_18000EE1C(&v75[1], &v68, v16, 0i64);
      v17 = v19;
      if ( v19 < 0 )
        goto LABEL_113;
      v20 = (unsigned __int16 *)v68;
      do
      {
        v21 = *(unsigned __int16 *)((char *)a2 + (_QWORD)v20 - v68);
        v22 = *v20 - v21;
        if ( v22 )
          break;
        ++v20;
      }
      while ( v21 );
      if ( v22 )
        goto LABEL_12;
    }
    else if ( !wcscmp(a2, L"msft:rm/algorithm/pkey/2009") )
    {
LABEL_12:
      v18 = 2147749922i64;
LABEL_13:
      v17 = v18;
      goto LABEL_114;
    }
  }
  if ( a4 )
  {
    LODWORD(Size) = 13;
    v77 = 0i64;
    v78 = 0;
    v79 = 0;
    v23 = &v77;
    v17 = sub_180011E1C(StringUuid, &Uuid, (__int64)&v68);
    if ( v17 < 0 )
      goto LABEL_6;
    v24 = (unsigned int)Size;
    if ( !(_DWORD)Size )
      v23 = 0i64;
    if ( !(unsigned __int8)sub_18000E7E8(a1 + 112, &Uuid, &v67) )
      goto LABEL_24;
    if ( !(unsigned __int8)sub_18000E8E8(
                             a1 + 128,
                             *(unsigned int *)(*(_QWORD *)(*(_QWORD *)(a1 + 120) + 8i64 * v67) + 20i64),
                             &v67,
                             v25) )
    {
      v17 = -2147418113;
      goto LABEL_6;
    }
    v26 = v67;
    if ( LODWORD(v75[3]) )
    {
      if ( LODWORD(v75[3]) != *(_DWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * v67) + 4i64) )
        goto LABEL_24;
    }
    if ( a2 )
    {
      v27 = *(_QWORD *)(a1 + 136);
      v28 = *(char **)(*(_QWORD *)(v27 + 8i64 * v67) + 8i64);
      v29 = (char *)a2 - v28;
      do
      {
        v30 = *(unsigned __int16 *)&v28[v29];
        v31 = *(unsigned __int16 *)v28 - v30;
        if ( v31 )
          break;
        v28 += 2;
      }
      while ( v30 );
      if ( v31 )
        goto LABEL_24;
      if ( v68 )
      {
        v32 = *(unsigned __int16 **)(*(_QWORD *)(v27 + 8i64 * v67) + 8i64);
        v33 = v68 - (_QWORD)v32;
        do
        {
          v34 = *(unsigned __int16 *)((char *)v32 + v33);
          v35 = *v32 - v34;
          if ( v35 )
            break;
          ++v32;
        }
        while ( v34 );
        if ( v35 )
        {
LABEL_24:
          v18 = 2147749921i64;
          goto LABEL_13;
        }
      }
    }
    v17 = 0;
    if ( HIDWORD(v75[6]) )
    {
      if ( !(_DWORD)v24
        || (_DWORD)v24 == 13
        && v75[9] == *v23
        && LODWORD(v75[10]) == *((_DWORD *)v23 + 2)
        && BYTE4(v75[10]) == *((_BYTE *)v23 + 12) )
      {
        goto LABEL_50;
      }
    }
    else if ( (unsigned int)v24 <= 0xD )
    {
      v75[9] = 0i64;
      LODWORD(v75[10]) = 0;
      BYTE4(v75[10]) = 0;
      if ( (_DWORD)v24 )
      {
        memcpy(&v75[9], v23, v24);
        HIDWORD(v75[6]) = 1;
      }
      goto LABEL_50;
    }
    v17 = -2147217375;
    sub_1800038E8(2147749921i64);
LABEL_50:
    sub_180003B30((unsigned int)v17);
    if ( v17 < 0 )
      sub_1800038E8((unsigned int)v17);
    sub_180003B30((unsigned int)v17);
    if ( v17 < 0 )
      goto LABEL_6;
    v36 = *(_QWORD *)(a1 + 136) + 8 * v26;
    goto LABEL_54;
  }
  v37 = a5;
  if ( a5 )
  {
    if ( a6 )
    {
      v38 = LODWORD(v75[3]);
      if ( LODWORD(v75[3]) )
        goto LABEL_79;
      v54 = 0;
      if ( (a6 & 0x1FFFFFFF) == a6 )
      {
        v14 = 8 * a6;
      }
      else
      {
        v54 = -2147024362;
        sub_1800038E8(2147942934i64);
      }
      sub_180003B30((unsigned int)v54);
      v17 = v54;
      if ( v54 < 0 )
      {
        v18 = (unsigned int)v54;
        goto LABEL_114;
      }
      ProcessHeap = GetProcessHeap();
      v56 = HeapAlloc(ProcessHeap, 0, v14);
      v57 = 0i64;
      v13 = v56;
      if ( !v56 )
        goto LABEL_67;
      v58 = 0;
      v59 = Size;
      v60 = v56;
      while ( (unsigned __int8)sub_18000E8E8(a1 + 128, (unsigned int)*v37, &v67, v57) )
      {
        if ( v59 )
        {
          v61 = *(unsigned __int16 **)(*(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * v67) + 8i64);
          v62 = v59 - (_QWORD)v61;
          do
          {
            v63 = *(unsigned __int16 *)((char *)v61 + v62);
            v64 = *v61 - v63;
            if ( v64 )
              break;
            ++v61;
          }
          while ( v63 );
          if ( v64 )
            break;
        }
        ++v58;
        ++v37;
        *v60++ = *(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * v67);
        if ( v58 >= a6 )
          goto LABEL_109;
      }
    }
LABEL_77:
    v18 = 2147749926i64;
    goto LABEL_13;
  }
  v38 = LODWORD(v75[3]);
  if ( !LODWORD(v75[3]) )
  {
    v39 = Size;
    v9 = *(_DWORD *)(a1 + 132);
    if ( !Size )
    {
      if ( !v9 )
        goto LABEL_4;
      v36 = *(_QWORD *)(a1 + 136);
LABEL_110:
      if ( v9 >= 1 )
        goto LABEL_111;
LABEL_4:
      v17 = -2147217373;
LABEL_6:
      v18 = (unsigned int)v17;
LABEL_114:
      sub_1800038E8(v18);
      goto LABEL_115;
    }
    v17 = 0;
    if ( v9 )
    {
      if ( (v9 & 0x1FFFFFFF) == v9 )
      {
        v14 = 8 * v9;
      }
      else
      {
        v17 = -2147024362;
        sub_1800038E8(2147942934i64);
      }
    }
    else
    {
      v14 = 0;
    }
    sub_180003B30((unsigned int)v17);
    if ( v17 < 0 )
      goto LABEL_6;
    v40 = GetProcessHeap();
    v13 = HeapAlloc(v40, 0, v14);
    if ( v13 )
    {
      v41 = 0;
      for ( i = 0; i < (unsigned int)v9; ++i )
      {
        v43 = *(_QWORD *)(a1 + 136);
        v44 = *(unsigned __int16 **)(*(_QWORD *)(v43 + 8i64 * i) + 8i64);
        v45 = v39 - (_QWORD)v44;
        do
        {
          v46 = *(unsigned __int16 *)((char *)v44 + v45);
          v47 = *v44 - v46;
          if ( v47 )
            break;
          ++v44;
        }
        while ( v46 );
        if ( !v47 )
        {
          v48 = v41++;
          v13[v48] = *(_QWORD *)(v43 + 8i64 * i);
        }
      }
      v9 = v41;
LABEL_109:
      LODWORD(v36) = (_DWORD)v13;
      goto LABEL_110;
    }
LABEL_67:
    v13 = 0i64;
    v18 = 2147942414i64;
    goto LABEL_13;
  }
LABEL_79:
  if ( !(unsigned __int8)sub_18000E8E8(a1 + 128, v38, &v67, 0i64) )
  {
    v17 = -2147217388;
    goto LABEL_6;
  }
  if ( a6 )
  {
    for ( j = 0; j < a6; ++j )
    {
      if ( *v37 == LODWORD(v75[3]) )
        break;
      ++v37;
    }
    if ( j >= a6 )
      goto LABEL_77;
  }
  if ( LODWORD(v75[1]) || __PAIR64__(HIDWORD(v75[1]), 0) != LODWORD(v75[2]) || HIDWORD(v75[2]) )
  {
    v19 = sub_18000EE1C(&v75[1], &v68, v49, 0i64);
    v17 = v19;
    if ( v19 >= 0 )
    {
      v51 = (unsigned __int16 *)v68;
      do
      {
        v52 = *(unsigned __int16 *)((char *)v51
                                  + *(_QWORD *)(*(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * v67) + 8i64)
                                  - v68);
        v53 = *v51 - v52;
        if ( v53 )
          break;
        ++v51;
      }
      while ( v52 );
      if ( v53 )
        goto LABEL_12;
      goto LABEL_94;
    }
LABEL_113:
    v18 = (unsigned int)v19;
    goto LABEL_114;
  }
LABEL_94:
  v36 = *(_QWORD *)(a1 + 136) + 8i64 * v67;
LABEL_54:
  v9 = 1;
LABEL_111:
  v19 = sub_180014DA4((unsigned int)v75, v36, v9, a7, (__int64)&v71);
  v17 = v19;
  if ( v19 < 0 )
    goto LABEL_113;
  v19 = sub_180014730(a1, v75, v71, v73);
  v17 = v19;
  if ( v19 < 0 )
    goto LABEL_113;
LABEL_115:
  sub_180003B30((unsigned int)v17);
  if ( v13 )
  {
    v65 = GetProcessHeap();
    HeapFree(v65, 0, v13);
  }
  if ( v74 && v70 )
    sub_180012E7C();
  return (unsigned int)v17;
}

PidgenX Retail
__int64 __fastcall sub_18002C79C(
        __int64 a1,
        const wchar_t *a2,
        __int64 a3,
        unsigned __int16 *a4,
        _DWORD *a5,
        unsigned int a6,
        int a7,
        __int64 *a8)
{
  int v9; // r14d
  void (__fastcall *v13)(char *); // rbx
  _QWORD *v14; // rbx
  __int64 (__fastcall *v15)(__int64, char *); // rdi
  unsigned int v16; // esi
  __int64 v17; // rdi
  __int64 v18; // rcx
  int v19; // eax
  unsigned __int16 *v20; // rax
  int v21; // edx
  int v22; // ecx
  unsigned int v23; // esi
  unsigned __int16 *v24; // rcx
  __int64 v25; // rdi
  __int64 v26; // r8
  signed __int64 v27; // r13
  int v28; // edx
  int v29; // eax
  __int64 v30; // rdx
  int v31; // r8d
  int v32; // eax
  int v33; // eax
  _DWORD *v34; // r12
  __int64 v35; // rdx
  HANDLE v36; // rax
  __int64 v37; // r8
  signed int j; // edx
  __int64 v39; // r11
  char *v40; // rcx
  signed __int64 v41; // r10
  int v42; // r9d
  int v43; // eax
  unsigned int i; // eax
  __int64 v45; // r8
  unsigned __int16 *v46; // rax
  int v47; // edx
  int v48; // ecx
  int v49; // edi
  HANDLE ProcessHeap; // rax
  _QWORD *v51; // rax
  unsigned int v52; // esi
  char *v53; // rax
  signed __int64 v54; // r8
  int v55; // edx
  int v56; // ecx
  __int64 v57; // rax
  HANDLE v58; // rax
  unsigned int v60; // [rsp+30h] [rbp-B9h] BYREF
  __int64 v61; // [rsp+38h] [rbp-B1h] BYREF
  __int64 *v62; // [rsp+40h] [rbp-A9h]
  __int64 v63; // [rsp+48h] [rbp-A1h]
  __int64 v64; // [rsp+50h] [rbp-99h] BYREF
  char v65[8]; // [rsp+60h] [rbp-89h] BYREF
  int v66; // [rsp+68h] [rbp-81h] BYREF
  int v67; // [rsp+6Ch] [rbp-7Dh]
  int v68; // [rsp+70h] [rbp-79h]
  int v69; // [rsp+74h] [rbp-75h]
  unsigned int v70; // [rsp+78h] [rbp-71h]
  UUID Uuid; // [rsp+C0h] [rbp-29h] BYREF
  __int64 v72; // [rsp+D0h] [rbp-19h] BYREF
  int v73; // [rsp+D8h] [rbp-11h]
  char v74; // [rsp+DCh] [rbp-Dh]

  v9 = a6;
  v62 = a8;
  v13 = (void (__fastcall *)(char *))sub_1800101C8(&unk_1800DF420);
  v13(v65);
  sub_1800071F0(v13);
  v64 = 0i64;
  v14 = 0i64;
  v63 = a1 + 8;
  if ( a1 != -8 )
    sub_18002BAE4(a1 + 8);
  v15 = (__int64 (__fastcall *)(__int64, char *))sub_18000B6C4(&unk_1800D7400);
  v16 = v15(a3, v65);
  sub_1800071F0(v15);
  if ( v16 == -2147217327 )
    goto LABEL_4;
  LODWORD(v17) = 0;
  if ( (v16 & 0x80000000) != 0 )
    goto LABEL_6;
  if ( a2 )
  {
    if ( v66 || v67 || v68 || v69 )
    {
      v19 = sub_180021570(&v66, &v61);
      v16 = v19;
      if ( v19 < 0 )
        goto LABEL_98;
      v20 = (unsigned __int16 *)v61;
      do
      {
        v21 = *(unsigned __int16 *)((char *)a2 + (_QWORD)v20 - v61);
        v22 = *v20 - v21;
        if ( v22 )
          break;
        ++v20;
      }
      while ( v21 );
      if ( v22 )
        goto LABEL_13;
    }
    else if ( !wcscmp(a2, L"msft:rm/algorithm/pkey/2009") )
    {
LABEL_13:
      v18 = 2147749922i64;
LABEL_14:
      v16 = v18;
      goto LABEL_99;
    }
  }
  if ( a4 )
  {
    v60 = 13;
    v72 = 0i64;
    v73 = 0;
    v74 = 0;
    v19 = sub_18002A618(a4, &Uuid, (__int64)&v61);
    v16 = v19;
    if ( v19 >= 0 )
    {
      v23 = v60;
      if ( !(unsigned __int8)sub_180027D88(a1 + 112, &Uuid, &v60) )
        goto LABEL_23;
      if ( !(unsigned __int8)sub_180027B94(
                               a1 + 128,
                               *(unsigned int *)(*(_QWORD *)(*(_QWORD *)(a1 + 120) + 8i64 * (int)v60) + 20i64),
                               &v60) )
      {
        v16 = -2147418113;
        goto LABEL_6;
      }
      v25 = (int)v60;
      if ( v70 )
      {
        v24 = *(unsigned __int16 **)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60);
        if ( v70 != *((_DWORD *)v24 + 1) )
          goto LABEL_23;
      }
      if ( a2 )
      {
        v26 = *(_QWORD *)(a1 + 136);
        v24 = *(unsigned __int16 **)(*(_QWORD *)(v26 + 8i64 * (int)v60) + 8i64);
        v27 = (char *)a2 - (char *)v24;
        do
        {
          v28 = *(unsigned __int16 *)((char *)v24 + v27);
          v29 = *v24 - v28;
          if ( v29 )
            break;
          ++v24;
        }
        while ( v28 );
        if ( v29 )
          goto LABEL_23;
        if ( v61 )
        {
          v24 = *(unsigned __int16 **)(*(_QWORD *)(v26 + 8i64 * (int)v60) + 8i64);
          v30 = v61 - (_QWORD)v24;
          do
          {
            v31 = *(unsigned __int16 *)((char *)v24 + v30);
            v32 = *v24 - v31;
            if ( v32 )
              break;
            ++v24;
          }
          while ( v31 );
          if ( v32 )
          {
LABEL_23:
            v18 = 2147749921i64;
            goto LABEL_14;
          }
        }
      }
      v33 = sub_180021758(v24, &v72, v23, v65);
      v16 = v33;
      if ( v33 < 0 )
        sub_180002E08((unsigned int)v33);
      sub_180002E28(v16);
      if ( (v16 & 0x80000000) != 0 )
        goto LABEL_6;
      v17 = *(_QWORD *)(a1 + 136) + 8 * v25;
      goto LABEL_42;
    }
LABEL_98:
    v18 = (unsigned int)v19;
    goto LABEL_99;
  }
  v34 = a5;
  if ( a5 )
  {
    if ( !a6 )
      goto LABEL_63;
    v35 = v70;
    if ( !v70 )
    {
      v49 = a6 & 0x1FFFFFFF;
      if ( (a6 & 0x1FFFFFFF) != a6 )
        sub_180002E08(2147942934i64);
      v16 = a6 != v49 ? 0x80070216 : 0;
      sub_180002E28(v16);
      if ( v49 != a6 )
        goto LABEL_6;
      ProcessHeap = GetProcessHeap();
      v51 = HeapAlloc(ProcessHeap, 0, 8 * a6);
      v14 = v51;
      LODWORD(v17) = (_DWORD)v51;
      if ( !v51 )
        goto LABEL_53;
      v52 = 0;
      v61 = (__int64)v51;
      while ( (unsigned __int8)sub_180027B94(a1 + 128, (unsigned int)*v34, &v60) )
      {
        if ( a2 )
        {
          v53 = *(char **)(*(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60) + 8i64);
          v54 = (char *)a2 - v53;
          do
          {
            v55 = *(unsigned __int16 *)&v53[v54];
            v56 = *(unsigned __int16 *)v53 - v55;
            if ( v56 )
              break;
            v53 += 2;
          }
          while ( v55 );
          if ( v56 )
            break;
        }
        ++v52;
        ++v34;
        v57 = v61;
        *(_QWORD *)v61 = *(_QWORD *)(*(_QWORD *)(a1 + 136) + 8i64 * (int)v60);
        v61 = v57 + 8;
        if ( v52 >= a6 )
          goto LABEL_95;
      }
LABEL_63:
      v18 = 2147749926i64;
      goto LABEL_14;
    }
LABEL_65:
    if ( !(unsigned __int8)sub_180027B94(a1 + 128, v35, &v60) )
    {
      v16 = -2147217388;
      goto LABEL_6;
    }
    if ( !a6 )
      goto LABEL_75;
    for ( i = 0; i < a6; ++i )
    {
      if ( *v34 == v70 )
        break;
      ++v34;
    }
    if ( i < a6 )
    {
LABEL_75:
      if ( v66 || v67 || v68 || v69 )
      {
        v19 = sub_180021570(&v66, &v61);
        v16 = v19;
        if ( v19 < 0 )
          goto LABEL_98;
        v45 = *(_QWORD *)(a1 + 136);
        v46 = (unsigned __int16 *)v61;
        do
        {
          v47 = *(unsigned __int16 *)((char *)v46 + *(_QWORD *)(*(_QWORD *)(v45 + 8i64 * (int)v60) + 8i64) - v61);
          v48 = *v46 - v47;
          if ( v48 )
            break;
          ++v46;
        }
        while ( v47 );
        if ( v48 )
          goto LABEL_13;
      }
      else
      {
        v45 = *(_QWORD *)(a1 + 136);
      }
      LODWORD(v17) = v45 + 8 * v60;
LABEL_42:
      v9 = 1;
      goto LABEL_96;
    }
    goto LABEL_63;
  }
  v35 = v70;
  if ( v70 )
    goto LABEL_65;
  v9 = *(_DWORD *)(a1 + 132);
  if ( !a2 )
  {
    if ( v9 )
      v17 = *(_QWORD *)(a1 + 136);
    goto LABEL_95;
  }
  v16 = 0;
  if ( v9 )
  {
    LODWORD(v17) = 8 * v9;
    if ( (v9 & 0x1FFFFFFF) != v9 )
    {
      v16 = -2147024362;
      sub_180002E08(2147942934i64);
      LODWORD(v17) = v60;
    }
  }
  sub_180002E28(v16);
  if ( (v16 & 0x80000000) != 0 )
    goto LABEL_6;
  v36 = GetProcessHeap();
  v14 = HeapAlloc(v36, 0, (unsigned int)v17);
  LODWORD(v17) = (_DWORD)v14;
  if ( !v14 )
  {
LABEL_53:
    v18 = 2147942414i64;
    goto LABEL_14;
  }
  v37 = 0i64;
  for ( j = 0; j < (unsigned int)v9; ++j )
  {
    v39 = *(_QWORD *)(a1 + 136);
    v40 = *(char **)(*(_QWORD *)(v39 + 8i64 * j) + 8i64);
    v41 = (char *)a2 - v40;
    do
    {
      v42 = *(unsigned __int16 *)&v40[v41];
      v43 = *(unsigned __int16 *)v40 - v42;
      if ( v43 )
        break;
      v40 += 2;
    }
    while ( v42 );
    if ( !v43 )
    {
      v14[v37] = *(_QWORD *)(v39 + 8i64 * j);
      v37 = (unsigned int)(v37 + 1);
    }
  }
  v9 = v37;
LABEL_95:
  if ( v9 < 1 )
  {
LABEL_4:
    v16 = -2147217373;
LABEL_6:
    v18 = v16;
LABEL_99:
    sub_180002E08(v18);
    goto LABEL_100;
  }
LABEL_96:
  v19 = sub_18002CF44((unsigned int)v65, v17, v9, a7, (__int64)&v61);
  v16 = v19;
  if ( v19 < 0 )
    goto LABEL_98;
  v19 = sub_18002A448(a1, (__int64)v65, v61, v62);
  v16 = v19;
  if ( v19 < 0 )
    goto LABEL_98;
LABEL_100:
  sub_180002E28(v16);
  if ( v14 )
  {
    v58 = GetProcessHeap();
    HeapFree(v58, 0, v14);
  }
  if ( v63 )
    sub_180029C38();
  sub_1800024DC(&v64);
  return v16;
}