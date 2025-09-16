#include <iostream>
#include <random>
#include <windows.h>
using namespace std;

struct TrabalhoImpressao {
    int id_job;
    char nome_arquivo[50];
    int numero_paginas;
};

int main(int argc, char* argv[]){
    TrabalhoImpressao impressaoUm;

    const char* pipeName = R"(\\.\pipe\impressao)";
    if(argc > 1 && strcmp(argv[1], "filho") == 0){
        strcpy(impressaoUm.nome_arquivo, argv[2]);
        impressaoUm.id_job = atoi(argv[3]);
        impressaoUm.numero_paginas = atoi(argv[4]);

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

    }else {
        for(int i = 0; i < 5; i++){

            STARTUPINFOA si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            ZeroMemory(&pi, sizeof(pi));

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(1,20);

            char nomeArquivo[50];
            sprintf(nomeArquivo, "arquivo_%d.txt", i+1);
            int idJob = i + 1;
            int numPaginas = (i + 1) * dist(gen);

            // Monta a string para passar como argumento
            char cmdLine[200];
            sprintf(cmdLine, "cliente.exe filho %s %d %d", nomeArquivo, idJob, numPaginas);

            BOOL criou = CreateProcessA(
                NULL,
                cmdLine,
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi
            );

            if(!criou) cout << "falha ao criar processo filho" << endl;
            else{
                cout << "processo filho criado, PID: " << pi.dwProcessId << endl;
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }
    }   
}