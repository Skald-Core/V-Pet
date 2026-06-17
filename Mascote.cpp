#include "Mascote.h"
#include <algorithm>

Mascote::Mascote(std::wstring n, std::wstring esp) 
    : nome(n), especie(esp), idade(0), saciedade(2), felicidade(5), saude(4), energia(5), posX(200.f), velocidade(1.5f), estaDormindo(false) {}

std::wstring Mascote::getArtAtual() const {
    if (estaDormindo) return getIconeDormir();
    if (felicidade >= 7 && saude > 1) return getIconeFeliz();
    if (felicidade <= 3 || saude <= 1) return getIconeTriste();
    return getIconeNeutro();
}

void Mascote::alternarSono() {
    estaDormindo = !estaDormindo;
}

void Mascote::setEstaDormindo(bool valor) {
    estaDormindo = valor;
}

void Mascote::atualizarMovimento() {
    if (estaDormindo) return;

    posX += velocidade;

    if (posX > 420.f) {
        posX = 420.f;
        velocidade = -velocidade; 
    }
    else if (posX < 100.f) {
        posX = 100.f;
        velocidade = -velocidade; 
    }
}

void Mascote::envelhecer() {
    if (idade < 5) {
        idade++;
    }
}

void Mascote::gastarEnergia() {
    if (energia > 0) {
        energia--;
    } else {
        felicidade = std::max(0, felicidade - 1);
        saude = std::max(0, saude - 1);
    }
}

void Mascote::recuperarEnergiaEIdade() {
    energia = 5;
    saciedade = 1;
    envelhecer();
}

void Mascote::alimentar(const ItemComida& comida, std::wstring& msgLog) {
    if (estaDormindo) {
        msgLog = nome + L" esta dormindo e nao pode comer agora!";
        return;
    }
    gastarEnergia();
    if (comida.getNome() == getComidaCerta()) {
        saciedade = std::min(4, saciedade + 1);
        felicidade = std::min(10, felicidade + 1);
        msgLog = nome + L" adorou o(a) " + comida.getNome() + L"!";
    } else {
        felicidade = std::max(0, felicidade - 2);
        saude = std::max(0, saude - 1); // Saúde cai e não pode ser recuperada
        msgLog = nome + L" nao gosta de " + comida.getNome() + L" e passou mal!";
    }
}

void Mascote::brincar(std::wstring& msgLog) {
    if (estaDormindo) {
        msgLog = nome + L" esta dormindo e nao quer brincar!";
        return;
    }
    gastarEnergia();
    if (energia == 0 || saciedade == 0) {
        felicidade = std::max(0, felicidade - 2);
        saude = std::max(0, saude - 1);
        msgLog = nome + L" estava com fome/cansado e odiou brincar!";
    } else {
        felicidade = std::min(10, felicidade + 2);
        saciedade--;
        if (rand() % 3 == 0) {
            energia = energia - 2;
            msgLog = nome + L" gastou toda a energia brincando!";
        } else {
            msgLog = nome + L" se divertiu muito!";
        }
    }
}