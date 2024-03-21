#include <iostream>
#include <vector>
#include <math.h>
#include <windows.h>
#include <process.h>
#include <ctime>
#include <random>

#define SEED 100401

using namespace std;

int k = 0, soma = 0,  numsub, sementematriz;
long long int lines = -1, columns = -1, totalPrimos, dividir = 1;
time_t tempoDeExecucao, inicio, fim;

typedef struct {
    int id;

    int startLine, startColumn, endLine, endColumn;
} PARAMETRO;

HANDLE secaoCritica;

vector<HANDLE> idThread;
vector<HANDLE> idThread2;
vector<vector<long long int>> matriz;
vector<vector<long long int>> matriz2;
vector<PARAMETRO> vetorParametro;
vector<PARAMETRO> vetorParametro2;

void menu();
void GeradorNum(long long int lines, long long int columns, int sementematriz);
int isPrime(int n);
void esvaziarMatriz();
void preencherParametros(int numberThreads, long long int lines, long long int columns, long long int dividir);
void esvaziarParametros();
void thread(void* matrizParametros);
void thread2(void* matrizParametros);
void createThreads(int numberThreads, int switchThread, long long int dividir);

//Main Principal
int main()
{
    srand(SEED);
    setlocale(LC_ALL, "pt_BR.UTF-8");
    secaoCritica = CreateMutex(NULL, FALSE, NULL);
    menu();

    return 0;
}

//menu de interacao
void menu(){
    int menuint = 0, numThreads = 1, testevalida = -1, executado = 0;

    do {
        printf("----------------------------------------------------------------------\n");
        printf("---------------------------------Menu---------------------------------\n");
        printf("----------------------------------------------------------------------\n");
        printf("1 - Defina o tamanho da matriz -\n");
        printf("2 - Defina a semente de criacao da matriz - \n");
        printf("3 - Preencher a matriz com numeros aleatorios -\n");
        printf("4 - Definir o numero de Submatrizes -\n");
        printf("5 - Definir o numero de Threads -\n");
        printf("6 - Executar -\n");
        printf("7 - Visualizar o tempo de execucao e quantidade de numeros primos -\n");
        printf("8 - Encerrar -\n");
        scanf("%d", &menuint);
        printf("----------------------------------------------------------------------\n");
        printf("----------------------------------------------------------------------\n");
        printf("----------------------------------------------------------------------\n");

        switch (menuint){
            case 1:
                esvaziarMatriz();
                printf("Digite um tamanho para o numero de linhas: \n");
                cin >> lines;
                printf("Digite um tamanho para o numero de colunas: \n");
                cin >> columns;
                printf("\n");
                testevalida = -1;
                matriz.resize(lines, vector<long long int>(columns));
                break;

            case 2:
                printf("Defina a semente de geracao dos numeros aleatorios: \n");
                scanf("%d", &sementematriz);
                printf("\n");
                printf("Semente gerada \n\n");
            break;

            case 3:
                if (lines == -1 || columns == -1){ 
                    printf("Tamanho da Matriz nao definido \n");
                }else {
                printf("Preenchendo a matriz com numeros aleatorios \n");
                GeradorNum(lines, columns, sementematriz);
                testevalida = 1;
                totalPrimos = 0;
                printf("Matriz foi preenchida.\n\n");
            }
            break;

            case 4:
                printf("Deseja dividir em quantas submatrizes: \n");
                cin >> dividir;
                printf("\n");
                printf("Matriz dividida \n\n");
            break;

            case 5:
                printf("Deseja Utilizar quantas Threads: \n");
                scanf("%d", &numThreads);
                printf("\n");
            break;

            case 6:
            if (testevalida <= 0){
                printf("Matriz ainda não preenchida \n\n");
            }
            else {
                //totalPrimos = 0;
                printf("Aguarde enquanto procuramos os numeros primos... \n");
                preencherParametros(numThreads, lines, columns, dividir);
                printf("Terminado! \n\n");
                executado = 1;
            }
            cout << "\n";
            break;

            case 7:
                if (executado == 0){
                    printf("Programa não executado anteriormente!\n");
                }else {
                    cout << "\nA quantidade total de primos desta matriz e: " << totalPrimos << endl;
                    cout << "\nO tempo de execucao deste programa foi: " << tempoDeExecucao << endl;
                    
                }
                cout << "\n";
            break;

        default:
        break;
        }
        } while (menuint != 8);
}


