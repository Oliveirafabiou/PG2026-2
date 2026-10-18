#include <iostream>
#include "Lista1.h"

namespace Lista1 {
    void run() {
        int option;
        while (true) {
            std::cout << "\n--- Submenu Lista 1 ---\n";
            std::cout << "1. Exercício 1 (Triângulos e Modos de Desenho)\n";
            std::cout << "2. Exercício 2 (Geometrias Paramétricas)\n";
            std::cout << "3. Voltar ao Menu Principal\n";
            std::cout << "Opção: ";
            std::cin >> option;

            if (option == 1) {
                runLista1Ex1();
            } else if (option == 2) {
                runLista1Ex2();
            } else if (option == 3) {
                break;
            } else {
                std::cout << "Opção inválida! Tente novamente.\n";
            }
        }
    }
}