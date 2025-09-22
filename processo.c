#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

// Função auxiliar para diferença de dias
static int diffDias(const char *data1, const char *data2) {
    if (strlen(data1) < 10 || strlen(data2) < 10) return -1;
    struct tm t1 = {0}, t2 = {0};
    sscanf(data1, "%d-%d-%d", &t1.tm_year, &t1.tm_mon, &t1.tm_mday);
    sscanf(data2, "%d-%d-%d", &t2.tm_year, &t2.tm_mon, &t2.tm_mday);
    t1.tm_year -= 1900; t1.tm_mon -= 1;
    t2.tm_year -= 1900; t2.tm_mon -= 1;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    if (time1 == -1 || time2 == -1) return -1;
    double diff = difftime(time2, time1) / (60*60*24);
    return (int)diff;
}

// Processa o CSV inteiro
int processarArquivo(const char *entrada, const char *saida) {
    FILE *fp = fopen(entrada, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo %s\n", entrada);
        return 0;
    }

    FILE *out = fopen(saida, "w");
    if (!out) {
        printf("Erro ao criar arquivo %s\n", saida);
        fclose(fp);
        return 0;
    }
    fprintf(out, "id_processo;sigla_tribunal;julgadom1\n");

    char linha[10000];
    int totalProcessos = 0;
    int violencia = 0, feminicidio = 0, ambiental = 0, quilombolas = 0, indigenas = 0, infancia = 0;

    char maisAntigoId[MAX_STR] = "";
    char maisAntigoData[MAX_STR] = "9999-12-31";

    long long cnm1 = 0, primeirasentm1 = 0, julgadom1 = 0, desm1 = 0, susm1 = 0;

    // estatísticas item 10
    long long somaDias = 0;
    int countDias = 0;
    int minDias = 9999999, maxDias = -1;

    fgets(linha, sizeof(linha), fp); // pula cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        totalProcessos++;
        Processo p = {0};
        char *ptr = linha;
        char *start = ptr;
        int col = 0;

        while (*ptr != '\0') {
            if (*ptr == ';' || *ptr == '\n' || *ptr == '\r') {
                *ptr = '\0';
                switch (col) {
                    case 0: strcpy(p.id_processo, start); break;
                    case 5: strcpy(p.sigla_tribunal, start); break;
                    case 8: strcpy(p.id_ultimo_oj, start); break;
                    case 9: strcpy(p.dt_recebimento, start); break;
                    case 11: p.flag_violencia_domestica = atoi(start); break;
                    case 12: p.flag_feminicidio = atoi(start); break;
                    case 13: p.flag_ambiental = atoi(start); break;
                    case 14: p.flag_quilombolas = atoi(start); break;
                    case 15: p.flag_indigenas = atoi(start); break;
                    case 16: p.flag_infancia = atoi(start); break;
                    case 18: strcpy(p.dt_resolvido, start); break;
                    case 19: p.cnm1 = atoll(start); break;
                    case 20: p.primeirasentm1 = atoll(start); break;
                    case 24: p.julgadom1 = atoll(start); break;
                    case 25: p.desm1 = atoll(start); break;
                    case 26: p.susm1 = atoll(start); break;
                }
                start = ptr + 1;
                col++;
            }
            ptr++;
        }

        
        if (strcmp(p.dt_recebimento, maisAntigoData) < 0) {
            strcpy(maisAntigoData, p.dt_recebimento);
            strcpy(maisAntigoId, p.id_processo);
        }

        
        if (p.flag_violencia_domestica) violencia++;
        if (p.flag_feminicidio) feminicidio++;
        if (p.flag_ambiental) ambiental++;
        if (p.flag_quilombolas) quilombolas++;
        if (p.flag_indigenas) indigenas++;
        if (p.flag_infancia) infancia++;

        
        int dias = diffDias(p.dt_recebimento, p.dt_resolvido);
        if (dias >= 0) {
            somaDias += dias;
            countDias++;
            if (dias < minDias) minDias = dias;
            if (dias > maxDias) maxDias = dias;
        }

        
        cnm1 += p.cnm1;
        primeirasentm1 += p.primeirasentm1;
        julgadom1 += p.julgadom1;
        desm1 += p.desm1;
        susm1 += p.susm1;

        
        if (p.julgadom1 > 0) {
            fprintf(out, "%s;%s;%lld\n", p.id_processo, p.sigla_tribunal, p.julgadom1);
        }
    }

    fclose(fp);
    fclose(out);

    
    printf("\n=== Resultados Globais ===\n");
    printf("1) Numero de processos: %d\n", totalProcessos);
    printf("3) Processo mais antigo: %s (%s)\n", maisAntigoId, maisAntigoData);
    printf("4) Violencia domestica: %d\n", violencia);
    printf("5) Feminicidio: %d\n", feminicidio);
    printf("6) Ambiental: %d\n", ambiental);
    printf("7) Quilombolas: %d\n", quilombolas);
    printf("8) Indigenas: %d\n", indigenas);
    printf("9) Infancia: %d\n", infancia);

    if (countDias > 0) {
        double mediaDias = (double)somaDias / countDias;
        printf("10) Dias entre recebimento e resolucao:\n");
        printf("    Processos validos: %d\n", countDias);
        printf("    Minimo: %d dias\n", minDias);
        printf("    Maximo: %d dias\n", maxDias);
        printf("    Media: %.2f dias\n", mediaDias);
    }

    long long denomGlobal = cnm1 + desm1 - susm1;
    double meta = 0.0;
    if (denomGlobal > 0) {
        meta = (double)julgadom1 / denomGlobal * 100.0;
    }
    printf("11) Cumprimento da Meta 1 (global): %.4f%%\n", meta);

    printf("12) Arquivo '%s' gerado com processos julgados.\n", saida);

    return totalProcessos;
}


int buscarUltimoOJ(const char *entrada, const char *id_proc, char *resultado) {
    FILE *fp = fopen(entrada, "r");
    if (!fp) return 0;

    char linha[10000];
    
    if (!fgets(linha, sizeof(linha), fp)) {
        fclose(fp);
        return 0;
    }

    while (fgets(linha, sizeof(linha), fp)) {
        Processo p;
        p.id_processo[0] = '\0';
        p.id_ultimo_oj[0] = '\0';

        char *token = strtok(linha, ";");
        int col = 0;
        while (token) {
            switch (col) {
                case 0:
                    strncpy(p.id_processo, token, MAX_STR-1);
                    p.id_processo[MAX_STR-1] = '\0';
                    break;
                case 8:
                    strncpy(p.id_ultimo_oj, token, MAX_STR-1);
                    p.id_ultimo_oj[MAX_STR-1] = '\0';
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ";");
            col++;
        }

        if (p.id_processo[0] != '\0' && strcmp(p.id_processo, id_proc) == 0) {
            strncpy(resultado, p.id_ultimo_oj, MAX_STR-1);
            resultado[MAX_STR-1] = '\0';
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}