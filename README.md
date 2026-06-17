import os

# Conteúdo detalhado para o README do VPet Retro
readme_content = """# VPet Retro - Enhanced Edition 🐾

Um simulador de animal de estimação virtual (Virtual Pet) interativo e minimalista desenvolvido em **C++** utilizando a biblioteca gráfica **SFML (Simple and Fast Multimedia Library)**. O projeto adota boas práticas de **Orientação a Objetos (POO)**, herança, polimorfismo e programação genérica (templates).

---

## 🚀 Novas Funcionalidades e Melhorias Visuais

Esta versão (*Enhanced Edition*) traz uma reformulação completa na interface gráfica, usabilidade e mecânicas de jogo:

1. **Barras de Status Dinâmicas (Estética Otimizada):** - Os atributos de **Saúde (HP)**, **Fome (Alimentação)**, **Felicidade** e **Energia** foram reconstruídos como barras gráficas coloridas proporcionais (`sf::RectangleShape`).
   - Evita a dependência de suporte a glifos de emojis complexos/coloridos em fontes `.ttf` padrão, eliminando erros de renderização (*caracteres corrompidos ou invisíveis*).
2. **Submenu de Alimentação Secundário:**
   - A ação de comida foi removida do HUD principal. Agora, ao clicar em **Alimentar**, um submenu dinâmico e contextual surge na parte inferior da tela com as opções disponíveis de alimentos (**Carne**, **Peixe**, **Cenoura** e a opção de **Voltar**).
3. **Mecânica de Sono Controlada:**
   - O mascote **não dorme mais sozinho por tempo**. Ele entra em repouso estritamente quando o jogador clica na ação **Dormir/Acordar**.
   - Ao dormir, sua energia é restaurada totalmente, mas o metabolismo consome seus recursos: a **saciedade cai instantaneamente para 1**, simulando que o pet acordará faminto.
4. **Remoção de Cura de Saúde (Dificuldade Hardcore):**
   - A ação genérica de "cuidar" foi permanentemente removida. **A saúde do pet não pode ser recuperada**. Cometer erros ao alimentá-lo com a comida errada ou brincar com ele exausto causará danos irreversíveis à integridade física do mascote.

---

## 🏗️ Estrutura e Arquitetura do Projeto

O código está modularizado e estruturado em cinco arquivos principais:

* **`Inventario.h`**: Contém a classe genérica `Inventario<T>` utilizando *templates* para gerenciar coleções de itens, além da estrutura de dados encapsulated `ItemComida`.
* **`Botao.h`**: Encapsula elementos visuais e de colisão (`sf::RectangleShape` e `sf::Text`) para os botões interativos do jogo, com suporte a estados ativos e inativos.
* **`Mascote.h` / `Mascote.cpp`**: Core da lógica do jogo. Contém a classe abstrata base `Mascote` que gerencia os estados físicos, envelhecimento e movimentação. As espécies derivadas via Herança e Polimorfismo são:
    * `Cachorro`: Adora **Carne**. Representação ASCII: `u^___^u`.
    * `Gato`: Adora **Peixe**. Representação ASCII: `(^'-'^)`.
    * `Coelho`: Adora **Cenoura**. Representação ASCII: `/(>= x =<)\\`.
* **`main.cpp`**: Centraliza o loop de renderização da SFML, captura de eventos do rato/teclado, fluxo do jogo (Menu de Seleção $\\rightarrow$ Criação de Nome $\\rightarrow$ Simulação Ativa $\\rightarrow$ Game Over) e desenho dos componentes do painel HUD.

---

## 🎮 Regras do Jogo e Condições de Fim

O ciclo de vida do pet progride à medida que executa ações e dorme:
* **Idade Máxima:** O objetivo é fazer o pet atingir **5 anos** de forma saudável para concluir sua jornada.
* **Alimentação Errada:** Dar um alimento que não condiz com a espécie (ex: Peixe para o Coelho) reduz a Felicidade em 2 pontos e **reduz a Saúde em 1 ponto de forma permanente**.
* **Brincar:** Aumenta a felicidade em 2 pontos e consome 1 de energia. Se jogado com energia ou saciedade zeradas, o pet odeia a ação e perde 1 de saúde.
* **Condições de Derrota:**
    * Se a **Saúde (HP)** chegar a `0`, o pet falece (uma lápide ASCII `RIP` é renderizada).
    * Se a **Felicidade** chegar a `0`, o pet foge de casa.

---

## 🛠️ Como Compilar e Executar

### Pré-requisitos
Certifique-se de que possui o compilador GCC/G++ compatível com C++17 ou superior instalado e as dependências da biblioteca **SFML 3.0+** ou **SFML 2.6+** devidamente configuradas no seu ambiente de desenvolvimento.

O arquivo de fonte `font.ttf` deve obrigatoriamente estar presente na mesma pasta do binário executável gerado.

### Linha de Comando (GCC/MinGW)
Para compilar todos os módulos vinculando as dependências gráficas da SFML, utilize o seguinte comando no terminal:

```bash
g++ -O3 main.cpp Mascote.cpp -o VPetGame -lsfml-graphics -lsfml-window -lsfml-system