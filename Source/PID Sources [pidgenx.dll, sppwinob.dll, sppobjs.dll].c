/* Tsforge .. Source .. */

public string GetPid2()
{
	if (pid2 != null)
	{
		return pid2;
	}

	pid2 = "";

	if (Algorithm == PKeyAlgorithm.PKEY2005)
	{
		string mpc = GetMPC();
		string serialHigh;
		int serialLow;
		int lastPart;

		if (EulaType == "OEM")
		{
			serialHigh = "OEM";
			serialLow = ((Group / 2) % 100) * 10000 + (Serial / 100000);
			lastPart = Serial % 100000;
		}
		else
		{
			serialHigh = (Serial / 1000000).ToString("D3");
			serialLow = Serial % 1000000;
			lastPart = ((Group / 2) % 100) * 1000 + new Random().Next(1000);
		}

		int checksum = 0;

		foreach (char c in serialLow.ToString())
		{
			checksum += int.Parse(c.ToString());
		}
		checksum = 7 - (checksum % 7);

		pid2 = string.Format("{0}-{1}-{2:D6}{3}-{4:D5}", mpc, serialHigh, serialLow, checksum, lastPart);
	}

	return pid2;
}

public byte[] GetPid3()
{
	BinaryWriter writer = new BinaryWriter(new MemoryStream());
	writer.Write(0xA4);
	writer.Write(0x3);
	writer.WriteFixedString(GetPid2(), 24);
	writer.Write(Group);
	writer.WriteFixedString(PartNumber, 16);
	writer.WritePadding(0x6C);
	byte[] data = writer.GetBytes();
	byte[] crc = BitConverter.GetBytes(~Utils.CRC32(data.Reverse().ToArray())).Reverse().ToArray();
	writer.Write(crc);

	return writer.GetBytes();
}

public byte[] GetPid4()
{
	BinaryWriter writer = new BinaryWriter(new MemoryStream());
	writer.Write(0x4F8);
	writer.Write(0x4);
	writer.WriteFixedString16(GetExtendedPid(), 0x80);
	writer.WriteFixedString16(ActivationId.ToString(), 0x80);
	writer.WritePadding(0x10);
	writer.WriteFixedString16(Edition, 0x208);
	writer.Write(Upgrade ? (ulong)1 : 0);
	writer.WritePadding(0x50);
	writer.WriteFixedString16(PartNumber, 0x80);
	writer.WriteFixedString16(Channel, 0x80);
	writer.WriteFixedString16(EulaType, 0x80);

	return writer.GetBytes();
}

public string GetExtendedPid()
{
	string mpc = GetMPC();
	int serialHigh = Serial / 1000000;
	int serialLow = Serial % 1000000;
	int licenseType;
	uint lcid = Utils.GetSystemDefaultLCID();
	int build = Environment.OSVersion.Version.Build;
	int dayOfYear = DateTime.Now.DayOfYear;
	int year = DateTime.Now.Year;

	switch (EulaType)
	{
		case "OEM":
			licenseType = 2;
			break;

		case "Volume":
			licenseType = 3;
			break;

		default:
			licenseType = 0;
			break;
	}

	return string.Format(
		"{0}-{1:D5}-{2:D3}-{3:D6}-{4:D2}-{5:D4}-{6:D4}.0000-{7:D3}{8:D4}",
		mpc,
		Group,
		serialHigh,
		serialLow,
		licenseType,
		lcid,
		build,
		dayOfYear,
		year
	);
}

 /**
 * Origin: PidgenX.dll
 * __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryEncode(__m128i *a1)
 * Takes the String (input) and packs it into Binary (storage).
 
 __int64 __fastcall BinaryEncode(
    _WORD  *a1,   // [IN]  String (Default: "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX")
    _OWORD *a2,   // [OUT] Binary (Output: 16-byte raw data)
    __int64 a3,   // [IN]  Length (Default: 29)
    _DWORD *a4    // [OUT] N-Bit  (Output: 0 for Standard, 1 for N-Edition)
 */
 
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

