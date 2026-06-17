Aqui está uma proposta completa para a página de **Wiki do Projeto** no padrão ideal para submissão académica ou portfólios técnicos de engenharia de software (Wiki Projetos TF).

Esta estrutura está totalmente alinhada com os conceitos de **POO Avançada**, **C++ Moderno** e as correções de arquitetura que garantem a completude do seu ecossistema **VPet Retro**.

---

# 📑 Wiki / README: VPet Retro (POO Enhanced Edition)

## 🎯 1. Visão Geral do Projeto

O **VPet Retro** é um simulador interativo de animal de estimação virtual (*Virtual Pet*) inspirado nos clássicos chaveiros Tamagotchi dos anos 90. O software foi desenvolvido em **C++** utilizando a biblioteca gráfica **SFML (Simple and Fast Multimedia Library) 3.0+**, aplicando conceitos rigorosos de **Programação Orientada a Objetos (POO)**, padrões de design para interfaces gráficas e boas práticas de arquitetura de software limpa.

### Objetivos do Desenvolvimento

* **Encapsulamento Estrito:** Eliminar qualquer exposição direta de atributos a escopos externos, centralizando mutações de estado em contratos de métodos controlados.
* **Polimorfismo Dinâmico:** Garantir que o motor principal do jogo manipule os animais de forma agnóstica através de uma classe base abstrata.
* **Segurança de Memória (Modern C++):** Erradicação de ponteiros brutos (`raw pointers`) e vazamentos de memória através do uso de ponteiros inteligentes (`std::unique_ptr`).
* **Programação Genérica:** Criação de estruturas de inventário reutilizáveis baseadas em *Templates*.

---

## 🏗️ 2. Arquitetura do Sistema e Conceitos de POO

O projeto é estruturado de forma modular e desacoplada em cinco componentes principais:

```
📦 VPet-Retro
 ┣ 📜 main.cpp          <- Orquestrador do Loop, Máquina de Estados e Renderização
 ┣ 📜 Mascote.h/.cpp    <- Abstração Base (Mascote) e Especializações Polimórficas
 ┣ 📜 Inventario.h      <- Estrutura Genérica de Dados (Template) para Itens
 ┣ 📜 Botao.h           <- Componente Reutilizável de UI Encapsulado
 ┗ 📜 font.ttf          <- Assets de Renderização de Tipografia

```

### 🧬 Mapeamento de Conceitos POO Aplicados

#### A. Abstração e Classes Abstratas

A classe `Mascote` atua como um contrato abstrato. Ela não pode ser instanciada diretamente por conter funções virtuais puras (`= 0`). Ela define o ciclo de vida genérico aplicável a qualquer espécie de criatura virtual:

```cpp
virtual std::wstring getIconeFeliz() const = 0;
virtual std::wstring getComidaCerta() const = 0;

```

#### B. Encapsulamento de Nível Acadêmico

Ao contrário de modelagens frágeis que utilizam modificadores `protected` ou `public` para atributos de estado, todos os dados do mascote (como `saciedade`, `felicidade`, `saude` e `energia`) são declarados estritamente como `private`.

* As subclasses e classes externas comunicam-se apenas através de getters públicos const-safe (`int getSaude() const`) e métodos de comportamento coordenado (ex: `alimentar(...)`).

#### C. Herança e Polimorfismo Rebatido

As classes `Cachorro`, `Gato` e `Coelho` herdam os atributos e comportamentos de `Mascote`. O polimorfismo é executado em tempo de execução (`Runtime Polymorphism`) pelo loop principal. Quando o orquestrador chama `pet->getArtAtual()`, o comportamento dinâmico é resolvido com base na subclasse real instanciada.

#### D. Programação Genérica via Templates

A classe `Inventario<T>` implementa um container genérico. No contexto atual do projeto, ela armazena instâncias de `ItemComida`, mas sua estrutura parametrizada permite reutilização futura imediata para gerenciar outros tipos de objetos (como `ItemBrinquedo` ou `Medicamento`), sem alterar uma única linha de código interno do container.

---

## 📊 3. Diagrama de Classes UML (Concordância de Implementação)

O diagrama abaixo reflete a hierarquia de dependência e herança exata encontrada no código fonte atualizado:

