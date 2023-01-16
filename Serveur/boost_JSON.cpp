#include <boost>
#include <sstream>
#include <iostream>

int main() {
    // Exemple de données JSON sous forme de chaîne
    std::string json_data = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";

    // Création d'un flux de données à partir de la chaîne JSON
    std::stringstream json_stream(json_data);

    // Création d'un arbre de propriétés pour stocker les données JSON
    boost::property_tree::ptree pt;

    // Lecture des données JSON à partir du flux de données
    boost::property_tree::read_json(json_stream, pt);

    // Affichage des données en utilisant un itérateur
    for (const auto& item : pt) {
        std::cout << item.first << ": " << item.second.get_value<std::string>() << std::endl;
    }

    return 0;
}