/**
 * Origin: Sppobjs.dll
 * Logic: Serializes license data into the legacy 164-byte Product ID structure.
 */

// Original Name: sub_18013BA14
NTSTATUS BuildDigitalProductId3(Context* ctx, HWND hWnd, void* reserved, Pid3Buffer* output)
{
    NTSTATUS status;
    wchar_t formattedPid[48]; // Local stack buffer for the PID string

    // 1. Validation: Verify the buffer is specifically sized for PID3 (164 bytes)
    if ( output->TotalSize != 164 ) 
    {
        status = STATUS_INVALID_PARAMETER; // 0x80070057
        sub_18008A114(status);             // Log Error
        return status;
    }

    // 2. Setup: Clear data area and set Header version to 3
    memset(output->Data, 0, 156); 
    output->TotalSize = 164;
    output->Version = 3;

    // 3. String Generation: Call the Algorithm Dispatcher (Sppobjs.sub_18013BBB4)
    // This decides if the PID is modern PKEY 2009 or Legacy Mod7.
    status = sub_18013BBB4(ctx, hWnd, 0, formattedPid);
    if ( status < 0 ) goto Exit;

    // 4. Data Packing: Fill the binary slots
    // Copy the PID string (e.g., "00330-80000-...") to Offset 8
    status = sub_18013C5F8(output->PidString, 24, formattedPid);
    if ( status < 0 ) goto Exit;

    // Set Group ID from Algorithm Context
    output->GroupId = ctx->PKeyAlgorithm->GroupId; // Offset 32

    // Copy Part Number from License Properties
    status = sub_18013C5F8(output->PartNumber, 16, ctx->License->PartNumber); // Offset 36
    if ( status < 0 ) goto Exit;

    // 5. Metadata Mapping
    output->ProductKeyID = ctx->KeyGuid;           // Offset 52
    sub_18013B9D0(ctx->InstallDate, output->Date); // Offset 72 (Day/Year)
    output->KeyType = ctx->KeyTypeFlag;            // Offset 76

    // 6. Channel ID: Determine OEM (2) vs Retail (3) vs Volume (sub_18013C75C)
    status = sub_18013C75C(ctx, &output->ChannelType); // Offset 80
    if ( status < 0 ) goto Exit;

    // 7. Hardware Binding: Copy 8-byte Hardware seed
    status = sub_18013C5F8(output->HardwareID, 8, &GlobalHardwareSeed); // Offset 92
    if ( status < 0 ) goto Exit;

    // 8. Integrity Seal: Finalize with a 4-byte Checksum (sub_18013B974)
    // Calculates CRC of first 160 bytes and writes result to Offset 160.
    sub_18013B974(output, 0, output->Checksum);

Exit:
    sub_18008D904(status); // Clean up / Return Status
    return status;
}


/**
 * Origin: Sppobjs.dll
 * Logic: Scans license properties to classify the distribution channel.
 */

// Original Name: sub_18013C75C
NTSTATUS IdentifyLicenseChannel(Context* ctx, DWORD* outChannelType)
{
    NTSTATUS status;
    BOOL isFound = FALSE; // This corresponds to the stack variable v8

    // 1. Check for "OEM" Channel
    // ctx + 120 points to the parsed XML license property bag
    status = SearchLicenseProperty(ctx->PropertyBag, L"OEM", &isFound);
    
    if (status < 0) goto Error; // Search failed (e.g. out of memory)

    // If "OEM" keyword was found in the license
    if (isFound) 
    {
        *outChannelType = 2; // Magic Number for OEM
        return STATUS_SUCCESS;
    }

    // 2. Check for "Volume" Channel
    status = SearchLicenseProperty(ctx->PropertyBag, L"Volume", &isFound);
    if (status >= 0) 
    {
        // 3. Check for "Retail" Channel
        status = SearchLicenseProperty(ctx->PropertyBag, L"Retail", &isFound);
        if (status >= 0) 
        {
            // 4. Check for "VT" (Volume Trial / Validation Type)
            status = SearchLicenseProperty(ctx->PropertyBag, L"VT", &isFound);
            
            if (status >= 0) 
            {
                // If we went through all keywords and found NOTHING
                status = 0x89FA0001; // Error: Unknown License Channel
                goto Error;
            }
        }
    }

Error:
    LogError(status);    // sub_18008A114
    Cleanup(status);    // sub_18008D904
    return status;
}

