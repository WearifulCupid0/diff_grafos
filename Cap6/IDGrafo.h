#ifndef IDGRAFO_H
#define IDGRAFO_H

#include <stdlib.h>

int compararVertices(void* a, void* b) {
    return *(int*)a - *(int*)b;
}

static void dfsConexo(pVertice v, int* visitado, int n, FuncaoComparacao comparar) {
    visitado[*(int*)v->info] = 1;
    Noh* noh = v->listaAdjacencias->primeiro;
    while (noh) {
        pVertice adj = noh->info;
        int id = *(int*)adj->info;
        if (!visitado[id])
            dfsConexo(adj, visitado, n, comparar);
        noh = noh->prox;
    }
}

int grafoConexo(pDGrafo g, FuncaoComparacao comparar) {
    int n = g->listaVertices->quantidade;
    if (n == 0) return 1;

    int* visitado = calloc(n, sizeof(int));
    pVertice inicial = g->listaVertices->primeiro->info;
    dfsConexo(inicial, visitado, n, comparar);

    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            free(visitado);
            return 0;
        }
    }

    free(visitado);
    return 1;
}

int grafoBipartido(pDGrafo g, FuncaoComparacao comparar) {
    int n = g->listaVertices->quantidade;
    int* cor = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) cor[i] = -1;

    pDLista fila = criarLista();
    Noh* noh = g->listaVertices->primeiro;
    while (noh) {
        pVertice v = noh->info;
        int vid = *(int*)v->info;
        if (cor[vid] == -1) {
            cor[vid] = 0;
            incluirInfo(fila, v);

            while (fila->quantidade > 0) {
                pVertice u = fila->primeiro->info;
                excluirInfo(fila, u, comparar);
                int uid = *(int*)u->info;
                Noh* adjNoh = u->listaAdjacencias->primeiro;
                while (adjNoh) {
                    pVertice w = adjNoh->info;
                    int wid = *(int*)w->info;
                    if (cor[wid] == -1) {
                        cor[wid] = 1 - cor[uid];
                        incluirInfo(fila, w);
                    } else if (cor[wid] == cor[uid]) {
                        destruirLista(fila);
                        free(cor);
                        return 0;
                    }
                    adjNoh = adjNoh->prox;
                }
            }
        }
        noh = noh->prox;
    }

    destruirLista(fila);
    free(cor);
    return 1;
}

int existeCaminhoEuleriano(pDGrafo g, FuncaoComparacao comparar) {
    if (!grafoConexo(g, comparar)) return 0;

    int impares = 0;
    Noh* noh = g->listaVertices->primeiro;
    while (noh) {
        pVertice v = noh->info;
        if (v->listaAdjacencias->quantidade % 2 != 0) impares++;
        noh = noh->prox;
    }
    return impares == 0 || impares == 2;
}

static int hamiltonianoRec(pVertice atual, int* visitado, int n, int count, pVertice inicio) {
    visitado[*(int*)atual->info] = 1;
    if (count == n) return 1;

    Noh* noh = atual->listaAdjacencias->primeiro;
    while (noh != NULL) {
        pVertice viz = noh->info;
        int id = *(int*)viz->info;
        if (!visitado[id]) {
            if (hamiltonianoRec(viz, visitado, n, count + 1, inicio))
                return 1;
        }
        noh = noh->prox;
    }

    visitado[*(int*)atual->info] = 0;
    return 0;
}

int existeCaminhoHamiltoniano(pDGrafo g, FuncaoComparacao comparar) {
    int n = g->listaVertices->quantidade;
    if (n == 0) return 1;

    int* visitado = calloc(n, sizeof(int));
    pVertice inicio = g->listaVertices->primeiro->info;
    int result = hamiltonianoRec(inicio, visitado, n, 1, inicio);

    free(visitado);
    return result;
}

#endif