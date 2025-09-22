#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_STR 200

typedef struct {
    char id_processo[MAX_STR];
    char numero_sigilo[MAX_STR];
    char sigla_grau[MAX_STR];
    char procedimento[MAX_STR];
    char ramo_justica[MAX_STR];
    char sigla_tribunal[MAX_STR];
    char id_tribunal[MAX_STR];
    char recurso[MAX_STR];
    char id_ultimo_oj[MAX_STR];
    char dt_recebimento[MAX_STR];
    char id_ultima_classe[MAX_STR];
    int flag_violencia_domestica;
    int flag_feminicidio;
    int flag_ambiental;
    int flag_quilombolas;
    int flag_indigenas;
    int flag_infancia;
    char decisao[MAX_STR];
    char dt_resolvido[MAX_STR];
    long long cnm1;
    long long primeirasentm1;
    long long baixm1;
    long long decm1;
    long long mpum1;
    long long julgadom1;
    long long desm1;
    long long susm1;
} Processo;

typedef struct {
    char sigla[MAX_STR];
    long long violencia;
    long long feminicidio;
    long long ambiental;
    long long quilombolas;
    long long indigenas;
    long long infancia;
    long long cnm1;
    long long primeirasentm1;
    long long julgadom1;
    long long desm1;
    long long susm1;
} ResumoTribunal;

// Processamento streaming
int processarArquivo(const char *entrada, const char *saida);

// Busca por id_processo -> retorna id_ultimo_oj
int buscarUltimoOJ(const char *entrada, const char *id_proc, char *resultado);

#endif
