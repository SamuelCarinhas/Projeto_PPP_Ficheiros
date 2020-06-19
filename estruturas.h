#ifndef PROJETO_PPP_FICHEIROS_ESTRUTURAS_H
#define PROJETO_PPP_FICHEIROS_ESTRUTURAS_H

#endif

#include "constantes.h"

struct orcamento {
    char tipo[MAX_STRING];
    double montante;
};

struct despesa {
    char descricao[MAX_STRING];
    char tipo[MAX_STRING];
    double montante;
};
