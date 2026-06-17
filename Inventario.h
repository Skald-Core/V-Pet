#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <vector>
#include <string>

template <typename T>
class Inventario {
private:
    std::vector<T> itens;
public:
    void adicionar(const T& item) {
        itens.push_back(item);
    }
    T& operator[](size_t index) {
        return itens.at(index);
    }
    size_t tamanho() const { return itens.size(); }
};

class ItemComida {
private:
    std::wstring nome;
    std::wstring icone;
public:
    ItemComida(std::wstring n, std::wstring i) : nome(n), icone(i) {}
    std::wstring getNome() const { return nome; }
    std::wstring getIcone() const { return icone; }
};

#endif