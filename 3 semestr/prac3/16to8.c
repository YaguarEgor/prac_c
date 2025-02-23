#include <stdio.h>
#include <string.h>
#include "both.h"


unsigned short reverse(unsigned short utf16) {
    return (utf16 << 8) | (utf16 >> 8);
}

void convert(FILE* inp, FILE* out, int BOM) {

    unsigned short utf16;
    fread(&utf16, sizeof(unsigned short), 1, inp);
    if (utf16 == 0xFEFF) {
        BOM = LE;
        fread(&utf16, sizeof(unsigned short), 1, inp);
    }
    else if (utf16 == 0xFFFE){
        BOM = BE;
        fread(&utf16, sizeof(unsigned short), 1, inp);
    }
    do {
        if (BOM == BE) {
            utf16 = reverse(utf16);
        }
        if (utf16 <= 0x7F) {
            
            char utf8 = (char)utf16;
            fwrite(&utf8, 1, 1, out);
        } else if (utf16 <= 0x7FF) {
            char utf8[2];
            utf8[0] = 0xC0 | (char)((utf16 >> 6) & 0x1F);
            utf8[1] = 0x80 | (char)(utf16 & 0x3F);
            fwrite(utf8, 1, 2, out);
        } else {
            char utf8[3];
            utf8[0] = 0xE0 | (char)((utf16 >> 12) & 0x0F);
            utf8[1] = 0x80 | (char)((utf16 >> 6) & 0x3F);
            utf8[2] = 0x80 | (char)(utf16 & 0x3F);
            fwrite(utf8, 1, 3, out);
        }
    }
    while (fread(&utf16, sizeof(unsigned short), 1, inp) == 1);
}

int main(int argc, char** argv) {
    char* input_name = NULL;
    char* output_name = NULL;
    int BOM = getArgumentsFromCMD(&input_name, &output_name, argc, argv);
    if (BOM == -1) {
        return 1;
    }
    FILE* input = fopen(input_name, "r");
    FILE* output = fopen(output_name, "w");
    input = input ? input : stdin;
    output = output ? output : stdout;
    convert(input, output, BOM);
    fclose(input);
    fclose(output);
    return 0;
}