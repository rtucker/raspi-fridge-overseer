#include <stdio.h>
#include "iface_w1_gpio.h"

char input_vectors[10][2] = {{0x07, 0xD0}, {0x05, 0x50}, {0x01, 0x91}, {0x00, 0xA2}, {0x00, 0x08}, {0x00, 0x00}, {0xFF, 0xF8}, {0xFF, 0x5E}, {0xFE, 0x6F}, {0xFC, 0x90}};
float output_vectors[10] = {125, 85, 25.0625, 10.125, 0.5, 0, -0.5, -10.125, -25.0625, -55};

char testit(char byte1, char byte2, float expected)
{
    float result;

    result = convertTemperature(byte1, byte2);

    if (result != expected)
    {
        fprintf(stderr, "invalid result: 0x%.2x%.2x -> %g (expected: %g)\n", byte1, byte2, result, expected);
        return -1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    int i;
    char result = 0;

    printf("testing temperature conversions:\n");
    for (i = 0; i < 10; i++)
    {
        printf("0x%.2x%.2x ?= %g: ", input_vectors[i][0], input_vectors[i][1], output_vectors[i]);
        if (testit(input_vectors[i][0], input_vectors[i][1], output_vectors[i]) == 0)
        {
            printf("ok\n");
        }
        else
        {
            printf("FAIL\n");
            result = -1;
        }
    }

    return result;
}
