#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "estado.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void apaga_coordenada(void *coordenadaPtr) {
    free(coordenadaPtr);
}

void apaga_jogada(void *jogada) {
    JOGADA *j = (JOGADA *) (jogada);
    list_free(j->posicoes);
    free(j);
}

COORDENADA MOV_POSX      = {1, 0};
COORDENADA MOV_POSY      = {0, 1};
COORDENADA MOV_NEGX      = {-1, 0};
COORDENADA MOV_NEGY      = {0, -1};
COORDENADA MOV_POSX_POSY = {1, 1};
COORDENADA MOV_POSX_NEGY = {1, -1};
COORDENADA MOV_NEGX_POSY = {-1, 1};
COORDENADA MOV_NEGX_NEGY = {-1, -1};


/* =============================================================================
 * inicialia_jogo
 * -- Aloca um novo jogo
 * -- Retorna um ponteiro para a estrura ESTADO se consegui alocar, se não
 *    retorna NULL
 * =============================================================================
 */
ESTADO* 
inicializa_jogo(char peca, int modo, int nivel) {

    int i, j;

    ESTADO *e = (ESTADO*) malloc (sizeof(ESTADO));

    if (e == NULL) {
        return e;
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            e->grelha[i][j] = VAZIA;
        }
    }

    e->grelha[3][4] = VALOR_X;
    e->grelha[4][3] = VALOR_X;
    e->grelha[3][3] = VALOR_O;
    e->grelha[4][4] = VALOR_O;
    switch (peca) {
        case 'O': 
            e->peca     = VALOR_O;
            e->bot      = VALOR_X;
            break;

        case 'X': 
            e->peca     = VALOR_X;
            e->bot      = VALOR_O;
            break;
    }
    e->modo = modo;
    e->jogadas_anteriores = list_alloc(NULL, apaga_jogada);
    e->fim_do_jogo = 0;
    e->nivel = nivel;

    return e;
}


/* =============================================================================
 * finaliza_jogo
 * -- Desaloca um jogo
 * =============================================================================
 */
void finaliza_jogo(ESTADO *e) {
    list_free(e->jogadas_anteriores);
    free(e);
}


/* =============================================================================
 * print_menu_jogo
 * -- Imprime o menu do jogo
 * =============================================================================
 */
void print_menu() {
    printf("\n");
    printf("\t\t\t\t******************************** Menu ********************************\n");
    printf("\t\t\t\t** N <peca>         - Novo Jogo                                     **\n");
    printf("\t\t\t\t** L <ficheiro>     - Ler Jogo de ficheiro                          **\n");
    printf("\t\t\t\t** E <ficheiro>     - Guardar Jogo em ficheiro                      **\n");
    printf("\t\t\t\t** J <L> <C>        - Jogar na posicao (L, C)                       **\n");
    printf("\t\t\t\t** S                - Imprime um '.' nas posicoes com jogada válida **\n");
    printf("\t\t\t\t** H                - Imprime um '?' na posicao sugerida            **\n");
    printf("\t\t\t\t** U                - Desfazer ultima jogada                        **\n");
    printf("\t\t\t\t** A <peça> <nível> - Novo jogo contra computador                   **\n");
    printf("\t\t\t\t** Q                - sair                                          **\n");
    printf("\t\t\t\t**********************************************************************\n");
}


/* =============================================================================
 * printa
 * -- Imprime o estado do jogo
 * =============================================================================
 */
void printa(ESTADO e, FILE *stream) {
    
    int i, j;
    char c = ' ';

    for (i = 0; i < 8; i++) {
        if (stream == stdout) {
            fprintf(stream, "\t\t\t\t");
        }
        for (j = 0; j < 8; j++) {
            switch (e.grelha[i][j]) {
                case VALOR_O:
                    c = 'O';
                    break;
    
                case VALOR_X:
                    c = 'X';
                    break;
            
                case VALOR_P:
                    c = '.';
                    break;

                case VALOR_H:
                    c = '?';
                    break;

                case VAZIA: 
                    c = '-';
                    break;
            }
            fprintf(stream, "%c ", c);
        }
        fprintf(stream, "\n");
    }
    if (stream == stdout) {
        fprintf(stream, "\t\t\t\tPontuação\n");
        fprintf(stream, "\t\t\t\tJogador x: %d\n", conta_numero_de_pecas(&e, VALOR_X));
        fprintf(stream, "\t\t\t\tJogador O: %d\n", conta_numero_de_pecas(&e, VALOR_O));
    }
}


