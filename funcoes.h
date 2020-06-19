#ifndef PROJETO_PPP_FICHEIROS_FUNCOES_H
#define PROJETO_PPP_FICHEIROS_FUNCOES_H

#endif

#include "bibliotecas.h"
#include "estruturas.h"

int lerLinha(FILE *, char *, int);
double converterDouble(char *);
int lerOrcamento(struct orcamento *);
int lerDespesa(struct despesa *);
int abrirFicheiro(FILE **, char *, char *);
char * removerEspacosFronteiras(char *);
