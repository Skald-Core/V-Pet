#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Inventario.h"
#include "Mascote.h"
#include "Botao.h"

std::wstring capturarNomePelaTela(sf::RenderWindow& window, sf::Font& font) {
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

void desenharBarra(sf::RenderWindow& window, sf::Font& font, const std::wstring& label, int atual, int maximo, float x, float y, sf::Color cor) {
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
    sf::RenderWindow window(sf::VideoMode({600, 540}), "VPet Retro - Enhanced Edition");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        std::wcout << L"Erro ao carregar font.ttf!\n";
        return -1;
    }

    sf::Clock relogioGeral;

    sf::RectangleShape painelHUD({560.f, 150.f});
    painelHUD.setFillColor(sf::Color(20, 20, 28));
    painelHUD.setOutlineThickness(2.f);
    painelHUD.setOutlineColor(sf::Color(80, 80, 100));
    painelHUD.setPosition({20.f, 15.f});

    sf::RectangleShape painelLog({560.f, 40.f});
    painelLog.setFillColor(sf::Color(15, 35, 45));
    painelLog.setPosition({20.f, 485.f});

    Inventario<ItemComida> menu;
    menu.adicionar(ItemComida(L"Carne", L"Carne"));
    menu.adicionar(ItemComida(L"Peixe", L"Peixe"));
    menu.adicionar(ItemComida(L"Cenoura", L"Cenoura"));

    int estadoJogo = 0;
    int especieEscolhida = 0; 
    Mascote* pet = nullptr;
    std::wstring logTexto = L"Escolha uma especie para comecar.";
    bool subMenuAlimentar = false;

    sf::Text txtNomeEspecie(font, L"", 16); txtNomeEspecie.setFillColor(sf::Color::Yellow); txtNomeEspecie.setPosition({35.f, 25.f});
    sf::Text txtIdade(font, L"", 14); txtIdade.setFillColor(sf::Color(200, 200, 200)); txtIdade.setPosition({35.f, 50.f});
    sf::Text txtArt(font, L"", 48); txtArt.setFillColor(sf::Color(255, 215, 0)); txtArt.setPosition({300.f, 240.f});
    sf::Text txtLog(font, L"", 14); txtLog.setFillColor(sf::Color(150, 220, 255)); txtLog.setPosition({35.f, 495.f});

    // Botão de fechar programa
    Botao btnSair(font);
    btnSair.shape.setSize({180.f, 45.f}); btnSair.shape.setFillColor(sf::Color(180, 45, 45)); 
    btnSair.shape.setPosition({210.f, 380.f});
    btnSair.texto.setString(L"Fechar Jogo"); btnSair.texto.setCharacterSize(16);
    btnSair.texto.setFillColor(sf::Color::White); btnSair.texto.setPosition({255.f, 392.f});

    // Seleção inicial
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

    // Botões Principais do Painel Inferior
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

    // Botões do Submenu de Comida (Inicialmente invisíveis)
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

                    if (estadoJogo == 0) {
                        for (auto& b : botoesSelecao) {
                            if (b.shape.getGlobalBounds().contains(mousePos)) {
                                especieEscolhida = b.idacao;
                                estadoJogo = 1; 
                                break;
                            }
                        }
                    }
                    else if (estadoJogo == 2 && pet) {
                        if (!subMenuAlimentar) {
                            for (auto& b : botoesJogo) {
                                if (b.shape.getGlobalBounds().contains(mousePos)) {
                                    if (b.idacao == 1) {
                                        subMenuAlimentar = true;
                                    } else if (b.idacao == 2) {
                                        pet->brincar(logTexto);
                                    } else if (b.idacao == 3) {
                                        pet->alternarSono();
                                        if (pet->getEstaDormindo()) {
                                            logTexto = pet->getNome() + L" foi dormir e esta recuperando energias.";
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
                                if (b.shape.getGlobalBounds().contains(mousePos)) {
                                    if (b.idacao >= 1 && b.idacao <= 3) {
                                        pet->alimentar(menu[b.idacao - 1], logTexto);
                                    }
                                    subMenuAlimentar = false; 
                                    break;
                                }
                            }
                        }
                    }
                    else if (estadoJogo == 3) {
                        if (btnSair.shape.getGlobalBounds().contains(mousePos)) {
                            window.close();
                        }
                    }
                }
            }
        }

        if (estadoJogo == 1) {
            std::wstring nomeDigitado = capturarNomePelaTela(window, font);
            if (especieEscolhida == 1) pet = new Cachorro(nomeDigitado);
            else if (especieEscolhida == 2) pet = new Gato(nomeDigitado);
            else pet = new Coelho(nomeDigitado);

            logTexto = pet->getNome() + L" nasceu!";
            estadoJogo = 2; 
            continue; 
        }

        if (estadoJogo == 2 && pet) {
            pet->atualizarMovimento();

            if (pet->getSaude() <= 0) {
                logTexto = L"[FIM DE JOGO] " + pet->getNome() + L" faleceu por falta de cuidados.";
                estadoJogo = 3;
            } else if (pet->getFelicidade() <= 0) {
                logTexto = L"[FIM DE JOGO] " + pet->getNome() + L" fugiu de casa!";
                estadoJogo = 3;
            } else if (pet->getIdade() >= 5) {
                logTexto = L"[JORNADA CONCLUIDA] " + pet->getNome() + L" virou um adulto saudável e independente!";
                estadoJogo = 3;
            }
        }

        window.clear(sf::Color(33, 33, 44));

        if (estadoJogo == 0) {
            sf::Text txtIntro(font, L"Escolha a especie do seu companheiro:", 18);
            txtIntro.setFillColor(sf::Color::White); txtIntro.setPosition({40.f, 140.f});
            window.draw(txtIntro);
            for (auto& b : botoesSelecao) {
                window.draw(b.shape);
                window.draw(b.texto);
            }
        }
        else if (estadoJogo >= 2) {
            window.draw(painelHUD);

            txtNomeEspecie.setString(pet->getNome() + L" (" + (pet->getEstaDormindo() ? L"Dormindo" : L"Ativo") + L")");
            txtIdade.setString(L"Idade: " + std::to_wstring(pet->getIdade()) + L" / 5 anos");
            window.draw(txtNomeEspecie);
            window.draw(txtIdade);

            // Barras estéticas e organizadas substituindo os emojis problemáticos por texto claro
            desenharBarra(window, font, L"SAUDE [HP]", pet->getSaude(), 4, 280.f, 25.f, sf::Color(220, 50, 50));
            desenharBarra(window, font, L"FOME [ALIM]", pet->getSaciedade(), 4, 280.f, 55.f, sf::Color(230, 140, 10));
            desenharBarra(window, font, L"FELICIDADE", pet->getFelicidade(), 10, 280.f, 85.f, sf::Color(50, 190, 50));
            desenharBarra(window, font, L"ENERGIA", pet->getEnergia(), 5, 280.f, 115.f, sf::Color(240, 220, 20));

            if (estadoJogo == 2) {
                txtArt.setString(pet->getArtAtual());
                txtArt.setPosition({pet->getPosX(), 240.f});
                sf::FloatRect bounds = txtArt.getLocalBounds();
                txtArt.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

                if (pet->getVelocidade() < 0 && !pet->getEstaDormindo()) txtArt.setScale({-1.f, 1.f});
                else txtArt.setScale({1.f, 1.f});

                window.draw(txtArt);

                for (auto& b : botoesJogo) {
                    window.draw(b.shape);
                    window.draw(b.texto);
                }

                if (subMenuAlimentar) {
                    for (auto& b : botoesComida) {
                        window.draw(b.shape);
                        window.draw(b.texto);
                    }
                }
            }
            else if (estadoJogo == 3) {
                sf::Text txtFinal(font, L"", 20);
                txtFinal.setFillColor(sf::Color(160, 160, 160));
                txtFinal.setPosition({200.f, 210.f});

                if (pet && pet->getSaude() <= 0) {
                    txtFinal.setString(L"     .-------.\n    /   RIP   \\\n   /           \\\n   |   " + pet->getNome() + L"   |\n   |           |\n   =============");
                } else if (pet && pet->getFelicidade() <= 0) {
                    txtFinal.setString(L"   Casa Vazia\n   Seu pet partiu.");
                } else {
                    txtFinal.setString(L"   Parabens!\n Seu pet cresceu feliz.");
                    txtFinal.setFillColor(sf::Color::Yellow);
                }
                window.draw(txtFinal);

                window.draw(btnSair.shape);
                window.draw(btnSair.texto);
            }
        }

        window.draw(painelLog);
        txtLog.setString(logTexto);
        window.draw(txtLog);
        
        window.display();
    }

    delete pet;
    return 0;
}