/* =============================================================================
 * ganha_posicoes
 * -- Descobre todas as pecas que um jogador pode ganhar em determinada direcao
 *    numa dada jogada
 * =============================================================================
 */
void 
ganha_posicoes(ESTADO *e, int linha, int coluna, list_t* posicoesPtr, COORDENADA direcao) {

    int numero_pecas = 0;

    VALOR peca_oponente = e->peca % 2 + 1;

    COORDENADA coord;

    coord.x = linha  + direcao.x;
    coord.y = coluna + direcao.y;

    if ((coord.x <= 8 && coord.x >= 1) && (coord.y <= 8 && coord.y >= 1)) {

        if (e->grelha[coord.x - 1][coord.y - 1] == e->peca) {
            return;
        }

        while ((coord.x <= 8 && coord.x >= 1) && (coord.y <= 8 && coord.y >= 1) && e->grelha[coord.x - 1][coord.y - 1] == peca_oponente) {
            COORDENADA* posicao_ganha = (COORDENADA*) malloc(sizeof(COORDENADA));
            posicao_ganha->x = coord.x;
            posicao_ganha->y = coord.y;
            coord.x += direcao.x;
            coord.y += direcao.y;
            list_push_back(posicoesPtr ,(void *) posicao_ganha);
            numero_pecas++;
        }

        if ((coord.x <= 0 || coord.x >= 9 || coord.y <= 0 || coord.y >= 9) || e->grelha[coord.x - 1][coord.y - 1] == VAZIA) {
            while (numero_pecas > 0) {
                list_pop_back(posicoesPtr);
                numero_pecas--;
            }
        }
    }
}


/* =============================================================================
 * jogada_valida
 * -- Verifica se uma dada jogada e valida e descobre todas as pecas que essa
 *    jogada ganha
 * -- Se a lista de pecas ganhas com uma dada jogada for vazia, entao a jogada
 *    nao e valida 
 * =============================================================================
 */
int jogada_valida(ESTADO *e, int linha, int coluna, list_t* posicoesPtr) {

    if ((linha <= 8 && linha >= 1) && (coluna <= 8 && coluna >= 1) && e->grelha[linha - 1][coluna - 1] == VAZIA) {
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_POSX);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_POSY);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_NEGX);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_NEGY);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_POSX_POSY);
   		ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_POSX_NEGY);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_NEGX_POSY);
    	ganha_posicoes(e, linha, coluna, posicoesPtr, MOV_NEGX_NEGY);
    }
    return !list_isEmpty(posicoesPtr);
}


/* =============================================================================
 * colocar_peca
 * -- Preenche o tabuleiro com todas as pecas ganhas numa dada jogada
 * =============================================================================
 */
void colocar_peca(ESTADO *e, int linha, int coluna, list_t* posicoesPtr) {
    
    COORDENADA  posicao;

    list_iter_t it;
    
    e->grelha[linha - 1][coluna - 1] = e->peca;
    list_iter_reset(&it, posicoesPtr);

    while (list_iter_hasNext(&it, posicoesPtr)) {
        posicao = *((COORDENADA *) (list_iter_next(&it, posicoesPtr)));
        e->grelha[posicao.x - 1][posicao.y - 1] = e->peca;
    }
}


/* =============================================================================
 * colocar_jogada
 * -- Adiciona as pecas ganhas a lista de jogadas efectudas
 * =============================================================================
 */
