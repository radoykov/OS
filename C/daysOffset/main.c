#include <stdio.h>

typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

int isLeap(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int daysInMonth(int month, int year)
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeap(year))
        return 29;
    return days[month - 1];
}

int dateToDays(Date d)
{
    int days = d.day;
    // Add days for months before current month
    for (int m = 1; m < d.month; m++)
    {
        days += daysInMonth(m, d.year);
    }
    // Add days for years before current year
    for (int y = 1800; y < d.year; y++) // 1800 here we can adjust the range and improve our complexity
    {
        days += isLeap(y) ? 366 : 365;
    }
    return days;
}

int daysBetween(Date *start, Date *end)
{
    int startDays = dateToDays(*start);
    int endDays = dateToDays(*end);
    return endDays - startDays;
}

int main()
{
    Date d1 = {8, 8, 2020};
    Date d2 = {8, 8, 2100};
    printf("Days between: %d\n", daysBetween(&d1, &d2));
    return 0;
}
