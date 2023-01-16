#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

int main() {
    // Exemple de données JSON sous forme de chaîne
    std::string json_data = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";

    // Création d'un flux de données à partir de la chaîne JSON
    std::stringstream json_stream(json_data);

    // Création d'un arbre de propriétés pour stocker les données JSON
    boost::property_tree::ptree pt;

    // Lecture des données JSON à partir du flux de données
    boost::property_tree::read_json(json_stream, pt);

    try {
        // Ouverture ou création de la base de données
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        // Création de la table si elle n'existe pas
        db.exec("CREATE TABLE IF NOT EXISTS data (name TEXT, age INTEGER, city TEXT)");

        // Préparation de la requête d'insertion
        SQLite::Statement insert(db, "INSERT INTO data (name, age, city) VALUES (?, ?, ?)");

        // Insertion des données JSON dans la table
        insert.bind(1, pt.get<std::string>("name"));
        insert.bind(2, pt.get<int>("age"));
        insert.bind(3, pt.get<std::string>("city"));
        insert.exec();

        std::cout << "Données insérées dans la base de données" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'insertion des données dans la base de données: " << e.what() << std::endl;
    }
    return 0;
}
