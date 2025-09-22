#include <stdio.h>
#include "processo.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_csv>\n", argv[0]);
        return 1;
    }

    // Processa tudo (itens 1-12, exceto 2)
    processarArquivo(argv[1], "processos_julgados.csv");

    // Item 2: buscar id_ultimo_oj por id_processo
    char idBusca[MAX_STR], resultado[MAX_STR];
    printf("\nDigite um id_processo para buscar o id_ultimo_oj: ");
    scanf("%s", idBusca);

    if (buscarUltimoOJ(argv[1], idBusca, resultado)) {
        printf("2) id_ultimo_oj do processo %s: %s\n", idBusca, resultado);
    } else {
        printf("Processo %s não encontrado.\n", idBusca);
    }

    return 0;
}