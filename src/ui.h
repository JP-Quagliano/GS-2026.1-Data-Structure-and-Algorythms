#ifndef UI_H
#define UI_H

/* ============================================================
 * ui.h
 * ------------------------------------------------------------
 * Modulo responsavel pela apresentacao visual no terminal:
 *   - cores ANSI
 *   - animacoes simples (spinner, loading)
 *   - banner, menu e separadores
 *
 * Compatibilidade Windows:
 *   chame habilitarCoresWindows() UMA UNICA VEZ no inicio do
 *   programa (em main.c) para ativar o suporte a ANSI no
 *   Windows 10+. Sem isso, os codigos de cor aparecem como
 *   "lixo" no terminal antigo.
 * ============================================================ */

/* --- Codigos ANSI de cores e estilos ---
 * Formato: "\033[<codigo>m"
 *   0  = reset      1  = negrito
 *   31 = vermelho   32 = verde     33 = amarelo
 *   34 = azul       35 = magenta   36 = ciano   37 = branco
 */
#define COR_RESET    "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE    "\033[1;32m"
#define COR_AMARELO  "\033[1;33m"
#define COR_AZUL     "\033[1;34m"
#define COR_MAGENTA  "\033[1;35m"
#define COR_CIANO    "\033[1;36m"
#define COR_BRANCO   "\033[1;37m"

/* Habilita Virtual Terminal Processing no Windows (sem efeito em Linux) */
void habilitarCoresWindows(void);

/* Configura o terminal para exibir UTF-8 (acentos) corretamente */
void configurarCodificacao(void);

/* Limpa a tela do terminal */
void limparTela(void);

/* Exibe o banner artistico de abertura do sistema */
void exibirBanner(void);

/* Exibe o menu principal de opcoes */
void exibirMenu(void);

/* Aguarda o usuario pressionar ENTER */
void pausar(void);

/* Animacao tipo "Carregando..." com pontos crescentes durante X ms */
void animacaoCarregamento(const char *mensagem, int duracao_ms);

/* Imprime uma linha separadora estilizada */
void exibirSeparador(void);

/* Exibe a tela de encerramento do sistema */
void exibirEncerramento(void);

#endif
