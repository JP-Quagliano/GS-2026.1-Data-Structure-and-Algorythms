/* ============================================================
 * ui.c
 * ------------------------------------------------------------
 * Implementacao das funcoes de interface do terminal.
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "ui.h"

/* ------------------------------------------------------------
 * Pausa portavel em milissegundos (interna ao modulo).
 * ------------------------------------------------------------ */
static void dormir_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void habilitarCoresWindows(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void configurarCodificacao(void) {
#ifdef _WIN32
    /* configura o terminal do Windows para UTF-8 */
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void exibirBanner(void) {
    printf(COR_CIANO);
    printf("\n");
    printf("   ============================================================\n");
    printf("    ____   ____   _    ____  _____   ____ ___ _____ _____ \n");
    printf("   / ___| |  _ \\ / \\  / ___|| ____| / ___|_ _|_   _| ____|\n");
    printf("   \\___ \\ | |_) / _ \\ | |    |  _|   \\___ \\| |  | | |  _|  \n");
    printf("    ___) ||  __/ ___ \\| |___ | |___   ___) | |  | | | |___ \n");
    printf("   |____(_)_| /_/   \\_\\____||_____| |____/___| |_| |_____|\n");
    printf("\n");
    printf("           SISTEMA DE MONITORAMENTO DE MISSAO ESPACIAL          \n");
    printf("                     v1.0  |  FIAP 2026.1                      \n");
    printf("   ============================================================\n");
    printf(COR_RESET "\n");
}

void exibirMenu(void) {
    printf(COR_CIANO "\n  +------------------- MENU PRINCIPAL -------------------+\n" COR_RESET);
    printf("  | " COR_VERDE "1" COR_RESET " - Inserir leitura manual                       |\n");
    printf("  | " COR_VERDE "2" COR_RESET " - Iniciar simulacao automatica (rand)          |\n");
    printf("  | " COR_VERDE "3" COR_RESET " - Visualizar status atual                      |\n");
    printf("  | " COR_VERDE "4" COR_RESET " - Executar analise completa                    |\n");
    printf("  | " COR_VERDE "5" COR_RESET " - Exibir historico de leituras                 |\n");
    printf("  | " COR_VERDE "6" COR_RESET " - Estatisticas da missao                       |\n");
    printf("  | " COR_VERDE "7" COR_RESET " - Limpar historico                             |\n");
    printf("  | " COR_VERMELHO "0" COR_RESET " - Encerrar sistema                             |\n");
    printf(COR_CIANO "  +------------------------------------------------------+\n" COR_RESET);
    printf("  Selecione uma opcao: ");
}

void pausar(void) {
    printf(COR_AMARELO "\n  Pressione ENTER para continuar..." COR_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* esvazia buffer */ }
}

void animacaoCarregamento(const char *mensagem, int duracao_ms) {
    const char frames[] = {'|', '/', '-', '\\'};
    int n_frames = 4;
    int delay = 80;             /* ms entre frames */
    int total = duracao_ms / delay;

    for (int i = 0; i < total; i++) {
        printf("\r%s %c", mensagem, frames[i % n_frames]);
        fflush(stdout);
        dormir_ms(delay);
    }
    /* limpa a linha apos a animacao */
    printf("\r");
    int len = (int)strlen(mensagem) + 4;
    for (int i = 0; i < len; i++) printf(" ");
    printf("\r");
    fflush(stdout);
}

void exibirSeparador(void) {
    printf(COR_CIANO "  ------------------------------------------------------\n" COR_RESET);
}

void exibirEncerramento(void) {
    printf(COR_CIANO "\n");
    printf("   ============================================================\n");
    printf("            SISTEMA ENCERRADO. MISSAO CONCLUIDA.\n");
    printf("                  Boa viagem, comandante!\n");
    printf("   ============================================================\n");
    printf(COR_RESET "\n");
}