```
    +--------------------------------------+
    |          Inventario<T>               |
    +--------------------------------------+
    | - itens: std::vector<T>              |
    +--------------------------------------+
    | + adicionar(item: const T&): void    |
    | + operator[](index: size_t): T&      |
    | + tamanho(): size_t                  |
    +--------------------------------------+
                       | (Armazena)
                       v
    +--------------------------------------+            +------------------------+
    |             ItemComida               |            |         Botao          |
    +--------------------------------------+            +------------------------+
    | - nome: std::wstring                 |            | + shape: RectangleShape|
    | - icone: std::wstring                |            | + texto: sf::Text      |
    +--------------------------------------+            | + idacao: int          |
    | + getNome(): std::wstring            |            | + ativo: bool          |
    | + getIcone(): std::wstring           |            +------------------------+
    +--------------------------------------+            | + Botao(font: Font)    |
                       ^                                | + desenhar(win): void  |
                       | (Passado como Parâmetro)       | + foiClicado(p): bool  |
                       |                                +------------------------+
    +--------------------------------------+
    |               Mascote                | <--- [ Abstrata ]
    +--------------------------------------+
    | - nome, especie: std::wstring        |
    | - idade, saciedade, saude: int       |
    | - felicidade, energia: int           |
    | - posX, velocidade: float            |
    | - estaDormindo: bool                 |
    +--------------------------------------+
    | + Mascote(n, esp)                    |
    | + getters de atributos...            |
    | + alternarSono(): void               |
    | + atualizarMovimento(): void         |
    | + alimentar(comida, log): void       |
    | + brincar(log): void                 |
    | + /getIconeFeliz()/: wstring = 0     |
    | + /getComidaCerta()/: wstring = 0    |
    +--------------------------------------+
         ^             ^             ^
         | (Herança)   | (Herança)   | (Herança)
  +-----------+  +-----------+  +-----------+
  | Cachorro  |  |   Gato    |  |  Coelho   |
  +-----------+  +-----------+  +-----------+

```

---

## 🎮 4. Regras de Negócio e Ciclo de Vida

O ciclo de simulação obedece a regras de transição estritas baseadas nas ações do usuário coletadas pelo painel UI.

```
       [ Seleção de Espécie ] 
                 │
                 ▼
       [ Digitação do Nome ] 
                 │
                 ▼
 ┌────────►[ Execução do Jogo ]◄─────────┐
 │               │                       │
 │               ├───────────────────────┼──────────────────────┐
 │               ▼                       ▼                      ▼
 │         (Ação: Brincar)      (Ação: Alimentar)        (Ação: Dormir)
 │         - Gasta Energia       - Alimento Certo:        - Restaura Energia
 │         - Aumenta Felicidade    + Fome / + Felicidade  - Reduz Fome (Acorda)
 │         - Se exausto:         - Alimento Errado:       - Avança Idade (+1 Ano)
 │           Perde Saúde (-1)      Passa mal (-1 HP)            │
 │               │                       │                      │
 └───────────────┴───────────────────────┴──────────────────────┘
                 │
                 ▼ (Verificação de Condição de Fim)
   ┌─────────────────────────────┼──────────────────────────────┐
   ▼                             ▼                              ▼
[ Saúde <= 0 ]            [ Felicidade <= 0 ]             [ Idade >= 5 ]
  Estado: Faleceu           Estado: Fugiu de Casa           Estado: Sucesso! vPet
 (Lápide R.I.P)             (Abandono)                      Cresceu Saudável

```

---

## 🛠️ 5. Qualidade do Código e Decisões Modernas de C++

### Gerenciamento Seguro de Memória (`std::unique_ptr`)

Nas abordagens clássicas de C++, a troca de mascotes exigiria alocações brutas com `new` e desalocações manuais com `delete`, propensas a vazamentos caso uma exceção fosse lançada. Este projeto mitiga completamente esses problemas adotando semântica de movimentação de escopo seguro:

```cpp
// Instanciação polimórfica auto-gerenciada
std::unique_ptr<Mascote> pet = nullptr;
if (especieEscolhida == 1) pet = std::make_unique<Cachorro>(nomeDigitado);

```

Ao sair do loop ou fechar a janela, o destrutor de `std::unique_ptr` desaloca a memória na Heap de forma totalmente automática.

### Componentização de Interface Gráfica

Para evitar que a rotina principal `main.cpp` ficasse poluída com checagens repetitivas de geometria e posicionamento de tela, a lógica de interação do usuário foi isolada dentro do tipo `Botao`. O método `foiClicado(sf::Vector2f)` encapsula testes de colisão e estados de ativação em um único ponto reutilizável.

### Tipagem Forte com Máquinas de Estado (`enum class`)

A lógica de transição de telas foi blindada usando um enumerador fortemente tipado (`enum class EstadoJogo`). Isso remove totalmente os chamados "números mágicos" (`estado == 1`, `estado == 2`), evitando erros de atribuição indesejada de tipos inteiros puros em tempo de compilação.

---

## 🚀 6. Como Compilar e Executar

### Pré-requisitos

* Compilador GCC/G++ configurado com suporte para **C++17** ou superior.
* Biblioteca **SFML 3.0+** devidamente instalada no sistema (pastas `/include` e `/lib` mapeadas nas variáveis de ambiente).

### Comando de Compilação (Via Terminal/GCC)

Navegue até a pasta raiz onde estão situados os arquivos fontes e execute:

```bash
g++ -std=c++17 main.cpp Mascote.cpp -o vpet'.exe -lsfml-graphics -lsfml-window -lsfml-system

```

### Execução

Certifique-se de que o arquivo de asset `font.ttf` esteja localizado no mesmo diretório de trabalho do executável gerado.

```bash
./vpet.exe

```