#include "despesas.h"
#include "funcoes.h"
#include "bibliotecas.h"
#include "constantes.h"

static long long int posDespesa(struct despesa procurar, FILE * ficheiro) {
    fseek(ficheiro, 0 ,SEEK_SET);

    while(1) {
        struct despesa despesa;
        fread(&despesa, sizeof(struct despesa), 1, ficheiro);
        if(feof(ficheiro)) break;

        if(strcasecmp(procurar.tipo, despesa.tipo) == 0 &&
            strcasecmp(procurar.descricao, despesa.descricao) == 0 &&
            procurar.montante == despesa.montante) return (long long int) (ftell(ficheiro) - sizeof(struct despesa));
    }

    return ERRO_NAO_ENCONTRADO;
}

void inserirDespesa(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "ab") == ERRO_FICHEIRO) return;

    struct despesa novaDespesa;

    if(lerDespesa(&novaDespesa) == ERRO_GERAL) {
        fclose(ficheiro);
        return;
    }

    fwrite(&novaDespesa, sizeof(struct despesa), 1, ficheiro);
    fclose(ficheiro);

    printf("Despesa inserida com sucesso\n");
}

void editarDespesa(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "r+b") == ERRO_FICHEIRO) return;                                          // Erro ao abrir ficheiro

    char descricao[MAX_STRING], tipo[MAX_STRING], montante_str[MAX_STRING_DOUBLE];
    printf("Introduza a descrição da despesa a editar: ");
    if(lerLinha(stdin, descricao, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Descrição demasiado longa\n");
        fclose(ficheiro);
        return;
    }

    printf("Introduza o tipo de despesa a editar: ");
    if(lerLinha(stdin, tipo, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    printf("Introduza o montante da despesa a editar: ");
    if(lerLinha(stdin, montante_str, MAX_STRING_DOUBLE) == ERRO_ESPACO) {
        fprintf(stderr, "Montante demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    double montante = converterDouble(montante_str);
    if(montante == ERRO_CONVERCAO) {
        fprintf(stderr, "Montante inválido\n");
        fclose(ficheiro);
        return;
    }

    struct despesa procurar;
    strcpy(procurar.descricao, descricao);
    strcpy(procurar.tipo, tipo);
    procurar.montante = montante;

    long long int posicao = posDespesa(procurar, ficheiro);
    if(posicao == ERRO_NAO_ENCONTRADO) {
        fprintf(stderr, "Despesa não encontrada\n");
        fclose(ficheiro);
        return;
    }

    char nova_descricao[MAX_STRING], novo_tipo[MAX_STRING], novo_montante_str[MAX_STRING_DOUBLE];
    printf("Introduza a nova descrição da despesa a editar: ");
    if(lerLinha(stdin, nova_descricao, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Descrição demasiado longa\n");
        fclose(ficheiro);
        return;
    }

    printf("Introduza o novo tipo de despesa a editar: ");
    if(lerLinha(stdin, novo_tipo, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo demasiado longo\n");
        fclose(ficheiro);
        return;
    }

    printf("Introduza o novo montante da despesa a editar: ");
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

    struct despesa nova_despesa;
    strcpy(nova_despesa.descricao, nova_descricao);
    strcpy(nova_despesa.tipo, novo_tipo);
    nova_despesa.montante = novo_montante;

    fflush(ficheiro);                                                                                                   // Alterar o ficheiro para modo escrita
    fseek(ficheiro, posicao, SEEK_SET);                                                                                 // Ir para a posição da despesa a editar
    fwrite(&nova_despesa, sizeof(struct despesa), 1, ficheiro);                                                     // Reescrever a despesa

    fclose(ficheiro);

    printf("Despesa editada com sucesso\n");
}

void adicionarFicheiroDespesa(char * nomeFicheiro) {
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

        char * descricao = removerEspacosFronteiras(strtok(linha, "-"));
        if (descricao == NULL) {
            fprintf(stderr, "Erro na formatação da linha %d\n", pos);
            continue;
        }

        char * tipo = removerEspacosFronteiras(strtok(NULL, "-"));
        if (tipo == NULL) {
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

        struct despesa nova_despesa;
        strcpy(nova_despesa.descricao, descricao);
        strcpy(nova_despesa.tipo, tipo);
        nova_despesa.montante = montante;

        free(descricao);
        free(tipo);
        free(str_montante);

        fwrite(&nova_despesa, sizeof(struct despesa), 1, ficheiro);
    }

    fclose(ficheiro);
    fclose(ficheiroAdicionar);

    printf("Ficheiro adicionado com sucesso\n");
}

void limparDespesas(char * nomeFicheiro) {
    FILE * file = NULL;
    if(abrirFicheiro(&file, nomeFicheiro, "wb") == ERRO_FICHEIRO) return;
    fclose(file);
    printf("Ficheiro de despesas limpo com sucesso\n");
}

void mostrarDespesas(char * nomeFicheiro) {
    FILE * ficheiro = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "rb") == ERRO_FICHEIRO) return;

    while(1) {
        struct despesa despesa;
        fread(&despesa, sizeof(struct despesa), 1, ficheiro);
        if(feof(ficheiro)) break;

        printf("Despesa:\n\tDescrição: %s\n\tTipo: %s\n\tMontante: %.2f\n", despesa.descricao, despesa.tipo, despesa.montante);
    }

    fclose(ficheiro);
}

void removerDespesa(char * nomeFicheiro) {
    char temp_str[] = "tempFile.ppp_project";
    FILE * ficheiro = NULL, * ficheiroTemporario = NULL;
    if(abrirFicheiro(&ficheiro, nomeFicheiro, "rb") == ERRO_FICHEIRO) return;
    if(abrirFicheiro(&ficheiroTemporario, temp_str, "wb") == ERRO_FICHEIRO) return;

    int encontrado = 0;

    char descricao_remover[MAX_STRING];
    printf("Introduza a descrição da despesa a remover: ");
    if(lerLinha(stdin, descricao_remover, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Descrição muito longa.\n");
        fclose(ficheiro);
        fclose(ficheiroTemporario);
        remove(temp_str);
        return;
    }

    char tipo_remover[MAX_STRING];
    printf("Introduza o tipo da despesa a remover: ");
    if(lerLinha(stdin, tipo_remover, MAX_STRING) == ERRO_ESPACO) {
        fprintf(stderr, "Tipo muito longo.\n");
        fclose(ficheiro);
        fclose(ficheiroTemporario);
        remove(temp_str);
        return;
    }

    char montante_str_remover[MAX_STRING_DOUBLE];
    printf("Introduza o montante da despesa a remover: ");
    if(lerLinha(stdin, montante_str_remover, MAX_STRING_DOUBLE) == ERRO_ESPACO) {
        fprintf(stderr, "Montante muito longo.\n");
        fclose(ficheiro);
        fclose(ficheiroTemporario);
        remove(temp_str);
        return;
    }

    double montante_remover = converterDouble(montante_str_remover);
    if (montante_remover == ERRO_CONVERCAO) {
        fprintf(stderr, "Montante inválido\n");
        fclose(ficheiro);
        fclose(ficheiroTemporario);
        remove(temp_str);
        return;
    }

    while(1) {
        struct despesa despesa;
        fread(&despesa, sizeof(struct despesa), 1, ficheiro);
        if(feof(ficheiro)) break;

        if(strcasecmp(descricao_remover, despesa.descricao) != 0 &&
            strcasecmp(tipo_remover, despesa.tipo) != 0 &&
            montante_remover != despesa.montante)
            fwrite(&despesa, sizeof(struct orcamento), 1 ,ficheiroTemporario);
        else
            encontrado = 1;
    }

    fclose(ficheiro);
    fclose(ficheiroTemporario);

    if(rename(temp_str, nomeFicheiro) == -1) {
        fprintf(stderr, "Erro a remover a despesa\n");
        return;
    }

    if(!encontrado) {
        printf("Despesa não encontrada\n");
        return;
    }

    printf("Despesa removida com sucesso\n");
}
