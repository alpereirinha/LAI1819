#ifndef FUNCOES_H
#define FUNCOES_H

#define TAMANHO_LINHA 255
#define NOME_FICHEIRO 255

void executa_N(ESTADO **e, char peca);
void executa_L(ESTADO **e, char nome_ficheiro[]);
void executa_E(ESTADO *e, char nome_ficheiro[]);
void executa_J(ESTADO *e, int linha, int coluna);
void executa_S(ESTADO *e);
void executa_H(ESTADO *e);
void executa_U(ESTADO *e);
void executa_A(ESTADO **e, char peca, int nivel);
void executa_Q(ESTADO *e);
#endif