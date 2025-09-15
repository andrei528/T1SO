#include <iostream>
#include <windows.h>
using namespace std;

struct TrabalhoImpressao {
    int id_job;
    char nome_arquivo[50];
    int numero_paginas;
};

int main() {
    const char* pipeName = R"(\\.\pipe\impressao)";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_INBOUND, //Só pode ler
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        1,
        512,
        512,
        0,
        NULL
    );

    cout << "Esperando cliente se conectar" << endl;
    ConnectNamedPipe(hPipe, NULL);
    cout << "Cliente conectado" << endl;

    TrabalhoImpressao recebido;
    DWORD bytesRead;

    ReadFile(hPipe, &recebido, sizeof(TrabalhoImpressao), &bytesRead, NULL);

    cout << "Recebido do cliente: " << recebido.nome_arquivo << "\n";
    return 0;
}