// pdf_association1.cpp : Defines the entry point for the console application.
//

// https://www.freebuf.com/articles/system/130288.html

#include "stdafx.h"
#include <Windows.h>
#include <ShlObj.h>

// MIDL_INTERFACE("1c5c9d10-1225-4c97-8c51-98e1b6f0d4e0")
// MIDL_INTERFACE("591209C7-767B-42B2-9FBA-44EE4615F2C7")
MIDL_INTERFACE("229D59E2-F94A-402E-9A9F-3B84A1ACED77")
IApplicationAssociationRegistrationInternal : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE ClearUserAssociations(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetProgIdAsDefault(
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppRegistryName,
		/* [string][in] */ __RPC__in_string LPCWSTR pszSet,
		/* [in] */ ASSOCIATIONTYPE atSetType) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetAppAsDefault(
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppRegistryName,
		/* [string][in] */ __RPC__in_string LPCWSTR pszSet,
		/* [in] */ ASSOCIATIONTYPE atSetType) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetAppAsDefaultAll(
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppRegistryName) = 0;

	virtual HRESULT STDMETHODCALLTYPE QueryAppIsDefault(
		/* [string][in] */ __RPC__in_string LPCWSTR pszQuery,
		/* [in] */ ASSOCIATIONTYPE atQueryType,
		/* [in] */ ASSOCIATIONLEVEL alQueryLevel,
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppRegistryName,
		/* [out] */ __RPC__out BOOL *pfDefault) = 0;

	virtual HRESULT STDMETHODCALLTYPE QueryAppIsDefaultAll(
		/* [in] */ ASSOCIATIONLEVEL alQueryLevel,
		/* [string][in] */ __RPC__in_string LPCWSTR pszAppRegistryName,
		/* [out] */ __RPC__out BOOL *pfDefault) = 0;

	virtual HRESULT STDMETHODCALLTYPE QueryCurrentDefault(
		/* [string][in] */ __RPC__in_string LPCWSTR pszQuery,
		/* [in] */ ASSOCIATIONTYPE atQueryType,
		/* [in] */ ASSOCIATIONLEVEL alQueryLevel,
		/* [string][out] */ __RPC__deref_out_opt_string LPWSTR *ppszAssociation) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetDefaultBrowserInfo(
		ASSOCIATIONTYPE atQueryType,
		LPWSTR * ppszAssociation) = 0;

	virtual HRESULT STDMETHODCALLTYPE RestoreDefaultBrowserContractRegistration(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE IsBrowserAssociation(LPCWSTR, int *) = 0;

	virtual HRESULT STDMETHODCALLTYPE ExportUserAssociations(LPCWSTR) = 0;

	virtual HRESULT STDMETHODCALLTYPE ApplyUserAssociations(LPCWSTR) = 0;

	virtual HRESULT STDMETHODCALLTYPE UpdateProtocolCapabilityCache(LPCWSTR, int) = 0;
};

HRESULT set_pdf_as_default()
{
	IApplicationAssociationRegistration *paar = NULL;

	HRESULT hr = CoCreateInstance(
		CLSID_ApplicationAssociationRegistration,
		NULL,
		CLSCTX_INPROC,
		__uuidof(IApplicationAssociationRegistration),
		(void **)&paar
		);
	if (SUCCEEDED(hr))
	{
		IApplicationAssociationRegistrationInternal *paari = NULL;

		HRESULT hr = paar->QueryInterface(
			__uuidof(IApplicationAssociationRegistrationInternal),
			(void **)&paari
			);
		if (SUCCEEDED(hr))
		{
			hr = paari->SetAppAsDefault(
				L"Adobe Acrobat Reader DC",
				L".pdf",
				AT_FILEEXTENSION
				);
			paari->Release();
		}
		else
		{
			printf("QueryInterface failed: 0x%08x\n", hr);
		}
	}
	else
	{
		printf("CoCreateInstance failed: 0x%08x\n", hr);
	}

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, NULL);
	set_pdf_as_default();
	CoUninitialize();

	return 0;
}

