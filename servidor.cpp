#include <iostream>
#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include "fila.hpp"
using namespace std;

struct TrabalhoImpressao {
    int id_job;
    char nome_arquivo[50];
    int numero_paginas;
};

Fila <TrabalhoImpressao> filaImpressoes;
mutex mutexThreads;
condition_variable condVar;
thread pool[3];

void imprimir(int id) {
    while(true){    
        TrabalhoImpressao imprimir;

        unique_lock<mutex> lock(mutexThreads);
        condVar.wait(lock, [] { return !vazia(&filaImpressoes); });

        pop(&filaImpressoes, imprimir);
        lock.unlock();

        cout << "Imprimindo " << imprimir.nome_arquivo << "..." << endl << endl;
        Sleep(2000);
        cout << "Impressao completa. " << endl;
        cout << "Id: " << imprimir.id_job << endl;
        cout << "Nome: " << imprimir.nome_arquivo << endl;
        cout << "Numero de paginas: " << imprimir.numero_paginas << endl << endl;

        ofstream ofs("log_servidor.txt", ios::app);
        ofs << "Id: " << imprimir.id_job << ", Nome: " << imprimir.nome_arquivo << ", Num. paginas: " << imprimir.numero_paginas << endl;
        ofs.close();
}

}

int main() {
    inicializarFila(&filaImpressoes);

    for(int i = 0; i < 3; i++){
        pool[i] = thread(imprimir, i + 1);
    }

    const char* pipeName = R"(\\.\pipe\impressao)";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_INBOUND, //Só pode ler
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        3,
        512,
        512,
        0,
        NULL
    );

    cout << "Esperando cliente se conectar" << endl;

    while(true) {
        bool connect = ConnectNamedPipe(hPipe, NULL);
        if(connect) {
            cout << "Cliente conectado" << endl;
            TrabalhoImpressao recebido;
            DWORD bytesRead;

            ReadFile(hPipe, &recebido, sizeof(TrabalhoImpressao), &bytesRead, NULL);

            cout << "Recebido do cliente: " << recebido.nome_arquivo << endl;

            {
                lock_guard<mutex> lock(mutexThreads);
                push(&filaImpressoes, recebido);
            }
            condVar.notify_one();

            DisconnectNamedPipe(hPipe);
        }
        else {
            cout << "Erro ao conectar o pipe";
            break;
        }
    }

    return 0;
}