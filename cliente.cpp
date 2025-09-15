#include <iostream>
#include <windows.h>
using namespace std;

struct TrabalhoImpressao {
    int id_job;
    char nome_arquivo[50];
    int numero_paginas;
};

int main(){
    TrabalhoImpressao impressaoUm;
    impressaoUm.id_job = 1;
    strcpy(impressaoUm.nome_arquivo, "Primeira_impressao.pdf");
    impressaoUm.numero_paginas = 5;

    const char* pipeName = R"(\\.\pipe\impressao)";

    HANDLE hPipe = CreateFileA(
        pipeName,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    DWORD bytesWritten;
    bool ok = WriteFile(hPipe, &impressaoUm, sizeof(TrabalhoImpressao), &bytesWritten, NULL);
    if(!ok){
        cout << "Não foi possivel escrever" << endl;
    }

    CloseHandle(hPipe);
}