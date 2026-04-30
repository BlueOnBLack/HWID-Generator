function Get-PKeyData {
    param (
        [string]$key,
        [string]$configPath,
        [switch]$AsObject
    )

    $HWID       = 0L
    $MPC        = [IntPtr]::Zero
    $IID        = ""
    $Edition    = ""
    $Channel    = ""
    $Partnum    = ""

    # for the right way to calculate IID
    # use SLGenerateOfflineInstallationIdEx
    # cause we dont know the hwid value

    # to receive the confirmation ID ........
    # you will have to have the extended product id too.
    #Call-WebService -requestType 1 -installationId $ppwszInstallation -extendedProductId $extPid

    $results = @()

    try {
        # Validate input
        if ([string]::IsNullOrWhiteSpace($key) -or [string]::IsNullOrWhiteSpace($configPath)) {
            throw "KEY and CONFIG PATH cannot be empty."
        }
            
        try {
            
            $ret = $Global:PIDGENX::GetPKeyData(
                $key, $configPath, $Mpc, [IntPtr]::Zero, $HWID,
                [ref]$IID, [ref]$Edition, [ref]$Channel, [ref]$Partnum,
                [IntPtr]::Zero
            )

        } catch {
                
			<#
            >>> .InnerException Class <<<
            -----------------------------

            ErrorCode      : -1979645951
            Message        : Exception from HRESULT: 0x8A010001
            Data           : {}
            InnerException : 
            TargetSite     : Int32 PidGenX(System.String, System.String, System.String, Int32, IntPtr, IntPtr, IntPtr)
            StackTrace     :    at 0.PidGenX(String , String , String , Int32 , IntPtr , IntPtr , IntPtr )
								            at CallSite.Target(Closure , CallSite , Object , String , String , String , Int32 , IntPtr , IntPtr , Object )
            HelpLink       : 
            Source         : 4
            HResult        : -1979645951
            #>

            # Access the inner exception
            $innerException = $_.Exception.InnerException

            # Get the HResult directly
            $HResult   = $innerException.HResult
            $ErrorCode = $innerException.ErrorCode

            # Convert HResult to hexadecimal
            $HResultHex = "0x{0:X8}" -f $HResult

            throw "HRESULT: $ErrorText ($HResultHex)"
        }

        if ($ret -ne 0x0) {

            $HResultHex = "0x{0:X8}" -f $ret
            $ErrorText = Parse-ErrorMessage -MessageId $HResultHex
            throw "HRESULT: $ErrorText ($HResultHex)"
        }

        if ($AsObject) {
            return (
                [PSObject]@{
                    Edition = $Edition
                    Channel = $Channel
                    Partnum = $Partnum
                    IID     = $IID
                }
            )
        } else {
            $results += @{ Property = "Edition"; Value = $Edition }
            $results += @{ Property = "Channel"; Value = $Channel }
            $results += @{ Property = "Partnum"; Value = $Partnum }
            $results += @{ Property = "IID";     Value = $IID }
            return $results
        }
    } catch {
        if ($AsObject) {
            return (
                [PSObject]@{
                    Error = "$($_.Exception.Message)"
                }
            )
        } else {
            return (
                @{ Property = "Error"; Value = "$($_.Exception.Message)" }
            )
        }
    }
}
// >>>>>>>>>>
__int64 __fastcall GetPKeyData(
        wchar_t *Str,
        __int64 a2,
        const wchar_t *a3,
        __int64 a4,
        __int64 a5,
        _QWORD *a6,
        _QWORD *a7,
        _QWORD *a8,
        _QWORD *a9,
        __int64 a10)
{
  _OWORD *v12; // rbx
  void *v13; // rdi
  int v14; // ecx
  unsigned int v15; // r14d
  int v16; // eax
  const wchar_t *v17; // rcx
  __int64 v18; // rax
  int v19; // r10d
  unsigned int v20; // edx
  HANDLE ProcessHeap; // rax
  _OWORD *v22; // rax
  __int64 v23; // rdx
  __int64 v24; // r9
  int v25; // eax
  LPVOID v26; // rax
  LPVOID v27; // rax
  LPVOID v28; // rax
  LPVOID v29; // rax
  LPVOID v30; // rdx
  void *v31; // rsi
  HANDLE v32; // rax
  void *v33; // rsi
  HANDLE v34; // rax
  void *v35; // rsi
  HANDLE v36; // rax
  void *v37; // rsi
  HANDLE v38; // rax
  HANDLE v39; // rax
  HANDLE v40; // rax
  void *v41; // rbx
  HANDLE v42; // rax
  void *v43; // rbx
  HANDLE v44; // rax
  LPVOID lpMem; // [rsp+48h] [rbp-49h] BYREF
  LPVOID v47; // [rsp+50h] [rbp-41h] BYREF
  int v48; // [rsp+58h] [rbp-39h]
  LPVOID v49; // [rsp+60h] [rbp-31h] BYREF
  __int64 v50; // [rsp+68h] [rbp-29h] BYREF
  LPVOID v51; // [rsp+70h] [rbp-21h] BYREF
  LPVOID v52; // [rsp+78h] [rbp-19h] BYREF
  LPVOID v53; // [rsp+80h] [rbp-11h] BYREF
  __int64 v54[2]; // [rsp+88h] [rbp-9h] BYREF

  v54[0] = 0i64;
  v12 = 0i64;
  v47 = 0i64;
  v13 = 0i64;
  v50 = 0i64;
  v49 = 0i64;
  v53 = 0i64;
  v52 = 0i64;
  v51 = 0i64;
  lpMem = 0i64;
  v48 = 0;
  if ( !a6 || !a7 || !a8 || !a9 )
  {
LABEL_2:
    v14 = -2147024809;
    v15 = -2147024809;
LABEL_32:
    sub_1800038E8(v14);
    goto LABEL_33;
  }
  if ( a2 )
    v16 = sub_1800057E0(v54, L"%s", a2);
  else
    v16 = sub_180004D94(Str, v54);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  if ( !a3 )
  {
    v16 = sub_1800039A0(Str);
    v15 = v16;
    if ( v16 >= 0 )
    {
      v17 = L"03612";
      goto LABEL_11;
    }
LABEL_31:
    v14 = v16;
    goto LABEL_32;
  }
  v17 = a3;
LABEL_11:
  v16 = sub_180003364(v17, &v47);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v16 = sub_180015270((__int64)&v50);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v16 = (*(__int64 (__fastcall **)(__int64, __int64, _QWORD, _QWORD, _QWORD))(*(_QWORD *)v50 + 40i64))(
          v50,
          v54[0],
          0i64,
          0i64,
          0i64);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v16 = (*(__int64 (__fastcall **)(__int64, _QWORD, wchar_t *, _QWORD, _DWORD, LPVOID *))(*(_QWORD *)v50 + 64i64))(
          v50,
          0i64,
          Str,
          0i64,
          0,
          &lpMem);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  if ( *(_DWORD *)(*((_QWORD *)lpMem + 15) + 48i64) )
  {
    v18 = *((_QWORD *)lpMem + 16);
    v19 = *(_DWORD *)(v18 + 48);
    v20 = (unsigned int)(*((_QWORD *)lpMem + 18) / 0x3E8ui64) % (*(_DWORD *)(v18 + 52) - v19 + 1);
    *((_DWORD *)lpMem + 6) = v19 + v20;
    *((_DWORD *)lpMem + 16) = (v19 + v20) % 0xF4240;
    *((_DWORD *)lpMem + 15) = *((_DWORD *)lpMem + 6) / 0xF4240u;
  }
  ProcessHeap = GetProcessHeap();
  v12 = HeapAlloc(ProcessHeap, 0, 0x58ui64);
  if ( !v12 )
    goto LABEL_2;
  v22 = lpMem;
  *v12 = *((_OWORD *)lpMem + 2);
  v12[1] = v22[3];
  v12[2] = v22[4];
  v12[3] = v22[5];
  v12[4] = v22[6];
  *((_QWORD *)v12 + 10) = *((_QWORD *)v22 + 14);
  v16 = sub_180003364(*(_QWORD *)(*((_QWORD *)lpMem + 15) + 24i64), &v53);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v16 = sub_180003364(*(_QWORD *)(*((_QWORD *)lpMem + 15) + 40i64), &v52);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v16 = sub_180003364(*(_QWORD *)(*((_QWORD *)lpMem + 16) + 24i64), &v51);
  v15 = v16;
  if ( v16 < 0 )
    goto LABEL_31;
  v25 = sub_180006A94((__int64)v12 + 8, v23, a5, v24, &v49);
  ....
}
// >>>>>>>>>>
PS C:\Users\Administrator> Get-PKeyData -key K8KNG-MGG4H-KX82M-M8QYW-DGRFH -configPath "C:\windows\System32\spp\tokens\pkeyconfig\pkeyconfig.xrm-ms"