//Gera Numeros Aleatorios
void GeradorNum(long long int lines, long long int columns, int sementematriz){
    // Inicializa o gerador de números aleatórios com a semente
    std::srand(sementematriz);
    // Gera números aleatórios entre 0 e 99999999 e os armazena na matriz
    for(int i = 0; i < lines; i++){
        for(int j = 0; j < columns; j++){
            int random_number = std::rand() % 100000000;
            matriz[i][j] = random_number;            
        }
    }

}

//Mostra se o Numero e ou nao Primo
int isPrime(int number) {
    if (number <= 1) {
        return false;
    }
    for (int i = 2; i <= number/2; i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

//Esvazia a matriz
void esvaziarMatriz() {
    matriz.swap(matriz2);
    matriz2.clear();
    idThread.swap(idThread2);
    idThread2.clear();

    esvaziarParametros();
}

// Inicializa os vetores com parâmetros para as threads processarem os dados da matriz.

void preencherParametros(int numberThreads, long long int lines, long long int columns, long long int dividir) {
    PARAMETRO addDataMatriz;
    int totalNumbers = 0, totalEachThread = 0, firstLine = 0, lastLine = 0 , firstColumn = 0, lastColumn = 0, decider = 0;

    totalNumbers = lines * columns;
    totalEachThread = totalNumbers / dividir;
    lastLine = totalEachThread;
    lastColumn = totalEachThread;

    if (lines >= columns) {
        if ((lines * columns) % dividir == 0) {
            for (int i = 0; i < dividir; i++) {
                addDataMatriz.id = i;
                addDataMatriz.startColumn = firstColumn;
                addDataMatriz.startLine = firstLine;
                addDataMatriz.endLine = lastLine;
                vetorParametro.push_back(addDataMatriz);

                for (int j = firstLine; j < lastLine; j++) {
                    if (j % lines == 0 && j != 0) firstColumn++;
                }

                firstLine = lastLine;
                lastLine = lastLine + totalEachThread;
            }
        }
        else {
            for (int i = 0; i < dividir; i++) {
                addDataMatriz.id == i;
                if (i == (dividir - 1)) {
                    addDataMatriz.startColumn = firstColumn;
                    addDataMatriz.startLine = firstLine;
                    addDataMatriz.endLine = lines * columns;
                    vetorParametro.push_back(addDataMatriz);
                }
                else {
                    addDataMatriz.startColumn = firstColumn;
                    addDataMatriz.startLine = firstLine;
                    addDataMatriz.endLine = lastLine;
                    vetorParametro.push_back(addDataMatriz);
                }

                for (int j = firstLine; j < lastLine; j++) {
                    if (j % lines == 0 && j != 0)firstColumn++;
                }

                firstLine = lastLine;
                lastLine = lastLine + totalEachThread;
            }
        }
        decider = 1;
    }
    else {
        if ((lines * columns) % dividir == 0) {
            for (int i = 0; i < dividir; i++) {
                addDataMatriz.id = i;
                addDataMatriz.startColumn = firstColumn;
                addDataMatriz.startLine = firstLine;
                addDataMatriz.endColumn = lastColumn;
                vetorParametro.push_back(addDataMatriz);

                for (int j = firstColumn; j < lastColumn; j++) {
                    if (j % columns == 0 && j != 0) firstLine++;
                }

                firstColumn = lastColumn;
                lastColumn = lastColumn + totalEachThread;
            }
        }
        else {
            for (int i = 0; i < dividir; i++) {
                if (i == dividir - 1) {
                    addDataMatriz.id = i;
                    addDataMatriz.startColumn = firstColumn;
                    addDataMatriz.startLine = firstLine;
                    addDataMatriz.endColumn = lines * columns;
                    vetorParametro.push_back(addDataMatriz);
                }
                else {
                    addDataMatriz.id = i;
                    addDataMatriz.startColumn = firstColumn;
                    addDataMatriz.startLine = firstLine;
                    addDataMatriz.endColumn = lastColumn;
                    vetorParametro.push_back(addDataMatriz);
                }

                for (int j = firstColumn; j < lastColumn; j++) {
                    if (j % columns == 0 && j != 0) firstLine++;
                }

                firstColumn = lastColumn;
                lastColumn = lastColumn + totalEachThread;
            }
        }
        decider = 2;
    }


    createThreads(numberThreads, decider, dividir);
    esvaziarParametros();
}

// Cria threads para processar os dados da matriz.

void createThreads(int numberThreads, int switchThread, long long int dividir) {
    if (switchThread == 1) {
        for (int i = 0; i < dividir; i++) {
            idThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, &vetorParametro[i], CREATE_SUSPENDED, NULL));
        }
        inicio = time(0);
        for (int i = 0; i < numberThreads; i++) {
            for (int j = 0; j < dividir; j++)
            {
                ResumeThread(idThread[j]);
            }
        }
        
        WaitForMultipleObjects(numberThreads, idThread.data(), TRUE, INFINITE);
        fim = time(0);
        
    }
    else {
        for (int i = 0; i < dividir; i++) {
            idThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread2, &vetorParametro[i], CREATE_SUSPENDED, NULL));
        }
        inicio = time(0);
        for (int i = 0; i < numberThreads; i++) {
            for (int j = 0; j < dividir; j++)
            {
                ResumeThread(idThread[j]);
            }
        }
        WaitForMultipleObjects(numberThreads, idThread.data(), TRUE, INFINITE);
        fim = time(0);
    }
    tempoDeExecucao = fim - inicio;
}

