#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

// ---------------- Helpers ----------------

// Convert a 32-bit unsigned integer to a binary string
static void to_32bit_binary(uint32_t value, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = (value >> i) & 1 ? '1' : '0';
    }
    out[32] = '\0';
}

// ---------------- Required Functions ----------------

// 1. Octal → Binary (3 bits per digit)
void oct_to_bin(const char *oct, char *out) {
    size_t len = strlen(oct);
    char buffer[256] = {0};  // generous space
    char *ptr = buffer;

    for (size_t i = 0; i < len; i++) {
        int digit = oct[i] - '0';
        ptr[0] = (digit & 4) ? '1' : '0';
        ptr[1] = (digit & 2) ? '1' : '0';
        ptr[2] = (digit & 1) ? '1' : '0';
        ptr += 3;
    }
    *ptr = '\0';
    strcpy(out, buffer);
}

// 2. Octal → Hex (via binary)
void oct_to_hex(const char *oct, char *out) {
    char bin[100];
    oct_to_bin(oct, bin);

    int len = strlen(bin);
    int pad = (4 - (len % 4)) % 4;

    // Shift and pad so length is multiple of 4
    char padded[100];
    for (int i = 0; i < pad; i++) {
        padded[i] = '0';
    }
    strcpy(padded + pad, bin);

    // Convert groups of 4 bits to hex
    int out_pos = 0;
    for (int i = 0; i < strlen(padded); i += 4) {
        int val = (padded[i] - '0') * 8 +
                  (padded[i+1] - '0') * 4 +
                  (padded[i+2] - '0') * 2 +
                  (padded[i+3] - '0');
        if (val < 10)
            out[out_pos++] = '0' + val;
        else
            out[out_pos++] = 'A' + (val - 10);
    }
    out[out_pos] = '\0';

    // Strip leading zeros, unless result is just "0"
    int start = 0;
    while (out[start] == '0' && out[start+1] != '\0') {
        start++;
    }
    if (start > 0) {
        memmove(out, out + start, strlen(out + start) + 1);
    }
}

// 3. Hex → Binary (4 bits per digit, keep leading zeros)
void hex_to_bin(const char *hex, char *out) {
    char *ptr = out;
    for (size_t i = 0; i < strlen(hex); i++) {
        int c = toupper(hex[i]);
        int val;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else val = 0;
        ptr[0] = (val & 8) ? '1' : '0';
        ptr[1] = (val & 4) ? '1' : '0';
        ptr[2] = (val & 2) ? '1' : '0';
        ptr[3] = (val & 1) ? '1' : '0';
        ptr += 4;
    }
    *ptr = '\0';
}

// 4. Sign-Magnitude
void to_sign_magnitude(int n, char *out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    } else {
        uint32_t mag = (uint32_t)(-n);
        uint32_t val = (1u << 31) | mag;
        to_32bit_binary(val, out);
    }
}

// 5. One’s Complement
void to_ones_complement(int n, char *out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    } else {
        uint32_t mag = (uint32_t)(-n);
        uint32_t flipped = ~mag;
        to_32bit_binary(flipped, out);
    }
}

// 6. Two’s Complement
void to_twos_complement(int n, char *out) {
    uint32_t val = (uint32_t)n;
    to_32bit_binary(val, out);
}