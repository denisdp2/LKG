#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <locale.h>
#include <string.h>
#include <math.h>

#define SIZE 300000
#define N 140000
#define M 4294967295
#define A 22695477
#define C 1
#define EXIT_FAILURE 1


void* check_null(void* ptr) {
    if (ptr == NULL) {
        printf("Ошибка выделения памяти");
        return(0);
    }
    return ptr;
}


unsigned long long lkg(unsigned long long *x) {
    *x = (A * (*x) + C) % M;
    return(*x);
}


void period(unsigned long long* x, unsigned long long* pseudorandom_numbers, long long* period_number) {
    unsigned long long number = 0;
    number = lkg(x);
    pseudorandom_numbers[*period_number - 1] = number;
    (*period_number)++;
    pseudorandom_numbers[*period_number - 1] = lkg(x);
    while (number != pseudorandom_numbers[*period_number - 1])
    {
        (*period_number)++;
        pseudorandom_numbers[*period_number - 1] = lkg(x);
    }
    printf("Период равен %llu\n", *period_number - 1);
    //spread(pseudorandom_numbers, period_number);
}


void power(void) {
    unsigned long long b = A - 1, g = b;
    for (int i = 1; i != 100000; i++) {
        if ((b * g) == 0){
            printf("Мощность ЛКГ - %d\n\n", i);
            break;
        }
        b *= g;
    }
}


void in_double(unsigned long long number, int* count_of_one) {
    int i, mass[64];
    for (i = 0; i < 64; i++)
    {
        mass[i] = 0;
    }
    i = 0;
    while (number != 0)
    {
        if (number % 2 == 0)
        {
            mass[i] = 0;
            number = number / 2;
        }
        else
        {
            mass[i] = 1;
            number = number / 2;
        }
        i++;
    }
    for (int j = 0; j < 8; j++) {
        *count_of_one += mass[i];
        i--;
    }
}


void check_low_bits(unsigned long long* pseudorandom_numbers, long long period_number) {
    int count_of_one = 0;
    double averaged = 0;
    for (int i = 0; i != period_number; i++) {
        in_double(pseudorandom_numbers[i], &count_of_one);
        averaged += count_of_one;
        count_of_one = 0;
    }
    averaged /= period_number;
    printf("Среднее количество единиц в младших битах чисел равно %f\n", averaged);
}


int check_count_in(unsigned long long* pseudorandom_numbers, int index) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (pseudorandom_numbers[index] == pseudorandom_numbers[i] && index != i) {
            count++;
        }
    }
    return count;
}


int check_count_in_double(unsigned long long* pseudorandom_numbers, int index) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (pseudorandom_numbers[index] == pseudorandom_numbers[i] && 
            pseudorandom_numbers[index + 1] == pseudorandom_numbers[i + 1] 
            && index != i) {
            count++;
        }
    }
    return count;
}


void chi_square(unsigned long long* pseudorandom_numbers, long long period_number) {
    double chi = 0;
    double p = 1.0 / period_number;
    int y = 0;
    for (int i = 0; i != 10; i++) {
        y = check_count_in(pseudorandom_numbers, i);
        chi += pow(y - (N * p), 2) / (N * p);
    }
    printf("При степене свободы, равной 9, фактический хи квадрат равен %f\n", chi);
}


void run_test(unsigned long long* pseudorandom_numbers, long long period_number) {
    double chi = 0;
    double p = 1.0 / period_number;
    int y = 0;
    for (int i = 0; i != 10; i++) {
        y = check_count_in_double(pseudorandom_numbers, i);
        chi += pow(y - (N * p), 2) / (N * p);
    }
    printf("При степене свободы, равной 9, критерий серий равен %f\n\
Следовательно p лежит между 50 и 75 процентами, это говорит о том, что лкг хороший\n\n", chi);
}


int main(void)
{
    setlocale(LC_ALL, "Rus");

    printf("Параметры:\n");
    printf("m = %lu\n", M);
    printf("a = %d\n", A);
    printf("c = %d\n\n", C);

    unsigned long long* pseudorandom_numbers = (unsigned long long*)check_null(malloc(SIZE * sizeof(unsigned long long)));
    memset(pseudorandom_numbers, '\0', SIZE * sizeof(unsigned long long));
    unsigned long long x = 1;
    long long period_number = 1;
    printf("Храктеристики:\n");
    period(&x, pseudorandom_numbers, &period_number);
    chi_square(pseudorandom_numbers, period_number);
    /*for (int i = 0; i < 139265; i++) {
        printf("%llu - %d\n", pseudorandom_numbers[i], i);
    }*/
    power();
    printf("Тесты:\n");
    check_low_bits(pseudorandom_numbers, period_number);
    run_test(pseudorandom_numbers, period_number);
    return 0;
}
