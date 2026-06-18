# 🐾 VPet — Enhanced OOP Edition

## 📖 Sobre o Projeto

**VPet** é um simulador de animal de estimação virtual inspirado nos clássicos dispositivos Tamagotchi da década de 1990. O projeto foi desenvolvido em **C++ moderno**, utilizando a biblioteca gráfica **SFML 3.0+**, com foco na aplicação de conceitos avançados de **Programação Orientada a Objetos (POO)**, arquitetura modular e boas práticas de desenvolvimento de software.

O objetivo principal é demonstrar a construção de um sistema interativo completo empregando abstração, encapsulamento, herança, polimorfismo, programação genérica e gerenciamento seguro de memória.

---

# 🎯 Objetivos do Projeto

O desenvolvimento do VPet foi orientado pelos seguintes princípios:

* Aplicar **encapsulamento rigoroso**, evitando acesso direto aos atributos internos dos objetos.
* Implementar **polimorfismo dinâmico** por meio de uma hierarquia de classes baseada em abstração.
* Utilizar recursos de **C++ Moderno**, eliminando vazamentos de memória através de `std::unique_ptr`.
* Construir componentes reutilizáveis utilizando **templates**.
* Organizar o sistema em módulos independentes e de fácil manutenção.
* Desenvolver uma interface gráfica simples e intuitiva utilizando SFML.

---

# 🏗️ Arquitetura do Sistema

O projeto está organizado em módulos independentes que representam responsabilidades específicas dentro da aplicação.

```text
📦 VPet
 ┣ 📜 main.cpp
 ┣ 📜 Mascote.h
 ┣ 📜 Mascote.cpp
 ┣ 📜 Inventario.h
 ┣ 📜 Botao.h
 ┗ 📜 font.ttf
```

### Responsabilidades dos Módulos

| Arquivo          | Responsabilidade                                        |
| ---------------- | ------------------------------------------------------- |
| `main.cpp`       | Loop principal, gerenciamento de estados e renderização |
| `Mascote.h/.cpp` | Modelo de domínio dos mascotes e especializações        |
| `Inventario.h`   | Estrutura genérica para gerenciamento de itens          |
| `Botao.h`        | Componente reutilizável da interface gráfica            |
| `font.ttf`       | Recurso tipográfico da aplicação                        |

---

# 🧬 Conceitos de Programação Orientada a Objetos

## 1. Abstração

A classe `Mascote` representa uma abstração genérica de qualquer animal virtual existente no sistema.

Como cada espécie possui características próprias, a classe contém métodos virtuais puros que obrigam as subclasses a fornecerem implementações específicas.

```cpp
virtual std::wstring getIconeFeliz() const = 0;
virtual std::wstring getComidaCerta() const = 0;
```

Dessa forma, a classe não pode ser instanciada diretamente.

---

## 2. Encapsulamento

Todos os atributos internos do mascote são declarados como `private`.

```cpp
private:
    int saude;
    int felicidade;
    int energia;
    int saciedade;
```

O acesso ao estado interno ocorre exclusivamente por meio de métodos públicos controlados.

```cpp
int getSaude() const;
void alimentar(...);
void brincar(...);
```

Essa abordagem preserva a integridade dos dados e evita alterações indevidas por código externo.

---

## 3. Herança

As espécies concretas derivam da classe base `Mascote`.

```cpp
class Cachorro : public Mascote
class Gato : public Mascote
class Coelho : public Mascote
```

Cada subclasse herda os comportamentos comuns e especializa apenas os aspectos específicos de sua espécie.

---

## 4. Polimorfismo

O sistema manipula os mascotes através de referências para a classe base.

```cpp
std::unique_ptr<Mascote> pet;
```

Durante a execução, chamadas como:

```cpp
pet->getArtAtual();
```

são resolvidas dinamicamente conforme o tipo real do objeto instanciado.

Esse mecanismo permite adicionar novas espécies sem alterar o código principal do jogo.

---

## 5. Programação Genérica (Templates)

O inventário foi implementado utilizando templates.

```cpp
template<typename T>
class Inventario
```

Atualmente ele armazena objetos do tipo `ItemComida`, porém pode ser reutilizado para qualquer outro tipo sem modificações internas.

Exemplos futuros:

* `Inventario<ItemBrinquedo>`
* `Inventario<Medicamento>`
* `Inventario<Acessorio>`

---

# 📊 Diagrama UML Simplificado