// Esvaziar Parametros

void esvaziarParametros() {
    vetorParametro.swap(vetorParametro2);
    vetorParametro2.clear();
}

// Responsável por processar uma parte dos dados da matriz e encontrar números primos.

void thread(void* matrizParametros){
    PARAMETRO* dataMatriz;
    dataMatriz = (PARAMETRO*)matrizParametros;

    int travessiaLinha = 0, contadorPrimos = 0, travessiaColunas = 0;

    travessiaColunas = dataMatriz->startColumn;
    travessiaLinha = dataMatriz->startLine % lines;

    for (int i = dataMatriz->startLine; i < dataMatriz->endLine; i++) {       
        if (i % lines == 0 && i != 0){ travessiaLinha = 0;}

        if (travessiaLinha % lines == 0 && i != 0){travessiaColunas++;}

        if (isPrime(matriz[travessiaLinha][travessiaColunas])){ contadorPrimos++;}

        travessiaLinha++;
    }

    WaitForSingleObject(secaoCritica, INFINITE);

    totalPrimos += contadorPrimos;

    ReleaseMutex(secaoCritica);

    _endthread();
}

// Responsável por processar uma parte dos dados da matriz e encontrar números primos.

void thread2(void* matrizParametros) {
    PARAMETRO* dataMatriz;
    dataMatriz = (PARAMETRO*)matrizParametros;

    int travessiaLinha = 0, contadorPrimos = 0, travessiaColunas = 0;

    travessiaColunas = dataMatriz->startColumn % columns;
    travessiaLinha = dataMatriz->startLine;

    for (int i = dataMatriz->startColumn; i < dataMatriz->endColumn; i++) {
        if (i % columns == 0 && i != 0){ travessiaColunas = 0;}

        if (travessiaColunas % columns == 0 && i != 0){travessiaLinha++;}

        if (isPrime(matriz[travessiaLinha][travessiaColunas])) {contadorPrimos++;}

        travessiaColunas++;
        
    }

    WaitForSingleObject(secaoCritica, INFINITE);

    totalPrimos += contadorPrimos;

    ReleaseMutex(secaoCritica);

    _endthread();
}