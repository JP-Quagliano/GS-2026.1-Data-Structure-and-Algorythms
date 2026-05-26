# 🧠 Explicação da Lógica Utilizada

> Documento explicativo dos módulos, decisões de projeto e algoritmos do
> Sistema de Monitoramento de Missão Espacial.

---

## 1. Visão geral da arquitetura

O sistema foi projetado de forma **modular**, com cada arquivo `.c` tratando de uma única responsabilidade. Essa separação respeita o princípio da **coesão alta + acoplamento baixo**, facilitando manutenção, testes e leitura do código pelo professor avaliador.

```
   +----------+      +------------+      +----------+
   |  main.c  |----->|  sensores  |----->|          |
   |  (menu)  |      +------------+      |          |
   |          |                          | histórico|
   |          |----->|   análise  |<-----| (buffer) |
   |          |      +------------+      |          |
   |          |----->|     ui     |      +----------+
   +----------+      +------------+
```

O `main.c` é o orquestrador: recebe a opção do menu (via `switch()`) e delega a execução para o módulo apropriado.

---

## 2. Estrutura de dados — Buffer Circular

### O que é
Um **buffer circular** (também chamado de "ring buffer") é um vetor de tamanho fixo onde, ao atingir o final, a próxima escrita retorna ao início, sobrescrevendo o dado mais antigo.

### Por que escolhemos
1. **Memória previsível** — nunca cresce indefinidamente.
2. **Realismo** — sistemas reais de telemetria espacial mantêm apenas as últimas N leituras.
3. **Demonstração genuína da matéria** — Data Structures and Algorithms exige usar uma estrutura de dados não-trivial. Vetor solto não conta.

### Como funciona no código

Em `historico.h` declaramos a struct:

```c
typedef struct {
    int   id;
    float temperatura;
    int   energia;
    int   comunicacao;
    char  timestamp[20];
    char  status[60];
} LeituraSensor;

LeituraSensor historico[MAX_LEITURAS];  // MAX_LEITURAS = 100
int total_leituras;     // quantas leituras válidas existem (0 a MAX)
int proxima_posicao;    // índice físico onde a próxima leitura será escrita
```

### Algoritmo de inserção (`adicionarLeitura`)
```c
historico[proxima_posicao] = nova;
proxima_posicao = (proxima_posicao + 1) % MAX_LEITURAS;  // <-- a "circularidade"
if (total_leituras < MAX_LEITURAS) total_leituras++;
```

O operador `%` (módulo) é o que torna o buffer **circular**: quando `proxima_posicao` chega a 100, `100 % 100 == 0` e voltamos ao início.

### Algoritmo de leitura (`obterLeituraPorIndice`)
Quando o buffer já deu a volta, a leitura mais antiga não está mais no índice físico 0 — está em `proxima_posicao` (a próxima a ser sobrescrita é a mais antiga). Por isso:

```c
int inicio = (total_leituras < MAX_LEITURAS) ? 0 : proxima_posicao;
int idx    = (inicio + indice_logico) % MAX_LEITURAS;
```

Isso garante que `obterLeituraPorIndice(0)` sempre retorna a leitura mais antiga e `obterLeituraPorIndice(total_leituras - 1)` retorna a mais recente, independentemente de o buffer ter dado a volta ou não.

---

## 3. Regras de análise (módulo `analise.c`)

As três regras obrigatórias do briefing foram implementadas como **alertas empilháveis** — uma única leitura pode disparar múltiplos alertas simultaneamente:

| Condição | Status atribuído |
|----------|------------------|
| `temperatura > 80` | `ALERTA-TEMP` |
| `temperatura > 65` (e ≤ 80) | `ATENCAO-TEMP` |
| `energia < 20` | `ALERTA-ENERG` |
| `energia < 40` (e ≥ 20) | `ATENCAO-ENERG` |
| `comunicacao == 0` | `ALERTA-COMUN` |
| Nenhum dos acima | `NORMAL` |

**Por que dois níveis (ATENCAO vs ALERTA)?**
Sistemas reais não esperam o problema acontecer para reagir — eles têm zonas de **aviso** antes da zona **crítica**. Implementamos isso para deixar o sistema mais realista e demonstrar pensamento de engenharia.

