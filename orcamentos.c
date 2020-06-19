#include "orcamentos.h"
#include "funcoes.h"
#include "bibliotecas.h"
#include "constantes.h"

static long long int posOrcamento(char * tipoOrcamento, FILE * ficheiro) {
    fseek(ficheiro, 0 ,SEEK_SET);

    while(1) {
        struct orcamento orcamento;
        fread(&orcamento, sizeof(struct orcamento), 1, ficheiro);
        if(feof(ficheiro)) break;

        if(strcasecmp(tipoOrcamento, orcamento.tipo) == 0) return (long long int) (ftell(ficheiro) - sizeof(struct orcamento));
    }

    return ERRO_NAO_ENCONTRADO;
}

void inserirOrcamento(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "ab") == ERRO_FICHEIRO) return;

    struct orcamento novoOrcamento;

    if(lerOrcamento(&novoOrcamento) == ERRO_GERAL) {
        fclose(ficheiro);
        return;
    }

    fwrite(&novoOrcamento, sizeof(struct orcamento), 1, ficheiro);
    fclose(ficheiro);

    printf("Orçamento inserido com sucesso\n");
}

void editarOrcamento(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "r+b") == ERRO_FICHEIRO) return;                                          // Erro ao abrir ficheiro

    char orcamento[MAX_STRING];
    printf("Introduza o tipo de orçamento a editar: ");
    if(lerLinha(stdin, orcamento, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo de orçamento demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    long long int posicao = posOrcamento(orcamento, ficheiro);
    if(posicao == ERRO_NAO_ENCONTRADO) {
        fprintf(stderr, "Orcamento não encontrado\n");
        fclose(ficheiro);
        return;
    }

    struct orcamento novo_orcamento;

    char novo_tipo[MAX_STRING];
    printf("Introduza o novo tipo de orcamento: ");
    if(lerLinha(stdin, novo_tipo, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo de orçamento demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    char novo_montante_str[MAX_STRING_DOUBLE];
    printf("Introduza o novo montante: ");
    if(lerLinha(stdin, novo_montante_str, MAX_STRING_DOUBLE) == ERRO_ESPACO) {
        fprintf(stderr, "Montante demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    double novo_montante = converterDouble(novo_montante_str);
    if(novo_montante == ERRO_CONVERCAO) {
        fprintf(stderr, "Montante inválido\n");
        fclose(ficheiro);
        return;
    }

    strcpy(novo_orcamento.tipo, novo_tipo);
    novo_orcamento.montante = novo_montante;

    fflush(ficheiro);                                                                                                   // Alterar o ficheiro para modo escrita
    fseek(ficheiro, posicao, SEEK_SET);                                                                                 // Ir para a posição do orçamento a editar
    fwrite(&novo_orcamento, sizeof(struct orcamento), 1, ficheiro);                                                 // Reescrever o orçamento

    fclose(ficheiro);

    printf("Orçamento editado com sucesso\n");
}

void adicionarFicheiroOrcamento(char * nomeFicheiro) {
    FILE * ficheiro = NULL, * ficheiroAdicionar = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "ab") == ERRO_FICHEIRO) return;                                           // Erro ao abrir ficheiro

    char nomeFicheiroAdicionar[MAX_STRING];
    printf("Introduza o nome do ficheiro a adicionar: ");
    if(lerLinha(stdin, nomeFicheiroAdicionar, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Nome do ficheiro muito longo.\n");
        fclose(ficheiro);
        return;
    }
    if(abrirFicheiro(&ficheiroAdicionar, nomeFicheiroAdicionar, "r") == ERRO_FICHEIRO) {
        fclose(ficheiro);
        return;                                                                                                         // Erro ao abrir ficheiro
    }

    char linha[MAX_STRING];
    int pos = 0, resultado;
    double montante;
    while(1) {
        pos++;
        resultado = lerLinha(ficheiroAdicionar, linha, MAX_STRING);

        if (resultado == EOF) break;

        if (resultado == ERRO_ESPACO) {
            fprintf(stderr, "Linha %d demasiado grande\n", pos);
            continue;
        }

        char * orcamento = removerEspacosFronteiras(strtok(linha, "-"));
        if (orcamento == NULL) {
            fprintf(stderr, "Erro na formatação da linha %d\n", pos);
            continue;
        }
        char * str_montante = removerEspacosFronteiras(strtok(NULL, "-"));
        if (str_montante == NULL) {
            fprintf(stderr, "Erro na formatação da linha %d\n", pos);
            continue;
        }

        montante = converterDouble(str_montante);
        if (montante == ERRO_CONVERCAO) {
            fprintf(stderr, "Erro de conversão do montante na linha %d\n", pos);
            continue;
        }

        struct orcamento novo_orcamento;
        novo_orcamento.montante = montante;
        strcpy(novo_orcamento.tipo, orcamento);

        free(orcamento);
        free(str_montante);
        fwrite(&novo_orcamento, sizeof(struct orcamento), 1, ficheiro);
    }

    fclose(ficheiro);
    fclose(ficheiroAdicionar);

    printf("Ficheiro adicionado com sucesso\n");
}

void limparOrcamentos(char * nomeFicheiro) {
    FILE * file = NULL;
    if(abrirFicheiro(&file, nomeFicheiro, "wb") == ERRO_FICHEIRO) return;
    fclose(file);
    printf("Ficheiro de orçamentos limpo com sucesso\n");
}

void mostrarOrcamentos(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "rb") == ERRO_FICHEIRO) return;

    while(1) {
        struct orcamento orcamento;
        fread(&orcamento, sizeof(struct orcamento), 1, ficheiro);
        if(feof(ficheiro)) break;

        printf("Orçamento:\n\tTipo: %s\n\tMontante: %.2f\n", orcamento.tipo, orcamento.montante);
    }

    fclose(ficheiro);
}

void removerOrcamento(char * nomeFicheiro) {
    char temp_str[] = "tempFile.ppp_project";
    FILE * ficheiro = NULL, * ficheiroTemporario = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "rb") == ERRO_FICHEIRO) return;
    if(abrirFicheiro(&ficheiroTemporario, temp_str, "wb") == ERRO_FICHEIRO) return;

    int encontrado = 0;

    char orcamento_remover[MAX_STRING];
    printf("Introduza o nome do orçamento a remover: ");
    if(lerLinha(stdin, orcamento_remover, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Nome do orçamento muito longo.\n");
        fclose(ficheiro);
        fclose(ficheiroTemporario);
        remove(temp_str);
        return;
    }

    while(1) {
        struct orcamento orcamento;
        fread(&orcamento, sizeof(struct orcamento), 1, ficheiro);
        if(feof(ficheiro)) break;

        if(strcasecmp(orcamento_remover, orcamento.tipo) != 0)
            fwrite(&orcamento, sizeof(struct orcamento), 1 ,ficheiroTemporario);
        else
            encontrado = 1;
    }

    fclose(ficheiro);
    fclose(ficheiroTemporario);

    if(rename(temp_str, nomeFicheiro) == -1) {
        fprintf(stderr, "Erro a remover o orçamento\n");
        return;
    }

    if(!encontrado) {
        printf("Orçamento não encontrado\n");
        return;
    }

    printf("Orçamento removido com sucesso\n");
}
