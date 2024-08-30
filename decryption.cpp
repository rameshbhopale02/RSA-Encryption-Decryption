#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

long long int *temp, *decryptedMessage, i;
char *encryptedMessage;

void decryptMessage();
void readEncryptedMessage();
long long int modularExponentiation(long long int base, long long int exponent, long long int mod);

void readEncryptedMessage()
{

    FILE *keyFile;
    FILE *encryptedFile;
    keyFile = fopen("SecondKey.txt", "r");
    encryptedFile = fopen("Encrypted.txt", "r");

    long long int x = 0, sum = 0, isNegative = 0, ch = 0;
    do
    {
        ch = getc(keyFile);
        if (ch == '-')
        {
            isNegative = 1;
        }
        if (ch != ' ')
        {
            if (ch >= '0' && ch <= '9')
            {
                long long int digit = ch - '0';
                sum = sum + digit;
                sum = sum * 10;
            }
        }
        else
        {
            if (isNegative)
            {
                sum = sum / 10;
                sum = sum - (sum * 2);
                isNegative = 0;
                temp[x] = sum;
                x++;
                sum = 0;
            }
            else
            {
                sum = sum / 10;
                temp[x] = sum;
                x++;
                sum = 0;
            }
        }
    } while (ch != EOF);

    x = 0;
    do
    {
        ch = getc(encryptedFile);
        if (ch != -1)
        {
            encryptedMessage[x] = ch;
            x++;
        }
    } while (ch != EOF);

    encryptedMessage[x] = -1;
}

void decryptMessage(long long int key, long long int n)
{
    FILE *resultFile;
    resultFile = fopen("Result.txt", "wb+");

    long long int ct, k;
    i = 0;
    while (encryptedMessage[i] != -1)
    {
        ct = temp[i];
        k = modularExponentiation(ct, key, n);
        decryptedMessage[i] = k + 96;
        i++;
    }
    decryptedMessage[i] = -1;
    printf("\n\nTHE DECRYPTED MESSAGE IS : - \n\n");
    for (i = 0; decryptedMessage[i] != -1; i++)
    {
        fprintf(resultFile, "%c", decryptedMessage[i]);
        printf("%c", decryptedMessage[i]);
    }
    printf("\n");
}

long long int modularExponentiation(long long int base, long long int exponent, long long int mod)
{
    if (exponent == 0)
        return 1;

    long long int result = 1;
    base = base % mod;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent = exponent >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main()
{

    encryptedMessage = (char *)malloc(10000000 * sizeof(char));
    decryptedMessage = (long long int *)malloc(10000000 * sizeof(long long int));
    temp = (long long int *)malloc(10000000 * sizeof(long long int));

    long long int key;
    long long int n;

    cout << endl
         << "Enter Private key (d, n)  : "; 
    cin >> key >> n;

    fflush(stdin);

    readEncryptedMessage();
    decryptMessage(key, n);
    printf("\n\n");
    return 0;
}
