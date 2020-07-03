#include <stdio.h>
#include <stdlib.h>
#include "estado.h"
#include "funcoes.h"

int main()
{
    ESTADO *e = NULL;

    char peca;
    char opcao;
    char linha[TAMANHO_LINHA + 1];
    char nome_ficheiro[NOME_FICHEIRO + 1];

    int l,c;
    int nivel;
    int numTokens;

    print_menu();

    printf("\n\t\t\t\tOpcão: ");

    while (fgets(linha, TAMANHO_LINHA + 1, stdin)) {

        sscanf(linha, "%c", &opcao);

        switch (opcao) {

            case 'N':
                numTokens = sscanf(linha, "%c %c", &opcao, &peca);
                if (numTokens == 2) {executa_N(&e, peca);}
                else {printf("\t\t\t\t\nArgumentos inválidos\n");}
                break;

            case 'L':
                numTokens = sscanf(linha, "%c %[^\n]", &opcao, nome_ficheiro);
                if (numTokens == 2) {executa_L(&e, nome_ficheiro);}
                else {printf("\t\t\t\t\nArgumentos inválidos\n");}
                break;

            case 'E':
                numTokens = sscanf(linha, "%c %[^\n]", &opcao, nome_ficheiro);
                if (numTokens == 2) {executa_E(e, nome_ficheiro);}
                else {printf("\t\t\t\t\nArgumentos inválidos\n");}
                break;

            case 'J':
                numTokens = sscanf(linha, "%c %d %d", &opcao, &l, &c);
                if (numTokens == 3) {executa_J(e, l, c);}
                else {printf("\t\t\t\t\nArgumentos inválidos\n");}
                break;

            case 'S':
            	executa_S(e);
                break;

            case 'H':
            	executa_H(e);
                break;

            case 'U':
                executa_U(e);
                break;

            case 'A':
                numTokens = sscanf(linha, "%c %c %d", &opcao, &peca, &nivel);
                if (numTokens == 3) {executa_A(&e, peca, nivel);}
                else {printf("\t\t\t\t\nArgumentos inválidos\n");}
                break;

            case 'Q':
                executa_Q(e);
                return EXIT_SUCCESS;
            
            default:
            	printf("\n\t\t\t\tOpcão inválida\n");
        }

        print_menu();
        printf("\n\t\t\t\tOpcão: ");
    }

    return 0;
}