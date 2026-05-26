#ifndef ANALISE_H
#define ANALISE_H

/* ============================================================
 * analise.h
 * ------------------------------------------------------------
 * Modulo responsavel pela aplicacao das regras de monitoramento
 * exigidas pelo briefing da missao:
 *
 *   Temperatura > 80   -> Alerta de superaquecimento
 *   Energia     < 20   -> Economia de energia
 *   Comunicacao = 0    -> Falha de comunicacao
 *
 * Tambem oferece operacoes estatisticas sobre o historico
 * (media, minimo, maximo, contagem de alertas).
 * ============================================================ */

#include "historico.h"

/* Limites operacionais (centralizados em constantes para facil ajuste) */
#define TEMP_LIMITE_CRITICO     80.0f
#define TEMP_LIMITE_ATENCAO     65.0f
#define ENERGIA_LIMITE_CRITICO  20
#define ENERGIA_LIMITE_ATENCAO  40

/* Analisa UMA leitura individualmente e atualiza o campo "status" dela.
 * Recebe ponteiro para permitir modificar a struct in-place.
 */
void analisarLeitura(LeituraSensor *leitura);

/* Reanalisa todas as leituras do historico (caso os limites mudem) */
void analiseCompleta(void);

/* Imprime estatisticas agregadas do historico inteiro */
void exibirEstatisticas(void);

/* Imprime o status atual (ultima leitura) com formatacao destacada */
void exibirStatusAtual(void);

/* Conta quantas leituras no historico estao em estado de ALERTA */
int contarAlertas(void);

/* Operacoes estatisticas sobre temperatura no historico */
float calcularMediaTemperatura(void);
float obterTemperaturaMaxima(void);
float obterTemperaturaMinima(void);

/* Operacoes estatisticas sobre energia no historico */
float calcularMediaEnergia(void);
int   obterEnergiaMinima(void);

/* Conta quantas leituras tiveram falha de comunicacao */
int contarFalhasComunicacao(void);

#endif
