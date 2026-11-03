#include <stdio.h>
#include <stdlib.h>

// Affiche les codes hexadécimaux et décimaux de chaque octet d'une chaîne
void print_bytes(const char *str) {
    int i = 0;
    printf("Analyse de la chaîne : \"%s\"\n", str);
    printf("Position | Octet (hex) | Octet (dec) | Type\n");
    printf("-------------------------------------------\n");
    while (str[i] != '\0') {
        unsigned char c = (unsigned char)str[i];
        printf("%7d |    0x%02X    |     %3d    | ", i, c, c);

        if (c <= 127) {
            printf("ASCII simple\n");
        } else {
            printf("Caractère > 127 (ASCII étendu ou UTF-8)\n");
        }
        i++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage : %s chaine\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_bytes(argv[1]);

    return EXIT_SUCCESS;
}

