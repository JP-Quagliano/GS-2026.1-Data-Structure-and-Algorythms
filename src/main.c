/* ============================================================
 * main.c
 * ------------------------------------------------------------
 * Ponto de entrada do Sistema de Monitoramento de Missao
 * Espacial (GS2026.1 - Data Structures and Algorithms).
 *
 * Responsabilidades:
 *   - Inicializar todos os subsistemas (sensores, historico, UI)
 *   - Apresentar o menu principal em loop
 *   - Roteamento das opcoes via switch() conforme exigido pelo
 *     briefing da atividade
 *
 * Modulos do projeto:
 *   historico.[ch]  -> estrutura de dados (buffer circular)
 *   sensores.[ch]   -> coleta manual e simulada
 *   analise.[ch]    -> regras de alerta e estatisticas
 *   ui.[ch]         -> cores, animacoes e formatacao
 * ============================================================ */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "historico.h"
#include "sensores.h"
#include "analise.h"
#include "ui.h"

/* ------------------------------------------------------------
 * Le um inteiro do teclado com validacao basica. Retorna -1
 * se o usuario digitar algo invalido (forca o menu a redesenhar).
 * ------------------------------------------------------------ */
static int lerOpcaoMenu(void) {
    char linha[16];
    int valor;
    if (fgets(linha, sizeof(linha), stdin) == NULL) return -1;
    if (sscanf(linha, "%d", &valor) != 1) return -1;
    return valor;
}

/* ------------------------------------------------------------
 * Solicita ao usuario quantas leituras simular (1 a 50).
 * ------------------------------------------------------------ */
static int solicitarQuantidadeSimulacao(void) {
    char linha[16];
    int q;
    while (1) {
        printf("  Quantas leituras deseja simular (1 a 50)? ");
        if (fgets(linha, sizeof(linha), stdin) == NULL) return 0;
        if (sscanf(linha, "%d", &q) == 1 && q >= 1 && q <= 50) {
            return q;
        }
        printf(COR_VERMELHO "  [!] Valor invalido. Digite um numero entre 1 e 50.\n" COR_RESET);
    }
}

/* ------------------------------------------------------------
 * Pede confirmacao S/N para acoes destrutivas (limpar historico).
 * ------------------------------------------------------------ */
static int confirmar(const char *mensagem) {
    char linha[16];
    printf(COR_AMARELO "  %s (s/n): " COR_RESET, mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) return 0;
    return (linha[0] == 's' || linha[0] == 'S');
}

int main(void) {
    /* --- inicializacao dos subsistemas --- */
    habilitarCoresWindows();    /* ativa ANSI no Windows 10+      */
    configurarCodificacao();    /* terminal em UTF-8              */
    inicializarHistorico();     /* zera o buffer circular         */
    inicializarSensores();      /* semente do rand() com time()   */

    limparTela();
    exibirBanner();
    animacaoCarregamento("  Inicializando sistemas da nave", 1500);

    int opcao;
    int executando = 1;

    while (executando) {
        exibirMenu();
        opcao = lerOpcaoMenu();

        switch (opcao) {
            case 1: {
                LeituraSensor nova = coletarLeituraManual();
                adicionarLeitura(nova);
                printf(COR_VERDE "\n  [OK] Leitura #%d registrada. Status: %s\n" COR_RESET,
                       nova.id, nova.status);
                pausar();
                break;
            }
            case 2: {
                int q = solicitarQuantidadeSimulacao();
                if (q > 0) executarSimulacaoContinua(q);
                pausar();
                break;
            }
            case 3:
                exibirStatusAtual();
                pausar();
                break;

            case 4:
                analiseCompleta();
                pausar();
                break;

            case 5:
                exibirHistorico();
                pausar();
                break;

            case 6:
                exibirEstatisticas();
                pausar();
                break;

            case 7:
                if (confirmar("Tem certeza que deseja apagar todo o historico?")) {
                    limparHistorico();
                } else {
                    printf(COR_CIANO "  Operacao cancelada.\n" COR_RESET);
                }
                pausar();
                break;

            case 0:
                if (confirmar("Encerrar o sistema?")) {
                    executando = 0;
                }
                break;

            default:
                printf(COR_VERMELHO "\n  [!] Opcao invalida. Digite um numero do menu.\n" COR_RESET);
                pausar();
                break;
        }

        if (executando) limparTela();
    }

    exibirEncerramento();
    return 0;
}
