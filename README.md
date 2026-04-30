# Windows Licensing & SPP Interop Toolkit

A collection of low-level PowerShell functions designed to interface with Windows Software Protection Platform (SPP) components. This toolkit facilitates the encoding, decoding, and extraction of metadata from Microsoft Product Keys and Hardware Identifiers (HWID) by calling unmanaged methods within internal system DLLs.

## 🚀 Capabilities

*   **Product Key Processing**: Implements Base24 encoding/decoding logic (including modern 'N' bit handling) to convert between 25-character strings and 16-byte binary arrays.
*   **HWID Extraction**: Bridges with `LicensingWinRT.dll` to retrieve and convert hardware-specific data pointers into short-form identifiers.
*   **PID Generation**: Interfaces with `pidgenx.dll`, `sppobjs.dll`, and `sppwinob.dll` to map internal structures like Group ID, Serial, and Security ID.
*   **Installation ID (IID) Generation**: 
    *   Includes logic to construct `MSFT_PKEY_DATA` structures for generating IIDs.
    *   Serves as a programmatic alternative to the standard `SLGenerateOfflineInstallationIdEx` API call, allowing for deeper inspection and manual forgery of activation requests.

## 🛠 Prerequisites

This script requires the **NativeInteropLib** PowerShell module to handle unmanaged memory allocation and function invocation. 

Ensure the following DLLs are available in a `\SppDll\` subdirectory:
* `sppobjs.dll`
* `sppwinob.dll`
* `pidgenx.dll`
* `LicensingWinRT.dll`
* `pidgenxInsider.dll`

## 📖 Usage

### Encoding a Key to Binary
```powershell
$Key = "XXXXX-XXXXX-XXXXX-XXXXX-XXXXX"
$BinaryKey = Encode-ProductKey -ProductKey $Key
