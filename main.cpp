#include <iostream>
#include <string>

int main() {
    std::string nom;
    std::cout << "Quel est ton nom ? ";
    std::getline(std::cin, nom);
    
    std::cout << "Bonjour " << nom << " ! Tu compiles en C++." << std::endl;
    
    return 0;
}