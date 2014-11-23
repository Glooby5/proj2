/*
 * Soubor:  proj2.c
 * Datum:   2012/11/23
 * Autor:   Jan Kaderabek, xkader13@fit.vutbr.cz
 * Projekt: Iteracni vypocty, c. 2 pro predmet IZP
 * Popis:   Program umoznuje vypcoet tangens pomoci Taylorova polynomu
            a zretezenych zlomku a nasledne porovnani s funkci z knihovny
            math.h. Dale umoznuje vypocet vzdalenosti a vysky mereneho objektu
            pomoci udaju ze senzoru.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPS 0.0000000001

typedef enum {  EOK, /* Vse v poradku */
                EPARCOUNT, /* Chybny pocet argumentu */
                EPARNOTNUM, /* Argument neni ciselna hodnota ikdyz je to ocekavano */
                EPARVALUE /* Hodnota prametru nesplnuje povoleny rozsah */
             } TStatus; /**< Reprezntuje kod chyboveho stavu */

typedef enum {  HELP,
                TAN,
                MEASURE
             }  TOperation; /**< Reprezntuje operaci kterou program bude provadet */

typedef struct {
                 TStatus status;
                 unsigned int N;
                 unsigned int M;
                 double A;
                 double B;
                 double X;
               } TValue; /**< Reprezntuje zadane hodnoty pro vypocty */

typedef struct { TStatus status;
                 TOperation operation;
                 TValue values;
               } TParams; /**< Reprezntuje kompletne zpracovane parametry pri spusteni programu */

const char *EMSG[] = {
    [EOK] = "Vse v poradku\n",
    [EPARCOUNT] = "Chybny pocet parametru!\n",
    [EPARNOTNUM] = "Ocekavany parametr neni cislo!\n",
    [EPARVALUE] = "Hodnota parametru nesplnuje povoleny rozsah!\n"
}; /**< Pole s chybovymi hlaskami */


void printHelp()
{
    printf("Pro vypocet tangens lze program spoustet nasledovne:\n");
    printf("./proj2 --tan A N M\n\n");
    printf("A - uhel alfa v radianech\n");
    printf("N a M - udava rozsah iteraci pro srovnani vypoctu (0 < N <= M < 14)\n\n");

    printf("Pro vzpocet a ymereni vydalensoti lze program spoustet nasledovne:\n");
    printf("./proj2 [-c X] -m A [B]\n\n");
    printf("A - uhel alfa v radianech (0 < A <= 1.4 < pi/2).\n");
    printf("B - uhel beta v radianech (0 < A <= 1.4 < pi/2). \n");
    printf("Argument -c nastavuje vysku mericiho pristroje c pro vypocet.\n");
    printf("Vyska c je dana argumentem X (0 < X <= 100).\nArgument je volitelny - implicitni vyska je 1.5 metru.\n");
}

/**
 * Z parametru programu nacte hodnoty pro vypocty tangens
 * @param argc Pocet argumentu
 * @param argv Pole argumentu
 * @return Struktura se zpracovanymi hodnotami
 */
TValue getTanValues(int argc, char *argv[])
{
    TValue values;

    if (argc < 5)
    {
        values.status = EPARCOUNT;
        return values;
    }

    values.status = EOK;
    char *endptr;

    values.A = strtod(argv[2], &endptr);

    if (endptr == argv[2] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
    }

    values.N = strtol(argv[3], &endptr, 10);

    if (endptr == argv[3] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
    }

    values.M = strtol(argv[4], &endptr, 10);

    if (endptr == argv[4] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
    }

    if (values.N > 0 && values.M >= values.N && values.M < 14)
    {
        return values;
    }

    values.status = EPARVALUE;
    return values;
}

/**
 * Z parametru programu nacte hodnoty pro vypocet vzdalenosti a vysky
 * @param argc Pocet argumentu
 * @param argv Pole argumentu
 * @return Struktura se zpracovanymi hodnotami
 */
TValue getMeasureValues(int argc, char *argv[])
{
    TValue values;

    if (argc < 3)
    {
        values.status = EPARCOUNT;
        return values;
    }

    values.status = EOK;
    values.B = -1;
    values.X = 1.5;
    char *endptr;

    values.A = strtod(argv[2], &endptr);

    if (endptr == argv[2] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
        return values;
    }

    if (values.A <= 0 || values.A > 1.4)
    {
        values.status = EPARVALUE;
        return values;
    }

    if (argc > 3)
    {
        values.B = strtod(argv[3], &endptr);

        if (endptr == argv[2] || *endptr != '\0')
        {
            values.status = EPARNOTNUM;
            return values;
        }

        if (values.B <= 0 || values.A > 1.4)
        {
            values.status = EPARVALUE;
        }
    }

    return values;
}

/**
 * Z parametru programu nacte hodnoty pro vypocet vzdalenosti a vysky
 * pokud je zadan nepovinny parametr pro C
 * @param argc Pocet argumentu
 * @param argv Pole argumentu
 * @return Struktura se zpracovanymi hodnotami
 */
