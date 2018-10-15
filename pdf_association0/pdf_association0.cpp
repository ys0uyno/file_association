// pdf_association0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <ShlObj.h>

void set_app_as_default_win81(const TCHAR *pszAppRegistryName, const TCHAR *pszSet, ASSOCIATIONTYPE atSetType)
{
	const GUID CLSID_ApplicationAssociationRegistration =
	{ 0x591209C7, 0x767B, 0x42B2,{ 0x9F, 0xBA, 0x44, 0xEE, 0x46, 0x15, 0xF2, 0xC7 } };

	const IID IID1_ApplicationAssociationRegistration =
	{ 0x4E530B0A, 0xE611, 0x4C77,{ 0xA3, 0xAC, 0x90, 0x31, 0xD0, 0x22, 0x28, 0x1B } };

	const IID IID2_ApplicationAssociationRegistration =
	{ 0x229D59E2, 0xF94A, 0x402E,{ 0x9A, 0x9F, 0x3B, 0x84, 0xA1, 0xAC, 0xED, 0x77 } };

	const IID IID3_ApplicationAssociationRegistration =
	{ 0xC7225171, 0xB9A7, 0x4CF7,{ 0x86, 0x1F ,0x85, 0xAB, 0x7B, 0xA3, 0xC5, 0xB2 } };

	/* can try IID below if any problems occur
	const IID IID3_ApplicationAssociationRegistration =
	{ 0x1C5C9D10, 0x1225, 0x4C97,{ 0x8C, 0x51, 0x98, 0xE1, 0xB6, 0xF0, 0xD4, 0xE0 } };
	*/

	IApplicationAssociationRegistration *paar1 = nullptr;
	IApplicationAssociationRegistration *paar2 = nullptr;
	IApplicationAssociationRegistration *paar3 = nullptr;

	HRESULT hr = CoCreateInstance(
		CLSID_ApplicationAssociationRegistration,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID1_ApplicationAssociationRegistration,
		(void **)&paar1
		);
	if (SUCCEEDED(hr) && paar1)
	{
		hr = paar1->QueryInterface(
			IID2_ApplicationAssociationRegistration,
			(void **)&paar2
			);
		if (SUCCEEDED(hr) && paar2)
		{
			hr = (*(int(__stdcall **)(PVOID, PVOID, PVOID, DWORD))(*(DWORD *)paar2 + 0x10))
				(paar2, (PVOID)pszAppRegistryName, (PVOID)pszSet, atSetType);
			if (SUCCEEDED(hr))
			{
				printf("IID2_ApplicationAssociationRegistration + 0x10 succeeded\n");
				return;
			}
		}
		else
		{
			printf("QueryInterface IID2_ApplicationAssociationRegistration failed: %d\n",
				GetLastError());
		}

		hr = paar1->QueryInterface(
			IID3_ApplicationAssociationRegistration,
			(void **)&paar3
			);
		if (SUCCEEDED(hr) && paar3)
		{
			// HRESULT SetAppAsDefault(LPCWSTR, LPCWSTR, ASSOCIATIONTYPE);
			hr = (*(int(__stdcall **)(PVOID, PVOID, PVOID, DWORD))(*(DWORD *)paar3 + 0x10))
				(paar3, (PVOID)pszAppRegistryName, (PVOID)pszSet, atSetType);
			if (SUCCEEDED(hr))
			{
				printf("IID3_ApplicationAssociationRegistration + 0x10 succeeded\n");
			}
		}
		else
		{
			printf("QueryInterface IID3_ApplicationAssociationRegistration failed: %d\n",
				GetLastError());
		}
	}
	else
	{
		printf("CoCreateInstance IID1_ApplicationAssociationRegistration failed: %d\n",
			GetLastError());
	}
}

void set_app_as_default(const TCHAR *pszAppRegistryName, const TCHAR *pszSet, ASSOCIATIONTYPE atSetType)
{
	CoInitializeEx(NULL, NULL);

	// test fine in win8.1-32/64bit, also in win10-64bit
	set_app_as_default_win81(pszAppRegistryName, pszSet, atSetType);

	CoUninitialize();
}

int _tmain(int argc, _TCHAR* argv[])
{
	set_app_as_default(L"SumatraPDF", L".pdf", AT_FILEEXTENSION);

	return 0;
}

