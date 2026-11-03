#include <stdio.h>
#include <stdlib.h> // pour EXIT_SUCCESS et EXIT_FAILURE

// Détecte la longueur d'un caractère UTF-8 en octets à partir du premier octet
int utf8_char_len(unsigned char c) {
    if ((c & 0x80) == 0)       return 1;   // ASCII : 0xxxxxxx
    if ((c & 0xE0) == 0xC0)    return 2;   // 110xxxxx
    if ((c & 0xF0) == 0xE0)    return 3;   // 1110xxxx
    if ((c & 0xF8) == 0xF0)    return 4;   // 11110xxx
    return -1; // octet invalide ou continuation (pas le début d'un caractère)
}

// Décode un caractère UTF-8 en son code Unicode (point de code)
int utf8_decode(const unsigned char *s, int len) {
    int codepoint = 0;
    if (len == 1) {
        codepoint = s[0];
    } else if (len == 2) {
        codepoint = ((s[0] & 0x1F) << 6) |
                    (s[1] & 0x3F);
    } else if (len == 3) {
        codepoint = ((s[0] & 0x0F) << 12) |
                    ((s[1] & 0x3F) << 6) |
                    (s[2] & 0x3F);
    } else if (len == 4) {
        codepoint = ((s[0] & 0x07) << 18) |
                    ((s[1] & 0x3F) << 12) |
                    ((s[2] & 0x3F) << 6) |
                    (s[3] & 0x3F);
    }
    return codepoint;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s chaine_utf8\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned char *s = (unsigned char *)argv[1];
    int i = 0;

    printf("Analyse de la chaîne UTF-8 : %s\n\n", argv[1]);

    while (s[i] != '\0') {
        int len = utf8_char_len(s[i]);
        if (len == -1) {
            fprintf(stderr, "Erreur : octet invalide ou continuation inattendue à la position %d\n", i);
            return EXIT_FAILURE;
        }

        // Vérifier que la chaîne contient assez d'octets pour ce caractère
        for (int j = 1; j < len; j++) {
            if ((s[i + j] & 0xC0) != 0x80) { // doit commencer par 10xxxxxx
                fprintf(stderr, "Erreur : octet de continuation invalide à la position %d\n", i + j);
                return EXIT_FAILURE;
            }
        }

        int codepoint = utf8_decode(&s[i], len);

        printf("Caractère à la position %d : longueur = %d octet(s), code Unicode = U+%04X\n",
               i, len, codepoint);

        i += len;
    }

    return EXIT_SUCCESS;
}