TValue getEMeasureValues(int argc, char *argv[])
{
    TValue values;

    if (argc < 5)
    {
        values.status = EPARCOUNT;
        return values;
    }

    if (strcmp(argv[3], "-m"))
    {
        values.status = EPARVALUE;
        return values;
    }

    values.status = EOK;
    values.B = -1;
    char *endptr;

    values.X = strtod(argv[2], &endptr);

    if (endptr == argv[2] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
        return values;
    }

    if (values.X <= 0 || values.X > 100)
    {
        values.status = EPARVALUE;
    }

    values.A = strtod(argv[4], &endptr);

    if (endptr == argv[4] || *endptr != '\0')
    {
        values.status = EPARNOTNUM;
        return values;
    }

    if (values.A <= 0 || values.A > 1.4)
    {
        values.status = EPARVALUE;

    }

    if (argc > 5)
    {
        values.B = strtod(argv[5], &endptr);

        if (endptr == argv[5] || *endptr != '\0')
        {
            values.status = EPARNOTNUM;
            return values;
        }

        if (values.B <= 0 || values.A > 1.4)
        {
            values.status = EPARVALUE;
        }
    }

    return values;
}

/**
 * Zpracuje parametry zadane pri spusteni programu
 * @param argc Pocet argumentu
 * @param argv Pole argumentu
 * @return Struktura se zpracovanymi parametry
 */
TParams setArgs(int argc, char *argv[])
{
    TParams params;
    params.status = EOK;

    if (argc < 2)
    {
        params.status = EPARCOUNT;
        return params;
    }

    if (!strcmp(argv[1], "--help"))
    {
        params.operation = HELP;
        return params;
    }
    else if (!strcmp(argv[1], "--tan"))
    {
        params.operation = TAN;
        params.values = getTanValues(argc, argv);
        params.status = params.values.status;
    }
    else if (!strcmp(argv[1], "-m"))
    {
        params.operation = MEASURE;
        params.values = getMeasureValues(argc, argv);
        params.status = params.values.status;

    }
    else if (!strcmp(argv[1], "-c"))
    {
        params.operation = MEASURE;
        params.values = getEMeasureValues(argc, argv);
        params.status = params.values.status;
    }

    return params;
}

/**
 * Vypocita hodnotu tangens zadaneho uhlu pomoci Taylorova polynomu
 * @param x Uhel pro vypocet
 * @param n Rozvoj polynomu
 * @return Vysledny tangens
 */
double taylor_tan(double x, unsigned int n)
{
    double const numerator_array[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569,
        6404582, 443861162, 18888466084, 113927491862, 58870668456604};
    double const denominator_array[13] = {1, 3, 15, 315, 2835, 155925, 6081075,
        638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};

    double sum = 0.0;
    double powerx = x;

    for (unsigned int i = 0; i < n; i++)
    {
        sum += (powerx * numerator_array[i]) / denominator_array[i];
        powerx *= x * x;
    }

    return sum;
}

/**
 * Vypocita hodnotu tangens zadaneho uhlu pomoci zretezeneho zlomku
 * @param x Uhel pro vypocet
 * @param n Rozvoj zretezeneho zlomku
 * @return Vysledny tangens
 */
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

/**
 * Vypocita absolutni hodnotu
 * @param x Hodnota pro vypocet
 * @return Absolutni hodnota
 */
double my_fabs(double val)
{
    return (val < 0) ? -val : val;
}

/**
 * Spocita tangens nekolika zpusoby a vypocita absolutni chyby
 * @param val Hodnoty pro vypocet
 */
void calculateTan(TValue val)
{
    double tan_lib = tan(val.A);
    double tan_taylor;
    double tan_cfrac;
    double te;
    double ce;

    for (unsigned int i = val.N; i <= val.M; i++)
    {
        tan_taylor = taylor_tan(val.A, i);
        tan_cfrac = cfrac_tan(val.A, i);

        te = my_fabs(tan_taylor - tan_lib);
        ce = my_fabs(tan_cfrac - tan_lib);

        printf("%d %e %e %e %e %e\n", i, tan_lib, tan_taylor, te, tan_cfrac, ce);
    }
}

/**
 * Spocita tangens pomoci cfrac_tan() se zadanou presnosti
 * @param a Uhel pro vypocet
 * @param eps Presnost vypoctu
 * @return Tangens zadaneho uhlu
 */
double getAcurateTan(double a, double eps)
{
    double prevTan = 0.0;
    double actTan = cfrac_tan(a, eps);

    for (unsigned int i = 2; my_fabs(actTan - prevTan) > eps; i++)
    {
        prevTan = actTan;
        actTan = cfrac_tan(a, i);
    }

    return actTan;
}

/**
 * Spocita vzdalenost a vysku mereneho objektu
 * @param val Hodnoty pro vypocet
 */
void calculateDistance(TValue val)
{
    double distance = val.X / getAcurateTan(val.A, EPS);
    printf("%.10e\n", distance);

    if (val.B != -1)
    {
        double height = val.X + getAcurateTan(val.B, EPS) * distance;
        printf("%.10e\n", height);
    }
}

int main(int argc, char **argv)
{
    TParams params = setArgs(argc, argv);

    if (params.status != EOK)
    {
        fprintf(stderr, EMSG[params.status]);
        return EXIT_FAILURE;
    }

    switch (params.operation)
    {
        case HELP:
            printHelp();
            break;
        case TAN:
            calculateTan(params.values);
            break;
        case MEASURE:
            calculateDistance(params.values);
            break;
    }

    return EXIT_SUCCESS;
}
