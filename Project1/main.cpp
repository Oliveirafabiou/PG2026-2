#include <iostream>
#include "Lista1/Lista1.h"
#include "Lista2/Lista2.h"
#include "Lista3/Lista3.h"

int main() {
    int option;
    while (true) {
        std::cout << "\nMenu Principal:\n";
        std::cout << "1. Lista 1\n";
        std::cout << "2. Lista 2\n";
        std::cout << "3. Lista 3\n";
        std::cout << "4. Sair\n";
        std::cout << "Opcao: ";
        std::cin >> option;

        if (option == 1) {
            Lista1::run();
        } else if (option == 2) {
            Lista2::run();
        } else if (option == 3) {
            Lista3::run();
        } else if (option == 4) {
            break;
        } else {
            std::cout << "Opcao invalida! Tente novamente.\n";
        }
    }
    return 0;
}