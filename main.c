#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// prototypes
void oct_to_bin(const char*, char*);
void oct_to_hex(const char*, char*);
void hex_to_bin(const char*, char*);
void to_sign_magnitude(int, char*);
void to_ones_complement(int, char*);
void to_twos_complement(int, char*);

int main(void) {
    FILE *f = fopen("A2_test.txt", "r");
    if (!f) { perror("A2_test.txt"); return 1; }

    char func[64], arg1[64], expected[128];
    char line[256];
    int testnum = 1, pass = 0, total = 0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || strlen(line) <= 1) continue;
        int n = sscanf(line, "%s %s %s", func, arg1, expected);
        if (n < 3) continue;

        char actual[256];
        if (strcmp(func, "oct_to_bin") == 0) {
            oct_to_bin(arg1, actual);
        } else if (strcmp(func, "oct_to_hex") == 0) {
            oct_to_hex(arg1, actual);
        } else if (strcmp(func, "hex_to_bin") == 0) {
            hex_to_bin(arg1, actual);
        } else if (strcmp(func, "to_sign_magnitude") == 0) {
            to_sign_magnitude(atoi(arg1), actual);
        } else if (strcmp(func, "to_ones_complement") == 0) {
            to_ones_complement(atoi(arg1), actual);
        } else if (strcmp(func, "to_twos_complement") == 0) {
            to_twos_complement(atoi(arg1), actual);
        } else continue;

        total++;
        int ok = strcmp(actual, expected) == 0;
        if (ok) pass++;

        printf("Test %d: %s(%s) -> Expected: %s, Got: %s [%s]\n",
               testnum++, func, arg1, expected, actual, ok ? "PASS" : "FAIL");
    }
    fclose(f);

    printf("Summary: %d/%d tests passed\n", pass, total);
    return 0;
}