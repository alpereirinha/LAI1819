#ifndef PROJ_ESTADO_H
#define PROJ_ESTADO_H

#include "list.h"

typedef struct coordenada {
	int x;
	int y;	
} COORDENADA;

void apaga_coordenada(void *coordenadaPtr);
void apaga_jogada(void *jogada);

typedef enum {
	VAZIA, 
	VALOR_X, 
	VALOR_O,
	VALOR_P,
	VALOR_H
} VALOR;

typedef struct jogada {
	VALOR   peca;               /* peca que foi comida */
	list_t *posicoes;           /* lista de posicoes que foram comidas */
}JOGADA;

typedef struct estado {
    VALOR peca;                 /* peça do jogador que vai jogar! */
	VALOR bot;                  /* peca do bot */
    VALOR grelha[8][8];         /* grelha */
    int fim_do_jogo;            /* flag que indica se o jogo terminou */	
    int nivel;                  /* nivel do jogo contra o bot */				
    char modo;                  /* modo em que se está a jogar! 0-> manual, 1-> contra computador */
    list_t *jogadas_anteriores; /* todas as jogadas já efectuadas */
} ESTADO;


/* =============================================================================
 * inicialia_jogo
 * -- Aloca um novo jogo
 * -- Retorna um ponteiro para a estrura ESTADO se conseguio alocar, se não
 *    retorna NULL
 * =============================================================================
 */
ESTADO* 
inicializa_jogo(char peca, int modo, int nivel);


/* =============================================================================
 * finaliza_jogo
 * -- Desaloca um jogo
 * =============================================================================
 */
void    
finaliza_jogo(ESTADO *e);


/* =============================================================================
 * print_menu_jogo
 * -- Imprime o menu do jogo
 * =============================================================================
 */
void    
print_menu();


/* =============================================================================
 * printa
 * -- Imprime o estado do jogo
 * =============================================================================
 */
void    
printa(ESTADO e, FILE *stream);


/* =============================================================================
 * ganha_posicoes
 * -- Descobre todas as pecas que um jogador pode ganhar em determinada direcao
 *    numa dada jogada
 * =============================================================================
 */
void 
ganha_posicoes(ESTADO *e, int linha, int coluna, list_t* posicoesPtr, COORDENADA direcao);


/* =============================================================================
 * jogada_valida
 * -- Verifica se uma dada jogada e valida e descobre todas as pecas que essa
 *    jogada ganha
 * -- Se a lista de pecas ganhas com uma dada jogada for vazia, entao a jogada
 *    nao e valida 
 * =============================================================================
 */
int     
jogada_valida(ESTADO *e, int linha, int coluna, list_t* posicoesPtr);


/* =============================================================================
 * colocar_peca
 * -- Preenche o tabuleiro com todas as pecas ganhas numa dada jogada
 * -- Adiciona as pecas ganhas a lista de jogadas efectudas
 * =============================================================================
 */
void    
colocar_peca(ESTADO *e, int linha, int coluna, list_t* posicoesPtr);


/* =============================================================================
 * colocar_jogada
 * -- Adiciona as pecas ganhas a lista de jogadas efectudas
 * =============================================================================
 */
void 
colocar_jogada(ESTADO *e, int linha, int coluna, list_t* posicoesPtr);


/* =============================================================================
 * descobre_jogadas_validas
 * -- Descobre todas as jogadas validas que um jogador pode efectuar
 * =============================================================================
 */
void    
descobre_jogadas_validas(ESTADO *e, list_t* posicoes_jogada_validaPtr);


/* =============================================================================
 * descobre_melhor_jogada 
 * -- Descobre a melhor jogada valida que um jogador pode efectuar
 * =============================================================================
 */
COORDENADA*    
descobre_melhor_jogada(ESTADO *e, int nivel);


/* =============================================================================
 * fim_do_jogo
 * -- Verifica se o jogo chegou ao fim
 * -- Devolve 1 caso o jogo tenha chegado ao fim e 0 em caso contrario
 * =============================================================================
 */
int     
fim_do_jogo(ESTADO *e);


/* =============================================================================
 * print_fim_do_jogo
 * -- Imprime a mensagem de fim de jogo
 * =============================================================================
 */
void
print_fim_do_jogo(ESTADO *e);

/* =============================================================================
 * proximo_jogador
 * -- Calcula o proximo jogador a jogar
 * -- Calcula tambem se o jogo acabou
 * =============================================================================
 */
void
proximo_jogador(ESTADO *e);


/* =============================================================================
 * conta_numero_de_pecas
 * -- Devolve o numero de pecas de uma dada peca
 * =============================================================================
 */
int     
conta_numero_de_pecas(ESTADO *e, VALOR peca);

#endif 