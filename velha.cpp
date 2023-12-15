/*

Alunos: Gabriela Amadori - 11832916
        João Gabriel Manfré Nazar - 13733652
        João Pedro Mori Machado - 13671831
        João Vitor Alves de Medeiros - 13726762

Jogo da Velha - Multithreading para jogadores - Semáforo Mutex para zona crítica da alteração do tabuleiro

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

// Inicialização do semáforo binário do tipo mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Definicoes gerais de convencoes do programa
#define TAMANHO_TABULEIRO 3
#define POSICAO_NULA '-'
#define JOGADOR1 'X'
#define JOGADOR2 'O'


// Definição da classe tabuleiro
// Responsável por guardar os movimentos realizados e verificar o estado atual do jogo
class Tabuleiro {
    private:
        vector<vector<char>> matrizPosicoes;
        int tamanho = TAMANHO_TABULEIRO;

        // funcao que checa se o jogador com o caractere passado venceu em cada linha
        bool checarVitoriaLinha(char jogador) {

            int ocorrencias; // conta quantas vezes o char aparece

            // laço para cada linha
            for(int i = 0; i < tamanho; ++i) {
                ocorrencias = 0;

                // teste em cada posicao de coluna da linha
                for(int j = 0; j < tamanho; ++j) {
                    if(matrizPosicoes[i][j] == jogador) ++ocorrencias;
                    else break; // se nao for igual, o jogador ja nao ganhou na linha atual
                }

                // se o caractere apareceu em todas as colunas da linha, o jogador venceu
                if(ocorrencias == tamanho) return true;
            }

            return false;
        }

        // funcao que checa se o jogador com o caractere passado venceu em cada coluna
        bool checarVitoriaColuna(char jogador) {

            int ocorrencias; // conta quantas vezes o char aparece
            
            // laço para cada coluna
            for(int j = 0; j < tamanho; ++j) {
                ocorrencias = 0;

                // teste em cada posicao de linha da coluna
                for(int i = 0; i < tamanho; ++i) {
                    if(matrizPosicoes[i][j] == jogador) ++ocorrencias;
                    else break; // se nao for igual, o jogador ja nao ganhou na coluna atual
                }

                // se o caractere apareceu em todas as linhas da coluna, o jogador venceu
                if(ocorrencias == tamanho) return true;
            }

            return false;
        }

        // funcao que checa se o jogador com o caractere passado venceu na diagonal
        bool checarVitoriaDiagonal(char jogador) {

            int ocorrencias = 0; // conta quantas vezes o char aparece

            // laco para cada posicao da diagonal i = j
            for(int i = 0; i < tamanho; ++i) {
                if(matrizPosicoes[i][i] == jogador) ++ocorrencias;
                else break;
            }

            // se o caractere apareceu em todas as posicoes da diagonal, o jogador venceu
            if(ocorrencias == tamanho) return true;

            return false;
        }

        // funcao que checa se o jogador com o caractere passado venceu na diagonal secundaria
        bool checarVitoriaDiagonalSecundaria(char jogador) {

            int ocorrencias = 0; // conta quantas vezes o char aparece

            // laco para cada posicao da diagonal i = tamanho - j
            for(int i = 0; i < tamanho; ++i) {
                if(matrizPosicoes[i][tamanho-1-i] == jogador) ++ocorrencias;
                break;
            }

            if(ocorrencias == tamanho) return true;

            return false;
        }

    public:
        // Construtor padrão do tabuleiro
        // Define o tamanho da matriz de posicoes como o tamanho armazenado na classe do tabuleiro
        Tabuleiro() {
            matrizPosicoes.resize(tamanho);

            for(int i = 0; i < tamanho; ++i) 
                matrizPosicoes[i].assign(tamanho, POSICAO_NULA);

            cout << "matriz do tabuleiro dimensionada com sucesso\n";
        }

        // funcao que reseta a matriz de posicoes para o estado inicial
        void resetMatriz() {

            for(int i = 0; i < tamanho; ++i) {
                for(int j = 0; j < tamanho; ++j)
                    matrizPosicoes[i][j] = POSICAO_NULA;
            }

            cout << "matriz do tabuleiro reiniciada com sucesso\n";
        }

        // funcao que imprime a matriz de posicoes
         void imprimir() {

            cout << "\n******************\n\n";

            // impressao da legenda de coordenadas das colunas
            cout << "    ";
            for(int i = 1; i <= tamanho; ++i) {
                cout << i << "   ";
            }
            cout << endl << endl;

            // impressao de cada linha
            for(int i = 0; i < tamanho - 1; ++i) {
                cout << i+1 << "   ";
                cout << matrizPosicoes[i][0];
                for(int j = 1; j < tamanho; ++j){ 
                    cout <<  " | " << matrizPosicoes[i][j];
                } cout << endl;

                // impressao da separacao de cada linha
                cout << "    __";
                for(int j = 1; j < tamanho; ++j){
                    cout << "|___";
                } cout << endl;
            }

            // impressao da ultima linha fora do laço, questao de implementacao da ASCII Art
            cout << tamanho << "   ";
            cout << matrizPosicoes[tamanho-1][0];
            for(int j = 1; j < tamanho; ++j){ 
                cout <<  " | " << matrizPosicoes[tamanho-1][j];
            } cout << endl;

            cout << "\n******************\n\n";
        }

        // getter da matriz de posicoes
        vector<vector<char>> getMatriz() {
            return matrizPosicoes;
        }

        // funcao que insere um caractere na matriz de posicoes, e consequentemente no tabuleiro e no jogo
        bool inserirNaMatriz(int i, int j, char jogada) {
            if(i > tamanho - 1 || i < 0) {
                cout << "Inserção na matriz do tabuleiro falhou: a coordenada [i] esta fora da dimensao do tabuleiro\n";
                return false;
            }

            if(j > tamanho - 1 || j < 0) {
                cout << "Inserção na matriz do tabuleiro falhou: a coordenada [j] esta fora da dimensao do tabuleiro\n";
                return false;
            }

            if(matrizPosicoes[i][j] != POSICAO_NULA) {
                cout << "Inserção na matriz do tabuleiro falhou: a posicao [i][j] ja esta ocupada\n";
                return false;
            }

            matrizPosicoes[i][j] = jogada;
            return true;
        }

        // funcao que verifica se o jogador com o caractere passado venceu o jogo em algum dos metodos de vitoria
        bool checarVitoria(char jogador) {

            if(checarVitoriaLinha(jogador)) return true;

            if(checarVitoriaColuna(jogador)) return true;

            if(checarVitoriaDiagonal(jogador)) return true;

            if(checarVitoriaDiagonalSecundaria(jogador)) return true;

            return false;
        }
};

// Declarando o tabuleiro de forma global
Tabuleiro tabuleiro;

// funcao de jogada do jogador 1
void *jogada_jogador1(void*) {
    // lockando a região crítica, que é a jogada/alteraçao da matriz de posicoes, ou seja, o tabuleiro
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond_1, &mutex);

    //impressão do tabuleiro
    cout << "Jogador " << JOGADOR1 << " jogando..." << endl;
    
    // leitura da jogada e teste se é uma jogada valida
    bool cond = false;
    while(cond == false) {
        cout << "Insira a linha e a coluna da sua jogada, respectivamente: ";
        int i, j;
        cin >> i >> j;
        cond = tabuleiro.inserirNaMatriz(i - 1, j - 1, JOGADOR1);
    }

    // liberando regiao crítica para jogada do próximo jogador
    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}

// funcao de jogada do jogador 2
void *jogada_jogador2(void*) {
    // lockando a região crítica, que é a jogada/alteraçao da matriz de posicoes, ou seja, o tabuleiro
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond_2, &mutex);

    cout << "Jogador " << JOGADOR2 << " jogando..." << endl;
    
    // leitura da jogada e teste se é uma jogada valida
    bool cond = false;
    while(cond == false) {
        cout << "Insira a linha e a coluna da sua jogada, respectivamente: ";
        int i, j;
        cin >> i >> j;
        cond = tabuleiro.inserirNaMatriz(i - 1, j - 1, JOGADOR2);
    }

    // liberando regiao crítica para jogada do próximo jogador
    pthread_mutex_unlock(&mutex);

    return EXIT_SUCCESS;
}

// funcao que encapsula o teste de vitoria de cada jogador
bool verificar_jogo(char jogador) {

    cout << "Verificando se " << jogador << " venceu\n";

    if(!tabuleiro.checarVitoria(jogador)) return false;

    // else o jogador ganhou
    cout << "O jogador " << jogador << " venceu. Parabéns!!!\n\n";

    return true;
}

int main() {
    // Criando o tabuleiro e colocando todas as posiçoes como nula para o jogo começar
    tabuleiro.resetMatriz();

    // Criando uma thread para cada jogador
    pthread_t jogador1, jogador2;
    bool ganhou = false;

    for(int rodada = 0; rodada < TAMANHO_TABULEIRO * TAMANHO_TABULEIRO;) {

        int ret1_thread = pthread_create(&jogador1, NULL, jogada_jogador1, NULL);
        int ret2_thread = pthread_create(&jogador2, NULL, jogada_jogador2, NULL);

        // Vez do jogador1 jogar
        tabuleiro.imprimir();
        pthread_cond_signal(&cond_1);
        pthread_join(jogador1, NULL);
        if(ganhou = verificar_jogo(JOGADOR1)) break;
        ++rodada;

        if(rodada == TAMANHO_TABULEIRO * TAMANHO_TABULEIRO) break;

        // Vz do jogador2 jogar
        tabuleiro.imprimir();
        pthread_cond_signal(&cond_2);
        pthread_join(jogador2, NULL);
        if(ganhou = verificar_jogo(JOGADOR2)) break;
        ++rodada;
    }

    if(!ganhou) cout << "O jogo deu velha!!!\n\n";

    return 0;
}
