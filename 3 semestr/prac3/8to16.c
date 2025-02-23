#include <stdio.h>
#include <string.h>
#include "both.h"



unsigned short reverse(unsigned short utf16) {
    return (utf16 << 8) | (utf16 >> 8);
}

void convert(FILE* inp, FILE* out, int BOM) {
    char utf8;
    unsigned short utf16;
    if (BOM == BE) {
        utf16 = 0xFFFE;
    }
    else {
        utf16 = 0xFEFF;
    }
    fwrite(&utf16, sizeof(unsigned short), 1, out);
    while (fread(&utf8, sizeof(char), 1, inp) == 1) {
        if ((utf8 & 0x80) == 0) {
            utf16 = utf8;
            utf16 = (BOM == BE) ? reverse(utf16) : utf16;
            fwrite(&utf16, sizeof(unsigned short), 1, out);
        }
        else if ((utf8 & 0xE0) == 0xC0) {
            utf16 = utf8 & 0x1F;
            utf16 <<= 6;
            if (fread(&utf8, sizeof(char), 1, inp) == 1) {
                if ((utf8 & 0xC0) == 0x80) {
                    utf16 += utf8 & 0x3F;
                    utf16 = (BOM == BE) ? reverse(utf16) : utf16;
                    fwrite(&utf16, sizeof(unsigned short), 1, out);
                }
            }
        }
        else if ((utf8 & 0xE0) == 0xE0) {
            utf16 = utf8 & 0x0F;
            utf16 <<= 6;
            if (fread(&utf8, sizeof(char), 1, inp) == 1) {
                if ((utf8 & 0xC0) == 0x80) {
                    utf16 += utf8 & 0x3F;
                    utf16 <<= 6;
                    if (fread(&utf8, sizeof(char), 1, inp) == 1) {
                        if ((utf8 & 0xC0) == 0x80) {
                            utf16 += utf8 & 0x3F;
                            utf16 = BOM == BE ? reverse(utf16) : utf16;
                            fwrite(&utf16, sizeof(unsigned short), 1, out);
                        }
                    }
                }
            }
        }
    }
    
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