/**
 * Origin: Sppobjs.dll
 * Logic: Formats the legacy 20-character Product ID based on license channel.
 */

// Original Name: sub_18013BC64
NTSTATUS FormatLegacyProductId(Context* ctx, wchar_t* mpc, wchar_t* outputBuffer)
{
    NTSTATUS status;
    int channelType; // Received from IdentifyLicenseChannel (v13)
    DWORD serial;
    DWORD dateCode;
    DWORD groupId;
    DWORD checksum;

    // 1. Identify the license channel (OEM, Retail, Volume)
    status = IdentifyLicenseChannel(ctx, &channelType); // sub_18013C75C
    if (status < 0) goto Exit;

    // 2. Branching Logic based on Channel
    if (channelType != 2) // --- RETAIL / VOLUME PATH ---
    {
        dateCode = ctx->DateCode % 1000;              // Extract build/date segment
        serial   = ctx->SerialNumber % 1000000;       // Extract 6-digit serial
        checksum = CalculateMod7(serial);             // sub_18013BB5C (Mod7 check)
        
        groupId  = (ctx->KeyGroupId % 1000) + (1000 * ((ctx->Header->Flag >> 1) % 100));

        // Format: "MPC-Date-SerialSum-GroupId" (e.g., 00330-123-1234567-12345)
        status = SafePrintf(outputBuffer, 0x30, L"%05.5s-%03.3lu-%07.7lu-%05.5lu", 
                            mpc, dateCode, checksum, groupId);
    }
    else // --- OEM PATH ---
    {
        serial = ctx->SerialNumber;                   // Raw serial (v8)
        
        // Complex calculation mixing date, flags, and serial for the OEM segment
        DWORD oemInput = (serial % 1000000 / 100000) + 
                         10 * ((ctx->DateCode % 1000) + 1000 * ((ctx->Header->Flag >> 1) % 100));
        
        checksum = CalculateMod7(oemInput);           // sub_18013BB5C
        DWORD lowSerial = serial % 100000;            // Last 5 digits

        // Format: "MPC-OEM-SerialSum-LowSerial" (e.g., 00330-OEM-1234567-12345)
        status = SafePrintf(outputBuffer, 0x30, L"%05.5s-OEM-%07.7lu-%05.5lu", 
                            mpc, checksum, lowSerial);
    }

Exit:
    if (status < 0) LogError(status); // sub_18008A114
    Cleanup(status);                  // sub_18008D904
    return status;
}

/**
 * Origin: Sppobjs.dll
 * Logic: Formats the high-precision Extended Product ID (EPID).
 */

// Original Name: sub_18013BFA8
NTSTATUS FormatExtendedProductId(Context* ctx,  wchar_t* mpc, int languageId, int osVersion, int mysteryParam, wchar_t* outputBuffer)
{
    NTSTATUS status;
    DWORD channelType;  // OEM, Retail, etc.
    DWORD dayOfYear;    // 1-366
    DWORD year;         // e.g., 2026
    
    // 1. Identify License Channel (sub_18013C75C)
    status = IdentifyLicenseChannel(ctx, &channelType);
    if (status < 0) goto Error;

    // 2. Calculate Installation Date Info (sub_18013C654)
    // Converts ctx + 16 (FILETIME) to Day of Year and Year
    status = CalculateActivationDate(ctx->InstallTimestamp, &dayOfYear, &year);
    if (status < 0) goto Error;

    // 3. Prepare Buffer
    memset(outputBuffer, 0, 128); // Clear 128-byte buffer

    // 4. Data Extraction using Modulo (packing values into specific widths)
    DWORD groupId  = ctx->AlgorithmInfo->GroupId % 100000; // v13
    DWORD buildNum = ctx->BuildNumber % 1000;             // v14
    DWORD serial   = ctx->SerialNumber % 1000000;         // v15

    // 5. Final String Assembly
    // Template: "MPC-Group-Build-Serial-Channel-Lang-OS-Unknown-DayYear"
    // Example: "00330-10000-000-123456-02-1033-19044.0000-1112026"
    status = SafePrintf(
        outputBuffer,
        128,
        L"%05.5s-%05.5u-%03.3u-%06.6u-%02.2u-%04.4u-%04.4u.%04.4u-%03.3u%04.4u",
        mpc,            // %05.5s
        groupId,        // %05.5u
        buildNum,       // %03.3u
        serial,         // %06.6u
        channelType,    // %02.2u
        languageId,     // %04.4u
        osVersion,      // %04.4u
        mysteryParam,   // %04.4u
        dayOfYear,      // %03.3u
        year            // %04.4u
    );

    if (status >= 0) return status;

Error:
    LogError(status);    // sub_18008A114
    Cleanup(status);     // sub_18008D904
    return status;
}

