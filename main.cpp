////#include <stdio.h>
////#include <Windows.h>
////
////
////// 单向散列函数
////BOOL OneWayHash(PBYTE pData, DWORD dwSize, ALG_ID cType)
////{
////	HCRYPTPROV hCryptProv = NULL;
////	HCRYPTHASH hCryptHash = NULL;
////	PBYTE pHashData = NULL;
////	BOOL fOk = FALSE;
////	DWORD dwHashLen = 0, dwDataLen = sizeof(dwHashLen);
////
////	do
////	{
////		fOk = CryptAcquireContextA(&hCryptProv,
////			NULL,
////			NULL,
////			PROV_RSA_AES,
////			CRYPT_VERIFYCONTEXT
////		);
////		if (!fOk)
////		{
////			break;
////		}
////		// 创建一个空的HASH对象
////		fOk = CryptCreateHash(hCryptProv, cType, NULL, 0, &hCryptHash);
////		if (!fOk)
////		{
////			break;
////		}
////		// 生成对应的哈希值
////		if (!CryptHashData(hCryptHash, pData, dwSize, 0))
////		{
////			break;
////		}
////		if (!CryptGetHashParam(hCryptHash, HP_HASHSIZE, (PBYTE)&dwHashLen, &dwDataLen, 0))
////		{
////			break;
////		}
////		pHashData = new BYTE[dwHashLen];
////		if (NULL == pHashData)
////		{
////			break;
////		}
////		if (!CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashLen, 0))
////		{
////			break;
////		}
////		fOk = TRUE;
////	} while (FALSE);
////
////	if (fOk)
////	{
////		for (size_t nIdx = 0; nIdx < dwHashLen; ++nIdx)
////		{
////			printf("%02X", pHashData[nIdx]);
////		}
////		putchar('\n');
////	}
////	if (NULL != pHashData)
////	{
////		delete[] pHashData;
////		pHashData = NULL;
////	}
////	if (NULL != hCryptProv)
////	{
////		CryptReleaseContext(hCryptProv, 0);
////		hCryptProv = NULL;
////	}
////	if (NULL != hCryptHash)
////	{
////		CryptDestroyHash(hCryptHash);
////		hCryptHash = NULL;
////	}
////
////	return(fOk);
////}
////
////BOOL FileWriteBack(const char* pcszFilePathName, PBYTE pbData, size_t nBufSize)
////{
////	BOOL fOk = FALSE;
////	FILE* fp = NULL;
////
////	fp = fopen(pcszFilePathName, "wb");
////	if (NULL == fp)
////	{
////		return(fOk);
////	}
////	fwrite(pbData, nBufSize, 1, fp);
////	if (NULL != fp)
////	{
////		fclose(fp);
////		fp = NULL;
////	}
////	fOk = TRUE;
////
////	return(fOk);
////}
////
////
////BOOL ReadFromFile(LPCSTR lpcszFilePathName, PBYTE* ppData, DWORD& dwFileSize, size_t& nFileBufLen)
////{
////	BOOL fOk = FALSE;
////	FILE* fp = NULL;
////	LONG lSize = 0;
////	PBYTE pbData = NULL;
////
////	fp = fopen(lpcszFilePathName, "rb");
////	if (NULL == fp)
////	{
////		return(FALSE);
////	}
////	fseek(fp, 0, SEEK_END);
////	lSize = ftell(fp);
////	pbData = new BYTE[lSize * 16];
////	RtlZeroMemory(pbData, lSize * 16);
////	dwFileSize = lSize;
////	nFileBufLen = lSize * 16;
////	fseek(fp, 0, SEEK_SET);
////	fread(pbData, lSize, 1, fp);
////	if (NULL != fp)
////	{
////		fclose(fp);
////		fp = NULL;
////	}
////	*ppData = pbData;
////
////	return(TRUE);
////}
////
////
////// 256位AES算法加密与解密
////BOOL AESEncrypt(const PBYTE pcszPassword, size_t nPassSize, LPCSTR lpcszOrigFilePathName, LPCSTR lpcszEncFilePathName)
////{
////	HCRYPTPROV hCryptProv = NULL;
////	HCRYPTHASH hCryptHash = NULL;
////	HCRYPTKEY hCryptKey = NULL;
////	BOOL fOk = FALSE;
////	PBYTE pbData = NULL;
////	DWORD dwSize = 0;
////	size_t nBufLen = 0;
////
////	do
////	{
////		if (!ReadFromFile(lpcszOrigFilePathName, &pbData, dwSize, nBufLen))
////		{
////			break;
////		}
////		if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
////		{
////			break;
////		}
////		if (!CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash))
////		{
////			break;
////		}
////		if (!CryptHashData(hCryptHash, pcszPassword, nPassSize, 0))
////		{
////			break;
////		}
////		if (!CryptDeriveKey(hCryptProv, CALG_AES_256, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey))
////		{
////			break;
////		}
////		if (!CryptEncrypt(hCryptKey, NULL, TRUE, 0, pbData, &dwSize, nBufLen))
////		{
////			break;
////		}
////		fOk = FileWriteBack(lpcszEncFilePathName, pbData, dwSize);
////	} while (FALSE);
////	if (NULL != hCryptProv)
////	{
////		CryptReleaseContext(hCryptProv, 0);
////		hCryptProv = NULL;
////	}
////	if (NULL != hCryptHash)
////	{
////		CryptDestroyHash(hCryptHash);
////		hCryptHash = NULL;
////	}
////	if (NULL != hCryptKey)
////	{
////		CryptDestroyKey(hCryptKey);
////		hCryptKey = NULL;
////	}
////	if (NULL != pbData)
////	{
////		delete[] pbData;
////		pbData = NULL;
////	}
////
////	return(fOk);
////}
////
////BOOL AESDecrypt(const PBYTE pcszPassword,
////	size_t nPassSize,
////	LPCSTR lpcszOrigFilePathName,
////	LPCSTR lpcszEncFilePathName
////)
////{
////	BOOL fOk = FALSE;
////	PBYTE pbData = NULL;
////	DWORD dwFileLen = 0;
////	size_t nBufSize = 0;
////	HCRYPTPROV hCryptProv = NULL;
////	HCRYPTHASH hCryptHash = NULL;
////	HCRYPTKEY hCryptKey = NULL;
////
////	do
////	{
////		if (!ReadFromFile(lpcszOrigFilePathName, &pbData, dwFileLen, nBufSize))
////		{
////			break;
////		}
////		if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
////		{
////			break;
////		}
////		if (!CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash))
////		{
////			break;
////		}
////		if (!CryptHashData(hCryptHash, pcszPassword, nPassSize, 0))
////		{
////			break;
////		}
////		if (!CryptDeriveKey(hCryptProv, CALG_AES_256, hCryptHash, 0, &hCryptKey))
////		{
////			break;
////		}
////		if (!CryptDecrypt(hCryptKey, NULL, TRUE, 0, pbData, &dwFileLen))
////		{
////			break;
////		}
////		if (!FileWriteBack(lpcszEncFilePathName, pbData, dwFileLen))
////		{
////			break;
////		}
////		fOk = TRUE;
////	} while (FALSE);
////	if (NULL != pbData)
////	{
////		delete[] pbData;
////		pbData = NULL;
////	}
////	if (NULL != hCryptProv)
////	{
////		CryptReleaseContext(hCryptProv, 0);
////		hCryptProv = NULL;
////	}
////	if (NULL != hCryptHash)
////	{
////		CryptDestroyHash(hCryptHash);
////		hCryptHash = NULL;
////	}
////	if (NULL != hCryptKey)
////	{
////		CryptDestroyKey(hCryptKey);
////		hCryptKey = NULL;
////	}
////
////	return(fOk);
////}
////
////int main()
////{
////	// test string md5
////	// 
////	//char tmp_buff[] = "hello world";
////	//OneWayHash((PBYTE)tmp_buff, strlen(tmp_buff), CALG_MD5);
////
////
////	return 0;
////}
//
//
//// CryptoApi_Aes_Test.cpp : 定义控制台应用程序的入口点。
////
//
//#include <stdio.h>
//#include <Windows.h>
//#include <tchar.h>
//
//
////void ShowError(const char* pszText)
////{
////    char szErr[MAX_PATH] = { 0 };
////    ::sprintf(szErr, "%s Error[0x%x]\n", pszText, ::GetLastError());
////#ifdef _DEBUG
////    ::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
////#endif
////}
//
//
//// AES加密
//BOOL AesEncrypt(BYTE* pPassword, DWORD dwPasswordLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength)
//{
//    BOOL bRet = TRUE;
//    HCRYPTPROV hCryptProv = NULL;
//    HCRYPTHASH hCryptHash = NULL;
//    HCRYPTKEY hCryptKey = NULL;
//
//    do
//    {
//        // 获取CSP句柄
//        bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
//        if (FALSE == bRet)
//        {
//            printf("CryptAcquireContext");
//            break;
//        }
//
//        // 创建HASH对象
//        bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
//        if (FALSE == bRet)
//        {
//            printf("CryptCreateHash");
//            break;
//        }
//
//        // 对密钥进行HASH计算
//        bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
//        if (FALSE == bRet)
//        {
//            printf("CryptHashData");
//            break;
//        }
//
//        // 使用HASH来生成密钥
//        bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
//        if (FALSE == bRet)
//        {
//            printf("CryptDeriveKey");
//            break;
//        }
//
//        // 加密数据
//        bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
//        if (FALSE == bRet)
//        {
//            printf("CryptEncrypt");
//            break;
//        }
//
//    } while (FALSE);
//
//    // 关闭释放
//    if (hCryptKey)
//    {
//        ::CryptDestroyKey(hCryptKey);
//    }
//    if (hCryptHash)
//    {
//        ::CryptDestroyHash(hCryptHash);
//    }
//    if (hCryptProv)
//    {
//        ::CryptReleaseContext(hCryptProv, 0);
//    }
//
//    return bRet;
//}
//
//
//// AES解密
//BOOL AesDecrypt(BYTE* pPassword, DWORD dwPasswordLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength)
//{
//    BOOL bRet = TRUE;
//    HCRYPTPROV hCryptProv = NULL;
//    HCRYPTHASH hCryptHash = NULL;
//    HCRYPTKEY hCryptKey = NULL;
//
//    do
//    {
//        // 获取CSP句柄
//        bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
//        if (FALSE == bRet)
//        {
//            printf("CryptAcquireContext");
//            break;
//        }
//
//        // 创建HASH对象
//        bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
//        if (FALSE == bRet)
//        {
//            printf("CryptCreateHash");
//            break;
//        }
//
//        // 对密钥进行HASH计算
//        bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
//        if (FALSE == bRet)
//        {
//            printf("CryptHashData");
//            break;
//        }
//
//        // 使用HASH来生成密钥
//        bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
//        if (FALSE == bRet)
//        {
//            printf("CryptDeriveKey");
//            break;
//        }
//
//        // 解密数据
//        bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
//        if (FALSE == bRet)
//        {
//            printf("CryptDecrypt");
//            break;
//        }
//
//    } while (FALSE);
//
//    // 关闭释放
//    if (hCryptKey)
//    {
//        ::CryptDestroyKey(hCryptKey);
//    }
//    if (hCryptHash)
//    {
//        ::CryptDestroyHash(hCryptHash);
//    }
//    if (hCryptProv)
//    {
//        ::CryptReleaseContext(hCryptProv, 0);
//    }
//
//    return bRet;
//}
//
//#include <string.h>
//
//int test(int argc, char* argv[])
//{
//    BYTE pData[MAX_PATH] = "What is your name? DemonGan";
//
//    DWORD dwDataLength = 0, dwBufferLength = MAX_PATH;
//    DWORD i = 0;
//
//    //::RtlZeroMemory(pData, dwBufferLength);
//    dwDataLength = 1 + ::strlen((char*)pData);
//
//    printf("Text[%d]\n", dwDataLength);
//    for (i = 0; i < dwDataLength; i++)
//    {
//        printf("%x ", pData[i]);
//    }
//    printf("\n\n");
//
//    // AES 加密
//    AesEncrypt((BYTE*)"DemonGanDemonGan", 16, pData, dwDataLength, dwBufferLength);
//    printf("AES Encrypt[%d]\n", dwDataLength);
//    for (i = 0; i < dwDataLength; i++)
//    {
//        printf("%x ", pData[i]);
//    }
//    printf("\n\n");
//
//    // AES 解密
//    AesDecrypt((BYTE*)"DemonGanDemonGan", 16, pData, dwDataLength, dwBufferLength);
//    printf("AES Decrypt[%d]\n", dwDataLength);
//    for (i = 0; i < dwDataLength; i++)
//    {
//        printf("%x ", pData[i]);
//    }
//    printf("\n\n");
//
//    system("pause");
//    return 0;
//}
//
//
//
//// rsa encrypt
//
//BOOL FileWriteBack(const char* pcszFilePathName, PBYTE pbData, size_t nBufSize)
//{
//	BOOL fOk = FALSE;
//	FILE* fp = NULL;
//
//	fp = fopen(pcszFilePathName, "wb");
//	if (NULL == fp)
//	{
//		return(fOk);
//	}
//	fwrite(pbData, nBufSize, 1, fp);
//	if (NULL != fp)
//	{
//		fclose(fp);
//		fp = NULL;
//	}
//	fOk = TRUE;
//
//	return(fOk);
//}
//
//
//BOOL ReadFromFile(LPCSTR lpcszFilePathName, PBYTE* ppData, DWORD& dwFileSize, size_t& nFileBufLen)
//{
//	BOOL fOk = FALSE;
//	FILE* fp = NULL;
//	LONG lSize = 0;
//	PBYTE pbData = NULL;
//
//	fp = fopen(lpcszFilePathName, "rb");
//	if (NULL == fp)
//	{
//		return(FALSE);
//	}
//	fseek(fp, 0, SEEK_END);
//	lSize = ftell(fp);
//	pbData = new BYTE[lSize * 16];
//	RtlZeroMemory(pbData, lSize * 16);
//	dwFileSize = lSize;
//	nFileBufLen = lSize * 16;
//	fseek(fp, 0, SEEK_SET);
//	fread(pbData, lSize, 1, fp);
//	if (NULL != fp)
//	{
//		fclose(fp);
//		fp = NULL;
//	}
//	*ppData = pbData;
//
//	return(TRUE);
//}
//
//BOOL GenericKey(PBYTE* ppPublicKey, DWORD* pdwPublicKeyLength, PBYTE* ppPrivateKey, PDWORD pdwPrivateKeyLength)
//{
//	HCRYPTPROV hCryptProv = NULL;
//	HCRYPTKEY hCryptKey = NULL;
//	DWORD dwPrivateKeyLength = 0;
//	DWORD dwPublicKeyLength = 0;
//	PBYTE pPublicKey = NULL;
//	PBYTE pPrivateKey = NULL;
//	BOOL fOk = FALSE;
//
//	do
//	{
//		if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
//		{
//			break;
//		}
//		// 生成公私钥对
//		if (!CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hCryptKey))
//		{
//			break;
//		}
//		// 获取公钥的长度
//		if (!CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwPublicKeyLength))
//		{
//			break;
//		}
//		pPublicKey = new BYTE[dwPublicKeyLength];
//		RtlZeroMemory(pPublicKey, dwPublicKeyLength);
//		// 获取公钥
//		if (!CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, pPublicKey, &dwPublicKeyLength))
//		{
//			break;
//		}
//
//		// 获取私钥长度
//		if (!CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwPrivateKeyLength))
//		{
//			break;
//		}
//		pPrivateKey = new BYTE[dwPrivateKeyLength];
//		RtlZeroMemory(pPrivateKey, dwPrivateKeyLength);
//		// 生成私钥
//		if (!CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, pPrivateKey, &dwPrivateKeyLength))
//		{
//			break;
//		}
//		fOk = TRUE;
//		*ppPublicKey = pPublicKey;
//		*pdwPublicKeyLength = dwPublicKeyLength;
//		*ppPrivateKey = pPrivateKey;
//		*pdwPrivateKeyLength = dwPrivateKeyLength;
//	} while (FALSE);
//
//	if (hCryptKey)
//	{
//		CryptDestroyKey(hCryptKey);
//	}
//	if (hCryptProv)
//	{
//		CryptReleaseContext(hCryptProv, 0);
//	}
//
//	return(fOk);
//}
//
//BOOL RSAEncrypt(PBYTE pPublicKey, DWORD dwPublicKeyLength, PBYTE pData, DWORD& dwDataLength, DWORD dwBufferLength)
//{
//	BOOL fOk = FALSE;
//	HCRYPTPROV hCryptProv = NULL;
//	HCRYPTKEY hCryptKey = NULL;
//
//	do
//	{
//		fOk = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
//		if (!fOk)
//		{
//			break;
//		}
//		fOk = CryptImportKey(hCryptProv, pPublicKey, dwPublicKeyLength, NULL, 0, &hCryptKey);
//		if (!fOk)
//		{
//			break;
//		}
//		fOk = CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
//	} while (FALSE);
//
//	return(fOk);
//}
//
//BOOL RSADecrypt(PBYTE pPrivateKey, DWORD dwPrivateLength, PBYTE pData, DWORD& dwDataLength)
//{
//	BOOL fOk = FALSE;
//	HCRYPTPROV hCryptProv = NULL;
//	HCRYPTKEY hCryptKey = NULL;
//
//	do
//	{
//		if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
//		{
//			break;
//		}
//		if (!CryptImportKey(hCryptProv, pPrivateKey, dwPrivateLength, NULL, 0, &hCryptKey))
//		{
//			break;
//		}
//		if (!CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength))
//		{
//			break;
//		}
//		fOk = TRUE;
//	} while (FALSE);
//	if (hCryptKey)
//	{
//		CryptDestroyKey(hCryptKey);
//	}
//	if (hCryptProv)
//	{
//		CryptReleaseContext(hCryptProv, 0);
//	}
//
//	return(fOk);
//}
//
//#include <string.h>
//
//int rsa()
//{
//	char public_buff[] = "-----BEGIN PUBLIC KEY-----"
//		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCBZnAi4KV3QfaWeqbTvC4DnabAvtgq6DDtK7LXOBwMRqai5ldySlBsQb47hy + 2zYhGlpVhAgGO5xfI / DGPf8dYKF7qRXZVWtBE93Wtv2PHDu0eG + mEXKFEkM1XbmoQ9CCA0yOQyYa2DDI7guFRfLtdP2e + YrF3K8QnvecfDG54XQIDAQAB"
//		"-----END PUBLIC KEY-----";
//
//	char private_buff[] = "-----BEGIN PRIVATE KEY-----"
//		"MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIFmcCLgpXdB9pZ6ptO8LgOdpsC + 2CroMO0rstc4HAxGpqLmV3JKUGxBvjuHL7bNiEaWlWECAY7nF8j8MY9 / x1goXupFdlVa0ET3da2 / Y8cO7R4b6YRcoUSQzVduahD0IIDTI5DJhrYMMjuC4VF8u10 / Z75isXcrxCe95x8MbnhdAgMBAAECgYA1MSLPc4ERMPBokyqYxzYzDk9J1CmQGbpjBB + UEOCWu0Qsmns / 5ASDO9MzLTMT9hEa0zO8mNKDJ8PDPsgwsNoEbMH8GtjCnbL1toKTkWhLj1mpOvwCbiwB + 97KixiijTan55upu1uuT//ScMg7uzw934HMOxlX5+RqtIOYhFl/gQJBAOpfavE+6r01STOYwyZCJfnVG5NfF9OOi4KNZA97B25xXN2DcLpwnS1e/CPUEt4pQK+ii4GrsNQKuvcgYsaUkWkCQQCNV0FTfG3v68LZfPVa8pnm3I5eEkJ5p9/KFv2T49izBOGcHXiEtw4njoohyqVApbbi3TlLUpptzidmDlZUWhzVAkEAlLkV0J5fMM+GcwJmG3nwRTfQ9tRp9ypGJwL30sb8MT4ocrw8pWJeqvLSRx4FFwrQbGNV6WP8oqFogPcBFwghuQJAQka3GDnRp14rGSbSOlpmNm4XyB7R0xAYDUBomc+OjqtsS3M6TykzVvMs7JmEkeLi9lB8DEjopzKzINT6QEWdHQJBANA9FfdP7KB93+PgqugYVsYWrf/8GuPyROYdRQzMP96UH60MjiH3+Kqui3zu/eaUpBacZRQGojbrEpm0gpx1Qto="
//		"-----END PRIVATE KEY-----";
//
//	char tmp_buff[] = "hello world";
//	DWORD n_length = 0;
//	DWORD n_buff_length = 0;
//	
//	int n_public_key = strlen(public_buff);
//
//	RSAEncrypt((PBYTE)public_buff, n_public_key, (PBYTE)tmp_buff, n_length, n_buff_length);
//
//	int n = 999;
//
//	return 0;
//}



