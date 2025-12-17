#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXNUM(x, y) (x) > (y) ? (x) : (y)
#define SECOND2MAX(x, y, z, t) MAXNUM(x, y) > MAXNUM(z, t) ? MAXNUM(z, t) : MAXNUM(x, y)

void strrot(char str[])
{
    int len = strlen(str);
    for (int i = 0, k = len - 1; i < len / 2; i++, k--)
    {
        char temp = str[i];
        str[i] = str[k];
        str[k] = temp;
    }
    str[len] = '\0';
}

int isIdentifier(char *str)
{
    if (!str || !*str)
        return 0;

    if (!(isalpha((unsigned char)str[0]) || str[0] == '_'))
        return 0;

    for (int i = 1; str[i] != '\0'; i++)
    {
        if (!(isalnum((unsigned char)str[i]) || str[i] == '_'))
            return 0;
    }
    return 1;
}

int balanced(char *s)
{
    int one = 0, two = 0, three = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        switch (s[i])
        {
        case '(':
            one++;
            break;
        case '{':
            two++;
            break;
        case '[':
            three++;
            break;
        case ')':
            one--;
            break;
        case '}':
            two--;
            break;
        case ']':
            three--;
            break;

        default:
            break;
        }
    }
    return (one - two - three) > 0 ? 0 : 1;
}

int main()
{
    char text[50] = "Pesho is student in TUES!"; //! SEUT ni tneduts si ohseP
    strrot(text);
    printf("%s\n", text);

    printf("2max : %d\n", SECOND2MAX(1, 1, 4, 3));

    char str[20] = "0s", str2[20] = "_pwm5", str3[20] = "ddksk88+";
    printf("isIdent : %d\n", isIdentifier(text));
    printf("isIdent : %d\n", isIdentifier(str));
    printf("isIdent : %d\n", isIdentifier(str2));
    printf("isIdent : %d\n", isIdentifier(str3));

    printf("%d\n", balanced("()[]"));
    printf("%d\n", balanced("(}"));
    return 0;
}