/**
 * Origin: Sppobjs.dll
 * Logic: Formats the Product ID using the PKEY 2009 algorithm template.
 */

// Original Name: sub_18013BE18
NTSTATUS FormatModernProductId(int channelType, unsigned int groupId, unsigned int serial, unsigned int build, int isGenuine, wchar_t* outputBuffer)
{
    NTSTATUS status;
    
    // 1. Numerical Decomposition
    // Breaks the serial and group IDs into specific segments for the string
    unsigned int serialHigh = serial / 100000;         // v7
    unsigned int serialMid  = serial % 100000;         // v8 (Last 5 digits)
    unsigned int serialCheck = serialHigh % 10000;     // v9
    unsigned int groupSegment = (groupId / 10) % 100000; // v10

    // 2. Character Generation
    // Generates 'A' or 'B' based on the Genuine flag (65 is ASCII 'A')
    int char1 = 65;                    // 'A'
    int char2 = (isGenuine != 0) + 65; // 'A' if genuine, 'B' if not

    // 3. Format Branching
    if (channelType == 2) // --- OEM PATH ---
    {
        unsigned int groupDigit = groupId % 10;

        // Template: "Group-DigitSerial-Serial-AAOEM"
        // Example: "00330-01234-56789-AAOEM"
        status = SafePrintf(
            outputBuffer, 
            48, 
            L"%05.5lu-%01.1lu%04.4lu-%05.5lu-%c%cOEM", 
            groupSegment, 
            groupDigit, 
            serialCheck, 
            serialMid, 
            char1, 
            char2
        );
    }
    else // --- RETAIL / VOLUME PATH ---
    {
        unsigned int groupDigit = groupId % 10;
        unsigned int buildCode  = build % 1000;

        // Template: "Group-DigitSerial-Serial-AA-Build"
        // Example: "00330-01234-56789-AA-123"
        status = SafePrintf(
            outputBuffer, 
            48, 
            L"%05.5lu-%01.1lu%04.4lu-%05.5lu-%c%c%03.3lu", 
            groupSegment, 
            groupDigit, 
            serialCheck, 
            serialMid, 
            char1, 
            char2, 
            buildCode
        );
    }

    // 4. Error Handling
    if (status < 0) {
        LogError(status); // sub_18008A114
    }
    
    Cleanup(status); // sub_18008D904
    return status;
}

/**
 * Origin: Sppobjs.dll
 * Logic: Builds the massive 1272-byte DigitalProductId4 structure.
 */

