# 🚀 Sistema de Monitoramento de Missão Espacial

> **GS2026.1 — Data Structures and Algorithms**
> **Professor:** Erick Toshio Yamamoto
> **FIAP — 1º Semestre de 2026**

Sistema desenvolvido em **linguagem C** para monitoramento em tempo real de uma missão espacial experimental. O programa recebe dados simulados de sensores (temperatura, energia, comunicação), aplica regras de análise automática, emite alertas visuais e mantém um histórico organizado das leituras utilizando estrutura de dados do tipo **buffer circular**.

---

## 👥 Integrantes

| Nome | RM |
|------|-----|
| João Pedro do Vale Quagliano | 570233 |
| Matheus Levi Dagel | 571961 |

---

## ✨ Funcionalidades

### Obrigatórias (briefing)
- ✅ Cadastro manual de leituras (temperatura, energia, comunicação)
- ✅ Verificação automática com as três regras do enunciado:
  - Temperatura > 80 → **Alerta de superaquecimento**
  - Energia < 20 → **Economia de energia**
  - Comunicação = 0 → **Falha de comunicação**
- ✅ Menu interativo implementado com `switch()`
- ✅ Uso de estruturas condicionais, repetição, vetores e funções

### Adicionais (extras de excelência)
- ✅ **Histórico das leituras** em buffer circular (struct + vetor)
- ✅ **Simulação contínua dos sensores** com `rand()`
- ✅ **Cores no terminal** via códigos ANSI
- ✅ **Animações simples** (spinner de carregamento)
- ✅ **Estatísticas agregadas**: média/máx/mín de temperatura, média/mín de energia, contagem de falhas, índice de criticidade
- ✅ **Banner artístico** em ASCII art
- ✅ **Compilação multi-arquivo** com headers separados (5 módulos)
- ✅ **Validação robusta de entrada** (`fgets` + `sscanf`) — programa não trava com inputs inválidos

---

## 🗂️ Estrutura do Projeto

```
projeto_missao_espacial/
├── src/
│   ├── main.c           # Ponto de entrada e menu principal (switch)
│   ├── historico.h/.c   # Buffer circular (struct LeituraSensor)
│   ├── sensores.h/.c    # Coleta manual e simulada com rand()
│   ├── analise.h/.c     # Regras de alerta e estatísticas
│   └── ui.h/.c          # Cores ANSI, banner e animações
├── docs/
│   ├── EXPLICACAO_LOGICA.md    # Lógica de cada módulo (entregável)
│   ├── FLUXOGRAMA.md           # Fluxograma do sistema (Mermaid)
│   ├── COMO_COMPILAR_VS.md     # Passo a passo no Visual Studio
│   └── DEMONSTRACAO.md         # Roteiro da demonstração prática
├── .gitignore
├── LICENSE
└── README.md
```

---

## 🔧 Como Compilar e Executar

### Visual Studio (Windows)
Veja o passo a passo detalhado em [`docs/COMO_COMPILAR_VS.md`](docs/COMO_COMPILAR_VS.md).

Resumo:
1. Crie um projeto **"Console App"** em C (não C++).
2. Adicione todos os arquivos da pasta `src/` ao projeto.
3. Pressione `Ctrl+F5` para compilar e executar.

### Linha de comando (GCC / MinGW)
```bash
cd src
gcc -Wall -Wextra -std=c11 main.c sensores.c analise.c historico.c ui.c -o missao
./missao        # Linux/Mac
missao.exe      # Windows
```

---

## 📋 Menu do Sistema

```
+------------------- MENU PRINCIPAL -------------------+
| 1 - Inserir leitura manual                           |
| 2 - Iniciar simulação automática (rand)              |
| 3 - Visualizar status atual                          |
| 4 - Executar análise completa                        |
| 5 - Exibir histórico de leituras                     |
| 6 - Estatísticas da missão                           |
| 7 - Limpar histórico                                 |
| 0 - Encerrar sistema                                 |
+------------------------------------------------------+
```

---

## 🧠 Decisões Técnicas Relevantes

### Por que buffer circular?
Em vez de um vetor simples que estoura quando atinge o limite, o **buffer circular** sobrescreve automaticamente a leitura mais antiga. Isso simula um sistema real de telemetria espacial, onde os dados nunca param de chegar mas a memória é finita. **É a estrutura de dados que dá nome à matéria** — daí a escolha.

### Por que `fgets` + `sscanf` em vez de `scanf` puro?
`scanf` puro deixa lixo no buffer quando o usuário digita algo inválido (ex.: letras num campo numérico), criando loops infinitos no menu. `fgets` consome a linha inteira e `sscanf` extrai com segurança, eliminando o problema.

### Por que compilação multi-arquivo?
Demonstra domínio real de C (não pseudocódigo em um único arquivo) e respeita os critérios de **organização do código** e **uso correto da linguagem**. Cada módulo tem responsabilidade única.

Detalhamento completo em [`docs/EXPLICACAO_LOGICA.md`](docs/EXPLICACAO_LOGICA.md).

---

## 📊 Critérios de Avaliação Atendidos

| Critério | Peso | Como foi atendido |
|----------|------|-------------------|
| Funcionamento do sistema | 4,0 | Todas as funcionalidades operacionais + extras |
| Uso correto da linguagem C | 3,0 | Headers separados, structs, ponteiros, modularização |
| Organização do código | 2,0 | 5 módulos com responsabilidades únicas, comentários técnicos |
| Lógica implementada | 1,0 | Buffer circular, análise empilhada de alertas, estatísticas |

---

## 📄 Licença

Projeto acadêmico — FIAP 2026. Uso livre para fins educacionais.
