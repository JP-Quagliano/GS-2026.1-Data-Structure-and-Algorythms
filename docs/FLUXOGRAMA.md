# 📊 Fluxograma do Sistema

> **Entregável obrigatório do briefing.**
> Esse fluxograma é renderizado **automaticamente** pelo GitHub no preview do arquivo Markdown — não precisa abrir ferramenta externa.

---

## Fluxograma principal do sistema

```mermaid
flowchart TD
    A([INÍCIO]) --> B[Habilitar cores ANSI Windows]
    B --> C[Inicializar histórico e sensores]
    C --> D[Exibir banner de boas-vindas]
    D --> E[Animação de carregamento]
    E --> F{{Exibir Menu Principal}}

    F --> G[/Usuário escolhe opção/]
    G --> H{switch opção}

    H -->|1| I[Coletar leitura manual]
    H -->|2| J[Simulação automática com rand]
    H -->|3| K[Exibir status atual]
    H -->|4| L[Análise completa do histórico]
    H -->|5| M[Exibir histórico em tabela]
    H -->|6| N[Calcular estatísticas]
    H -->|7| O[Limpar histórico]
    H -->|0| P{Confirmar saída?}
    H -->|inválido| Q[Mensagem de erro]

    I --> R[Validar entrada]
    R --> S[analisarLeitura aplica regras]
    S --> T[adicionarLeitura no buffer circular]

    J --> U[gerar N leituras aleatórias]
    U --> V[Para cada leitura: analisar + adicionar]

    T --> F
    V --> F
    K --> F
    L --> F
    M --> F
    N --> F
    O --> F
    Q --> F

    P -->|Sim| W[Exibir tela de encerramento]
    P -->|Não| F
    W --> X([FIM])

    style A fill:#22c55e,stroke:#16a34a,color:#fff
    style X fill:#ef4444,stroke:#dc2626,color:#fff
    style F fill:#3b82f6,stroke:#2563eb,color:#fff
    style H fill:#f59e0b,stroke:#d97706,color:#fff
```

---

## Fluxograma da regra de análise (`analisarLeitura`)

```mermaid
flowchart TD
    A([Leitura recebida]) --> B[Zerar campo status]
    B --> C{temperatura > 80?}
    C -->|Sim| D[Adicionar 'ALERTA-TEMP']
    C -->|Não| E{temperatura > 65?}
    E -->|Sim| F[Adicionar 'ATENCAO-TEMP']
    E -->|Não| G[/temperatura OK/]

    D --> H{energia < 20?}
    F --> H
    G --> H

    H -->|Sim| I[Adicionar 'ALERTA-ENERG']
    H -->|Não| J{energia < 40?}
    J -->|Sim| K[Adicionar 'ATENCAO-ENERG']
    J -->|Não| L[/energia OK/]

    I --> M{comunicação = 0?}
    K --> M
    L --> M

    M -->|Sim| N[Adicionar 'ALERTA-COMUN']
    M -->|Não| O[/comunicação OK/]

    N --> P{status vazio?}
    O --> P

    P -->|Sim| Q[status = 'NORMAL']
    P -->|Não| R[/Manter alertas concatenados/]

    Q --> S([Retornar])
    R --> S

    style A fill:#22c55e,stroke:#16a34a,color:#fff
    style S fill:#ef4444,stroke:#dc2626,color:#fff
    style D fill:#fca5a5,stroke:#dc2626
    style I fill:#fca5a5,stroke:#dc2626
    style N fill:#fca5a5,stroke:#dc2626
    style F fill:#fde047,stroke:#ca8a04
    style K fill:#fde047,stroke:#ca8a04
    style Q fill:#86efac,stroke:#16a34a
```

---

## Fluxograma do buffer circular (`adicionarLeitura`)

```mermaid
flowchart TD
    A([Nova leitura chega]) --> B["Escrever em historico[proxima_posicao]"]
    B --> C["proxima_posicao = proxima_posicao + 1 modulo MAX"]
    C --> D{total_leituras < MAX?}
    D -->|Sim| E[Incrementar total_leituras]
    D -->|Não| F[Buffer cheio: sobrescreve a mais antiga]
    E --> G([Fim])
    F --> G

    style A fill:#22c55e,stroke:#16a34a,color:#fff
    style G fill:#ef4444,stroke:#dc2626,color:#fff
    style F fill:#fde047,stroke:#ca8a04
```

---

## Como o fluxograma foi feito

Foi escrito em **Mermaid**, uma linguagem de diagramas em texto puro que o GitHub renderiza nativamente em arquivos Markdown. Vantagens:

- ✅ **Versionável no Git** (texto puro, não imagem binária)
- ✅ **Renderiza no GitHub sem precisar de imagem externa**
- ✅ **Fácil de editar** — basta alterar o texto

Para visualizar antes de subir: cole o código em [mermaid.live](https://mermaid.live).
