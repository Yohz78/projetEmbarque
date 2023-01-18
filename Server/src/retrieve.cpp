#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <jsoncpp/json/json.h>

std::vector<Json::value> retrieve_data_from_master(){
    // Create an io_context object to manage the network connection
    boost::asio::io_context io_context;

    // Create an endpoint to represent the server's address
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("57.128.34.47"), 22);

    // Create a socket and bind it to the endpoint
    boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);

    // Start listening for incoming connections
    acceptor.listen();

    // Accept an incoming connection
    boost::asio::ip::tcp::socket socket(io_context);
    acceptor.accept(socket);

    // Read the data from the socket
    std::string data;
    read(socket, buffer(data));

    // Deserialize the vector
    std::vector<Json::Value> root;
    std::stringstream ss(data);
    boost::archive::text_iarchive ia(ss);
    ia >> root;

    return root;
}