void 
colocar_jogada(ESTADO *e, int linha, int coluna, list_t* posicoesPtr) {

    COORDENADA *posicaoPtr;
    JOGADA     *jogada = (JOGADA *) malloc (sizeof(JOGADA));

    if (jogada != NULL) {
        posicaoPtr = (COORDENADA *) malloc (sizeof(COORDENADA));
        posicaoPtr->x = linha;
        posicaoPtr->y = coluna;
    
        list_push_back(posicoesPtr, (void *) posicaoPtr);
        jogada->peca = e->peca % 2 + 1;
        jogada->posicoes = posicoesPtr; 
        list_push_back(e->jogadas_anteriores, (void *) jogada);
    }
}


/* =============================================================================
 * descobre_jogadas_validas
 * -- Descobre todas as jogadas validas que um jogador pode efectuar
 * =============================================================================
 */
void descobre_jogadas_validas(ESTADO *e, list_t* posicoes_jogada_validaPtr) {

	int i, j;

	COORDENADA *posicao_validaPtr;
	
	for (i = 1; i <= 8; i++) {
		for (j = 1; j <= 8; j++) {
			list_t* posicoesPtr = list_alloc(NULL, apaga_coordenada);
			if (jogada_valida(e, i, j, posicoesPtr)) {
				posicao_validaPtr = (COORDENADA *) malloc (sizeof(COORDENADA));
				posicao_validaPtr->x = i;
				posicao_validaPtr->y = j;
				list_push_back(posicoes_jogada_validaPtr, (void *) posicao_validaPtr);
                list_free(posicoesPtr);
			}
			else {
				list_free(posicoesPtr);
			}
		}
	}
}