```text
                    +-------------------+
                    |   Inventario<T>   |
                    +-------------------+
                    | vector<T> itens   |
                    +-------------------+
                              |
                              |
                              ▼
                    +-------------------+
                    |    ItemComida     |
                    +-------------------+
                    | nome              |
                    | icone             |
                    +-------------------+

+---------------------------------------------------+
|                     Mascote                       |
+---------------------------------------------------+
| nome, especie                                     |
| idade, saude, felicidade                          |
| energia, saciedade                                |
| posX, velocidade                                  |
| estaDormindo                                      |
+---------------------------------------------------+
| alimentar()                                       |
| brincar()                                         |
| atualizarMovimento()                              |
| alternarSono()                                    |
| getIconeFeliz() = 0                               |
| getComidaCerta() = 0                              |
+---------------------------------------------------+
           ▲                 ▲                 ▲
           │                 │                 │
    +-----------+     +-----------+     +-----------+
    | Cachorro  |     |   Gato    |     |  Coelho   |
    +-----------+     +-----------+     +-----------+

                    +-------------------+
                    |       Botao       |
                    +-------------------+
                    | shape             |
                    | texto             |
                    | ativo             |
                    +-------------------+
                    | desenhar()        |
                    | foiClicado()      |
                    +-------------------+
```

---

# 🎮 Regras de Negócio

O ciclo de vida do mascote é controlado pelas ações realizadas pelo usuário.

## Alimentar

### Alimento correto

* Aumenta a saciedade
* Aumenta a felicidade

### Alimento incorreto

* Reduz a saúde do mascote

---

## Brincar

* Aumenta a felicidade
* Consome energia

Caso a energia esteja muito baixa:

* O mascote perde saúde por exaustão

---

## Dormir

* Recupera energia
* Avança o tempo de vida do mascote
* Pode reduzir a fome ao acordar

---

# 🔄 Fluxo de Execução

```text
Escolha da Espécie
          │
          ▼
   Digitação do Nome
          │
          ▼
      Início do Jogo
          │
          ├── Alimentar
          ├── Brincar
          └── Dormir
          │
          ▼
 Verificação das Condições Finais
```

---

## Condições de Encerramento

### ☠️ Faleceu

```text
Saúde <= 0
```

O mascote morre devido ao estado crítico.

---

### 🚪 Fugiu de Casa

```text
Felicidade <= 0
```

O mascote abandona o jogador.

---

### 🏆 Sucesso

```text
Idade >= 5
```

O mascote alcança a fase adulta de forma saudável.

---

# 🛠️ Boas Práticas e Recursos Modernos de C++

## Gerenciamento Seguro de Memória

O projeto utiliza ponteiros inteligentes para evitar vazamentos de memória.

```cpp
std::unique_ptr<Mascote> pet;

pet = std::make_unique<Cachorro>(nomeDigitado);
```

A desalocação ocorre automaticamente quando o objeto sai de escopo.

---

## Componentização da Interface

A lógica de interação gráfica foi encapsulada na classe `Botao`.

```cpp
bool foiClicado(sf::Vector2f posicao);
```

Isso reduz o acoplamento entre interface e lógica de negócio.

---

## Máquina de Estados

As telas da aplicação são controladas por um enumerador fortemente tipado.

```cpp
enum class EstadoJogo
{
    Menu,
    EscolhaMascote,
    Nomeacao,
    Jogando,
    Finalizado
};
```

Benefícios:

* Eliminação de números mágicos
* Maior legibilidade
* Segurança de tipos em tempo de compilação

---

# 🚀 Compilação

## Pré-requisitos

* GCC/G++ com suporte a C++17 ou superior
* SFML 3.0+ instalada e configurada
* Arquivo `font.ttf` disponível no diretório do executável

---

## Compilando

```bash
g++ -std=c++17 main.cpp Mascote.cpp -o vpet.exe \
-lsfml-graphics \
-lsfml-window \
-lsfml-system
```

---

## Executando

Windows:

```bash
vpet.exe
```

Linux:

```bash
./vpet
```

---

# 📚 Tecnologias Utilizadas

* C++17
* SFML 3.0+
* STL (Standard Template Library)
* Smart Pointers (`std::unique_ptr`)
* Templates
* Programação Orientada a Objetos

---

# 👨‍💻 Competências Demonstradas

Este projeto evidencia domínio dos seguintes tópicos:

* Modelagem Orientada a Objetos
* Encapsulamento
* Herança
* Polimorfismo
* Classes Abstratas
* Templates
* Gerenciamento de Memória em C++ Moderno
* Arquitetura Modular
* Desenvolvimento de Interfaces com SFML
* Organização de Projetos de Software

---

## 📄 Licença

Projeto desenvolvido para fins acadêmicos e educacionais.
Sinta-se livre para estudar, modificar e expandir a aplicação.

## 🐙 GIT

[GitHub](https://github.com/Skald-Core/V-Pet)