Os limites estão centralizados em constantes no `analise.h`:
```c
#define TEMP_LIMITE_CRITICO     80.0f
#define TEMP_LIMITE_ATENCAO     65.0f
#define ENERGIA_LIMITE_CRITICO  20
#define ENERGIA_LIMITE_ATENCAO  40
```
Para ajustar os limiares basta editar uma única constante — sem caçar `if`s pelo código.

---

## 4. Simulação dos sensores (módulo `sensores.c`)

A simulação usa `rand()` semeado uma única vez na inicialização (`srand(time(NULL))`), garantindo sequências diferentes a cada execução.

```c
float temp     = 10.0f + ((float)rand() / RAND_MAX) * 100.0f;  // 10 a 110 °C
int   energia  = 5 + rand() % 96;                              // 5 a 100 %
int   comunic  = (rand() % 100) < 85 ? 1 : 0;                  // 85% chance OK
```

As faixas foram escolhidas propositalmente para **garantir que alertas ocorram** durante a demonstração — uma simulação que só gera leituras normais não exercita as regras de análise.

---

## 5. Validação de entrada

Todo input do usuário passa pelo padrão `fgets` + `sscanf`:

```c
char linha[64];
if (fgets(linha, sizeof(linha), stdin) != NULL) {
    if (sscanf(linha, "%f", &temp) == 1 && temp >= -200.0f && temp <= 500.0f) {
        // entrada válida
    }
}
```

**Por que não `scanf("%f", &temp)` direto?**
`scanf` puro deixa lixo no buffer quando o usuário digita "abc" num campo `%f`. Esse lixo é lido pelo próximo `scanf`, criando loops infinitos no menu. `fgets` consome a linha inteira (incluindo o `\n`), eliminando o problema. Isso é a diferença entre um programa de aula e um programa robusto.

---

## 6. Cores no terminal (módulo `ui.c`)

As cores usam códigos **ANSI escape sequences**: a sequência `\033[1;31m` ativa o vermelho-negrito, e `\033[0m` reseta.

No **Windows 10+**, o suporte a ANSI precisa ser ativado manualmente via API Win32:
```c
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwMode = 0;
GetConsoleMode(hOut, &dwMode);
SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
```

Sem essa chamada (feita uma única vez em `main()`), os códigos aparecem como lixo no terminal. Com ela, viram cores reais. O código usa `#ifdef _WIN32` para que essa parte só seja compilada no Windows — em Linux/Mac as cores funcionam nativamente.

---

## 7. Animação de carregamento

A animação tipo spinner usa quatro caracteres (`| / - \`) impressos no mesmo lugar do terminal, com `\r` (carriage return) para voltar o cursor ao início da linha:

```c
const char frames[] = {'|', '/', '-', '\\'};
for (int i = 0; i < total; i++) {
    printf("\r%s %c", mensagem, frames[i % 4]);
    fflush(stdout);
    Sleep(80);  // 80ms entre frames
}
```

`fflush(stdout)` força a impressão imediata (sem ele, o buffer da stdout só esvazia ao encontrar `\n`, e a animação não aparece).

---

## 8. Por que o sistema é "à prova de falhas"

| Risco potencial | Como foi mitigado |
|----------------|-------------------|
| Buffer overflow em strings | Todos os `strcpy/strcat` operam em buffers de tamanho conhecido e protegido. Usamos `fgets` com `sizeof(buffer)`. |
| Loop infinito em entrada inválida | `fgets + sscanf` com validação de retorno e faixa de valores. |
| Divisão por zero em estatísticas | Verificação `if (total_leituras == 0) return 0.0f;` em todas as funções de média. |
| Acesso fora dos limites do vetor | Operações de índice sempre usam `% MAX_LEITURAS`. |
| Cores virando lixo no Windows antigo | `habilitarCoresWindows()` ativa o modo virtual terminal. |
| Acentos quebrados no terminal | `SetConsoleOutputCP(65001)` configura UTF-8 no Windows. |
| Histórico vazio em operações | Mensagem amigável `"[!] Nenhuma leitura registrada"` em vez de crash. |

---

## 9. Conclusão

O sistema entrega todos os requisitos obrigatórios do briefing **e** todos os opcionais sugeridos, usando uma estrutura de dados não-trivial (buffer circular) que justifica o nome da disciplina. A modularização em cinco arquivos demonstra domínio real da linguagem C — não pseudocódigo amontoado em um único arquivo.

A robustez (validação de entrada, proteção contra divisão por zero, compatibilidade Windows/Linux) é o que separa um trabalho funcional de um trabalho **profissional**.
