#ifndef HISTORICO_H
#define HISTORICO_H

/* ============================================================
 * historico.h
 * ------------------------------------------------------------
 * Modulo responsavel pelo armazenamento das leituras dos
 * sensores em uma estrutura de dados do tipo "buffer circular".
 *
 * Justificativa da escolha (Data Structures and Algorithms):
 * - Vetor estatico de structs garante acesso O(1) por indice.
 * - Quando o vetor enche, a leitura mais antiga e sobrescrita
 *   automaticamente. Isso simula um sistema real de telemetria,
 *   que nunca para de receber dados mas tem memoria limitada.
 * ============================================================ */

#define MAX_LEITURAS 100   /* capacidade total do buffer circular */

/* Estrutura que representa uma unica leitura dos sensores da nave */
typedef struct {
    int   id;                 /* identificador sequencial da leitura      */
    float temperatura;        /* temperatura da nave em graus Celsius     */
    int   energia;            /* nivel de energia em percentual (0 a 100) */
    int   comunicacao;        /* 0 = falha, 1 = comunicacao operacional   */
    char  timestamp[20];      /* formato AAAA-MM-DD HH:MM:SS              */
    char  status[60];         /* descricao textual do status da leitura   */
} LeituraSensor;

/* Variaveis globais do modulo (definidas em historico.c) */
extern LeituraSensor historico[MAX_LEITURAS];
extern int total_leituras;       /* quantas leituras existem (0 a MAX)   */
extern int proxima_posicao;      /* proxima posicao livre no buffer      */
extern int contador_id;          /* contador global para gerar IDs unicos */

/* Inicializa o modulo (zera o buffer e contadores) */
void inicializarHistorico(void);

/* Insere uma nova leitura no buffer circular */
void adicionarLeitura(LeituraSensor nova);

/* Imprime todas as leituras armazenadas em formato de tabela */
void exibirHistorico(void);

/* Apaga todas as leituras e reinicia o buffer */
void limparHistorico(void);

/* Retorna a quantidade atual de leituras no buffer */
int obterTotalLeituras(void);

/* Retorna a leitura mais recente. Se nao houver, retorna struct zerada */
LeituraSensor obterUltimaLeitura(void);

/* Retorna a leitura na posicao logica indicada (0 = mais antiga) */
LeituraSensor obterLeituraPorIndice(int indice);

#endif