Name                           Value                                                                                                                                           
----                           -----                                                                                                                                           
Value                          IoTEnterpriseS                                                                                                                                  
Property                       Edition                                                                                                                                         
Value                          OEM:NONSLP                                                                                                                                      
Property                       Channel                                                                                                                                         
Value                          [Vb]X21-99682                                                                                                                                   
Property                       Partnum                                                                                                                                         
Value                          425068091656465646046967647235135916414204848077385098147320802                                                                                 
Property                       IID                                                                                                                                             
// >>>>>>>>>>
__int64 __fastcall sub_180006A94(
    __int64 a1,      // [In]  Pointer to Internal Data
    __int64 a2,      // [In]  Unused/Reserved (usually 0 or v23)
    __int64 a3,      // [In]  Hardware ID (HWID) or Seed
    __int64 a4,      // [In]  Reserved/Flags
    HLOCAL *a5       // [Out] Pointer to the generated IID string buffer
);
// >>>>>>>>>>
__int64 __fastcall GetPKeyData(.. .. .. .. ..) {
// ..........
v50 = 0i64;
v16 = sub_18000D160(v51, 0i64, Str, 0i64, 0, &lpMem); --> { return sub_180009A24(a1 + 16, a2, a3, a4, a5, a6); }
// ..........
if ( *(_DWORD *)(*((_QWORD *)lpMem + 15) + 48i64) )
{
  18 = *((_QWORD *)lpMem + 16);
  v19 = *(_DWORD *)(v18 + 48);
  v20 = (unsigned int)(*((_QWORD *)lpMem + 18) / 0x3E8ui64) % (*(_DWORD *)(v18 + 52) - v19 + 1);
  *((_DWORD *)lpMem + 6) = v19 + v20;
  *((_DWORD *)lpMem + 16) = (v19 + v20) % 0xF4240;
  *((_DWORD *)lpMem + 15) = *((_DWORD *)lpMem + 6) / 0xF4240u;
}
ProcessHeap = GetProcessHeap();
v12 = HeapAlloc(ProcessHeap, 0, 0x58ui64);
if ( !v12 )
  goto LABEL_2;
v22 = lpMem;
*v12 = *((_OWORD *)lpMem + 2);
v12[1] = v22[3];
v12[2] = v22[4];
v12[3] = v22[5];
v12[4] = v22[6];
*((_QWORD *)v12 + 10) = *((_QWORD *)v22 + 14);
// ..........
v23 = sub_180006A94((__int64)v12 + 8, a2, a5, a4, &v50);
// end of function 
}
// >>>>>>>>>>>
[+] PID Decoder Success
---------------------------
Group ID : 4365
Serial   : 99
Security : 120
---------------------------

MSFT_PKEY_DATA Context Hex Dump:
0000: 00 00 00 00 00 00 00 00 EF 72 06 66 09 78 FD 4C
0010: 8D 54 41 B7 FB 73 89 88 0D 11 00 00 00 00 00 00
0020: 63 00 00 00 00 00 00 00 78 00 00 00 00 00 00 00
0030: 01 00 00 00 01 00 00 00 0D 11 30 06 00 00 E0 01
0040: 00 00 00 00 00 83 09 00 C6 00 00 80 86 08 C0 03
0050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

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

