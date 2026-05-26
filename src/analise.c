/* ============================================================
 * analise.c
 * ------------------------------------------------------------
 * Implementacao das regras de monitoramento e estatisticas.
 *
 * Decisao de projeto:
 * - As regras de alerta sao "empilhaveis": uma mesma leitura
 *   pode disparar varios alertas simultaneamente (ex.: temp
 *   alta E energia baixa). O campo "status" concatena todos.
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "analise.h"
#include "ui.h"

void analisarLeitura(LeituraSensor *leitura) {
    if (leitura == NULL) return;

    /* zera o status para evitar lixo de outras analises */
    leitura->status[0] = '\0';

    int alertas = 0;

    if (leitura->temperatura > TEMP_LIMITE_CRITICO) {
        strcat(leitura->status, "ALERTA-TEMP ");
        alertas++;
    } else if (leitura->temperatura > TEMP_LIMITE_ATENCAO) {
        strcat(leitura->status, "ATENCAO-TEMP ");
    }

    if (leitura->energia < ENERGIA_LIMITE_CRITICO) {
        strcat(leitura->status, "ALERTA-ENERG ");
        alertas++;
    } else if (leitura->energia < ENERGIA_LIMITE_ATENCAO) {
        strcat(leitura->status, "ATENCAO-ENERG ");
    }

    if (leitura->comunicacao == 0) {
        strcat(leitura->status, "ALERTA-COMUN ");
        alertas++;
    }

    /* se nenhum alerta nem atencao foi gerado, marca como NORMAL */
    if (leitura->status[0] == '\0') {
        strcpy(leitura->status, "NORMAL");
    }
}

void analiseCompleta(void) {
    if (obterTotalLeituras() == 0) {
        printf(COR_AMARELO "\n  [!] Sem leituras para analisar.\n" COR_RESET);
        return;
    }

    animacaoCarregamento("  Reprocessando todas as leituras", 1000);

    for (int i = 0; i < total_leituras; i++) {
        analisarLeitura(&historico[i]);
    }

    printf(COR_VERDE "\n  [OK] Analise completa concluida sobre %d leituras.\n" COR_RESET,
           total_leituras);
}

int contarAlertas(void) {
    int total = 0;
    for (int i = 0; i < total_leituras; i++) {
        if (strstr(historico[i].status, "ALERTA") != NULL) {
            total++;
        }
    }
    return total;
}

int contarFalhasComunicacao(void) {
    int total = 0;
    for (int i = 0; i < total_leituras; i++) {
        if (historico[i].comunicacao == 0) total++;
    }
    return total;
}

float calcularMediaTemperatura(void) {
    if (total_leituras == 0) return 0.0f;
    float soma = 0.0f;
    for (int i = 0; i < total_leituras; i++) {
        soma += historico[i].temperatura;
    }
    return soma / total_leituras;
}

float calcularMediaEnergia(void) {
    if (total_leituras == 0) return 0.0f;
    float soma = 0.0f;
    for (int i = 0; i < total_leituras; i++) {
        soma += (float)historico[i].energia;
    }
    return soma / total_leituras;
}

float obterTemperaturaMaxima(void) {
    if (total_leituras == 0) return 0.0f;
    float max = historico[0].temperatura;
    for (int i = 1; i < total_leituras; i++) {
        if (historico[i].temperatura > max) max = historico[i].temperatura;
    }
    return max;
}

float obterTemperaturaMinima(void) {
    if (total_leituras == 0) return 0.0f;
    float min = historico[0].temperatura;
    for (int i = 1; i < total_leituras; i++) {
        if (historico[i].temperatura < min) min = historico[i].temperatura;
    }
    return min;
}

int obterEnergiaMinima(void) {
    if (total_leituras == 0) return 0;
    int min = historico[0].energia;
    for (int i = 1; i < total_leituras; i++) {
        if (historico[i].energia < min) min = historico[i].energia;
    }
    return min;
}

void exibirStatusAtual(void) {
    if (total_leituras == 0) {
        printf(COR_AMARELO "\n  [!] Nenhuma leitura disponivel. Insira dados primeiro.\n" COR_RESET);
        return;
    }

    LeituraSensor l = obterUltimaLeitura();
    const char *cor = COR_VERDE;
    if (strstr(l.status, "ALERTA") != NULL) cor = COR_VERMELHO;
    else if (strstr(l.status, "ATENCAO") != NULL) cor = COR_AMARELO;

    printf(COR_CIANO "\n  +----------------------------------------------------+\n");
    printf("  |              STATUS ATUAL DA MISSAO                |\n");
    printf("  +----------------------------------------------------+\n" COR_RESET);
    printf("  Leitura #..........: %s%d" COR_RESET "\n", cor, l.id);
    printf("  Timestamp..........: %s\n", l.timestamp);
    printf("  Temperatura........: %s%.2f C" COR_RESET "\n", cor, l.temperatura);
    printf("  Energia............: %s%d%%" COR_RESET "\n", cor, l.energia);
    printf("  Comunicacao........: %s%s" COR_RESET "\n",
           cor, (l.comunicacao ? "OPERACIONAL" : "FALHA"));
    printf("  Status.............: %s%s" COR_RESET "\n", cor, l.status);
    printf(COR_CIANO "  +----------------------------------------------------+\n" COR_RESET);
}

void exibirEstatisticas(void) {
    if (total_leituras == 0) {
        printf(COR_AMARELO "\n  [!] Sem dados suficientes para estatisticas.\n" COR_RESET);
        return;
    }

    printf(COR_CIANO "\n  +----------------------------------------------------+\n");
    printf("  |             ESTATISTICAS DA MISSAO                 |\n");
    printf("  +----------------------------------------------------+\n" COR_RESET);
    printf("  Total de leituras..........: %d\n", total_leituras);
    printf("  Temperatura media..........: %.2f C\n", calcularMediaTemperatura());
    printf("  Temperatura maxima.........: %s%.2f C" COR_RESET "\n",
           COR_VERMELHO, obterTemperaturaMaxima());
    printf("  Temperatura minima.........: %s%.2f C" COR_RESET "\n",
           COR_AZUL, obterTemperaturaMinima());
    printf("  Energia media..............: %.2f%%\n", calcularMediaEnergia());
    printf("  Energia minima registrada..: %s%d%%" COR_RESET "\n",
           COR_AMARELO, obterEnergiaMinima());
    printf("  Falhas de comunicacao......: %s%d" COR_RESET "\n",
           COR_VERMELHO, contarFalhasComunicacao());
    printf("  Leituras em ALERTA.........: %s%d" COR_RESET "\n",
           COR_VERMELHO, contarAlertas());

    /* indicador de saude geral da missao */
    float taxa_alerta = (float)contarAlertas() / (float)total_leituras;
    printf("  Indice de criticidade......: ");
    if (taxa_alerta < 0.15f) {
        printf(COR_VERDE "BAIXO (missao estavel)" COR_RESET "\n");
    } else if (taxa_alerta < 0.40f) {
        printf(COR_AMARELO "MEDIO (atencao requerida)" COR_RESET "\n");
    } else {
        printf(COR_VERMELHO "ALTO (missao em risco)" COR_RESET "\n");
    }
    printf(COR_CIANO "  +----------------------------------------------------+\n" COR_RESET);
}