// Original Name: sub_18013C100
NTSTATUS BuildDigitalProductId4(Context* ctx, void* unused, void* unused2, Pid4Buffer* output)
{
    NTSTATUS status;
    RPC_WSTR uuidString = NULL;
    wchar_t epidBuffer[64]; // Stores the Extended PID string
    
    // 1. Validation: The structure must be exactly 1272 bytes (0x4F8)
    if ( output->TotalSize != 1272 ) 
    {
        status = STATUS_INVALID_PARAMETER; // 0x80070057
        goto ExitWithError;
    }

    // 2. Activation Context: Get build-specific versioning info
    status = GetActivationContextInfo(&buildMinor, &buildMajor);
    if (status < 0) goto ExitWithError;

    // 3. String Generation: Format the Extended PID (EPID)
    // Uses the function we just looked at (sub_18013BFA8)
    status = FormatExtendedProductId(
        ctx, 
        ctx->MPC, 
        GetSystemDefaultLangID(), 
        buildMinor, 
        buildMajor, 
        epidBuffer);
    if (status < 0) goto ExitWithError;

    // 4. Memory Setup: Clear the buffer and set Version 4 header
    memset(output->PaddingArea, 0, 0x470);
    output->TotalSize = 1272;
    output->Version = 4;

    // 5. High-Speed Copy: Use XMM (128-bit) registers to move the EPID
    // This is the "v12 = v26[1]" logic in assembly
    Copy128Bit(output->EpidArea, epidBuffer);

    // 6. Hardware Binding (The "Lock")
    // Converts the binary Hardware UUID to a string
    status = UuidToStringW(ctx->HardwareId, &uuidString);
    if (status < 0) goto ExitWithError;

    // 7. Feature Flags: Mark if this is an Upgrade or Evaluation
    if (ctx->IsUpgrade > 1) 
        output->UpgradeFlag = 1;

    // 8. Security/Genuine State
    output->GenuineState = (ctx->Status != 0); // Is system genuine?

    // 9. Activation Seed: Copy specific licensing blobs
    output->ActivationBlob = ctx->LicenseBlob;

    // 10. The Integrity Seals (Hashing)
    // This function calls the CRC/Hashing engine multiple times:
    // A. Hash the Key ID
    sub_1800A4BB4(ctx->HashingContext, ctx->KeyId, 0x10);
    sub_18009BD7C(ctx->HashingContext, output->KeyIdHash);

    // B. Hash the ENTIRE 1272-byte structure (The Master Seal)
    sub_1800A4BB4(ctx->HashingContext, output, 1272);
    sub_18009BD7C(ctx->HashingContext, output->MasterHash);

ExitWithError:
    CleanupAndLog(status);
    return status;
}

/**
 * Origin: Pidgenx.dll
 * Logic: Low-level serialization and CRC32 checksumming for the 164-byte legacy structure.
 */

// Original Name: sub_1800073A4
NTSTATUS BuildDigitalProductId3_Pidgenx(Context* ctx, HWND hWnd, void* hardwareSeed, DigitalProductId3* output)
{
    NTSTATUS status;
    __int128 epidTemp[3]; 

    // 1. Validation
    if ( output->m_length != 164 ) // Offset +0x00
    {
        status = STATUS_INVALID_PARAMETER; 
        sub_1800038E8(status);             
        return status;
    }

    // 2. Setup: Clear data area starting from m_productId2 (Offset +0x08)
    memset(&output->m_productId2, 0, 0x9C);
    output->m_length = 164;               // Offset +0x00 (DWORD)
    output->m_versionMajor = 3;           // Offset +0x04 (WORD)
    output->m_versionMinor = 0;           // Offset +0x06 (WORD)

    // 3. String Generation
    status = sub_180006F80(ctx, hWnd);
    if ( status < 0 ) goto Exit;

    // 4. Data Packing: Copy PID string to m_productId2
    // Written to Offset +0x08 (24 bytes)
    status = sub_18000858C(output->m_productId2, 24, epidTemp);
    if ( status < 0 ) goto Exit;

    // 5. Metadata Mapping
    output->m_keyIdx = ctx->PKeyAlgorithm->GroupId;       // Offset +0x20 (DWORD)

    // Copy SKU / Part Number 
    // Written to Offset +0x24 (16 bytes)
    status = sub_18000858C(output->m_sku, 16, ctx->License->PartNumber); 
    if ( status < 0 ) goto Exit;

    // Copy ProductKey GUID (repurposed m_abCdKey)
    // Written to Offset +0x34 (16 bytes / OWORD)
    memcpy(output->m_abCdKey, &ctx->KeyGuid, 16);

    // 6. Date and Type Flags
    // Formula: (Ticks - 1601_Epoch) / Ticks_Per_Day
    if ( ctx->FileTime >= 0x19DB1DED53E8000 ) 
        output->m_time = (ctx->FileTime - 116444736000000000) / 0x989680; // Offset +0x48 (DWORD)
    else
        output->m_time = 0;

    output->m_random = ctx->KeyTypeFlag; // Offset +0x4C (DWORD)

    // 7. Channel ID and Hardware Binding
    status = sub_180008730(ctx, output->m_oemId); // Offset +0x50 (8 bytes)
    if ( status < 0 ) goto Exit;

    // Written to Offset +0x5C (8 bytes)
    status = sub_18000858C(output->m_hardwareIdStatic, 8, hardwareSeed); 
    if ( status < 0 ) goto Exit;

    // 8. Integrity Seal: Manual CRC32
    // Calculated over bytes 0-159. Result written to Offset +0xA0 (DWORD)
    output->m_crc32 = CalculateCrc32(output, 160);

Exit:
    sub_180003B30(status); 
    return status;
}

