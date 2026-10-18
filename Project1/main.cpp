#include <iostream>
#include "Lista1/Lista1.h"
#include "Lista2/Lista2.h"
#include "Lista3/Lista3.h"

int main() {
    int option;
    while (true) {
        std::cout << "\nMenu Principal:\n1. Lista 1\n2. Lista 2\n3. Lista 3\n4. Sair\nOpcao: ";
        std::cin >> option;
        
        if (option == 1) {
            // Menu Lista 1
        } else if (option == 2) {
            // Menu Lista 2
        } else if (option == 3) {
            Lista3::run();
        } else if (option == 4) {
            break;
        }
    }
    return 0;
}
