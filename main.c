#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {  OK,
                HELP,
                ERROR
             } TStatus;

typedef enum {  TAN,
                MESSURING,
                EMESSIRING
             }  TOperation;

typedef struct { int N;
                 int M;
                 int A;
                 int B;
                 int X;
               } TValue;

typedef struct { TStatus status;
                 TOperation operation,
                 TValue values;
               } TParams;

TParams setArgs(int argc, char *argv[])
{
    TParams params;
    params.status = OK;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "--help"))
        {
            params.status = HELP;
            return params;
        }
        else if (!strcmp(argv[1], "--tan"))
        {
            params.operation = TAN;
        }
        else if (!strcmp(argv[1], "-m"))
        {
            //nacist A a B
        }
        else if (!strcmp(argv[1], "-c"))
        {
            //mereni se zadanou vyskou
            //nnacis X a overit jestli dalsi argument je -m a nacist A a B
        }

    }
}

double const numerator_array[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
double const denominator_array[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};

double taylor_tan(double x, unsigned int n)
{
    double sum = 0.0;
    double powerx = x;

    for (unsigned int i = 0; i < n; i++)
    {
        sum += (powerx * numerator_array[i]) / denominator_array[i];
        powerx *= x * x;
    }

    return sum;
}

double cfrac_tan(double x, unsigned int n)
{
    double zlomek = 1.0;
    double scitanec, citatel;
    citatel = x * x;

    for (; n >= 1; n--)
    {
        scitanec = (n + 1)*2-1;
        zlomek = citatel / (scitanec - zlomek);
    }

    return x / (1 - zlomek);
}

int main(int argc, int **argv)
{
    printf("%e \n", tan(1.024));
    printf("%e \n", taylor_tan(1.024, 10));
    printf("%e \n", cfrac_tan(1.024, 10));
    return 0;
}
