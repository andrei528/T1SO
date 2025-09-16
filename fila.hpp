#include <iostream>
using namespace std;

template <typename T>
struct No {
    T impressao;
    No<T>*prox;
};

template <typename T>
struct Fila {
    No <T> *frente;
    No <T> *tras;
};

template <typename T>
void inicializarFila(Fila<T>* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

template <typename T>
bool vazia(Fila<T>* fila){
    return fila->frente == NULL;
}

template <typename T>
void push(Fila<T>* fila, const T& valor) {
    No<T>* novoNo = new No<T>;
    novoNo->impressao = valor;
    novoNo->prox = NULL;

    if(vazia(fila)){
        fila->frente = fila->tras = novoNo;
    }else {
        fila->tras->prox = novoNo;
        fila->tras = novoNo;
    }
}

template <typename T>
bool pop (Fila<T>* fila, T& valor){
    if (vazia(fila)) return false;

    No<T>* temp = fila->frente;
    valor = temp->impressao;
    fila->frente = temp->prox;

    if(fila->frente == NULL) {
        fila->tras = NULL;
    }
    delete temp;
    return true;
}
