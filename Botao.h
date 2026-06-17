#ifndef BOTAO_H
#define BOTAO_H

#include <SFML/Graphics.hpp>

struct Botao {
    sf::RectangleShape shape;
    sf::Text texto;
    int idacao;
    bool ativo;

    Botao(const sf::Font& font) : idacao(0), ativo(true) {
        texto.setFont(font);
    }

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