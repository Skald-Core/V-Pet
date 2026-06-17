#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>   // Necessário para std::unique_ptr e std::make_unique

#include "Inventario.h"
#include "Mascote.h"
#include "Botao.h"

// Enum strongly typed para melhor legibilidade da máquina de estados do jogo
enum class EstadoJogo {
    SelecaoEspecie,
    DigitandoNome,
    ExecutandoJogo,
    FimDeJogo
};

std::wstring capturarNomePelaTela(sf::RenderWindow& window, const sf::Font& font) {
    std::wstring nomeBuffer = L"";
    
    sf::Text txtTitulo(font, L"Digite o nome do seu VPet:", 24);
    txtTitulo.setFillColor(sf::Color::White);
    txtTitulo.setPosition({50.f, 150.f});

    sf::Text txtNomeDigitado(font, L"_", 30);
    txtNomeDigitado.setFillColor(sf::Color(255, 204, 0));
    txtNomeDigitado.setPosition({50.f, 220.f});

    sf::Text txtAviso(font, L"Pressione ENTER para confirmar", 14);
    txtAviso.setFillColor(sf::Color(180, 180, 180));
    txtAviso.setPosition({50.f, 300.f});

    bool digitando = true;

    while (digitando && window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                digitando = false;
            }
            else if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                if (textEvent->unicode == 13 || textEvent->unicode == '\r') {
                    if (!nomeBuffer.empty()) digitando = false;
                }
                else if (textEvent->unicode == 8) {
                    if (!nomeBuffer.empty()) nomeBuffer.pop_back();
                }
                else if (textEvent->unicode >= 32 && nomeBuffer.size() < 15) {
                    nomeBuffer += static_cast<wchar_t>(textEvent->unicode);
                }
            }
        }

        txtNomeDigitado.setString(nomeBuffer + L"_");
        window.clear(sf::Color(30, 30, 40));
        window.draw(txtTitulo);
        window.draw(txtNomeDigitado);
        window.draw(txtAviso);
        window.display();
    }
    return nomeBuffer;
}

