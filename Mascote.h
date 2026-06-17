#ifndef MASCOTE_H
#define MASCOTE_H

#include <string>
#include "Inventario.h"

class Mascote {
protected:
    std::wstring nome;
    std::wstring especie;
    int idade;
    int saciedade;
    int felicidade;
    int saude;
    int energia;
    
    float posX;
    float velocidade;
    bool estaDormindo; 

public:
    Mascote(std::wstring n, std::wstring esp);
    virtual ~Mascote() = default;

    std::wstring getNome() const { return nome; }
    int getIdade() const { return idade; }
    int getSaciedade() const { return saciedade; }
    int getFelicidade() const { return felicidade; }
    int getSaude() const { return saude; }
    int getEnergia() const { return energia; }
    bool getEstaDormindo() const { return estaDormindo; }
    
    float getPosX() const { return posX; }
    float getVelocidade() const { return velocidade; }

    virtual std::wstring getIconeFeliz() const = 0;
    virtual std::wstring getIconeNeutro() const = 0;
    virtual std::wstring getIconeTriste() const = 0;
    virtual std::wstring getIconeDormir() const = 0; 
    virtual std::wstring getComidaCerta() const = 0;

    std::wstring getArtAtual() const;

    void alternarSono();
    void setEstaDormindo(bool valor);
    void atualizarMovimento();
    void envelhecer();

    void gastarEnergia();
    void recuperarEnergiaEIdade();
    void alimentar(const ItemComida& comida, std::wstring& msgLog);
    void brincar(std::wstring& msgLog);
};

class Cachorro : public Mascote {
public:
    Cachorro(std::wstring n) : Mascote(n, L"Cachorro") {}
    std::wstring getIconeFeliz() const override { return L"u^___^u"; }
    std::wstring getIconeNeutro() const override { return L"u._.u"; }
    std::wstring getIconeTriste() const override { return L"uT.Tu"; }
    std::wstring getIconeDormir() const override { return L"u-.-u zZ"; }
    std::wstring getComidaCerta() const override { return L"Carne"; }
};

class Gato : public Mascote {
public:
    Gato(std::wstring n) : Mascote(n, L"Gato") {}
    std::wstring getIconeFeliz() const override { return L"(^'-'^)"; }
    std::wstring getIconeNeutro() const override { return L"(^._.^)"; }
    std::wstring getIconeTriste() const override { return L"(^T_T^)"; }
    std::wstring getIconeDormir() const override { return L"(^_-^) zZ"; }
    std::wstring getComidaCerta() const override { return L"Peixe"; }
};

class Coelho : public Mascote {
public:
    Coelho(std::wstring n) : Mascote(n, L"Coelho") {}
    std::wstring getIconeFeliz() const override { return L"/(>= x =<)\\"; }
    std::wstring getIconeNeutro() const override { return L"/(=. x .=)\\"; }
    std::wstring getIconeTriste() const override { return L"/(TxT)\\"; }
    std::wstring getIconeDormir() const override { return L"/(>.<)\\ zZ"; }
    std::wstring getComidaCerta() const override { return L"Cenoura"; }
};

#endif