// CryptoApi_Rsa_Test.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>
#include <stdio.h>

void ShowError(char* pszText)
{
	char szErr[MAX_PATH] = { 0 };
	sprintf(szErr, "%s Error[0x%x]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	//MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);

	sprintf(szErr, "%s Error[0x%x]\n", pszText, ::GetLastError());
#endif
}


// 生成公钥和私钥
BOOL GenerateKey(BYTE** ppPublicKey, DWORD* pdwPublicKeyLength, BYTE** ppPrivateKey, DWORD* pdwPrivateKeyLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;
	BYTE* pPublicKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE* pPrivateKey = NULL;
	DWORD dwPrivateKeyLength = 0;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptAcquireContext");
			break;
		}

		// 生成公私密钥对
		bRet = ::CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptGenKey");
			break;
		}

		// 获取公钥密钥的长度和内容
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptExportKey");
			break;
		}
		pPublicKey = new BYTE[dwPublicKeyLength];
		::RtlZeroMemory(pPublicKey, dwPublicKeyLength);
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, pPublicKey, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptExportKey");
			break;
		}

		// 获取私钥密钥的长度和内容
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptExportKey");
			break;
		}
		pPrivateKey = new BYTE[dwPrivateKeyLength];
		::RtlZeroMemory(pPrivateKey, dwPrivateKeyLength);
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, pPrivateKey, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptExportKey");
			break;
		}

		// 返回数据
		*ppPublicKey = pPublicKey;
		*pdwPublicKeyLength = dwPublicKeyLength;
		*ppPrivateKey = pPrivateKey;
		*pdwPrivateKeyLength = dwPrivateKeyLength;

	} while (FALSE);

	// 释放关闭
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// 公钥加密数据
BOOL RsaEncrypt(BYTE* pPublicKey, DWORD dwPublicKeyLength, BYTE* pData, DWORD& dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptAcquireContext");
			break;
		}

		// 导入公钥
		bRet = ::CryptImportKey(hCryptProv, pPublicKey, dwPublicKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptImportKey");
			break;
		}

		// 加密数据
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptImportKey");
			break;
		}

	} while (FALSE);

	// 释放并关闭
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// 私钥解密数据
BOOL RsaDecrypt(BYTE* pPrivateKey, DWORD dwProvateKeyLength, BYTE* pData, DWORD& dwDataLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptAcquireContext");
			break;
		}

		// 导入私钥
		bRet = ::CryptImportKey(hCryptProv, pPrivateKey, dwProvateKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptImportKey");
			break;
		}

		// 解密数据
		bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (FALSE == bRet)
		{
			ShowError((char*)"CryptDecrypt");
			break;
		}

	} while (FALSE);

	// 释放并关闭
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}

