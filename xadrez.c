#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Função para imprimir o tabuleiro
void imprimirTabuleiro(char tabuleiro[8][8]) {
    printf("\n  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}

// Função para verificar se a posição tem peça do mesmo jogador
int mesmaCor(char peca1, char peca2) {
    if (peca1 == '.' || peca2 == '.') return 0;
    return (isupper(peca1) && isupper(peca2)) || (islower(peca1) && islower(peca2));
}

// Movimento da Torre
int torreValida(char tabuleiro[8][8], int ori_lin, int ori_col, int des_lin, int des_col) {
    if (ori_lin != des_lin && ori_col != des_col) return 0;

    if (ori_lin == des_lin) {
        int dir = (des_col > ori_col) ? 1 : -1;
        for (int col = ori_col + dir; col != des_col; col += dir) {
            if (tabuleiro[ori_lin][col] != '.') return 0;
        }
    } else {
        int dir = (des_lin > ori_lin) ? 1 : -1;
        for (int lin = ori_lin + dir; lin != des_lin; lin += dir) {
            if (tabuleiro[lin][ori_col] != '.') return 0;
        }
    }
    return 1;
}

// Movimento do Bispo
int bispoValido(char tabuleiro[8][8], int ori_lin, int ori_col, int des_lin, int des_col) {
    if (abs(des_lin - ori_lin) != abs(des_col - ori_col)) return 0;

    int dir_lin = (des_lin > ori_lin) ? 1 : -1;
    int dir_col = (des_col > ori_col) ? 1 : -1;

    int lin = ori_lin + dir_lin;
    int col = ori_col + dir_col;

    while (lin != des_lin && col != des_col) {
        if (tabuleiro[lin][col] != '.') return 0;
        lin += dir_lin;
        col += dir_col;
    }

    return 1;
}

// Movimento do Cavalo
int cavaloValido(int ori_lin, int ori_col, int des_lin, int des_col) {
    int dlin = abs(des_lin - ori_lin);
    int dcol = abs(des_col - ori_col);
    return (dlin == 2 && dcol == 1) || (dlin == 1 && dcol == 2);
}

// Movimento da Rainha
int rainhaValida(char tabuleiro[8][8], int ori_lin, int ori_col, int des_lin, int des_col) {
    return torreValida(tabuleiro, ori_lin, ori_col, des_lin, des_col) ||
           bispoValido(tabuleiro, ori_lin, ori_col, des_lin, des_col);
}

// Movimento do Rei
int reiValido(int ori_lin, int ori_col, int des_lin, int des_col) {
    int dlin = abs(des_lin - ori_lin);
    int dcol = abs(des_col - ori_col);
    return (dlin <= 1 && dcol <= 1);
}

// Movimento do Peão
int peaoValido(char tabuleiro[8][8], int ori_lin, int ori_col, int des_lin, int des_col, char peca) {
    int direcao = (peca == 'P') ? -1 : 1;
    int linha_inicial = (peca == 'P') ? 6 : 1;

    // Movimento para frente
    if (ori_col == des_col) {
        if (des_lin == ori_lin + direcao && tabuleiro[des_lin][des_col] == '.') {
            return 1;
        }
        if (ori_lin == linha_inicial && des_lin == ori_lin + 2 * direcao &&
            tabuleiro[ori_lin + direcao][des_col] == '.' &&
            tabuleiro[des_lin][des_col] == '.') {
            return 1;
        }
    }

    // Captura diagonal
    if (abs(des_col - ori_col) == 1 && des_lin == ori_lin + direcao &&
        tabuleiro[des_lin][des_col] != '.') {
        return 1;
    }

    return 0;
}

int main() {
    char tabuleiro[8][8];
    int linha, coluna;

    // Inicializando o tabuleiro
    for (linha = 0; linha < 8; linha++) {
        for (coluna = 0; coluna < 8; coluna++) {
            tabuleiro[linha][coluna] = '.';
        }
    }

    // Posicionando as peças pretas
    char pecas[8] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
    for (coluna = 0; coluna < 8; coluna++) {
        tabuleiro[0][coluna] = pecas[coluna];
        tabuleiro[1][coluna] = 'p';
    }

    // Posicionando as peças brancas
    for (coluna = 0; coluna < 8; coluna++) {
        tabuleiro[6][coluna] = 'P';
        tabuleiro[7][coluna] = pecas[coluna] - 32;
    }

    char origem_coluna, destino_coluna;
    int origem_linha, destino_linha;
    int jogador = 1;
    char continuar;

    do {
        imprimirTabuleiro(tabuleiro);
        printf("\nJogador %d (%s)\n", jogador + 1, jogador == 1 ? "Branco" : "Preto");

        printf("Digite a posição de origem (ex: e2): ");
        scanf(" %c%d", &origem_coluna, &origem_linha);

        printf("Digite a posição de destino (ex: e4): ");
        scanf(" %c%d", &destino_coluna, &destino_linha);

        int ori_col = origem_coluna - 'a';
        int ori_lin = 8 - origem_linha;
        int des_col = destino_coluna - 'a';
        int des_lin = 8 - destino_linha;

        if (ori_col >= 0 && ori_col < 8 && ori_lin >= 0 && ori_lin < 8 &&
            des_col >= 0 && des_col < 8 && des_lin >= 0 && des_lin < 8) {

            char peca = tabuleiro[ori_lin][ori_col];
            char destino = tabuleiro[des_lin][des_col];

            if (peca == '.') {
                printf("Não há peça na posição de origem!\n");
            } else if ((jogador == 1 && islower(peca)) || (jogador == 0 && isupper(peca))) {
                printf("Essa peça não é sua!\n");
            } else if (mesmaCor(peca, destino)) {
                printf("Você não pode capturar sua própria peça!\n");
            } else {
                int valido = 0;

                switch (tolower(peca)) {
                    case 'r':
                        valido = torreValida(tabuleiro, ori_lin, ori_col, des_lin, des_col);
                        break;
                    case 'n':
                        valido = cavaloValido(ori_lin, ori_col, des_lin, des_col);
                        break;
                    case 'b':
                        valido = bispoValido(tabuleiro, ori_lin, ori_col, des_lin, des_col);
                        break;
                    case 'q':
                        valido = rainhaValida(tabuleiro, ori_lin, ori_col, des_lin, des_col);
                        break;
                    case 'k':
                        valido = reiValido(ori_lin, ori_col, des_lin, des_col);
                        break;
                    case 'p':
                        valido = peaoValido(tabuleiro, ori_lin, ori_col, des_lin, des_col, peca);
                        break;
                }

                if (valido) {
                    tabuleiro[des_lin][des_col] = peca;
                    tabuleiro[ori_lin][ori_col] = '.';
                    jogador = !jogador;
                } else {
                    printf("Movimento inválido!\n");
                }
            }
        } else {
            printf("Posição inválida!\n");
        }

        printf("Deseja continuar? (s/n): ");
        scanf(" %c", &continuar);

    } while (continuar == 's' || continuar == 'S');

    printf("Fim de jogo!\n");

    return 0;
}
