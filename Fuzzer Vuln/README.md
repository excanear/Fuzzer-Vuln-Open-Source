# FuzzerVuln - Advanced Coverage-Guided Fuzzer

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/yourusername/FuzzerVuln/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Windows](https://img.shields.io/badge/platform-Windows-blue)](https://en.wikipedia.org/wiki/Microsoft_Windows)

Um fuzzer avançado guiado por cobertura, inspirado no American Fuzzy Lop (AFL), implementado em C++17, C e x86_64 Assembly. Inclui interface de linha de comando e aplicativo GUI dedicado para facilitar o uso por iniciantes.

## 🚀 Visão Geral

O FuzzerVuln é uma ferramenta profissional de fuzzing que combina técnicas avançadas de mutação, instrumentação de código e análise de cobertura para maximizar a descoberta de bugs e vulnerabilidades. Desenvolvido com arquitetura modular, permite extensibilidade e personalização para diferentes tipos de aplicações.

### Principais Características

- **Fuzzing Guiado por Cobertura**: Utiliza instrumentação binária para rastrear execução e priorizar inputs que descobrem novos caminhos
- **Múltiplos Mutadores**: Conjunto abrangente de algoritmos de mutação (bitflip, arithmetic, splicing, dictionary-based)
- **Detecção Robusta de Crashes**: Identifica e classifica diferentes tipos de falhas
- **Gerenciamento de Corpus**: Minimização automática e expansão inteligente do conjunto de inputs
- **Interface em Tempo Real**: Monitoramento visual do progresso com estatísticas detalhadas
- **Aplicativo GUI**: Interface gráfica intuitiva para configuração e execução sem linha de comando
- **Arquitetura Modular**: Fácil extensão com novos mutadores e analisadores
- **Suporte Windows**: Otimizado para ambiente Windows com API nativa

## 📋 Tabela de Conteúdo

- [Instalação](#instalação)
- [Uso Rápido](#uso-rápido)
- [Aplicativo GUI](#aplicativo-gui)
- [Arquitetura](#arquitetura)
- [Componentes](#componentes)
- [Como Construir](#como-construir)
- [Exemplos](#exemplos)
- [Configuração](#configuração)
- [Detecção de Crashes](#detecção-de-crashes)
- [Corpus Management](#corpus-management)
- [Extensibilidade](#extensibilidade)
- [Performance](#performance)
- [Limitações](#limitações)
- [Contribuição](#contribuição)
- [Licença](#licença)
- [Referências](#referências)

## 🛠️ Instalação

### Pré-requisitos

- **Sistema Operacional**: Windows 10/11
- **Compilador**: MinGW-w64 GCC (versão 15+ recomendada)
- **Ferramentas**: Git, Make (opcional)

### Dependências

O projeto inclui todas as dependências necessárias. As seguintes DLLs são requeridas em runtime:
- `libstdc++-6.dll`
- `libgcc_s_seh-1.dll`
- `libwinpthread-1.dll`

### Download e Configuração

```bash
# Clone o repositório
git clone https://github.com/yourusername/FuzzerVuln.git
cd FuzzerVuln

# Execute o script de build
.\build.bat
```

Após o build, você terá:
- `fuzzer_engine.exe`: Versão de linha de comando
- `gui_app.exe`: Aplicativo GUI dedicado
- `example_target.exe`: Target de exemplo para teste

## 🚀 Uso Rápido

### Opções de Interface

O FuzzerVuln oferece duas formas de uso:

1. **Linha de Comando** (`fuzzer_engine.exe`): Para usuários avançados e automação
2. **Aplicativo GUI** (`gui_app.exe`): Interface gráfica intuitiva para iniciantes

### Comando Básico (Linha de Comando)

```bash
.\fuzzer_engine.exe <target> <corpus_dir> [--timeout <ms>] [--dry-run] [--gui]
```

### Exemplo Simples (Linha de Comando)

```bash
# Fuzz o exemplo incluído
.\fuzzer_engine.exe .\example_target.exe corpus --timeout 100
```

### Saída Esperada (Linha de Comando)

```
Instrumentation initialized
Input: Hello World
Done
Instrumentation initialized
Input: Hello Worôd
Done
...
Execs/sec: 15 | Total: 100 | Coverage: 5 | Crashes: 2 | Corpus: 3
```

## 🖥️ Aplicativo GUI

Para usuários que preferem interface gráfica, o FuzzerVuln inclui um aplicativo dedicado (`gui_app.exe`) que não requer conhecimento de linha de comando.

### Como Usar o GUI

1. **Execute o aplicativo**:
   ```bash
   .\gui_app.exe
   ```

2. **Configure os parâmetros**:
   - **Target Executable**: Clique "Browse" e selecione o programa a ser testado
   - **Corpus Directory**: Clique "Browse" e selecione a pasta com inputs iniciais
   - **Timeout (ms)**: Ajuste o tempo limite (padrão: 5000ms)
   - **Dry Run**: Marque para teste sem execução real

3. **Inicie o fuzzing**:
   - Clique "Start Fuzzing"
   - Uma janela de estatísticas aparecerá automaticamente
   - Monitore o progresso em tempo real

4. **Pare quando quiser**:
   - Clique "Stop" para interromper

### Vantagens do GUI

- **Intuitivo**: Navegação visual de arquivos e pastas
- **Seguro**: Validação automática de entradas
- **Visual**: Estatísticas em tempo real em janela dedicada
- **Acessível**: Perfeito para iniciantes em fuzzing

### Exemplo de Uso

```
1. Execute .\gui_app.exe
2. Clique "Browse" em "Target Executable" → Selecione example_target.exe
3. Clique "Browse" em "Corpus Directory" → Selecione pasta corpus/
4. Clique "Start Fuzzing"
5. Observe a janela de stats atualizar automaticamente
```

O FuzzerVuln segue uma arquitetura modular inspirada no AFL:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Input Queue   │───▶│   Mutators      │───▶│   Target Exec   │
│                 │    │                 │    │                 │
│ - Seed Corpus   │    │ - Bitflip       │    │ - Instrumentation│
│ - Generated     │    │ - Arithmetic    │    │ - Coverage Map  │
│ - Prioritized   │    │ - Splicing      │    │ - Crash Detect  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         ▲                       ▲                       │
         │                       │                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  Corpus Mgmt    │◀───│  Coverage       │◀───│   Statistics    │
│                 │    │  Analysis       │    │                 │
│ - Minimization  │    │                 │    │ - Exec/sec      │
│ - Expansion     │    │ - New Paths     │    │ - Coverage      │
│ - Deduplication │    │ - Hit Counts    │    │ - Crashes       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Fluxo de Execução

1. **Inicialização**: Carrega corpus inicial e configura instrumentação
2. **Loop Principal**:
   - Seleciona input da fila
   - Aplica mutações
   - Executa target instrumentado
   - Coleta cobertura e detecta crashes
   - Atualiza estatísticas e expande corpus
3. **Minimização**: Periodicamente reduz corpus duplicado

## 🔧 Componentes

### Core Engine (`src/engine/`)

- **`fuzzer_engine.cpp`**: Loop principal de fuzzing e orquestração
- **`input_scheduler.cpp`**: Gerenciamento da fila de inputs
- **`coverage_collector.cpp`**: Análise de mapa de cobertura
- **`crash_detector.cpp`**: Detecção e classificação de crashes

### Mutators (`src/mutators/`)

- **`bitflip_mutator.c`**: Inversão de bits individuais
- **`byteflip_mutator.c`**: Inversão de bytes
- **`arithmetic_mutator.c`**: Operações aritméticas
- **`dictionary_mutator.c`**: Injeção de palavras-chave
- **`splicing_mutator.c`**: Combinação de inputs existentes

### Instrumentation (`src/instrumentation/`)

- **`instrumentation.c`**: Configuração de memória compartilhada
- **`coverage_instrumentation.asm`**: Instrumentação assembly (não implementada)

### UI (`src/ui/`)

- **`text_ui.cpp`**: Interface de texto em tempo real para linha de comando
- **`gui_ui.cpp`**: Interface gráfica para monitoramento visual
- **`ui_base.h`**: Classe base abstrata para interfaces
- **`main_gui.cpp`**: Aplicativo GUI principal com configuração visual

### API (`src/mutators/mutator_api.c`)

- Sistema de registro de mutadores
- Interface C para extensibilidade

## 🏭 Como Construir

### Build Automático (Recomendado)

```bash
# Execute o script de build
.\build.bat
```

Este script:
- Compila todos os arquivos fonte
- Linka as bibliotecas necessárias
- Gera `fuzzer_engine.exe`, `gui_app.exe` e `example_target.exe`

### Build Manual

```bash
# Compilar objetos
g++ -std=c++17 -I src\instrumentation -c src\main.cpp -o build\main.o
g++ -std=c++17 -I src\instrumentation -c src\engine\fuzzer_engine.cpp -o build\fuzzer_engine.o
# ... (outros arquivos)

# Linkar
g++ -o fuzzer_engine.exe build\*.o -lpthread
```

### Verificação

```bash
# Verificar executáveis
dir *.exe
# fuzzer_engine.exe  (linha de comando)
# gui_app.exe        (aplicativo GUI)
# example_target.exe (target de exemplo)
```

## 📖 Exemplos

### Fuzzing Básico (Linha de Comando)

```bash
# Fuzz com timeout padrão (5s)
.\fuzzer_engine.exe .\my_app.exe corpus

# Fuzz com timeout curto para aplicações rápidas
.\fuzzer_engine.exe .\my_app.exe corpus --timeout 50

# Modo dry-run (sem execução real)
.\fuzzer_engine.exe .\my_app.exe corpus --dry-run

# Com interface gráfica de stats
.\fuzzer_engine.exe .\my_app.exe corpus --gui --timeout 100
```

### Fuzzing com GUI (Aplicativo Dedicado)

```bash
# Execute o aplicativo GUI
.\gui_app.exe

# Interface visual:
# 1. Selecione target: my_app.exe
# 2. Selecione corpus: corpus/
# 3. Timeout: 100ms
# 4. Clique "Start Fuzzing"
```

### Preparando Corpus

```
corpus/
├── seeds/
│   ├── input1.txt
│   ├── input2.bin
│   └── ...
└── crashes/
    └── (gerado automaticamente)
```

### Exemplo de Target

```c
// target.c - Programa a ser fuzzado
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 0;

    FILE *f = fopen(argv[1], "rb");
    if (!f) return 1;

    char buf[100];
    fread(buf, 1, sizeof(buf), f);
    fclose(f);

    // Potencial vulnerabilidade
    if (buf[0] == 'A' && buf[1] == 'B' && buf[2] == 'C') {
        // Crash intencional para teste
        *(volatile int*)0 = 0;
    }

    return 0;
}
```

## ⚙️ Configuração

### Arquivo de Configuração

O fuzzer suporta configuração via `fuzzer_config.ini`:

```ini
[General]
TimeoutMs = 5000
MaxExecs = 1000000

[Mutators]
EnableDictionary = true
DictionaryFile = dict.txt

[UI]
UpdateInterval = 10
```

### Variáveis de Ambiente

- `FUZZER_CORPUS_DIR`: Diretório padrão do corpus
- `FUZZER_TIMEOUT`: Timeout padrão em ms

## 💥 Detecção de Crashes

### Tipos de Crashes Detectados

- **Access Violation**: Tentativa de acesso inválido à memória
- **Division by Zero**: Divisão por zero
- **Stack Overflow**: Estouro de pilha
- **Invalid Instruction**: Instrução inválida

### Salvamento de Crashes

Crashes são salvados em `corpus/crashes/` com nome `crash_<exit_code>`:

```
corpus/crashes/
├── crash_3221225477  # Access violation
├── crash_3221225620  # Stack overflow
└── ...
```

### Análise de Crashes

```bash
# Examinar crash
hexdump -C corpus/crashes/crash_3221225477
```

## 📁 Corpus Management

### Estrutura do Corpus

```
corpus/
├── seeds/           # Inputs iniciais
├── queue/           # Inputs gerados (criado automaticamente)
└── crashes/         # Inputs que causam crashes
```

### Minimização

O fuzzer automaticamente:
- Remove inputs duplicados
- Mantém apenas inputs que descobrem nova cobertura
- Reduz tamanho do corpus para otimização

### Expansão

- Adiciona inputs que descobrem novos caminhos
- Prioriza inputs com alta diversidade
- Mantém equilíbrio entre exploração e exploração

## 🔌 Extensibilidade

### Adicionando Novo Mutador

1. Implemente a função em `src/mutators/`:

```c
void my_mutator(input_t *input, void *context) {
    // Lógica de mutação
    input->data[0] ^= 0xFF;  // Exemplo simples
}

void init_my_mutator() {
    register_mutator("my_mutator", my_mutator, NULL);
}
```

2. Registre no `mutator_api.c`:

```c
extern void init_my_mutator();

void init_mutators() {
    // ... outros
    init_my_mutator();
}
```

### Interface de Mutador

```c
typedef struct {
    uint8_t *data;
    size_t size;
} input_t;

typedef void (*mutator_func)(input_t *input, void *context);
```

## ⚡ Performance

### Métricas Típicas

- **Execuções/segundo**: 10-50 (depende do target)
- **Cobertura**: 50-200 blocos básicos típicos
- **Uso de Memória**: ~50MB para corpus médio
- **Uso de CPU**: 80-95% em máquina dedicada

### Otimizações

- **Memória Compartilhada**: Comunicação eficiente com target
- **Mutação In-Place**: Minimiza alocações
- **Timeout Inteligente**: Previne hangs
- **Corpus Priorizado**: Foca em inputs promissores

## 🚫 Limitações

- **Plataforma**: Windows only
- **Arquitetura**: x86_64 only
- **Target**: Binários compilados (não interpretados)
- **Cobertura**: Basic block level (não edge-level)
- **Concorrência**: Single-threaded

### Problemas Conhecidos

- Instrumentação assembly não implementada
- Suporte limitado a aplicações multi-threaded
- Detecção de hangs imperfeita

## 🤝 Contribuição

### Como Contribuir

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

### Diretrizes

- Siga o estilo de código existente
- Adicione testes para novas funcionalidades
- Atualize documentação
- Mantenha compatibilidade backward

### Áreas de Interesse

- Implementação de instrumentação assembly
- Suporte a outras plataformas
- Melhor detecção de hangs
- Melhorias na interface gráfica
- Integração com ferramentas de análise
- Suporte a múltiplas threads

## 📄 Licença

Este projeto está licenciado sob a MIT License - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 📚 Referências

- [American Fuzzy Lop (AFL)](https://lcamtuf.coredump.cx/afl/)
- [Fuzzing Book](https://www.fuzzingbook.org/)
- [Coverage-Guided Fuzzing Survey](https://arxiv.org/abs/1807.07490)
- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)

## 🙏 Agradecimentos

- Inspirado no trabalho de Michal Zalewski (AFL)
- Comunidade de security research
- Contribuidores do projeto
- Windows API e Win32 documentation para desenvolvimento da interface gráfica

---

**Nota**: Este é um projeto educacional e de pesquisa. Use responsávelmente e apenas em sistemas que você tem permissão para testar.