/*
Universidade Presbiteriana Mackenzie
Ciência da Computação – 05P
Computação Paralela
01/03/2025
Alan Meniuk Gleizer
RA 10416804
*/

/*
Atividade 03 - Mult. Matrizes com Threads
Codigo 1 - Ordem de Linha
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// OBS: código realiza multiplicação de matrizes QUADRADAS de mesmo tamanho para simplificar
// OBS: tam. da matriz precisa ser divisivel pelo numero de threads! caso contrario, descomentar "resto"

// função de multiplicação de um INTERVALO de linhas
// será chamada por cada thread
// parametros: matriz A, matriz B, matriz C (resultado), linhas inicial e final [inicial, final[
void multiplicarMatrizesPorLinha(const std::vector<std::vector<int>>& A,const std::vector<std::vector<int>>& B,std::vector<std::vector<int>>& C,int linhaInicio,int linhaFim) {
    int N = (int)A.size();  // para que N não seja parametro.. sizeA = B = C !!

    // percorre as linhas no INTERVALO
    for (int i = linhaInicio; i < linhaFim; i++) {
        // percorre TODAS as colunas
        for (int j = 0; j < N; j++) {
            int soma = 0;
            // soma parcial em k
            for (int k = 0; k < N; k++) {
                soma += A[i][k] * B[k][j];
            }
            // armazena em C[i][j]
            C[i][j] = soma;
        }
    }
}

int main() {
    // def tamanho das matrizes. mantive 1000 como no exemplo
    const int N = 500;

    // criação das matrizes A, B para multiplicação
    // matrizes tem todos os valores = 5. resultado deveria ser uma matriz com todos elementos = 25000
    std::vector<std::vector<int>> A(N, std::vector<int>(N, 5));
    std::vector<std::vector<int>> B(N, std::vector<int>(N, 5));

    // criação da matriz C (resultado), inicializada com 0
    std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

    // def. numero de threads
    int numThreads = 10;

    // inicio do cronometro.. mesmo codigo do exem0plo
    auto inicio = std::chrono::high_resolution_clock::now();

    // Vetor para armazenar as threads
    std::vector<std::thread> threads;

    // não é necessário, mas estava no exemplo do Geeks for Geeks
    //threads.reserve(numThreads);

    // calculo de quantas linhas cada thread irá processar
    int linhasPorThread = N / numThreads;
    
    // também estava no exemplo do Geeks for Geeks mas não será necessário pois temos 1000 linhas e 4 threads...
    // seria necessário se mod != 0
    //int resto = N % numThreads;

    int linhaAtual = 0;

    // criar as threads, cada uma processando um intervalo [linhaInicio,linhaFim[
    for (int t = 0; t < numThreads; t++) {

        // calculo dos parametros que serão passados para cada thread
        int bloco = linhasPorThread /*+ (t < resto ? 1 : 0)*/; // soma do resto não é necessária! ver comentario acima
        int linhaInicio = linhaAtual;
        int linhaFim = linhaInicio + bloco;
        linhaAtual = linhaFim;

        // executar a thread
        // obs geeks for geeks sugere usar emplace_back() ao invez de push, mas vamos seguir com sugestão do prof.
        // obs cref passa o paramewntro por REFERENCIA (c = const, não pode alterar)
        threads.push_back(std::thread(multiplicarMatrizesPorLinha,std::cref(A),std::cref(B),std::ref(C),linhaInicio,linhaFim));
    }

    // Aguarda as threads terminarem
    for (auto& th : threads) {
        th.join();
    }

    // parar o cronometro
    auto fim = std::chrono::high_resolution_clock::now();
    // calcular a duracao em microsegundos
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);

    // exibir o tempo de execução
    std::cout << "\nC[0][0] = " << C[0][0] << std::endl;
    std::cout << "Tempo de execucao (mult. mat. por linha, paralelo): " << duracao.count() << " microsegundos\n" << std::endl;

    return 0;
}
