#include <stdio.h>
#include <ctype.h>

int itoa(int n, char s[]) {
    sprintf(s, "%d", n);  
    return 0;
}

int atoi(int *n, char s[]) {
    int sign = 1, i = 0, result = 0;

    if (s[i] == '-') { sign = -1; i++; }
    else if (s[i] == '+') i++;

    if (!isdigit((unsigned char)s[i])) return -1;

    for (; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return -1;
        result = result * 10 + (s[i] - '0');
    }

    *n = sign * result;
    return 0;
}

int main() {
    char buf[20];
    int num;

    itoa(-1234, buf);
    printf("itoa: %s\n", buf);

    if (atoi(&num, "-1234") == 0) printf("atoi: %d\n", num);
    else printf("atoi: Грешка\n");

    if (atoi(&num, "asdf") == 0) printf("atoi: %d\n", num);
    else printf("atoi: Грешка\n");

    return 0;
}
