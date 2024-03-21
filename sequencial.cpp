#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


using namespace std;

double tempoDeExecucao;
int i, j, k = 0, soma = 0, dividir, semente;
long long int tam1 = -1, tam2 = -1, primos;
int matriz[20000][20000];


void mostradorprimo(const vector<vector<int>>& submatrizes);
vector<vector<int>> submatrizesfunc(long long int tam1, long long int tam2, int dividir);
int isPrime(int n);
void GeradorNum(long long int tam1, long long int tam2, int sementematriz);
void menu();



//Main Principal
int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8");
    menu();

    return 0;
}

//Menu de interacao

void menu(){
    int menuint = 0, matrizPreenchida = -1, executado = 0;

    do {
        printf("----------------------------------\n");
        printf("---------------Menu---------------\n");
        printf("----------------------------------\n");
        printf("1 - Defina o tamanho da matriz -\n");
        printf("2 - Defina a semente de criacao da matriz - \n");
        printf("3 - Preencher a matriz com numeros aleatorios -\n");
        printf("4 - Definir o numero de Submatrizes -\n");
        printf("5 - Executar -\n");
        printf("6 - Visualizar o tempo de execuçao e quantidade de numeros primos -\n");
        printf("7 - Encerrar -\n");
        scanf("%d", &menuint);
        printf("\n");

        switch (menuint){
            case 1:
                printf("Digite um tamanho para o numero de linhas: \n");
                cin >> tam1;
                printf("Digite um tamanho para o numero de colunas: \n");
                cin >> tam2;
                printf("\n");
                matrizPreenchida = -1;
                
                break;

            case 2:
                printf("Defina a semente de geracao dos numeros aleatorios: \n");
                scanf("%d", &semente);
                primos = 0;
            break;

            case 3:
                if (tam1 == -1 || tam2 == -1){ 
                    printf("Tamanho da Matriz nao definido \n");
                }else {
                printf("Preenchendo a matriz com numeros aleatorios \n");
                GeradorNum(tam1, tam2, semente);
                matrizPreenchida = 1;
                printf("Matriz foi preenchida.\n");
            }
            break;

            case 4:
                printf("Deseja dividir em quantas submatrizes: \n");
                scanf("%d", &dividir);
                primos =0;
                printf("\n");
            break;

            case 5:
            if (matrizPreenchida <= 0){
                printf("Matriz ainda não preenchida \n");
            }
            else {
                
                cout << "Aguarde enquanto procuramos os números primos..." << endl;
                vector<vector<int>> submatrizes(dividir);
                clock_t inicio = clock();
                submatrizes = submatrizesfunc(tam1, tam2, dividir);
                mostradorprimo(submatrizes);
                clock_t fim = clock();
                tempoDeExecucao = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;
                cout << "Terminado!" << endl;
                executado = 1;
            }
            cout << "\n";
            break;

            case 6:
                if (executado == 0)cout << "Programa não executado anteriormente!" << endl;
                else {
                    
                    printf("Total de numeros primos encontrados nas submatrizes: %d \n", primos);
                    cout << "O tempo de execucao deste programa foi: " << tempoDeExecucao << endl;
                }
                cout << "\n";
            break;

        default:
        break;
        }
        } while (menuint != 7);
}

//Gera Numeros Aleatorios
void GeradorNum(long long int tam1, long long int tam2, int semente){
    // Inicializa o gerador de números aleatórios com o tempo atual
    std::srand(semente);

    // Gera números aleatórios entre 0 e 99999999 e os armazena na matriz
    for(i = 0; i < tam1; i++){
        for(j = 0; j < tam2; j++){
            int random_number = std::rand() % 100000000;
            matriz[i][j] = random_number;            
        }
    }

}


//Mostra se o Numero e ou nao Primo
int isPrime(int n) {
    if (n <= 1) {
        return false;
    }


    for (int i = 2; i <= n/2; i++) {
        if (n % i == 0) {
            return false;
        }
    }


    return true;
}

//

vector<vector<int>> submatrizesfunc(long long int tam1, long long int tam2, int dividir){
    // calcular o tamanho de cada submatriz
    int tamanho = tam1 * tam2 / dividir;
   
    // criar as submatrizes
    vector<vector<int>> submatrizes(dividir);
    int idx = 0;
    for (i = 0; i < dividir; i++) {
        int elementos = 0;
        while (elementos < tamanho && idx < tam1 * tam2) {
            int linha = idx / tam2;
            int coluna = idx % tam2;
            submatrizes[i].push_back(matriz[linha][coluna]);
            elementos++;
            idx++;
        }
    }
    return submatrizes;
}

// Percorre as submatrizes para verirficar se o numero e primo

void mostradorprimo(const vector<vector<int>>& submatrizes){
    //Percorre cada submatriz e mostra se cada número é primo
    for(i = 0; i < submatrizes.size(); i++){
        for(j = 0; j < submatrizes[i].size(); j++){
            if (isPrime(submatrizes[i][j])){
                //printf("O Numero e Primo \n");
                primos += 1;
            } else {
                //printf("O Numero Nao e Primo \n");
            }
        }
    }
}
