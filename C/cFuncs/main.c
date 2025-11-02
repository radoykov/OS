#include <stdio.h>

void myStrcpy(char *destination, const char *source)
{
    unsigned long len = sizeof(source) / sizeof(char);
    unsigned i = 0;
    while (source[i] != '\0')
    {
        destination[i++] = source[i];
    }
    destination[i] = '\0';
}
int myStrcmp(const char *str1, const char *str2)
{
    unsigned i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return (str1[i] > str2[i]) ? 1 : -1;
        }
        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0')
    {
        return 0;
    }
    return (str1[i] == '\0') ? -1 : 1;
}
void *myMemcpy(void *destination, const void *source, size_t num)
{
    unsigned char *dest = (unsigned char *)destination;
    const unsigned char *src = (const unsigned char *)source;

    for (size_t i = 0; i < num; i++)
    {
        dest[i] = src[i];
    }
    return dest;
}
int myMemcmp(const void *dest, const void *src, size_t num)
{
    const unsigned char *d = (const unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < num; i++)
    {
        if (d[i] != s[i])
        {
            return (d[i] > s[i]) ? 1 : -1;
        }
    }
    return 0;
}

void *myMemset(void *pointer, int val, size_t num)
{
    unsigned char *ptr = (unsigned char *)pointer; // Remove const to allow writing
    for (size_t i = 0; i < num; i++)
    {
        ptr[i] = (unsigned char)val;
    }

    return pointer;
}

int main()
{
    char str1[20];
    myStrcpy(str1, "Hi, I am copied!");
    printf("Strcpy : %s\n", str1);

    char str2[20] = {'H', 'i'};
    char str3[20] = {'A', 'l', 'e', 'x', '!'};
    printf("%d\n", myStrcmp(str2, str3));

    struct
    {
        char name[40];
        int age;
    } person, person_copy;

    person.age = 17;
    myStrcpy(person.name, "Pesho");

    myMemcpy(&person_copy, &person, sizeof(person));
    printf("person_copy: %s, %d \n", person_copy.name, person_copy.age);

    person.age = 18;
    printf("%d\n", myMemcmp(&person, &person_copy, sizeof(person)));

    printf("Last : %s", (char *)myMemset(str2, '0', 1));

    return 0;
}