int minmax(ESTADO *e, int nivel, int peca_jogador) {
    
    int value;

    ESTADO estado_aux;

    COORDENADA *posicao;

    list_t* posicoesPtr;

    list_t *posicoes_jogada_validaPtr = list_alloc(NULL, apaga_coordenada);
    list_iter_t it;

    if (nivel == 0 || fim_do_jogo(e)) {
        list_free(posicoes_jogada_validaPtr);
        if(peca_jogador) {
            return conta_numero_de_pecas(e, e->peca) - conta_numero_de_pecas(e, e->peca % 2 + 1);
        }
        else {
            return - (conta_numero_de_pecas(e, e->peca) - conta_numero_de_pecas(e, e->peca % 2 + 1));
        }
    }

    if (peca_jogador) {
        value = INT_MIN;

        descobre_jogadas_validas(e, posicoes_jogada_validaPtr);
        list_iter_reset(&it, posicoes_jogada_validaPtr);

        while (list_iter_hasNext(&it, posicoes_jogada_validaPtr)) {
            posicao = (COORDENADA *)(list_iter_next(&it, posicoes_jogada_validaPtr));
            posicoesPtr = list_alloc(NULL, apaga_coordenada);
            estado_aux = *e;
            jogada_valida(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            colocar_peca(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            proximo_jogador(&estado_aux);
            list_free(posicoesPtr);
            value = MAX(value, minmax(&estado_aux, nivel - 1, 0));
        }
        list_free(posicoes_jogada_validaPtr);
        return value;
    }    

    else {
        value = INT_MAX;
        
        descobre_jogadas_validas(e, posicoes_jogada_validaPtr);

        list_iter_reset(&it, posicoes_jogada_validaPtr);

        while (list_iter_hasNext(&it, posicoes_jogada_validaPtr)) {

            posicao = (COORDENADA *)(list_iter_next(&it, posicoes_jogada_validaPtr));
            posicoesPtr = list_alloc(NULL, apaga_coordenada);
            estado_aux = *e;
            jogada_valida(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            colocar_peca(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            proximo_jogador(&estado_aux);
            list_free(posicoesPtr);
            value = MIN(value, minmax(&estado_aux, nivel - 1, 1));
        }
        list_free(posicoes_jogada_validaPtr);
        return value;
    }
}


/* =============================================================================
 * descobre_melhor_jogada 
 * -- Descobre a melhor jogada valida que um jogador pode efectuar
 * =============================================================================
 */
COORDENADA* descobre_melhor_jogada(ESTADO *e, int nivel) {
    
    list_t *posicoes_jogada_validaPtr = list_alloc(NULL, apaga_coordenada);
    list_iter_t it;
    list_t* posicoesPtr;
    ESTADO estado_aux;

    int value     = INT_MIN;
    int value_aux = INT_MIN;

    COORDENADA* posicao;
    COORDENADA* melhor_jogada = (COORDENADA*) malloc (sizeof(COORDENADA));
    melhor_jogada->x = -1;
    melhor_jogada->y = -1;

    descobre_jogadas_validas(e, posicoes_jogada_validaPtr);

    list_iter_reset(&it, posicoes_jogada_validaPtr);
    
    while (list_iter_hasNext(&it, posicoes_jogada_validaPtr)) {
            posicao = (COORDENADA *)(list_iter_next(&it, posicoes_jogada_validaPtr));
            posicoesPtr = list_alloc(NULL, apaga_coordenada);
            estado_aux = *e;
            jogada_valida(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            colocar_peca(&estado_aux, posicao->x, posicao->y, posicoesPtr);
            proximo_jogador(&estado_aux);
            list_free(posicoesPtr);
            value_aux = minmax(&estado_aux, nivel - 1, 0);
            if (value_aux > value) {
                melhor_jogada->x = posicao->x;
                melhor_jogada->y = posicao->y;
                value = value_aux;
            }
    }
    list_free(posicoes_jogada_validaPtr);

    return melhor_jogada;
}


/* =============================================================================
 * proximo_jogador
 * -- Calcula o proximo jogador a jogar
 * -- Calcula tambem se o jogo acabou
 * =============================================================================
 */
void proximo_jogador(ESTADO *e) {

	list_t *posicoes_jogada_validaPtr = list_alloc(NULL, apaga_coordenada); 

	e->peca = (e->peca % 2) + 1;

	descobre_jogadas_validas(e, posicoes_jogada_validaPtr);
	if (list_isEmpty(posicoes_jogada_validaPtr)) {
		
		e->peca = (e->peca % 2) + 1;

		descobre_jogadas_validas(e, posicoes_jogada_validaPtr);
        if (list_isEmpty(posicoes_jogada_validaPtr)) {
            list_free(posicoes_jogada_validaPtr);
            e->fim_do_jogo = 1;
        }
        else {
            list_free(posicoes_jogada_validaPtr);
            e->fim_do_jogo = 0;
        }
	}
    else {
        list_free(posicoes_jogada_validaPtr);
        e->fim_do_jogo = 0;
    }
}


/* =============================================================================
 * fim_do_jogo
 * -- Verifica se o jogo chegou ao fim
 * -- Devolve 1 caso o jogo tenha chegado ao fim e 0 em caso contrario
 * =============================================================================
 */
int     
fim_do_jogo(ESTADO *e) {
    return e->fim_do_jogo;
}


/* =============================================================================
 * print_fim_do_jogo
 * -- Imprime a mensagem de fim de jogo
 * =============================================================================
 */
void
print_fim_do_jogo(ESTADO *e) {
    int pecas_jogador_x = conta_numero_de_pecas(e, VALOR_X);
    int pecas_jogador_o = conta_numero_de_pecas(e, VALOR_O);

    if (pecas_jogador_x > pecas_jogador_o) {
        printf("\n\t\t\t\tO jogo terminou com a vitória do jogador X com %d pontos.\n", pecas_jogador_x);
    }
    else if (pecas_jogador_x < pecas_jogador_o) {
        printf("\n\t\t\t\tO jogo terminou com a vitória do jogador O com %d pontos.\n", pecas_jogador_o);
    }
    else {
        printf("\n\t\t\t\tO jogo terminou com um empate\n");
    }
}


/* =============================================================================
 * conta_numero_de_pecas
 * -- Devolve o numero de pecas de uma dada peca
 * =============================================================================
 */
int 
conta_numero_de_pecas(ESTADO *e, VALOR peca) {
	
    int i, j;
	int numero_pecas = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (e->grelha[i][j] == peca) {
				numero_pecas++;
			}
		}
	}

	return numero_pecas;
}