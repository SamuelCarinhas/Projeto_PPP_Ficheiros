#include "funcoes.h"
#include "bibliotecas.h"
#include "constantes.h"

int lerLinha(FILE * ficheiro, char * linha, int tamanhoMax) {
    int c, i = 0;

    while((c = fgetc(ficheiro)) != '\n' && c != '\r') {
        if(c == EOF) return c;
        if(i == tamanhoMax) break;
        linha[i++] = (char) c;
    }

    if(i >= tamanhoMax - 1) {
        if(c != '\n')
            while(fgetc(ficheiro) != '\n');                                                                             // Ler o resto da linha até o \n
        return ERRO_ESPACO;
    } else if(c == '\r') fgetc(ficheiro);                                                                               // Ler o \n

    linha[i] = '\0';
    return 1;
}

double converterDouble(char * string) {
    char * ponteiro;
    double resultado = strtod(string, &ponteiro);

    while(*ponteiro != '\0') {
        if('0' > *ponteiro || '9' < *ponteiro) return ERRO_CONVERCAO;
        ponteiro++;
    }

    return resultado;
}

int lerOrcamento(struct orcamento * novoOrcamento) {
    char orcamento[MAX_STRING];
    char str_montante[MAX_STRING_DOUBLE];

    printf("Introduza o tipo do orçamento: ");
    if(lerLinha(stdin, orcamento, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo demasiado longo\n");
        return ERRO_GERAL;
    }

    printf("Introduza o montante do orçamento: ");
    if(lerLinha(stdin, str_montante, MAX_STRING_DOUBLE) == ERRO_ESPACO) {
        fprintf(stderr, "Motante demasiado longo\n");
        return ERRO_GERAL;
    }

    double montante = converterDouble(str_montante);
    if(montante == ERRO_CONVERCAO) {
        fprintf(stderr, "Montante inválido\n");
        return ERRO_GERAL;
    }

    strcpy(novoOrcamento->tipo, orcamento);
    novoOrcamento->montante = montante;

    return 1;
}

int lerDespesa(struct despesa * novaDespesa) {
    char descricao[MAX_STRING];
    char tipo[MAX_STRING];
    char str_montante[MAX_STRING_DOUBLE];

    printf("Introduza a descrição da despesa: ");
    if(lerLinha(stdin, descricao, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Descrição demasiado longa\n");
        return ERRO_GERAL;
    }

    printf("Introduza o tipo de despesa: ");
    if(lerLinha(stdin, tipo, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo demasiado longo\n");
        return ERRO_GERAL;
    }

    printf("Introduza o montante da despesa: ");
    if(lerLinha(stdin, str_montante, MAX_STRING_DOUBLE) == ERRO_ESPACO) {
        fprintf(stderr, "Montante demasiado longo\n");
        return ERRO_GERAL;
    }

    double montante = converterDouble(str_montante);
    if(montante == ERRO_CONVERCAO) {
        fprintf(stderr, "Montante inválido\n");
        return ERRO_GERAL;
    }

    strcpy(novaDespesa->descricao, descricao);
    strcpy(novaDespesa->tipo, tipo);
    novaDespesa->montante = montante;

    return 1;
}

char * removerEspacosFronteiras(char * string) {
    char * fim = string + strlen(string) - 1;
    while(*string == ' ') string++; while(*fim == ' ') fim--;
    char * str_trim = (char *) malloc(fim-string+2);

    int i, max = (int) (fim-string);
    for(i = 0; i <= max; i++, string++) str_trim[i] = *string;
    str_trim[i] = '\0';

    return str_trim;
}

int abrirFicheiro(FILE ** file, char * nomeFicheiro, char * flag) {
    *file = fopen(nomeFicheiro, flag);
    if(*file == NULL) {
        fprintf(stderr, "O programa não conseguiu abrir o ficheiro %s\n", nomeFicheiro);
        return ERRO_FICHEIRO;
    }
    return 0;
}
