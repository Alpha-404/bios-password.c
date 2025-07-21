#include "asus.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 32
#define ZERO_CODE ((int)'0')

typedef struct {
    char *ymd;
    char *dmy;
} RegexPair;

int shuffle1(int a1, int a2) {
    int v3 = 2;
    for (int i = 0; i < a2; i++) {
        int v4 = v3;
        int v5 = a1;
        while (v5 > 0) {
            if (v5 < v4) {
                //[v5, v4] = [v4, v5];
                int x = v5;
                v5 = v4;
                v4 = x;
            }
            v5 %= v4;
        }
        if (v4 != 1) {
            v3++;
        }
    }
    return v3;
}

int shuffle2(int a1, int a2, int a3) {
    if (a1 >= a3) {
        a1 %= a3;
    }
    int result = a1;
    if (a2 != 1) {
        for (int i = 0; i < a2 - 1; i++) {
            result = a1 * result % a3;
        }
    }
    return result;
}

void initTable(int *output, int a1, int a2, int a3) {
    if (a1 == 0) a1 = 11;
    if (a2 == 0) a2 = 19;
    if (a3 == 0) a3 = 6;

    int table[TABLE_SIZE];

    table[0] = a1 + ZERO_CODE;
    table[1] = a2 + ZERO_CODE;
    table[2] = a3 + ZERO_CODE;
    table[3] = '6';
    table[4] = '7';
    table[5] = '8';
    table[6] = '9';

    int chksum = 0;
    for (int i = 0; i < 7; i++) {
        chksum += table[i];
    }

    for (int i = 7; i < TABLE_SIZE; i++) {
        chksum = (33676 * chksum + 12345);
        chksum = (uint32_t)chksum;
        table[i] = ((chksum >> 16) & 0x7FFF) % 43 + ZERO_CODE;
    }

    int v3 = a1 * a2;
    int v4 = shuffle1((a1 - 1) * (a2 - 1), a3);

    for (int i = 0; i < TABLE_SIZE; i++) {
        output[i] = shuffle2(table[i] - ZERO_CODE, v4, v3);
    }
}

void leftPad(char *dest, int number, int length) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", number);
    int pad = length - strlen(buf);
    memset(dest, '0', pad);
    strcpy(dest + pad, buf);
    dest[length] = '\0';
}

char *asusKeygen(const char* serial) {
    int year, month, day;

    if (!sscanf(serial, "%d-%d-%d", &year, &month, &day)) {
        return "Invalid date, idk how ts got past the regex";
    }

    char *password = malloc(9);
    if (!password) return NULL;

    char date[9];
    int table[TABLE_SIZE];

    initTable(table, 11, 19, 6);

    char y[5], m[3], d[3];
    leftPad(y, year, 4);
    leftPad(m, month, 2);
    leftPad(d, day, 2);

    snprintf(date, sizeof(date), "%s%s%s", y, m, d);
    uint32_t chksum = (uint32_t)strtoul(date, NULL, 16);

    for (int i = 0; i < 8; i++) {
        chksum = (33676 * chksum + 12345) & 0xFFFFFFFF;
        int index = (chksum >> 16) & 31;
        int pwdChar = table[index] % 36;
        password[i] = (pwdChar > 9)
            ? (char)(pwdChar + '7')
            : (char)(pwdChar + '0');
    }

    password[8] = '\0';
    return password;
}