// Size: 0x98 (152 bytes) // lpMem
typedef struct _PID_OBJ {
    uint64_t       RefCount;          // 0x00
    wchar_t*       ProductIdStr;      // 0x08
    FILETIME       ValidationTime;    // 0x10
    uint32_t       DataIdSequence;    // 0x18
    uint32_t       AlignmentPadding;  // 0x1C
    MSFT_PKEY_DATA Data;              // 0x20 - 0x78
    void*          pPKeyConfig;       // 0x78
    void*          pKeyBits;          // 0x80
    void*          pMetadata;         // 0x88
    uint8_t        RandomSeed[8];     // 0x90
} PID_OBJ;
// >>>>>>>>>>>
/**
 * @param a1 [In]  - Context: Session state and memory vtable (v51 + 0x10).
 * @param a2 [In]  - KeyStr: 25-character Product Key string.
 * @param a3 [In]  - CfgPath: Path to pkeyconfig.xrm-ms (XML license definitions).
 * @param a4 [In]  - Flags: Pointer to structure (Type, DataPtr, Size, Options).
 * @param a5 [In]  - Mode: Validation strictness (0 = standard, 1 = legacy).
 * @param a6 [Out] - Dest: Buffer for validated license metadata (PID4 structure).
 * @return HRESULT - 0 (S_OK) on success; PidGenX error code on failure.
 */
__int64 __fastcall sub_180009A24(__int64 a1, const wchar_t *a2, __int64 a3, __int64 a4, int a5, __int64 a6)
{
	// ....
	v11 = a2;
	v33 = a3;
	v31 = 0i64;
	v9 = 0i64;
	v8 = 0;
	
	/**
	 * @param a1 [In]  - Context: Session/State handle (v51 + 0x10).
	 * @param v11[In]  - KeyStr: Original 25-character Product Key string.
	 * @param v33[In]  - CfgPath: Path to pkeyconfig.xrm-ms (defines product ranges).
	 * @param v31[In]  - BinData: Optional; pointer to raw binary key data (if not using string).
	 * @param v9 [In]  - ValidBits: The verified/decoded key bits from sub_180009A24.
	 * @param v8 [In]  - BitSize: Size (in bytes) of the ValidBits buffer.
	 * @param a5 [In]  - Mode: PID generation flags (determines PID 3.0 vs 4.0 format).
	 * @param a6 [Out] - Dest: Pointer to the final generated Product ID (PID) structure.
	 */
	v18 = sub_180009338(a1, v11, v33, v31, v9, v8, a5, a6);
	// ....
}
// [IN] A1 & A2 (v75), [OUT] A4
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
  int v18; // ecx
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
  v15 = sub_1800090B0(a3, v75);
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
      v18 = -2147217374;
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
          v18 = -2147217375;
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
    sub_1800038E8(-2147217375);
