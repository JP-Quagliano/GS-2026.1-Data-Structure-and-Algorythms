/* ============================================================
 * sensores.c
 * ------------------------------------------------------------
 * Implementacao da coleta de dados dos sensores.
 *
 * Decisao de projeto:
 * - A validacao de entrada usa fgets() + sscanf() em vez de
 *   scanf() puro. Motivo: scanf() puro deixa lixo no buffer
 *   quando o usuario digita algo invalido (ex.: letras em um
 *   campo numerico) e isso causa loops infinitos no menu.
 *   fgets() consome a linha inteira, eliminando o problema.
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>  /* para Sleep() */
#else
#include <unistd.h>   /* para usleep() em sistemas POSIX */
#endif

#include "sensores.h"
#include "analise.h"
#include "ui.h"

/* ------------------------------------------------------------
 * Funcao auxiliar interna: pausa portavel em milissegundos.
 * ------------------------------------------------------------ */
static void dormir_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

/* ------------------------------------------------------------
 * Le uma linha inteira do teclado (segura, sem overflow).
 * ------------------------------------------------------------ */
static void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    /* remove o '\n' do final, se houver */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void inicializarSensores(void) {
    srand((unsigned int)time(NULL));
}

void gerarTimestamp(char *buffer, int tamanho) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", t);
}

LeituraSensor coletarLeituraManual(void) {
    LeituraSensor l;
    char linha[64];
    float temp;
    int energia, comunic;

    printf(COR_CIANO "\n  === COLETA MANUAL DE DADOS ===\n" COR_RESET);

    /* --- Temperatura --- */
    while (1) {
        printf("  Temperatura da nave (graus C, ex.: 25.5): ");
        lerLinha(linha, sizeof(linha));
        if (sscanf(linha, "%f", &temp) == 1 && temp >= -200.0f && temp <= 500.0f) {
            break;
        }
        printf(COR_VERMELHO "  [!] Valor invalido. Use um numero entre -200 e 500.\n" COR_RESET);
    }

    /* --- Energia --- */
    while (1) {
        printf("  Nivel de energia (%% de 0 a 100): ");
        lerLinha(linha, sizeof(linha));
        if (sscanf(linha, "%d", &energia) == 1 && energia >= 0 && energia <= 100) {
            break;
        }
        printf(COR_VERMELHO "  [!] Valor invalido. Use um numero inteiro de 0 a 100.\n" COR_RESET);
    }

    /* --- Comunicacao --- */
    while (1) {
        printf("  Comunicacao (1 = OK, 0 = FALHA): ");
        lerLinha(linha, sizeof(linha));
        if (sscanf(linha, "%d", &comunic) == 1 && (comunic == 0 || comunic == 1)) {
            break;
        }
        printf(COR_VERMELHO "  [!] Valor invalido. Digite 0 ou 1.\n" COR_RESET);
    }

    l.id          = ++contador_id;
    l.temperatura = temp;
    l.energia     = energia;
    l.comunicacao = comunic;
    gerarTimestamp(l.timestamp, sizeof(l.timestamp));
    strcpy(l.status, "PENDENTE");

    /* avalia status imediatamente */
    analisarLeitura(&l);

    return l;
}

LeituraSensor coletarLeituraSimulada(void) {
    LeituraSensor l;

    /* faixas de simulacao realistas para uma nave em operacao:
     * - temperatura entre 10.0 e 110.0 graus C (ocasionalmente alerta)
     * - energia entre 5 e 100 por cento
     * - comunicacao com 85% de chance de estar OK
     */
    float temp = 10.0f + ((float)rand() / RAND_MAX) * 100.0f;
    int energia = 5 + rand() % 96;
    int comunic = (rand() % 100) < 85 ? 1 : 0;

    l.id          = ++contador_id;
    l.temperatura = temp;
    l.energia     = energia;
    l.comunicacao = comunic;
    gerarTimestamp(l.timestamp, sizeof(l.timestamp));
    strcpy(l.status, "PENDENTE");

    analisarLeitura(&l);

    return l;
}

void executarSimulacaoContinua(int quantidade) {
    if (quantidade <= 0) {
        printf(COR_AMARELO "\n  [!] Quantidade invalida.\n" COR_RESET);
        return;
    }

    printf(COR_CIANO "\n  Iniciando simulacao de %d leituras...\n" COR_RESET, quantidade);
    animacaoCarregamento("  Calibrando sensores", 1200);

    for (int i = 0; i < quantidade; i++) {
        LeituraSensor l = coletarLeituraSimulada();
        adicionarLeitura(l);

        /* feedback visual em tempo real */
        const char *cor = COR_VERDE;
        if (strstr(l.status, "ALERTA") != NULL) cor = COR_VERMELHO;
        else if (strstr(l.status, "ATENCAO") != NULL) cor = COR_AMARELO;

        printf("  %s[Leitura #%03d]" COR_RESET
               "  T=%.2fC  E=%d%%  Com=%s  -> %s%s" COR_RESET "\n",
               cor, l.id, l.temperatura, l.energia,
               (l.comunicacao ? "OK" : "FALHA"),
               cor, l.status);

        dormir_ms(150);
    }

    printf(COR_VERDE "\n  [OK] Simulacao concluida: %d leituras registradas.\n" COR_RESET, quantidade);
}
