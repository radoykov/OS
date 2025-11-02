#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getCurrDigit(int num, int reset)
{
    static int a = 1;
     if (reset) {
        a = 1;
        return 0;
    }
    int digit = (num / a) % 10;
    a *= 10;
    return digit;
}

void num2str(int num, char *result)
{
    if (num == 0)
    {
        strcpy(result, "zero");
        return;
    }

    if (num < 0)
    {
        strcpy(result, "minus ");
        num = -num;
    }

    int len = snprintf(NULL, 0, "%d", num);
    if (len > 12)
        return;

    const char *ones[] = {
        "", "one", "two", "three", "four", "five",
        "six", "seven", "eight", "nine", "ten",
        "eleven", "twelve", "thirteen", "fourteen", "fifteen",
        "sixteen", "seventeen", "eighteen", "nineteen"};
    const char *tens[] = {
        "", "", "twenty", "thirty", "forty", "fifty",
        "sixty", "seventy", "eighty", "ninety"};
    const char *minions[] = {"", "thousand", "million", "billion"};

    // reset digit extractor
    getCurrDigit(num, 1);

    int num_groups = (len + 2) / 3; // number of 3-digit groups
    char parts[8][100];             
    int parts_cnt = 0;

    for (int i = 0; i < num_groups; i++)
    {
        int c = getCurrDigit(num, 0); // units
        int b = getCurrDigit(num, 0); // tens
        int a = getCurrDigit(num, 0); // hundreds

        int group = a * 100 + b * 10 + c;
        if (group == 0)
            continue; 

        char part[100] = {'\0'};
        // hundreds
        if (group >= 100)
        {
            sprintf(part + strlen(part), "%s hundred", ones[a]);
        }

        // last two digits
        int t = group % 100;
        if (t)
        {
            if (strlen(part))
                strcat(part, " ");
            if (t < 20)
            {
                strcat(part, ones[t]);
            }
            else
            {
                int tt = t / 10;
                int tu = t % 10;
                strcat(part, tens[tt]);
                if (tu)
                {
                    strcat(part, " ");
                    strcat(part, ones[tu]);
                }
            }
        }

        // append thousand, million, ...
        if (minions[i][0] != '\0')
        {
            strcat(part, " ");
            strcat(part, minions[i]);
        }

        // store this group's text descendingly
        strncpy(parts[parts_cnt++], part, sizeof(parts[0]) - 1);
        parts[parts_cnt - 1][sizeof(parts[0]) - 1] = '\0';
    }

    // build final result in correct order
    for (int i = parts_cnt - 1; i >= 0; i--)
    {
        if (strlen(result))
            strcat(result, " ");
        strcat(result, parts[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    int num = atoi(argv[1]);

    char buffer[100] = {'\0'};
    num2str(num, buffer);
    printf("%s\n", buffer);

    return 0;
}