void desenharBarra(sf::RenderWindow& window, const sf::Font& font, const std::wstring& label, int atual, int maximo, float x, float y, sf::Color cor) {
    sf::Text txtLabel(font, label, 14);
    txtLabel.setFillColor(sf::Color::White);
    txtLabel.setPosition({x, y});
    window.draw(txtLabel);

    sf::RectangleShape fundoBarra({120.f, 14.f});
    fundoBarra.setFillColor(sf::Color(50, 50, 60));
    fundoBarra.setPosition({x + 90.f, y + 4.f});
    window.draw(fundoBarra);

    if (atual > 0) {
        float pct = static_cast<float>(atual) / maximo;
        sf::RectangleShape preenchimento({120.f * pct, 14.f});
        preenchimento.setFillColor(cor);
        preenchimento.setPosition({x + 90.f, y + 4.f});
        window.draw(preenchimento);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode({600, 540}), "VPet Retro - POO Master Edition");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        std::wcout << L"Erro ao carregar font.ttf!\n";
        return -1;
    }

    sf::RectangleShape painelHUD({560.f, 150.f});
    painelHUD.setFillColor(sf::Color(20, 20, 28));
    painelHUD.setOutlineThickness(2.f);
    painelHUD.setOutlineColor(sf::Color(80, 80, 100));
    painelHUD.setPosition({20.f, 15.f});

    sf::RectangleShape painelLog({560.f, 40.f});
    painelLog.setFillColor(sf::Color(15, 35, 45));
    painelLog.setPosition({20.f, 485.f});

    Inventario<ItemComida> menuComidas;
    menuComidas.adicionar(ItemComida(L"Carne", L"Carne"));
    menuComidas.adicionar(ItemComida(L"Peixe", L"Peixe"));
    menuComidas.adicionar(ItemComida(L"Cenoura", L"Cenoura"));

    EstadoJogo estadoJogo = EstadoJogo::SelecaoEspecie;
    
    int especieEscolhida = 0; 
    std::unique_ptr<Mascote> pet = nullptr; // Modern C++: Gerenciamento automático de memória
    
    std::wstring logTexto = L"Escolha uma especie para comecar.";
    bool subMenuAlimentar = false;

    sf::Text txtNomeEspecie(font, L"", 16); txtNomeEspecie.setFillColor(sf::Color::Yellow); txtNomeEspecie.setPosition({35.f, 25.f});
    sf::Text txtIdade(font, L"", 14); txtIdade.setFillColor(sf::Color(200, 200, 200)); txtIdade.setPosition({35.f, 50.f});
    sf::Text txtArt(font, L"", 48); txtArt.setFillColor(sf::Color(255, 215, 0)); txtArt.setPosition({300.f, 240.f});
    sf::Text txtLog(font, L"", 14); txtLog.setFillColor(sf::Color(150, 220, 255)); txtLog.setPosition({35.f, 495.f});

    Botao btnSair(font);
    btnSair.shape.setSize({180.f, 45.f}); btnSair.shape.setFillColor(sf::Color(180, 45, 45)); 
    btnSair.shape.setPosition({210.f, 380.f});
    btnSair.texto.setString(L"Fechar Jogo"); btnSair.texto.setCharacterSize(16);
    btnSair.texto.setFillColor(sf::Color::White); btnSair.texto.setPosition({255.f, 392.f});

    std::vector<Botao> botoesSelecao;
    std::vector<std::wstring> nomesEspecies = {L"Cachorro", L"Gato", L"Coelho"};
    for (int i = 0; i < 3; ++i) {
        Botao b(font);
        b.shape.setSize({150.f, 50.f}); b.shape.setFillColor(sf::Color(60, 60, 80));
        b.shape.setPosition({45.f + i * 175.f, 240.f});
        b.texto.setString(nomesEspecies[i]); b.texto.setCharacterSize(16);
        b.texto.setFillColor(sf::Color::White); b.texto.setPosition({75.f + i * 175.f, 253.f});
        b.idacao = i + 1;
        botoesSelecao.push_back(b);
    }

    std::vector<Botao> botoesJogo;
    std::vector<std::wstring> acoesPrincipais = {L"Alimentar", L"Brincar", L"Dormir/Acordar"};
    for (int i = 0; i < 3; ++i) {
        Botao b(font);
        b.shape.setSize({160.f, 45.f}); b.shape.setFillColor(sf::Color(45, 90, 130));
        b.shape.setPosition({35.f + i * 185.f, 360.f});
        b.texto.setString(acoesPrincipais[i]); b.texto.setCharacterSize(15);
        b.texto.setFillColor(sf::Color::White); b.texto.setPosition({55.f + i * 185.f, 372.f});
        b.idacao = i + 1;
        botoesJogo.push_back(b);
    }

    std::vector<Botao> botoesComida;
    std::vector<std::wstring> comidas = {L"Carne", L"Peixe", L"Cenoura", L"Voltar"};
    for (int i = 0; i < 4; ++i) {
        Botao b(font);
        b.shape.setSize({120.f, 40.f});
        b.shape.setFillColor(i == 3 ? sf::Color(100, 100, 110) : sf::Color(40, 120, 80));
        b.shape.setPosition({40.f + i * 135.f, 425.f});
        b.texto.setString(comidas[i]); b.texto.setCharacterSize(14);
        b.texto.setFillColor(sf::Color::White); b.texto.setPosition({65.f + i * 135.f, 435.f});
        b.idacao = i + 1;
        botoesComida.push_back(b);
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

                    if (estadoJogo == EstadoJogo::SelecaoEspecie) {
                        for (auto& b : botoesSelecao) {
                            if (b.foiClicado(mousePos)) {
                                especieEscolhida = b.idacao;
                                estadoJogo = EstadoJogo::DigitandoNome; 
                                break;
                            }
                        }
                    }
                    else if (estadoJogo == EstadoJogo::ExecutandoJogo && pet) {
                        if (!subMenuAlimentar) {
                            for (auto& b : botoesJogo) {
                                if (b.foiClicado(mousePos)) {
                                    if (b.idacao == 1) {
                                        subMenuAlimentar = true;
                                    } else if (b.idacao == 2) {
                                        pet->brincar(logTexto);
                                    } else if (b.idacao == 3) {
                                        pet->alternarSono();
                                        if (pet->getEstaDormindo()) {
                                            logTexto = pet->getNome() + L" foi dormir e está recuperando energias.";
                                            pet->recuperarEnergiaEIdade();
                                        } else {
                                            logTexto = pet->getNome() + L" acordou renovado!";
                                        }
                                    }
                                    break;
                                }
                            }
                        } else {
                            for (auto& b : botoesComida) {
                                if (b.foiClicado(mousePos)) {
                                    if (b.idacao >= 1 && b.idacao <= 3) {
                                        pet->alimentar(menuComidas[b.idacao - 1], logTexto);
                                    }
                                    subMenuAlimentar = false; 
                                    break;
                                }
                            }
                        }
                    }
                    else if (estadoJogo == EstadoJogo::FimDeJogo) {
                        if (btnSair.foiClicado(mousePos)) {
                            window.close();
                        }
                    }
                }
            }
        }

        if (estadoJogo == EstadoJogo::DigitandoNome) {
            std::wstring nomeDigitado = capturarNomePelaTela(window, font);
            if (especieEscolhida == 1) pet = std::make_unique<Cachorro>(nomeDigitado);
            else if (especieEscolhida == 2) pet = std::make_unique<Gato>(nomeDigitado);
            else pet = std::make_unique<Coelho>(nomeDigitado);

            logTexto = pet->getNome() + L" nasceu!";
            estadoJogo = EstadoJogo::ExecutandoJogo; 
            continue; 
        }

        if (estadoJogo == EstadoJogo::ExecutandoJogo && pet) {
            pet->atualizarMovimento();

            if (pet->getSaude() <= 0) {
                logTexto = L"[FIM DE JOGO] " + pet->getNome() + L" faleceu por falta de cuidados.";
                estadoJogo = EstadoJogo::FimDeJogo;
            } else if (pet->getFelicidade() <= 0) {
                logTexto = L"[FIM DE JOGO] " + pet->getNome() + L" fugiu de casa!";
                estadoJogo = EstadoJogo::FimDeJogo;
            } else if (pet->getIdade() >= 5) {
                logTexto = L"[JORNADA CONCLUÍDA] " + pet->getNome() + L" virou um adulto saudável!";
                estadoJogo = EstadoJogo::FimDeJogo;
            }
        }

        window.clear(sf::Color(33, 33, 44));

        if (estadoJogo == EstadoJogo::SelecaoEspecie) {
            sf::Text txtIntro(font, L"Escolha a especie do seu companheiro:", 18);
            txtIntro.setFillColor(sf::Color::White); txtIntro.setPosition({40.f, 140.f});
            window.draw(txtIntro);
            for (auto& b : botoesSelecao) {
                b.desenhar(window);
            }
        }
        else if (estadoJogo == EstadoJogo::ExecutandoJogo || estadoJogo == EstadoJogo::FimDeJogo) {
            window.draw(painelHUD);

            txtNomeEspecie.setString(pet->getNome() + L" (" + (pet->getEstaDormindo() ? L"Dormindo" : L"Ativo") + L")");
            txtIdade.setString(L"Idade: " + std::to_wstring(pet->getIdade()) + L" / 5 anos");
            window.draw(txtNomeEspecie);
            window.draw(txtIdade);

            desenharBarra(window, font, L"SAUDE [HP]", pet->getSaude(), 4, 280.f, 25.f, sf::Color(220, 50, 50));
            desenharBarra(window, font, L"FOME [ALIM]", pet->getSaciedade(), 4, 280.f, 55.f, sf::Color(230, 140, 10));
            desenharBarra(window, font, L"FELICIDADE", pet->getFelicidade(), 10, 280.f, 85.f, sf::Color(50, 190, 50));
            desenharBarra(window, font, L"ENERGIA", pet->getEnergia(), 5, 280.f, 115.f, sf::Color(240, 220, 20));

            if (estadoJogo == EstadoJogo::ExecutandoJogo) {
                txtArt.setString(pet->getArtAtual());
                txtArt.setPosition({pet->getPosX(), 240.f});
                sf::FloatRect bounds = txtArt.getLocalBounds();
                txtArt.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

                if (pet->getVelocidade() < 0 && !pet->getEstaDormindo()) txtArt.setScale({-1.f, 1.f});
                else txtArt.setScale({1.f, 1.f});

                window.draw(txtArt);

                for (auto& b : botoesJogo) {
                    b.desenhar(window);
                }

                if (subMenuAlimentar) {
                    for (auto& b : botoesComida) {
                        b.desenhar(window);
                    }
                }
            }
            else if (estadoJogo == EstadoJogo::FimDeJogo) {
                sf::Text txtFinal(font, L"", 20);
                txtFinal.setFillColor(sf::Color(160, 160, 160));
                txtFinal.setPosition({200.f, 210.f});

                if (pet && pet->getSaude() <= 0) {
                    txtFinal.setString(L"     .-------.\n    /   RIP   \\\n   /           \\\n   |   " + pet->getNome() + L"   |\n   |           |\n   =============");
                } else if (pet && pet->getFelicidade() <= 0) {
                    txtFinal.setString(L"   Casa Vazia\n   Seu pet partiu.");
                } else {
                    txtFinal.setString(L"   Parabéns!\n Seu pet cresceu feliz.");
                    txtFinal.setFillColor(sf::Color::Yellow);
                }
                window.draw(txtFinal);
                btnSair.desenhar(window);
            }
        }

        window.draw(painelLog);
        txtLog.setString(logTexto);
        window.draw(txtLog);
        
        window.display();
    }

    // Não há mais necessidade de "delete pet;", o smart pointer lida com isso de forma totalmente segura.
    return 0;
}