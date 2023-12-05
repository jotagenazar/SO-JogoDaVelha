/*

Alunos: João Gabriel Manfré Nazar - 13733652
        João Pedro Mori Machado - 

*/

#include <iostream>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <vector>

using namespace std;


// Inicialização das variáveis de sincronização condicional
pthread_cond_t cond_1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_2 = PTHREAD_COND_INITIALIZER;

// Inicialização dos semáforos binários do tipo mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;


// Definicoes gerais de convencoes do programa
#define TAMANHO_TABULEIRO 3
#define POSICAO_NULA '-'


// Definição da classe tabuleiro
// Responsável por guardar os movimentos realizados e verificar o estado atual do jogo
class Tabuleiro {
    private:
        vector<vector<char>> matrizPosicoes;
        int tamanho = TAMANHO_TABULEIRO;

    public:

        // Construtor padrão do tabuleiro
        // Define o tamanho da matriz de posicoes como o tamanho armazenado na classe do tabuleiro
        Tabuleiro() {
            matrizPosicoes.resize(tamanho);

            for(int i = 0; i < tamanho; ++i) 
                matrizPosicoes[i].assign(tamanho, POSICAO_NULA);

            clog << "matriz do tabuleiro dimensionada com sucesso";
        }

        // funcao que reseta a matriz de posicoes para o estado inicial
        void resetMatriz() {

            for(int i = 0; i < tamanho; ++i) {
                for(int j = 0; j < tamanho; ++j)
                    matrizPosicoes[i][j] = POSICAO_NULA;
            }

            clog << "matriz do tabuleiro reiniciada com sucesso";
        }

        // getter da matriz de posicoes
        vector<vector<char>> getMatriz() {
            return matrizPosicoes;
        }

        // funcao que insere um caractere na matriz de posicoes, e consequentemente no tabuleiro e no jogo
        bool inserirNaMatriz(int i, int j, char jogada) {
            if(i > tamanho - 1) {
                clog << "inserção na matriz do tabuleiro falhou: a coordenada i é maior que a dimensao do tabuleiro";
                return false;
            }

            if(j > tamanho - 1) {
                clog << "inserção na matriz do tabuleiro falhou: a coordenada j é maior que a dimensao do tabuleiro";
                return false;
            }

            if(matrizPosicoes[i][j] != POSICAO_NULA) {
                clog << "inserção na matriz do tabuleiro falhou: a posicao ij ja esta ocupada";
                return false;
            }

            matrizPosicoes[i][j] = jogada;
            return true;
        }
};