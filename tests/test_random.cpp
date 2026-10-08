#include "random.hpp"
#include <iostream>
#include <vector>

int main() {

    if (generate_standard_normals(5, 42).size() != 5){
        std::cerr << "Echec : mauvaise taille du vecteur retourné \n" ; 
        return 1 ; 
    }

    const std::vector<double> first = generate_standard_normals(5,42) ;
    const std::vector<double> second = generate_standard_normals(5,42) ;
    if (!(first==second)){
        std::cerr << "Echec : résultat non déterministe \n" ; 
        return 1 ; 
    }  
    if (!generate_standard_normals(0, 42).empty()){
        std::cerr << "Echec : résultat de taille 0 non vide \n" ; 
        return 1 ; 
    }
    return 0; 
}