#include <string.h>

int main(int argc, char* argv[])
{
	BYTE* pPublicKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE* pPrivateKey = NULL;
	DWORD dwPrivateKeyLength = 0;
	BYTE* pData = NULL;
	DWORD dwDataLength = 0;
	DWORD dwBufferLength = 4096;
	DWORD i = 0;

	pData = new BYTE[dwBufferLength];
	if (NULL == pData)
	{
		return 1;
	}
	::RtlZeroMemory(pData, dwBufferLength);
	strcpy((char*)pData, "What is your name? DemonGan");

	dwDataLength = 1 + ::strlen((char*)pData);
	printf("Text[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\n\n");

	// 生成公钥和私钥
	GenerateKey(&pPublicKey, &dwPublicKeyLength, &pPrivateKey, &dwPrivateKeyLength);
	printf("Public Key[%d]\n", dwPublicKeyLength);
	for (i = 0; i < dwPublicKeyLength; i++)
	{
		printf("%.2x", pPublicKey[i]);
	}
	printf("\n");
	printf("Private Key[%d]\n", dwPrivateKeyLength);
	for (i = 0; i < dwPrivateKeyLength; i++)
	{
		printf("%.2x", pPrivateKey[i]);
	}
	printf("\n\n");

	char public_buff[] = "-----BEGIN PUBLIC KEY-----"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCBZnAi4KV3QfaWeqbTvC4DnabAvtgq6DDtK7LXOBwMRqai5ldySlBsQb47hy+2zYhGlpVhAgGO5xfI/DGPf8dYKF7qRXZVWtBE93Wtv2PHDu0eG+mEXKFEkM1XbmoQ9CCA0yOQyYa2DDI7guFRfLtdP2e+YrF3K8QnvecfDG54XQIDAQAB"
		"-----END PUBLIC KEY-----";

	char private_buff[] = "-----BEGIN PRIVATE KEY-----"
		"MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIFmcCLgpXdB9pZ6ptO8LgOdpsC+2CroMO0rstc4HAxGpqLmV3JKUGxBvjuHL7bNiEaWlWECAY7nF8j8MY9/x1goXupFdlVa0ET3da2/Y8cO7R4b6YRcoUSQzVduahD0IIDTI5DJhrYMMjuC4VF8u10/Z75isXcrxCe95x8MbnhdAgMBAAECgYA1MSLPc4ERMPBokyqYxzYzDk9J1CmQGbpjBB+UEOCWu0Qsmns/5ASDO9MzLTMT9hEa0zO8mNKDJ8PDPsgwsNoEbMH8GtjCnbL1toKTkWhLj1mpOvwCbiwB+97KixiijTan55upu1uuT//ScMg7uzw934HMOxlX5+RqtIOYhFl/gQJBAOpfavE+6r01STOYwyZCJfnVG5NfF9OOi4KNZA97B25xXN2DcLpwnS1e/CPUEt4pQK+ii4GrsNQKuvcgYsaUkWkCQQCNV0FTfG3v68LZfPVa8pnm3I5eEkJ5p9/KFv2T49izBOGcHXiEtw4njoohyqVApbbi3TlLUpptzidmDlZUWhzVAkEAlLkV0J5fMM+GcwJmG3nwRTfQ9tRp9ypGJwL30sb8MT4ocrw8pWJeqvLSRx4FFwrQbGNV6WP8oqFogPcBFwghuQJAQka3GDnRp14rGSbSOlpmNm4XyB7R0xAYDUBomc+OjqtsS3M6TykzVvMs7JmEkeLi9lB8DEjopzKzINT6QEWdHQJBANA9FfdP7KB93+PgqugYVsYWrf/8GuPyROYdRQzMP96UH60MjiH3+Kqui3zu/eaUpBacZRQGojbrEpm0gpx1Qto="
		"-----END PRIVATE KEY-----";



	// 公钥加密
	RsaEncrypt((BYTE*)pPublicKey, dwPublicKeyLength, pData, dwDataLength, dwBufferLength);
	printf("RSA Encrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\n\n");

	// 私钥解密
	RsaDecrypt((BYTE*)pPrivateKey, dwPrivateKeyLength, pData, dwDataLength);
	printf("RSA Decrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\npData is: %s\n", pData);

	// 释放
	if (pData)
	{
		delete[]pData;
		pData = NULL;
	}
	if (pPrivateKey)
	{
		delete[]pPrivateKey;
		pPrivateKey = NULL;
	}
	if (pPublicKey)
	{
		delete[]pPublicKey;
		pPublicKey = NULL;
	}

	system("pause");
	return 0;
}
