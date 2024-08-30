#include <iostream>
#include <random>
#include <string.h>
#include <ctime>
#include <stdlib.h>
using namespace std;


long long int p, q, n, t, isPrime, *publicExponents, *privateExponents, *temp, *decryptedMessage, i;
char *encryptedMessage;
char inputMessage[1000000];
char message[1000000];


long long int prime(long long int);
long long int calculatePrivateKey(long long int);
void encryptMessage();
void calculatePublicExponents();
long long int modularExponentiation(long long int base, long long int exponent, long long int mod);

long long int prime(long long int pr)
{
    long long int i, j;
    j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

void generateRandomPrimes()
{
    srand(time(0)); // Seed the random number generator

    do
    {
        p = rand() % 9999 + 1000; // Generate a random number between 1 and 1000
    } while (!prime(p));

    do
    {
        q = rand() % 9999 + 1000; // Generate a random number between 1 and 1000
    } while (!prime(q));
}

void calculatePublicExponents()
{
    long long int k = 0; // keep track of how many potential public exponents have been found
    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        isPrime = prime(i);
        if (isPrime == 1 && i != p && i != q)
        {
            publicExponents[k] = i;
            isPrime = calculatePrivateKey(publicExponents[k]);
            if (isPrime > 0)
            {
                privateExponents[k] = isPrime;
                k++;
            }
            if (k == 99)
                break;
        }
    }
}

long long int calculatePrivateKey(long long int x)
{
    long long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

void encryptMessage()
{
    FILE *keyFile;
    FILE *encryptedFile;
  
    keyFile = fopen("SecondKey.txt", "wb+");
    encryptedFile = fopen("Encrypted.txt", "wb+");
    long long int plaintext, ciphertext, key = publicExponents[0], k, length;
    i = 0;
    length = strlen(inputMessage);
    while (i != length)
    {
        plaintext = message[i];
        plaintext = plaintext - 96;

        k = modularExponentiation(plaintext, key, n);

        temp[i] = k;
        fprintf(keyFile, "%d ", temp[i]);
        ciphertext = k + 96;
        fprintf(encryptedFile, "%c", ciphertext);
        encryptedMessage[i] = ciphertext;
        i++;
    }
    encryptedMessage[i] = -1;
    printf("\n\n\nTHE ENCRYPTED MESSAGE IS :- \n\n");
    for (i = 0; encryptedMessage[i] != -1; i++)
        printf("%c", encryptedMessage[i]);
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
    publicExponents = (long long int *)malloc(10000000 * sizeof(long long int));
    privateExponents = (long long int *)malloc(10000000 * sizeof(long long int));
    temp = (long long int *)malloc(10000000 * sizeof(long long int));
    decryptedMessage = (long long int *)malloc(10000000 * sizeof(long long int));
    encryptedMessage = (char *)malloc(10000000 * sizeof(char));

    generateRandomPrimes(); // Generates random prime numbers for p and q
    cout << "Value of P is : " << p << endl;
    cout << "Value of Q is : " << q << endl;

    fflush(stdin);
    char choice;
    FILE *filePointer;

    char ch = 0, filename[100];
    printf("\nIf you want to choose a different file, press 'x' else press 'Enter' : ");
    scanf("%c", &choice);
    if (choice == 'x' || choice == 'X')
    {
        printf("\nEnter the filename to open (with extensions) : ");
        scanf("%s", &filename);
        filePointer = fopen(filename, "rb");
    }
    else
    {
        filePointer = fopen("msg.txt", "r");
    }
    long long int x = 0;
    printf("\n\nMESSAGE FOR ENCRYPTION :- \n");
    do
    {
        ch = getc(filePointer);
        i = ch;
        if (i != -1)
        {
            printf("%c", ch);
            inputMessage[x] = i;
            x++;
        }
    } while (ch != EOF);

    for (i = 0; inputMessage[i] != '\0'; i++)
        message[i] = inputMessage[i];
        
    n = p * q;
    t = (p - 1) * (q - 1);
    calculatePublicExponents();
    encryptMessage();
    printf("\n\n\n");

    cout << "Public Key  ( e , n ) : " << "( " << publicExponents[0] << " , " << n << " )" << endl;
    cout << "Private Key  ( d , n ) : " << "( " << privateExponents[0] << " , " << n << " )" << endl;

    free(publicExponents);
    free(privateExponents);
    free(temp);
    free(decryptedMessage);
    free(encryptedMessage);

    return 0;
}
