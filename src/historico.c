/* ============================================================
 * historico.c
 * ------------------------------------------------------------
 * Implementacao do buffer circular de leituras dos sensores.
 *
 * Como funciona o buffer circular:
 * - "proxima_posicao" indica onde a proxima leitura sera escrita.
 * - Quando proxima_posicao atinge MAX_LEITURAS, volta para 0.
 * - "total_leituras" so cresce ate MAX_LEITURAS (depois congela).
 * - A "primeira" leitura logica do historico esta em:
 *      indice_fisico = (proxima_posicao - total_leituras + MAX) % MAX
 *   se o buffer ja deu a volta.
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "historico.h"
#include "ui.h"

/* Definicao das variaveis globais declaradas como extern em historico.h */
LeituraSensor historico[MAX_LEITURAS];
int total_leituras    = 0;
int proxima_posicao   = 0;
int contador_id       = 0;

void inicializarHistorico(void) {
    total_leituras  = 0;
    proxima_posicao = 0;
    contador_id     = 0;
    memset(historico, 0, sizeof(historico));
}

void adicionarLeitura(LeituraSensor nova) {
    historico[proxima_posicao] = nova;
    proxima_posicao = (proxima_posicao + 1) % MAX_LEITURAS;
    if (total_leituras < MAX_LEITURAS) {
        total_leituras++;
    }
}

int obterTotalLeituras(void) {
    return total_leituras;
}

LeituraSensor obterUltimaLeitura(void) {
    LeituraSensor vazia = {0};
    if (total_leituras == 0) {
        return vazia;
    }
    /* a ultima leitura escrita esta em (proxima_posicao - 1) modulo MAX */
    int idx = (proxima_posicao - 1 + MAX_LEITURAS) % MAX_LEITURAS;
    return historico[idx];
}

LeituraSensor obterLeituraPorIndice(int indice) {
    LeituraSensor vazia = {0};
    if (indice < 0 || indice >= total_leituras) {
        return vazia;
    }
    /* converte o indice logico (0 = mais antigo) para indice fisico no array */
    int inicio = (total_leituras < MAX_LEITURAS)
                 ? 0
                 : proxima_posicao;
    int idx = (inicio + indice) % MAX_LEITURAS;
    return historico[idx];
}

void exibirHistorico(void) {
    if (total_leituras == 0) {
        printf(COR_AMARELO "\n  [!] Nenhuma leitura registrada ainda.\n" COR_RESET);
        return;
    }

    printf(COR_CIANO "\n  +-----+----------+--------+--------+---------------------+--------------------------+\n");
    printf("  | ID  |  TEMP    | ENERG. | COMUN. |     TIMESTAMP       |          STATUS          |\n");
    printf("  +-----+----------+--------+--------+---------------------+--------------------------+" COR_RESET "\n");

    for (int i = 0; i < total_leituras; i++) {
        LeituraSensor l = obterLeituraPorIndice(i);

        /* cor da linha conforme o status: vermelho se contem ALERTA */
        const char *cor = COR_VERDE;
        if (strstr(l.status, "ALERTA") != NULL) {
            cor = COR_VERMELHO;
        } else if (strstr(l.status, "ATENCAO") != NULL) {
            cor = COR_AMARELO;
        }

        printf("  | %s%-3d" COR_RESET " | %s%6.2fC" COR_RESET
               " | %s%5d%%" COR_RESET " | %s%6s" COR_RESET
               " | %-19s | %s%-24s" COR_RESET " |\n",
               cor, l.id,
               cor, l.temperatura,
               cor, l.energia,
               cor, (l.comunicacao == 1 ? "OK" : "FALHA"),
               l.timestamp,
               cor, l.status);
    }

    printf(COR_CIANO "  +-----+----------+--------+--------+---------------------+--------------------------+\n" COR_RESET);
    printf("  Total de leituras: %d / %d\n", total_leituras, MAX_LEITURAS);
}

void limparHistorico(void) {
    inicializarHistorico();
    printf(COR_VERDE "\n  [OK] Historico limpo com sucesso.\n" COR_RESET);
}
