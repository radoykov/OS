#include <stdio.h>
#include <math.h>

//Newton–Raphson Cube Root    O(loglog(1/ε))
double root_3(double a)
{
    double eps = 1e-9;
    if (a == 0.0)
        return 0.0;

    double x = a > 1 ? a / 3.0 : 1.0; // initial guess
    double next;

    do
    {
        next = (2.0 * x + a / (x * x)) / 3.0;
        if (fabs(next - x) < eps)
            break;
        x = next;
    } while (1);

    return next;
}

int main()
{
    int val;
    printf("Input : ");
    scanf("%d", &val);

    printf("Cube root of %d ≈ %f\n", val,  root_3(val));

    return 0;
}