#ifndef BOTAO_H
#define BOTAO_H

#include <SFML/Graphics.hpp>

struct Botao {
    sf::RectangleShape shape;
    sf::Text texto;
    int idacao;
    bool ativo;

    // Correção: Inicializando explicitamente o sf::Text com a fonte exigida pela SFML 3.0
    Botao(const sf::Font& font) : texto(font), idacao(0), ativo(true) {}

    void desenhar(sf::RenderWindow& window) {
        if (!ativo) return;
        window.draw(shape);
        window.draw(texto);
    }

    bool foiClicado(sf::Vector2f mousePos) const {
        return ativo && shape.getGlobalBounds().contains(mousePos);
    }
};

#endif