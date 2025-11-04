#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS_LEN 65
static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

int convertCharToInt(char a)
{
    for (int i = 0; i < DIGITS_LEN - 1; i++)
    {
        if (digits[i] == a)
            return i;
    }
    return -1;
}

void reverseString(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int isValidNumber(const char *number, int baseFrom)
{
    int start = (number[0] == '-') ? 1 : 0;
    for (int i = start; number[i] != '\0'; i++)
    {
        int val = convertCharToInt(number[i]);
        if (val == -1 || val >= baseFrom)
            return 0;
    }
    return 1;
}

unsigned long long myPow(unsigned base, unsigned exp)
{
    unsigned long long result = 1;
    while (exp > 0)
    {
        result *= base;
        --exp;
    }
    return result;
}

void addOneToBinary(char *binStr)
{
    int i = strlen(binStr) - 1; // skip null terminator
    while (i >= 0)
    {
        if (binStr[i] == '0')
        {
            binStr[i] = '1';
            break;
        }
        else
        {
            binStr[i] = '0';
            i--;
        }
    }
}

void invertBits(char *binStr)
{
    for (int i = 0; binStr[i] != '\0'; i++)
    {
        binStr[i] = (binStr[i] == '0' ? '1' : '0');
    }
}

// for 64 bit system -> 8bytes
char *base2base(char *res, int baseFrom, int baseTo, const char *number)
{
    // validation
    if (baseFrom < 2 || baseFrom > 64 || baseTo < 2 || baseTo > 64)
        return NULL;
    if (!isValidNumber(number, baseFrom))
    {
        printf("You use the wrong symbols not allowed in this system!");
        return NULL;
    }
    if (baseFrom != 10 && number[0] == '-')
    {
        printf("\nError not allowed '-' exept in decimal system!");
        return NULL;
    }

    // catching negative numbers
    if (number[0] == '-')
    {
        char bin[200] = {[0 ... 198] '0', [199] '\0'};
        base2base(bin + 64, 10, 2, number + 1);
        char *p = bin + 200 - 64 - 1; // rereferencing the pointer to last 64 digits

        invertBits(p);     // inverting
        addOneToBinary(p); // adding 1
        base2base(res, 2, baseTo, p);
        return res;
    }
    if (baseFrom != 10 && baseTo == 10)
    {
        char binary[64 + 1] = {'\0'}; // for determinating 0
        base2base(binary, baseFrom, 2, number);
        if (binary[0] == '1' && binary[63] != '\0') // check if it is negative and to be full number
        {
            invertBits(binary);     // inverting
            addOneToBinary(binary); // adding 1

            base2base(res + 1, 2, 10, binary);
            res[0] = '-';
            return res;
        }
    }
    // convertion number to decimal system
    size_t decNum = 0;
    int k = 0;
    for (int i = strlen(number) - 1; i >= 0; i--)
    {
        decNum += convertCharToInt(number[i]) * myPow(baseFrom, k++);
    }
    // check for overflow
    if (decNum < 0)
    {
        printf("\nError too big for convertion!");
        return NULL;
    }
    //  convertion to target system
    int remainder = 1;
    for (int i = 0; decNum != 0; i++)
    {
        res[i] = digits[decNum % baseTo];
        decNum /= baseTo;
    }
    reverseString(res);

    return res;
}
//to run gcc main.c -o main.out && ./main.out 16 10 FFFFFFFFFFFF12FC
int main(int argc, char const *argv[])
{
    if (argc != 4)
        return 1;
    char str[200] = {'\0'};
    base2base(str, atoi(argv[1]), atoi(argv[2]), argv[3]);
    int i = 0;
    while (str[i] != '\0')
    {
        if (i % 4 == 0 && i != 0)
        {
            printf(" ");
        }
        printf("%c", str[i++]);
    }
    printf("\n");

    return 0;
}
