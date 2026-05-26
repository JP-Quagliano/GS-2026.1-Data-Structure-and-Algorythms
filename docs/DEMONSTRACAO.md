# 🎬 Roteiro de Demonstração Prática

> Sequência de ações para demonstrar o sistema funcionando em todas as suas capacidades.
> Tempo total estimado: **3 a 5 minutos**.

---

## 🎯 Objetivo

Mostrar que o sistema:
1. Atende a **todos os requisitos obrigatórios** do briefing
2. Implementa **todos os extras** sugeridos
3. É **robusto** (não trava com inputs inválidos)
4. Tem **lógica correta** (alertas disparam quando deveriam)

---

## 📜 Roteiro

### Cena 1 — Inicialização (15s)
1. Abrir o terminal/console.
2. Rodar o executável.
3. **Comentar:** "O sistema inicia com banner ASCII, animação de carregamento e ativa cores ANSI no Windows automaticamente."

### Cena 2 — Inserção manual com leitura NORMAL (30s)
1. Digitar `1` no menu.
2. Inserir: temperatura `25`, energia `80`, comunicação `1`.
3. **Comentar:** "Sistema validou os três campos. Como nenhuma regra foi disparada, o status foi marcado como NORMAL em verde."
4. Pressionar ENTER.

### Cena 3 — Inserção manual com ALERTAS empilhados (45s)
1. Digitar `1` no menu novamente.
2. Inserir: temperatura `95` (>80), energia `15` (<20), comunicação `0`.
3. **Comentar:** "Esta leitura dispara as TRÊS regras de alerta simultaneamente: superaquecimento, energia crítica e falha de comunicação. O sistema concatena todos os alertas no campo status — não trata cada problema isoladamente."

### Cena 4 — Validação de entrada (30s)
1. Digitar `1` no menu.
2. Para temperatura, digitar `abc` (input inválido).
3. **Comentar:** "Veja: o sistema rejeita a entrada inválida e pede de novo, sem travar. Isso porque usamos `fgets + sscanf` em vez de `scanf` puro."
4. Inserir valores válidos para finalizar.

### Cena 5 — Simulação automática (45s)
1. Digitar `2` no menu.
2. Solicitar `15` leituras.
3. **Comentar:** "Aqui usamos `rand()` com semente baseada em `time()` para gerar leituras aleatórias. Cada leitura é avaliada em tempo real e classificada em verde, amarelo ou vermelho. Note como o sistema simula uma telemetria contínua de uma nave real."

### Cena 6 — Status atual (15s)
1. Digitar `3`.
2. **Comentar:** "Status da última leitura, com todos os campos formatados e codificados por cor."

### Cena 7 — Histórico completo (30s)
1. Digitar `5`.
2. **Comentar:** "Esta é a tabela do **histórico completo**. Importante: aqui está sendo usada a estrutura de dados **buffer circular** — vetor de structs com indexação modular. Quando atingirmos 100 leituras, a mais antiga é automaticamente sobrescrita."

### Cena 8 — Estatísticas (30s)
1. Digitar `6`.
2. **Comentar:** "O módulo de análise calcula média, máxima e mínima de temperatura, média e mínima de energia, total de falhas de comunicação, total de alertas e — extra — um **índice de criticidade da missão** baseado na taxa de alertas. Isso vai além do briefing."

### Cena 9 — Reanálise (15s)
1. Digitar `4`.
2. **Comentar:** "Esta opção reprocessa todas as leituras com as regras atuais. Útil se os limites mudarem."

### Cena 10 — Limpar histórico com confirmação (15s)
1. Digitar `7`.
2. Quando perguntar, digitar `n` (não).
3. **Comentar:** "Ações destrutivas pedem confirmação para evitar perda acidental de dados."

### Cena 11 — Encerramento (10s)
1. Digitar `0`.
2. Confirmar `s`.
3. **Comentar:** "Sistema encerra com mensagem de despedida."

---

## 🗣️ Pontos-chave a enfatizar (caso seja apresentação oral)

1. **"Buffer circular"** — não é um vetor comum. Demonstra estrutura de dados real.
2. **"Compilação modular"** — 5 arquivos `.c` separados com responsabilidades únicas.
3. **"Alertas empilháveis"** — uma leitura pode disparar múltiplos alertas; o sistema não trata problemas isoladamente.
4. **"Validação de entrada com `fgets + sscanf`"** — programa robusto, não trava com entradas inválidas.
5. **"Compatibilidade Windows/Linux"** — uso de `#ifdef _WIN32` para portabilidade.

---

## ❓ Perguntas que o professor pode fazer (preparação)

**P: Por que vocês escolheram buffer circular?**
R: Porque sistemas de telemetria reais não param de receber dados, mas a memória é finita. Buffer circular descarta automaticamente o dado mais antigo — comportamento ideal para esse cenário. Além disso, demonstra uma estrutura de dados não-trivial, que é o tema da matéria.

**P: O que acontece se eu inserir 101 leituras?**
R: A leitura 101 sobrescreve a leitura 1, a 102 sobrescreve a 2, e assim por diante. `proxima_posicao` volta a zero quando atinge 100 (`% MAX_LEITURAS`). O total permanece em 100.

**P: Por que separar em vários arquivos?**
R: Coesão alta e acoplamento baixo. Cada módulo tem uma responsabilidade única (histórico, sensores, análise, UI, orquestração). Facilita manutenção, testes e leitura. Em projetos reais com milhares de linhas, é impraticável manter tudo em um único arquivo.

**P: O que é ENABLE_VIRTUAL_TERMINAL_PROCESSING?**
R: É uma flag da API do Windows que ativa o processamento de sequências ANSI no terminal. Sem ela, no Windows os códigos de cor aparecem como texto literal. Em Linux/Mac, o suporte é nativo e a chamada é compilada fora via `#ifdef _WIN32`.

**P: Por que `fgets + sscanf` em vez de `scanf`?**
R: `scanf` deixa lixo no buffer da stdin quando recebe entrada inválida (ex.: letras num campo `%d`). Esse lixo é lido pelo próximo `scanf`, causando loops infinitos. `fgets` consome a linha inteira incluindo o `\n`, e `sscanf` extrai do buffer sem afetar a stdin. É o padrão recomendado para entrada robusta em C.