/**
 * Origin: Pidgenx.dll
 * Logic: Builds the 1272-byte DigitalProductId4 using XMM registers and dual-layer hashing.
 */

// Original Name: sub_1800079A8
NTSTATUS BuildDigitalProductId4_Pidgenx(Context* ctx, int buildInfo, void* hardwareId, DigitalProductId4* output)
{
    NTSTATUS status;
    RPC_WSTR StringUuid = NULL;
    __int128 xmm_storage[8]; 

    // 1. Validation
    if ( output->m_length != 1272 ) // Offset +0x00
    {
        return STATUS_INVALID_PARAMETER;
    }

    // 2. EPID Generation (formats context into xmm_storage)
    status = sub_180007844(ctx, buildInfo, GetSystemDefaultLangID(), xmm_storage);
    if ( status < 0 ) goto Exit;

    // 3. Header Setup
    memset(output->m_reserved, 0, 0x470); // Clears from Offset +0x88 (m_reserved)
    output->m_length = 1272;              // Offset +0x00 (DWORD)
    output->m_versionMajor = 4;           // Offset +0x04 (WORD)
    output->m_versionMinor = 0;           // Offset +0x06 (WORD / implied)

    // 4. High-Speed Copy (EPID)
    // Populates m_productId2Ex from Offset +0x08 (128 bytes)
    memcpy(output->m_productId2Ex, xmm_storage, 128);

    // 5. Hardware ID String
    status = UuidToStringW(ctx->HardwareId, &StringUuid);
    if (status != RPC_S_OK) goto Exit;
    // Written to Offset +0x88 (m_sku - repurposed for Hardware UUID)
    SafeStringCopy(output->m_sku, StringUuid, 64);

    // 6. Upgrade Flags
    if ( ctx->IsUpgrade > 1 ) output->m_isUpgrade = 1; // Offset +0x288 (BYTE)

    // 7. Edition and Key Mapping
    // Written to m_editionId at Offset +0x118 (260 WCHARs / 520 bytes)
    SafeStringCopy(output->m_editionId, ctx->KeyId, 260); 
    // Written to m_abCdKey at Offset +0x328 (16 bytes)
    memcpy(output->m_abCdKey, &ctx->KeyGuid, 16);        

    // 8. The First Seal: Hash of the binary key
    // Result written to m_abCdKeySHA256Hash at Offset +0x338 (32 bytes)
    GenerateHash(output->m_abCdKey, 16, output->m_abCdKeySHA256Hash);

    // 9. The Master Seal: Hash of full 1272-byte block
    // Result written to m_abSHA256Hash at Offset +0x358 (32 bytes)
    GenerateHash(output, 1272, output->m_abSHA256Hash);

Exit:
    if (StringUuid) RpcStringFreeW(&StringUuid);
    return status;
}