LABEL_50:
    sub_180003B30((unsigned int)v17);
    if ( v17 < 0 )
      sub_1800038E8(v17);
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
        sub_1800038E8(-2147024362);
      }
      sub_180003B30((unsigned int)v54);
      v17 = v54;
      if ( v54 < 0 )
      {
        v18 = v54;
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
    v18 = -2147217370;
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
      v18 = v17;
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
        sub_1800038E8(-2147024362);
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
    v18 = -2147024882;
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
    v18 = v19;
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
    sub_180012E7C(v74);
  return (unsigned int)v17;
}
__int64 __fastcall sub_180014730(__int64 a1, __int64 a2, __int64 a3, _QWORD *a4)
{
  int v5; // r14d
  int v9; // ebp
  void *v10; // rbx
  int v11; // edx
  int v12; // eax
  unsigned int v13; // edi
  int v14; // ecx
  __int64 v15; // rdi
  __int64 v16; // rbp
  HANDLE ProcessHeap; // rax
  char *v18; // rax
  char *v19; // rsi
  HANDLE v20; // rax
  char v22; // [rsp+70h] [rbp+8h] BYREF
  int v23; // [rsp+78h] [rbp+10h] BYREF

  v5 = a1 + 144;
  v9 = *(_DWORD *)(a2 + 32) + 1000000 * *(_DWORD *)(a2 + 28);
  v10 = 0i64;
  v11 = *(_DWORD *)(a3 + 4);
  v23 = 0;
  v22 = 0;
  v12 = sub_18000EC1C((int)a1 + 144, v11, 0, v9, (__int64)&v22, (__int64)&v23);
  v13 = v12;
  if ( v12 < 0 )
    goto LABEL_2;
  if ( v22 )
  {
    v13 = -2147217371;
LABEL_14:
    v14 = v13;
    goto LABEL_15;
  }
  v12 = sub_18000EC1C(v5, *(_DWORD *)(a3 + 4), 1, v9, (__int64)&v22, (__int64)&v23);
  v13 = v12;
  if ( v12 < 0 )
    goto LABEL_2;
  if ( !v22 )
  {
    v13 = -2147217372;
    goto LABEL_14;
  }
  v15 = *(_QWORD *)(*(_QWORD *)(a1 + 152) + 8i64 * v23);
  if ( !(unsigned __int8)sub_18000E7E8(a1 + 112, v15 + 4, &v23) )
  {
    v13 = -2147418113;
    goto LABEL_14;
  }
  v16 = *(_QWORD *)(*(_QWORD *)(a1 + 120) + 8i64 * v23);
  ProcessHeap = GetProcessHeap();
  v18 = (char *)HeapAlloc(ProcessHeap, 0, 0x98ui64);
  v19 = v18;
  if ( !v18 )
  {
    v13 = -2147024882;
    goto LABEL_14;
  }
  *(_QWORD *)v18 = 1i64;
  *((_OWORD *)v18 + 1) = 0i64;
  *((_QWORD *)v18 + 18) = 0i64;
  *((_QWORD *)v18 + 1) = 0i64;
  memset(v18 + 32, 0, 0x58ui64);
  *(_OWORD *)(v19 + 40) = 0i64;
  *(_OWORD *)(v19 + 56) = 0i64;
  *((_QWORD *)v19 + 9) = 0i64;
  *((_QWORD *)v19 + 15) = 0i64;
  v10 = v19;
  *((_QWORD *)v19 + 16) = 0i64;
  *((_QWORD *)v19 + 17) = 0i64;
  v12 = sub_180015728(v19, a2, v16, v15, a3);
  v13 = v12;
  if ( v12 < 0 )
  {
LABEL_2:
    v14 = v12;
LABEL_15:
    sub_1800038E8(v14);
    goto LABEL_16;
  }
  v10 = 0i64;
  *a4 = v19;
LABEL_16:
  sub_180003B30(v13);
  if ( v10 && _InterlockedExchangeAdd((volatile signed __int32 *)v10, 0xFFFFFFFF) == 1 )
  {
    sub_180003038(v10);
    v20 = GetProcessHeap();
    HeapFree(v20, 0, v10);
  }
  return v13;
}
__int64 __fastcall sub_180015728(
        __int64 a1,
        __int64 a2,
        __int64 a3,
        volatile signed __int32 *a4,
        const wchar_t **lpMem)
{
  void *v7; // rsi
  volatile signed __int32 *v8; // rdi
  const wchar_t **v9; // rbx
  unsigned int v10; // r15d
  int v11; // eax
  __int64 v12; // r8
  __int64 v13; // rcx
  int v14; // eax
  int v15; // ecx
  signed int LastError; // eax
  signed int v17; // eax
  struct _FILETIME v18; // rax
  __int64 v19; // rdi
  __int64 v20; // rbx
  HANDLE ProcessHeap; // rax
  void *v22; // rbx
  HANDLE v23; // rax
  void *v24; // rbx
  HANDLE v25; // rax
  void *v26; // r12
  HANDLE v27; // rax
  void *v28; // r14
  HANDLE v29; // rax
  void *v30; // r14
  HANDLE v31; // rax
  HANDLE v32; // rax
  HANDLE v33; // rax
  HANDLE v34; // rax
  RPC_WSTR StringUuid; // [rsp+20h] [rbp-51h] BYREF
  __int64 v37; // [rsp+28h] [rbp-49h] BYREF
  __int64 v38; // [rsp+30h] [rbp-41h] BYREF
  struct _FILETIME FileTime; // [rsp+38h] [rbp-39h] BYREF
  __int64 v40; // [rsp+40h] [rbp-31h]
  volatile signed __int32 *v41; // [rsp+48h] [rbp-29h]
  const wchar_t **v42; // [rsp+50h] [rbp-21h]
  RPC_WSTR String; // [rsp+58h] [rbp-19h] BYREF
  struct _SYSTEMTIME SystemTime; // [rsp+60h] [rbp-11h] BYREF

  v42 = lpMem;
  v38 = 0i64;
  v37 = 0i64;
  FileTime = 0i64;
  v7 = (void *)a3;
  SystemTime = 0i64;
  StringUuid = 0i64;
  v41 = a4;
  v40 = a3;
  _InterlockedIncrement((volatile signed __int32 *)a3);
  v8 = a4;
  _InterlockedIncrement(a4);
  v9 = lpMem;
  _InterlockedIncrement((volatile signed __int32 *)lpMem);
  if ( UuidToStringW((const UUID *)(a3 + 4), &StringUuid) )
  {
    v10 = -2147217375;
    sub_1800038E8(-2147217375);
    goto LABEL_40;
  }
  v11 = wcscmp(lpMem[1], L"msft:rm/algorithm/pkey/2009");
  v13 = a2 + 72;
  if ( v11 )
  {
    v14 = sub_1800153FC(v13, 12i64, v12, &v37);
    v10 = v14;
    if ( v14 < 0 )
      goto LABEL_5;
    v14 = sub_1800057E0(&v38, L"msft2005:%s&%s", StringUuid, v37);
  }
  else
  {
    v14 = sub_1800153FC(v13, 13i64, v12, &v37);
    v10 = v14;
    if ( v14 < 0 )
    {
LABEL_5:
      v15 = v14;
LABEL_6:
      sub_1800038E8(v15);
      goto LABEL_40;
    }
    v14 = sub_1800057E0(&v38, L"msft2009:%s&%s", StringUuid, v37);
  }
  v10 = v14;
  if ( v14 < 0 )
    goto LABEL_5;
  GetLocalTime(&SystemTime);
  if ( !SystemTimeToFileTime(&SystemTime, &FileTime) )
  {
    LastError = GetLastError();
    v10 = LastError;
    if ( LastError )
    {
      if ( LastError > 0 )
        v10 = (unsigned __int16)LastError | 0x80070000;
    }
    else
    {
      v10 = -2147467259;
    }
    goto LABEL_16;
  }
  v10 = 0;
  if ( dword_1800AFE08 == 1 )
  {
    if ( CryptGenRandom(hProv, 8u, (BYTE *)(a1 + 144)) )
      goto LABEL_25;
    v17 = GetLastError();
    v10 = v17;
    if ( v17 )
    {
      if ( v17 > 0 )
        v10 = (unsigned __int16)v17 | 0x80070000;
    }
    else
    {
      v10 = -2147467259;
    }
  }
  else
  {
    v10 = -2147418113;
  }
  sub_1800038E8(v10);
LABEL_25:
  sub_180003B30(v10);
  if ( (v10 & 0x80000000) != 0 )
  {
LABEL_16:
    v15 = v10;
    goto LABEL_6;
  }
  v18 = FileTime;
  v19 = v38;
  *(_OWORD *)(a1 + 32) = *(_OWORD *)a2;
  v38 = 0i64;
  *(_OWORD *)(a1 + 48) = *(_OWORD *)(a2 + 16);
  *(_OWORD *)(a1 + 64) = *(_OWORD *)(a2 + 32);
  *(_OWORD *)(a1 + 80) = *(_OWORD *)(a2 + 48);
  *(_OWORD *)(a1 + 96) = *(_OWORD *)(a2 + 64);
  *(_QWORD *)(a1 + 112) = *(_QWORD *)(a2 + 80);
  *(struct _FILETIME *)(a1 + 16) = v18;
  *(_DWORD *)(a1 + 24) = *(_DWORD *)(a2 + 32) + 1000000 * *(_DWORD *)(a2 + 28);
  v20 = *(_QWORD *)(a1 + 8);
  if ( v20 )
  {
    ProcessHeap = GetProcessHeap();
    HeapFree(ProcessHeap, 0, (LPVOID)(v20 - 4));
    sub_180003B30(0i64);
  }
  v7 = 0i64;
  if ( !v19 )
    v19 = 0i64;
  *(_QWORD *)(a1 + 8) = v19;
  v22 = *(void **)(a1 + 120);
  if ( v22 && !_InterlockedDecrement((volatile signed __int32 *)v22) )
  {
    sub_180003160(v22);
    v23 = GetProcessHeap();
    HeapFree(v23, 0, v22);
  }
  v8 = 0i64;
  *(_QWORD *)(a1 + 120) = v40;
  v24 = *(void **)(a1 + 128);
  if ( v24 && !_InterlockedDecrement((volatile signed __int32 *)v24) )
  {
    sub_1800032D8(v24);
    v25 = GetProcessHeap();
    HeapFree(v25, 0, v24);
  }
  v9 = 0i64;
  *(_QWORD *)(a1 + 128) = v41;
  v26 = *(void **)(a1 + 136);
  if ( v26 && !_InterlockedDecrement((volatile signed __int32 *)v26) )
  {
    sub_180003224(v26);
    v27 = GetProcessHeap();
    HeapFree(v27, 0, v26);
  }
  *(_QWORD *)(a1 + 136) = v42;
LABEL_40:
  sub_180003B30(v10);
  if ( StringUuid )
  {
    String = StringUuid;
    RpcStringFreeW(&String);
    StringUuid = 0i64;
  }
  if ( v37 )
  {
    v28 = (void *)(v37 - 4);
    v29 = GetProcessHeap();
    HeapFree(v29, 0, v28);
    sub_180003B30(0i64);
  }
  if ( v38 )
  {
    v30 = (void *)(v38 - 4);
    v31 = GetProcessHeap();
    HeapFree(v31, 0, v30);
    sub_180003B30(0i64);
  }
  if ( v9 && !_InterlockedDecrement((volatile signed __int32 *)v9) )
  {
    sub_180003224(v9);
    v32 = GetProcessHeap();
    HeapFree(v32, 0, v9);
  }
  if ( v8 && !_InterlockedDecrement(v8) )
  {
    sub_1800032D8(v8);
    v33 = GetProcessHeap();
    HeapFree(v33, 0, (LPVOID)v8);
  }
  if ( v7 && !_InterlockedDecrement((volatile signed __int32 *)v7) )
  {
    sub_180003160(v7);
    v34 = GetProcessHeap();
    HeapFree(v34, 0, v7);
  }
  return v10;
}
// >>>>>>>>>>
__int64 __fastcall sub_180006A94(__int64 a1, __int64 a2, __int64 a3, __int64 a4, HLOCAL *a5)
{
  HLOCAL v5; // rbx
  int v6; // ecx
  unsigned int v7; // edi
  __m128i v8; // xmm2
  __int128 v9; // xmm0
  __int64 v10; // xmm1_8
  int v11; // ecx
  int v12; // edx
  int v13; // eax
  HLOCAL hMem; // [rsp+20h] [rbp-40h] BYREF
  __int128 v16[2]; // [rsp+28h] [rbp-38h] BYREF
  __int64 v17; // [rsp+48h] [rbp-18h]

  v5 = 0i64;
  hMem = 0i64;
  if ( !a1 || !a5 )
  {
    v6 = -2147024809;
    v7 = -2147024809;
LABEL_18:
    sub_1800038E8(v6);
    goto LABEL_19;
  }
  v8 = *(__m128i *)a1;
  v9 = *(_OWORD *)(a1 + 16);
  v10 = *(_QWORD *)(a1 + 32);
  v11 = *(_DWORD *)(a1 + 8);
  v12 = _mm_cvtsi128_si32(v8);
  v16[0] = (__int128)v8;
  v17 = v10;
  v16[1] = v9;
  if ( v12 == -1200417387
    && v8.m128i_i32[1] == 1124836086
    && v11 == -288345
    && HIDWORD(_mm_srli_si128(v8, 8).m128i_u64[0]) == 1938627256 )
  {
    v13 = sub_1800065DC(v16, a3, a3, &hMem);
    goto LABEL_9;
  }
  if ( v12 != 1711698671
    || _mm_cvtsi128_si32(_mm_srli_si128(v8, 4)) != 1291679753
    || v11 != -1220455283
    || _mm_cvtsi128_si32(_mm_srli_si128(v8, 12)) != -2004257797 )
  {
    v7 = -2147467263;
    v6 = -2147467263;
    goto LABEL_18;
  }
  v13 = sub_180006738(v16, a3, a3, &hMem);
LABEL_9:
  v7 = v13;
  if ( v13 >= 0 )
  {
    *a5 = hMem;
  }
  else
  {
    sub_1800038E8(v13);
    v5 = hMem;
  }
LABEL_19:
  sub_180003B30(v7);
  if ( v5 )
    LocalFree(v5);
  return v7;
}
// >>>>>>>>>>
__int64 __fastcall sub_180006394(unsigned int a1, _BYTE *a2, __int64 a3, int a4)
{
  _BYTE *v4; // rbx
  unsigned int v5; // r14d
  _BYTE *v6; // rsi
  HLOCAL v7; // rbp
  SIZE_T v8; // r12
  _BYTE *v9; // rdi
  int v10; // ecx
  unsigned int v11; // r15d
  _BYTE *v12; // r15
  _BYTE *v13; // rcx
  SIZE_T v14; // rdx
  unsigned int v15; // edx
  _BYTE *v16; // r8
  __int64 v17; // rax
  unsigned int v18; // r13d
  int v19; // eax
  int v20; // eax
  signed __int64 v21; // rdx
  signed __int64 v22; // r8
  SIZE_T v23; // r10
  _BYTE *v24; // rcx
  unsigned int v25; // edx
  _BYTE *v26; // r8
  __int64 v27; // rcx
  int v29; // [rsp+20h] [rbp-68h]

  v4 = 0i64;
  v5 = a1 >> 1;
  v6 = 0i64;
  v7 = 0i64;
  v8 = a1 >> 1;
  v9 = LocalAlloc(0x40u, v8);
  if ( v9
    && (v4 = LocalAlloc(0x40u, v8)) != 0i64
    && (v6 = LocalAlloc(0x40u, v8)) != 0i64
    && (v12 = v9, (v7 = LocalAlloc(0x40u, (v5 + 3) & 0xFFFFFFFC)) != 0i64) )
  {
    if ( v5 )
    {
      v14 = v8;
      v13 = (_BYTE *)(a2 - v9);
      do
      {
        *v12 = v12[(_QWORD)v13];
        ++v12;
        --v14;
      }
      while ( v14 );
    }
    v15 = v5;
    if ( v5 < 2 * v5 )
    {
      v16 = &a2[v8];
      do
      {
        LOBYTE(v13) = *v16;
        v17 = v15 - v5;
        ++v15;
        ++v16;
        v4[v17] = (_BYTE)v13;
      }
      while ( v15 < 2 * v5 );
    }
    v18 = 0;
    v19 = 8 * v5;
    do
    {
      v20 = sub_180006BEC((int)v13, (int)v4, v5, a4, v29, v7, v19);
      v11 = v20;
      if ( v20 < 0 )
      {
        v10 = v20;
        goto LABEL_25;
      }
      if ( v5 )
      {
        v21 = v6 - v4;
        v13 = v4;
        v22 = v9 - v4;
        v23 = v8;
        do
        {
          v13[v21] = *v13;
          *v13 = v13[v22] ^ v13[(_BYTE *)v7 - v4];
          v13[v22] = v13[v21];
          ++v13;
          --v23;
        }
        while ( v23 );
      }
      a4 += 4;
      v19 = 8 * v5;
      ++v18;
    }
    while ( v18 < 0x10 );
    if ( v5 )
    {
      v24 = a2;
      do
      {
        *v24 = v24[v9 - a2];
        ++v24;
        --v8;
      }
      while ( v8 );
      v25 = 0;
      v26 = v4;
      do
      {
        v27 = v25 + v5;
        ++v25;
        a2[v27] = *v26++;
      }
      while ( v25 < v5 );
    }
  }
  else
  {
    v10 = -2147024882;
    v11 = -2147024882;
LABEL_25:
    sub_1800038E8(v10);
  }
  sub_180003B30(v11);
  if ( v7 )
    LocalFree(v7);
  if ( v6 )
    LocalFree(v6);
  if ( v4 )
    LocalFree(v4);
  if ( v9 )
    LocalFree(v9);
  return v11;
}
__int64 __fastcall sub_180006BEC(__int64 a1, __int64 a2, unsigned int a3, int *a4, int a5, void *a6, int a7)
{
  unsigned int v10; // ebx
  unsigned int v11; // edi
  char v13[16]; // [rsp+20h] [rbp-81h] BYREF
  int v14[24]; // [rsp+30h] [rbp-71h] BYREF
  int Src[6]; // [rsp+90h] [rbp-11h] BYREF

  v13[0] = 121;
  memset(v14, 0, sizeof(v14));
  v10 = 0;
  if ( a7 && (v11 = (unsigned int)(a7 + 31) >> 5, 4 * v11 <= 0x14) )
  {
    v14[16] = 1732584193;
    v14[17] = -271733879;
    v14[18] = -1732584194;
    v14[19] = 271733878;
    v14[20] = -1009589776;
    sub_180015D20(v14, v13, 1i64);
    sub_180015D20(v14, a2, a3);
    Src[0] = *a4;
    sub_180015D20(v14, Src, 4i64);
    sub_180015C00(v14, Src);
    memcpy(a6, Src, 4i64 * v11);
    *((_DWORD *)a6 + v11 - 1) >>= 32 * v11 - a7;
  }
  else
  {
    v10 = -2147024809;
    sub_1800038E8(-2147024809);
  }
  sub_180003B30(v10);
  return v10;
}
__int64 __fastcall sub_1800065DC(_DWORD *a1, __int64 a2, __int64 a3, _QWORD *a4)
{
  int v4; // r8d
  int v5; // eax
  unsigned __int64 v7; // r8
  void *v8; // rdi
  int v9; // eax
  unsigned int v10; // esi
  int v11; // eax
  void *v13; // [rsp+28h] [rbp-28h] BYREF
  __int64 Src[3]; // [rsp+30h] [rbp-20h] BYREF

  v4 = a1[4];
  v5 = a1[8];
  Src[1] = a2;
  v7 = (a1[7] != 0 ? 257i64 : 1i64) | (((a1[6] + 1000000 * a1[5]) & 0x3FFFFFFF | ((v5 & 0x3FF | ((unsigned __int64)(v4 & 0x3FF) << 10)) << 30)) << 9);
  Src[2] = 0i64;
  v8 = 0i64;
  Src[0] = v7;
  v13 = 0i64;
  v9 = sub_180006308(19i64, Src);
  v10 = v9;
  if ( v9 < 0 || (v9 = sub_1800060F4(Src), v10 = v9, v9 < 0) )
  {
    sub_1800038E8(v9);
  }
  else
  {
    v11 = sub_1800059C0(0i64, &v13);
    v10 = v11;
    if ( v11 >= 0 )
    {
      *a4 = v13;
    }
    else
    {
      sub_1800038E8(v11);
      v8 = v13;
    }
  }
  sub_180003B30(v10);
  if ( v8 )
    LocalFree(v8);
  if ( (v10 & 0x80000000) != 0 )
    sub_1800038E8(v10);
  sub_180003B30(v10);
  return v10;
}
__int64 __fastcall sub_180006738(__int64 a1, __int64 a2, __int64 a3, _QWORD *a4)
{
  unsigned int v5; // ebx
  unsigned __int8 v6; // dl
  __int64 v7; // r10
  __int16 v9; // ax
  __int64 v10; // r14
  __int64 v11; // r9
  _BYTE *v12; // r8
  char v13; // cl
  _BYTE *v14; // rdx
  __int64 v15; // r10
  unsigned __int8 v16; // cl
  int v17; // esi
  int v18; // ecx
  __int64 v19; // r9
  _BYTE *v20; // r8
  __int64 v21; // r10
  unsigned __int8 v22; // dl
  char v23; // al
  _BYTE *v24; // r8
  __int64 v25; // r10
  __int64 v26; // r9
  unsigned __int8 v27; // dl
  char v28; // al
  __int16 *v29; // r8
  __int64 v30; // r9
  unsigned __int8 v31; // dl
  char v32; // al
  __int64 v33; // rbx
  int v34; // eax
  int v35; // ecx
  _BYTE *v36; // r8
  __int64 v37; // r9
  unsigned __int8 v38; // dl
  char v39; // al
  int v40; // eax
  HLOCAL v41; // rdi
  int v42; // eax
  __int64 v44; // [rsp+20h] [rbp-50h] BYREF
  int v45[2]; // [rsp+28h] [rbp-48h]
  HLOCAL hMem; // [rsp+30h] [rbp-40h]
  __int128 v47; // [rsp+38h] [rbp-38h] BYREF
  int v48; // [rsp+48h] [rbp-28h]
  __int16 v49; // [rsp+4Ch] [rbp-24h] BYREF
  __int128 Src; // [rsp+50h] [rbp-20h] BYREF
  int v51; // [rsp+60h] [rbp-10h]
  __int16 v52; // [rsp+64h] [rbp-Ch]
  char v53; // [rsp+66h] [rbp-Ah]
  __int64 v54; // [rsp+A8h] [rbp+38h]

  v54 = a2;
  v45[0] = 0;
  v51 = 0;
  v52 = 0;
  v53 = 0;
  v48 = 0;
  v5 = 0;
  v6 = *(_BYTE *)(a1 + 37);
  v7 = a1 + 16;
  v49 = 0;
  v9 = *(_WORD *)(a1 + 32);
  v47 = 0i64;
  LOWORD(v47) = v9;
  v10 = 2i64;
  v11 = 2i64;
  BYTE2(v47) = *(_BYTE *)(a1 + 34);
  LOBYTE(v9) = *(_BYTE *)(a1 + 35) & 0xF;
  LODWORD(v44) = 0;
  BYTE3(v47) = (16 * v6) | v9 & 0xF;
  v12 = (char *)&v47 + 6;
  v13 = *(_BYTE *)(a1 + 38) >> 4;
  BYTE4(v47) = (v6 >> 4) | (16 * *(_BYTE *)(a1 + 38));
  v14 = (char *)&v47 + 5;
  BYTE5(v47) = v13 & 1;
  v15 = v7 - ((_QWORD)&v47 + 5);
  Src = 0i64;
  do
  {
    v16 = v14[v15];
    *v14 &= 1u;
    *v12 &= ~1u;
    *v14++ |= 2 * v16;
    *v12++ |= v16 >> 7;
    --v11;
  }
  while ( v11 );
  v17 = 0;
  v18 = *(_DWORD *)(a1 + 20);
  BYTE7(v47) ^= (BYTE7(v47) ^ (2 * *(_BYTE *)(a1 + 18))) & 0x1E;
  if ( v18 )
  {
    if ( 1000000 * v18 / 0xF4240u == v18 )
    {
      v5 = 1000000 * v18;
      LODWORD(v44) = 1000000 * v18;
    }
    else
    {
      v17 = -2147024362;
      sub_1800038E8(-2147024362);
    }
  }
  sub_180003B30((unsigned int)v17);
  if ( v17 < 0 )
    goto LABEL_32;
  if ( v5 + *(_DWORD *)(a1 + 24) < v5 )
  {
    v17 = -2147024362;
    sub_1800038E8(-2147024362);
  }
  else
  {
    LODWORD(v44) = v5 + *(_DWORD *)(a1 + 24);
    v17 = 0;
  }
  sub_180003B30((unsigned int)v17);
  if ( v17 < 0 )
    goto LABEL_32;
  v19 = 0i64;
  v20 = (char *)&v47 + 8;
  v21 = 3i64;
  do
  {
    v22 = *((_BYTE *)&v45[-2] + v19);
    v23 = *((_BYTE *)&v47 + v19 + 7);
    *v20 &= 0xE0u;
    *v20++ |= v22 >> 3;
    *((_BYTE *)&v47 + v19++ + 7) = v23 & 0x1F | (32 * v22);
    --v21;
  }
  while ( v21 );
  v24 = (char *)&v47 + 12;
  BYTE10(v47) = (32 * BYTE3(v44)) | BYTE10(v47) & 0x1F;
  v25 = 8i64;
  BYTE11(v47) = (*(_DWORD *)(a1 + 28) != 0 ? 8 : 0) | BYTE11(v47) & 0xF0 ^ (BYTE3(v44) >> 3) & 7;
  v26 = 0i64;
  do
  {
    v27 = *((_BYTE *)&v54 + v26);
    v28 = *((_BYTE *)&v47 + v26 + 11);
    *v24 &= 0xF0u;
    *v24++ |= v27 >> 4;
    *((_BYTE *)&v47 + v26++ + 11) = v28 & 0xF | (16 * v27);
    --v25;
  }
  while ( v25 );
  v29 = &v49;
  v30 = 0i64;
  do
  {
    v31 = *((_BYTE *)v45 + v30);
    v32 = *((_BYTE *)&v48 + v30 + 3);
    *(_BYTE *)v29 &= 0xF0u;
    *(_BYTE *)v29 |= v31 >> 4;
    v29 = (__int16 *)((char *)v29 + 1);
    *((_BYTE *)&v48 + v30++ + 3) = v32 & 0xF | (16 * v31);
    --v10;
  }
  while ( v10 );
  HIBYTE(v49) &= 0xFu;
  v33 = 22i64;
  v34 = sub_180006308(22i64, &v47);
  v17 = v34;
  if ( v34 < 0 )
  {
    v35 = v34;
LABEL_33:
    sub_1800038E8(v35);
    goto LABEL_34;
  }
  v36 = (char *)&Src + 1;
  v37 = 0i64;
  do
  {
    v38 = *((_BYTE *)&v47 + v37);
    v39 = *((_BYTE *)&Src + v37);
    *v36 &= 0xF8u;
    *v36++ |= v38 >> 5;
    *((_BYTE *)&Src + v37++) = v39 & 7 | (8 * v38);
    --v33;
  }
  while ( v33 );
  hMem = 0i64;
  v44 = 0i64;
  v40 = sub_1800060F4(&Src);
  v41 = hMem;
  v17 = v40;
  if ( v40 >= 0 )
  {
    v42 = sub_180005D58(hMem, &v44);
    v17 = v42;
    if ( v42 >= 0 )
    {
      *a4 = v44;
    }
    else
    {
      sub_1800038E8(v42);
      v33 = v44;
    }
  }
  else
  {
    sub_1800038E8(v40);
  }
  sub_180003B30((unsigned int)v17);
  if ( v33 )
    LocalFree((HLOCAL)v33);
  if ( v41 )
    LocalFree(v41);
  if ( v17 < 0 )
  {
LABEL_32:
    v35 = v17;
    goto LABEL_33;
  }
LABEL_34:
  sub_180003B30((unsigned int)v17);
  return (unsigned int)v17;
}
__int64 __fastcall sub_180006308(__int64 a1, __int64 a2)
{
  unsigned int v3; // edi
  int v4; // eax
  __int64 v5; // r8
  unsigned int v6; // ebx
  char v8[64]; // [rsp+20h] [rbp-58h] BYREF

  v3 = a1;
  v4 = sub_180006E78(a1, v8);
  v6 = v4;
  if ( v4 < 0 || (v4 = sub_180006394(v3, a2, v5, v8), v6 = v4, v4 < 0) )
    sub_1800038E8(v4);
  memset(v8, 0, sizeof(v8));
  sub_180003B30(v6);
  return v6;
}
__int64 __fastcall sub_1800060F4(void *Src, int a2, int a3, _QWORD *a4)
{
  __int64 v5; // r15
  unsigned int v7; // r14d
  char *v8; // rdi
  _WORD *v9; // rbx
  double v11; // xmm6_8
  double v12; // xmm6_8
  __int64 v13; // rbp
  int v14; // esi
  int v15; // ecx
  __int64 v16; // r9
  unsigned int v17; // ecx
  char *v18; // r8
  __int64 v19; // r10
  unsigned int v20; // ecx
  _WORD *v21; // rax

  v5 = a3;
  v7 = 0;
  v8 = 0i64;
  v9 = 0i64;
  sub_180003B30(0i64);
  v11 = log10(2.0);
  v12 = v11 * (double)a2 / log10(10.0);
  v13 = (unsigned int)((int)v12 + 1);
  if ( v12 <= (double)(int)v12 )
    v13 = (unsigned int)(int)v12;
  if ( !(_DWORD)v13 )
  {
    v14 = -2147024809;
LABEL_5:
    v15 = v14;
LABEL_6:
    sub_1800038E8(v15);
    goto LABEL_28;
  }
  if ( (int)v13 + 1 < (unsigned int)v13 )
  {
    v14 = -2147024362;
    sub_1800038E8(-2147024362);
  }
  else
  {
    v7 = v13 + 1;
    v14 = 0;
  }
  sub_180003B30((unsigned int)v14);
  if ( v14 < 0 )
    goto LABEL_5;
  v14 = 0;
  if ( v7 )
  {
    if ( (2 * v7) >> 1 == v7 )
    {
      v7 *= 2;
    }
    else
    {
      v14 = -2147024362;
      sub_1800038E8(-2147024362);
    }
  }
  else
  {
    v7 = 0;
  }
  sub_180003B30((unsigned int)v14);
  if ( v14 < 0 )
    goto LABEL_5;
  v9 = LocalAlloc(0x40u, v7);
  if ( !v9 )
  {
    v9 = 0i64;
LABEL_19:
    v15 = -2147024882;
    v14 = -2147024882;
    goto LABEL_6;
  }
  v8 = (char *)LocalAlloc(0x40u, (unsigned int)v5);
  if ( !v8 )
  {
    v8 = 0i64;
    goto LABEL_19;
  }
  memcpy(v8, Src, (unsigned int)v5);
  v16 = v5 - 1;
  v9[v13] = 0;
  do
  {
    v13 = (unsigned int)(v13 - 1);
    v17 = 0;
    if ( v16 >= 0 )
    {
      v18 = &v8[v16];
      v19 = v5;
      do
      {
        v20 = (unsigned __int8)*v18 + (v17 << 8);
        *v18-- = v20 / 0xA;
        v17 = v20 % 0xA;
        --v19;
      }
      while ( v19 );
    }
    v9[v13] = a0123456789[v17];
  }
  while ( (_DWORD)v13 );
  v21 = v9;
  v9 = 0i64;
  *a4 = v21;
LABEL_28:
  sub_180003B30((unsigned int)v14);
  if ( v9 )
    LocalFree(v9);
  if ( v8 )
    LocalFree(v8);
  return (unsigned int)v14;
}
__int64 __fastcall sub_180006E78(__int64 a1, _OWORD *a2)
{
  __int128 v3; // [rsp+20h] [rbp-40h]
  __int128 v4; // [rsp+30h] [rbp-30h]
  __int128 v5; // [rsp+40h] [rbp-20h]
  __int128 v6; // [rsp+50h] [rbp-10h]

  *(_QWORD *)&v3 = 0x84D8F8F0D45EC86Bui64;
  *((_QWORD *)&v3 + 1) = 0xF413937D2F2A4177ui64;
  *a2 = v3;
  *(_QWORD *)&v4 = 0xBB9515A2E6668A1Bui64;
  *((_QWORD *)&v4 + 1) = 0x972B328367B09D0Eui64;
  a2[1] = v4;
  *(_QWORD *)&v5 = 0xEEDC7D7CCDD9FE49ui64;
  *((_QWORD *)&v5 + 1) = 0xEB3B0BE7DF1207B0ui64;
  *(_QWORD *)&v6 = 0xCFA627FDDF98BD56ui64;
  *((_QWORD *)&v6 + 1) = 0x573A73F8C236845Di64;
  a2[2] = v5;
  a2[3] = v6;
  sub_180003B30(0i64);
  return 0i64;
}