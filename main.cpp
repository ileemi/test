
#include <iostream>
#include <openssl/aes.h>

using namespace std;

//对称加密的密钥
const char* mykey = "0123456789abcdef";

//原始明文
const char* mystr = "hello world, how are you, i am fine, think you hahah";

void decrypto(const char* ciphertext, const unsigned char* key, unsigned char* iv, unsigned char** outText)
{
    AES_KEY aes;
    AES_set_decrypt_key(key, 128, &aes);

    int datalen = strlen(mystr) + 1;

    unsigned char* decrypt = (unsigned char*)calloc(datalen, 1);
    AES_cbc_encrypt((unsigned char*)ciphertext, decrypt, datalen, &aes, iv, AES_DECRYPT);

    *outText = decrypt;
}

void encryptoText(const char* plainText, const unsigned char* key, unsigned char* iv, unsigned char** outText)
{
    AES_KEY aes;
    AES_set_encrypt_key(key, 128, &aes); //参数2为比特位

    //向上调整为16的倍数
    int datalen;
    if ((strlen(plainText) + 1) % 16 == 0)
    {
        datalen = strlen(plainText) + 1;
    }
    else
    {
        datalen = ((strlen(plainText) + 1) / 16 + 1) * 16;
    }
    unsigned char* cipherText = (unsigned char*)calloc(datalen, 1);

    AES_cbc_encrypt((unsigned char*)plainText, cipherText, datalen, &aes, iv, AES_ENCRYPT);

    *outText = cipherText;

}

int main()
{

    ////初始化向量，长度和密钥一致
    //unsigned char iv[16] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

    //unsigned char* cipher = NULL;
    ////加密
    //encryptoText(mystr, (unsigned char*)mykey, iv, &cipher);
    //cout << "密文：" << cipher << endl;
    //cout << "------------" << endl;

    unsigned char tmp_buff[] = {
        0x53, 0xbf, 0xe8, 0x4b, 0xe5, 0x89, 0x1a, 0x37, 
        0x00, 0x36, 0x87, 0x7e, 0x4c, 0xe6, 0xdc, 0x91,
        0x1a, 0xac, 0x47, 0x83, 0x0e, 0x95, 0x53, 0xca, 
        0xeb, 0xfb, 0xb7, 0x10, 0x98, 0x34, 0x08, 0xfd,
        0x4f, 0xb4, 0x32, 0xa9, 0xf4, 0x45, 0x04, 0xc0, 
        0x32, 0xb8, 0x39, 0x19, 0xca, 0xd6, 0xab, 0xdc
    };

    unsigned char key_buff[] = {
        0xcc, 0x8c, 0xd4, 0x1c, 0xef, 0x90, 0x7c, 0x4d, 
        0x21, 0x60, 0x69, 0x12, 0x2c, 0x4b, 0x89, 0x93
    };
    

    //解密
    unsigned char* text = NULL;

    unsigned char iv[16] = { 
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p' 
    };

    //memset(iv, 'a', sizeof(iv));
    decrypto((char*)tmp_buff, (unsigned char*)key_buff, iv, &text);
    cout << "明文：" << text << endl;
    //free(cipher);
    //free(text);

    return 0;
}