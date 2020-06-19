#include "orcamentos.h"
#include "despesas.h"
#include "constantes.h"

void printComandos(void);
int lerNumero(void);

int main(int argc, char * args[] ) {

    if(argc != 3) {
        fprintf(stderr, "Quantidade de argumentos inváliada\n");
        return ERRO_PROGRAMA;
    }

    FILE * ficheiroOrcamento = fopen(args[1], "r+b");
    if(ficheiroOrcamento == NULL) {
        fprintf(stderr, "O programa não conseguiu abrir o ficheiro de orçamentos\n");
        return ERRO_PROGRAMA;
    }
    fclose(ficheiroOrcamento);
    FILE * ficheiroDespesas = fopen(args[2], "r+b");
    if(ficheiroDespesas == NULL) {
        fprintf(stderr, "O programa não conseguiu abrir o ficheiro de despesas\n");
        return ERRO_PROGRAMA;
    }
    fclose(ficheiroDespesas);

    char * ficheiro_orcamentos_str = args[1];
    char * ficheiro_despesas_str = args[2];

    while(1) {
        printComandos();

        int command = lerNumero();

        switch (command) {
            case 0:
                return 0;
            case 1:
                inserirOrcamento(ficheiro_orcamentos_str);
                break;
            case 2:
                editarOrcamento(ficheiro_orcamentos_str);
                break;
            case 3:
                removerOrcamento(ficheiro_orcamentos_str);
                break;
            case 4:
                adicionarFicheiroOrcamento(ficheiro_orcamentos_str);
                break;
            case 5:
                limparOrcamentos(ficheiro_orcamentos_str);
                break;
            case 6:
                mostrarOrcamentos(ficheiro_orcamentos_str);
                break;
            case 7:
                inserirDespesa(ficheiro_despesas_str);
                break;
            case 8:
                editarDespesa(ficheiro_despesas_str);
                break;
            case 9:
                removerDespesa(ficheiro_despesas_str);
                break;
            case 10:
                adicionarFicheiroDespesa(ficheiro_despesas_str);
                break;
            case 11:
                limparDespesas(ficheiro_despesas_str);
                break;
            case 12:
                mostrarDespesas(ficheiro_despesas_str);
                break;
            default:
                fprintf(stderr, "Comando não encontrado.\n");
                break;
        }

    }

}

/*
 * Args: void
 * Return: int
 * Descrição: Lê um número do stdin
 */
int lerNumero(void) {
    int comando, erros;
    while(1) {
        printf("Introduza o comando: ");
        erros = 0;
        while (scanf("%d", &comando) != 1) {
            fprintf(stderr, "Comando inválido\n");
            while (getc(stdin) != '\n');                                                                                // Se houve um erro a ler um inteiro, ignorar o resto da linha até ao \n
            printf("Introduza o comando: ");
        }
        while (getc(stdin) != '\n') {                                                                                   // Verificar se não existe nada depois do inteiro lido anteriormente
            if(!erros)
                fprintf(stderr, "Comando inválido\n");
            erros = 1;
        }
        if(!erros) break;
    }
    return comando;
}

/*
 * Args: void
 * Return: void
 * Descrição: Printa no stdout a lista de comandos possíveis no projeto
 */
void printComandos(void) {
    printf("Comandos:\n");
    printf("\t0 - Terminar o programa\n");
    printf("\t1 - Inserir um novo orçamento\n");
    printf("\t2 - Editar um orçamento\n");
    printf("\t3 - Remover um orçamento\n");
    printf("\t4 - Adicionar ficheiro de texto de orçamentos\n");
    printf("\t5 - Limpar todos os orçamentos\n");
    printf("\t6 - Mostrar todos os orçamentos\n");
    printf("\t7 - Inserir uma nova despesa\n");
    printf("\t8 - Editar uma despesa\n");
    printf("\t9 - Remover uma despesa\n");
    printf("\t10 - Adicionar ficheiro de texto de despesas\n");
    printf("\t11 - Limpar todas as despesas\n");
    printf("\t12 - Mostrar todas as despesas\n");
}
