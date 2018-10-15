// pdf_association0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <ShlObj.h>

void set_app_as_default(TCHAR *pszAppRegistryName, TCHAR *pszSet, ASSOCIATIONTYPE atSetType)
{
	// for 32bit
	const GUID CLSID_ApplicationAssociationRegistration =
	{ 0x591209C7, 0x767B, 0x42B2,{ 0x9f, 0xba, 0x44, 0xee, 0x46, 0x15, 0xf2, 0xc7 } };

	const IID IID_ApplicationAssociationRegistration =
	{ 0x4E530B0A, 0xE611, 0x4C77,{ 0xa3, 0xac, 0x90, 0x31, 0xd0, 0x22, 0x28, 0x1b } };

	const IID IID2_ApplicationAssociationRegistration =
	{ 0x229D59E2, 0xF94A, 0x402E,{ 0x9a, 0x9f, 0x3b, 0x84, 0xa1, 0xac, 0xed, 0x77 } };

	const IID IID3_ApplicationAssociationRegistration =
	{ 0xC7225171, 0xB9A7, 0x4CF7,{ 0x86, 0x1F ,0x85, 0xAB, 0x7B, 0xA3, 0xC5, 0xB2 } };

	auto hRet = 0;
	auto ifA = false;
	auto cInit = CoInitializeEx(NULL, NULL);

	IApplicationAssociationRegistration *pAAR = nullptr;
	IApplicationAssociationRegistration *pAAR2 = nullptr;
	IApplicationAssociationRegistration *pAAR3 = nullptr;
	IApplicationAssociationRegistration *pAAR4 = nullptr;

	HRESULT hr = CoCreateInstance(
		CLSID_ApplicationAssociationRegistration,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ApplicationAssociationRegistration,
		(void **)&pAAR
		);
	if (hr >= 0 && pAAR)
	{
		hr = pAAR->QueryInterface(
			IID2_ApplicationAssociationRegistration,
			(void **)&pAAR2
			);
		if (!FAILED(hr))
		{
			hr = (*(int(__stdcall **)(PVOID, PVOID, PVOID, DWORD))(*(DWORD *)pAAR2 + 0x10))
				(pAAR2, pszAppRegistryName, pszSet, atSetType);
			if (!FAILED(hr))
				ifA = true;

			if (pAAR2)
				pAAR2->Release();
			pAAR2 = 0;
		}
		else
		{
			printf("QueryInterface IID2_ApplicationAssociationRegistration failed: %d\n",
				GetLastError());
		}

		if (!ifA)
		{
			hr = pAAR->QueryInterface(
				IID3_ApplicationAssociationRegistration,
				(void **)&pAAR3
				);
			if (!FAILED(hr))
			{
				// test fine in win8.1-32bit
				// HRESULT SetAppAsDefault(LPCWSTR pszAppRegistryName, LPCWSTR pszSet, ASSOCIATIONTYPE atSetType);
				hr = (*(int(__stdcall **)(PVOID, PVOID, PVOID, DWORD))(*(DWORD *)pAAR3 + 0x10))
					(pAAR3, pszAppRegistryName, pszSet, atSetType);
				if (!FAILED(hr))
					ifA = true;

				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))
					(pAAR, 1, pszAppRegistryName, &hRet);

				if (pAAR3)
					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR3 + 0x08))(pAAR3);
				pAAR3 = 0;
			}
			else
			{
				printf("QueryInterface IID3_ApplicationAssociationRegistration failed: %d\n",
					GetLastError());
			}
		}

		if (!ifA)
		{
			hr = pAAR->QueryInterface(IID3_ApplicationAssociationRegistration, (void **)&pAAR4);
			if (!FAILED(hr))
			{
				hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR4 + 0x14))
					(pAAR4, pszAppRegistryName);
				if (!FAILED(hr))
					ifA = true;

				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))
					(pAAR, 1, pszAppRegistryName, &hRet);

				if (pAAR4)
					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR4 + 0x08))(pAAR4);
				pAAR4 = 0;
			}
			else
			{
				printf("QueryInterface IID3_ApplicationAssociationRegistration failed: %d\n",
					GetLastError());
			}
		}

		hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))
			(pAAR, 1, pszAppRegistryName, &hRet);

		if (pAAR)
			pAAR->Release();
		pAAR = 0;
	}
	else
	{
		printf("CoCreateInstance IID_ApplicationAssociationRegistration failed: %d\n",
			GetLastError());
	}

	if (cInit >= 0)
		CoUninitialize();

	return;
}

int _tmain(int argc, _TCHAR* argv[])
{
	set_app_as_default(L"SumatraPDF", L".pdf", AT_FILEEXTENSION);

	return 0;
}

