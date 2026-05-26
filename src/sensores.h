#ifndef SENSORES_H
#define SENSORES_H

/* ============================================================
 * sensores.h
 * ------------------------------------------------------------
 * Modulo responsavel pela aquisicao de dados dos sensores,
 * seja por entrada manual do operador ou por simulacao
 * automatica (usando rand() com semente baseada em time()).
 * ============================================================ */

#include "historico.h"

/* Coleta uma leitura inserida manualmente pelo usuario (validada) */
LeituraSensor coletarLeituraManual(void);

/* Gera uma leitura simulada aleatoriamente (sensores) */
LeituraSensor coletarLeituraSimulada(void);

/* Executa N leituras simuladas em sequencia com pequena pausa */
void executarSimulacaoContinua(int quantidade);

/* Preenche o buffer com o timestamp atual no formato AAAA-MM-DD HH:MM:SS */
void gerarTimestamp(char *buffer, int tamanho);

/* Inicializa o gerador de numeros aleatorios. Chamar uma vez no main. */
void inicializarSensores(void);

#endif
