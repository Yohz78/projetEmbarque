#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>
#include "src/pca/pca.h"
#include <sstream>
#include <cstring>
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <jsoncpp/json/json.h>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>



#define INTERVALLE_RECUP 5
#define INTERVALLE_ENVOI_SERVEUR 60

using namespace boost::asio;

/**
 * @brief This function read the sensor data from the slave over TX/RX communication
 * 
 * @param fd 
 * @return std::string 
 */
std::string read_sensor_data(int fd) {
    PCA9685 pca(1,0x40);
    pca.init();
    while (true) {
        char data[10000];
        int index = 0; 
        while (serialDataAvail(fd) > 0) {
            int c = serialGetchar(fd);
            if (c < 0) {
                std::cout << "Error: Unable to receive data over UART" << std::endl;
            }
            data[index] = c;
            index++;
        }
        data[index] = '\0';
        //std::cout << "Donnée reçue"<< data << std::endl;
        return data;
        sleep(1); // Fait une pause pendant interval secondes
    }
}

/**
 * @brief This function parse the data from the read_sensor_data(fd) function and move the 
 * Yellow flag accordingly. Then it writes the data into the file data.json
 * 
 * @param fd 
 */
void read_and_write(int fd, std::vector<Json::Value> &res){
        PCA9685 pca(1,0x40);
        pca.init();
        int i = 0;
        int pos_tracker = 0;
        while (true) {
        std::string string_data = read_sensor_data(fd);
        if(!string_data.empty()){
            std::istringstream json_stream(string_data);
            Json::Value root;
            Json::CharReaderBuilder builder;
            JSONCPP_STRING errs;
            bool parsingSuccessful = Json::parseFromStream(builder,
                                                       json_stream,
                                                       &root,
                                                       &errs);
            if (!parsingSuccessful) {
                std::cout << "Error parsing JSON" << std::endl;
            }
            //double presence = root["mvt"].asDouble();
            std::cout << root << std::endl;
            if(pos_tracker==0){
                pca.moveBlueFlag(45);
                pos_tracker=2;
            }
            if(pos_tracker==2){
                pca.moveBlueFlag(135);
                pos_tracker=3;
            }
            if(pos_tracker==3){
                pca.moveBlueFlag(45);
                pos_tracker=2;
            }
            std::cout << "Data received and treated"<< std::endl;
            res.push_back(root);
            std::cout << res[i] << std::endl;
            i++;
        }else{
            std::cout << "No data available, Blue flag back to rest position." << std::endl;
            pca.moveBlueFlag(180);
            pos_tracker=0;
        } 
        sleep(INTERVALLE_RECUP);
        }
        pthread_exit(NULL);
}

/**
 * @brief This function transfer the flux of data to the server located at ubuntu@57.128.34.47
 * 
 */
void send_boost(std::vector<Json::Value> &res){

    // Create an io_context object to manage the network connection
        boost::asio::io_context io_context;
    
    // Create a ssl context
        ssl::context ctx{ssl::context::sslv23};
    
    // Set the private key file
        ctx.use_private_key_file("opom__227__0_", ssl::context::pem);
        
    // Create an endpoint to represent the server's address
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("57.128.34.47"), 22);
    
    // Create a ssl socket and connect it to the endpoint
        ssl::stream<ip::tcp::socket> socket(io_context, ctx);
        socket.lowest_layer().connect(endpoint);
    
    // Perform the SSL/TLS handshake
        socket.handshake(ssl::stream_base::client);

    // Serialize the vector
        // std::stringstream ss;
        // boost::archive::text_oarchive oa(ss);
        // oa << res;
        // std::string data = ss.str();

    // Send the serialized data to the server
        write(socket, buffer(res));

    // Close the socket
        socket.shutdown();
        socket.lowest_layer().close();
    // Clean the vector 
    res.clear();
}

int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    vector<Json::Value> res;
    read_and_write(fd,res);
    
    /*pthread_t thread_rw, thread_send;

    pthread_create(&thread_rw, NULL, read_and_write, &fd, &res);
    pthread_create(&thread_send, NULL,send_boost, &fd, &res);
    pthread_join(thread_rw, NULL);
    pthread_join(thread_send, NULL);*/ 

    // C  est ici que l'on va avoir besoin des threads, un qui recupere de la data et un qui envoie, les deux a differents intervalles
    //read_and_write(fd, res);

    serialClose(fd); // Ferme le port série
    return 0;
}
