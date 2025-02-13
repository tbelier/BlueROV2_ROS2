// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include <cmath>

#include "message2/msg/usbl.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <seatrac_driver/SeatracDriver.h>
#include <seatrac_driver/messages/Messages.h>
#include <seatrac_driver/commands.h>

using namespace std::chrono_literals;
using namespace narval::seatrac;

// Cette fonctio est sensee ecrire  les data sur le log d'entree
void log_write(std::ofstream &log, messages::PingResp &data_response)
{
    log << float(data_response.acoFix.position.northing) / 10.0 << ", ";
    log << float(data_response.acoFix.position.easting) / 10.0 << ", ";
    log << float(data_response.acoFix.position.depth) / 10.0 << ", ";
    log << float(data_response.acoFix.usbl.azimuth) / 10.0 << ", ";
    log << float(data_response.acoFix.usbl.elevation) / 10.0 << ",";
    log << float(data_response.acoFix.range.dist) / 10.0 << ",";
    log << float(data_response.acoFix.depthLocal) / 10.0 << std::endl;
}

std::string current_time()
{
    time_t now = time(0);

    // convert now to string form
    char *dt = ctime(&now);

    char *test = new char[8];
    for (int i = 0; i < 8; i++)
    {
        test[i] = dt[i + 11];
    }
    std::string s(test, 8);
    return s;
}

std::string get_current_time()
{
    // Obtenir l'heure et la date actuelles
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    // Créer un flux pour formater la date et l'heure
    std::stringstream ss;
    ss << (now->tm_year + 1900)                                  // Année (aaa)
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) // Mois (mm)
       << std::setw(2) << std::setfill('0') << now->tm_mday      // Jour (dd)
       << "_"
       << std::setw(2) << std::setfill('0') << now->tm_hour // Heure (hh)
       << std::setw(2) << std::setfill('0') << now->tm_min  // Minute (minmin)
       << std::setw(2) << std::setfill('0') << now->tm_sec; // Seconde (ss)

    return ss.str();
}

/// MODIFIER TOUS LES usbl::Usbl par message2::msg::Usbl

// Cette fonction ecrit le message sous format Usbl qui sera publish
void write_message(message2::msg::Usbl &data, const narval::seatrac::messages::PingResp &data_response, rclcpp::Node &node_, int k, float &yaw0)
{
    data.header.stamp = node_.get_clock()->now(); // ros::Time::now();
    data.azimuth = float(data_response.acoFix.usbl.azimuth) / 10;
    data.elevation = float(data_response.acoFix.usbl.elevation) / 10;
    data.range = float(data_response.acoFix.range.dist) / 10;
    data.depth = float(data_response.acoFix.depthLocal) / 10;

    float az = data.azimuth * M_PI / 180;
    float el = -data.elevation * M_PI / 180;
    data.position.x = data.range * sin(az) * cos(el);
    data.position.y = data.range * sin(az) * sin(el);
    data.position.z = data.range * cos(el);
    /**
        RCLCPP_INFO(node_.get_logger(), "--- k : %d", k);
    if (k == 0)
    {
        yaw0 = float(data_response.acoFix.attitudeYaw) / 10;
        data.yaw = yaw0;
        RCLCPP_INFO(node_.get_logger(), "--- yaw0 : %f", yaw0);
    }
    else
    {
        float dyaw = float(data_response.acoFix.attitudeYaw) / 10 - yaw0;
        data.yaw += dyaw;
        data.yaw = fmod(fmod(data.yaw, 360.0) + 360.0, 360.0);
        RCLCPP_INFO(node_.get_logger(), "--- dyaw : %f", dyaw);
    }
    */

    data.yaw = float(data_response.acoFix.attitudeYaw) / 10;
    data.pitch = float(data_response.acoFix.attitudePitch) / 10;
    data.roll = float(data_response.acoFix.attitudeRoll) / 10;

    data.beacon_id = float(data_response.acoFix.srcId);
    /**
    //TODO ::::
    // Ouvrir le fichier en mode ajout
    std::string info1 = "data1";
    std::string info2 = "data2";
    std::string file_name = "output.txt";
    std::ofstream file(file_name, std::ios::app);

    if (!file)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier !" << std::endl;
        return;
    }

    // Obtenir la date et l'heure actuelles
    std::string current_time = get_current_time();

    // Écrire la ligne dans le fichier avec le format demandé
    //file << current_time << "T" << std::to_string(static_cast<int>(data.beacon_id)) << "_" << std::to_string(static_cast<int>(data.azimuth)) << "_" << std::to_string(static_cast<int>(data.elevation)) << "_" << std::to_string(static_cast<int>(data.range)) << "_" << std::to_string(static_cast<int>(data.depth)) << "_" << std::to_string(static_cast<int>(data.position.x)) << "_" << std::to_string(static_cast<int>(data.position.y)) << "_" << std::to_string(static_cast<int>(data.position.z)) << std::endl;

    file << current_time << "T" << std::to_string(static_cast<int>(data.beacon_id)) << "_" << std::to_string(data.azimuth) << "_" << std::to_string(data.elevation) << "_" << std::to_string(data.depth) << "_" << std::to_string(data.range) << "_" << std::to_string(data.position.x) << "_" << std::to_string(data.position.y) << "_" << std::to_string(data.position.z) << std::endl;

    // Fermer le fichier
    file.close();
    **/
}

void ecriture(message2::msg::Usbl data, std::ofstream &donnee_ecrite)
{
    float az = data.azimuth * M_PI / 180;
    float elev = data.elevation * M_PI / 180;
    float x_reel = data.range * cos(az) * sin(elev);
    float y_reel = data.range * sin(az) * sin(elev);
    float z_reel = data.range * cos(elev);
    float range_verif = pow(pow(x_reel, 2) + pow(y_reel, 2) + pow(z_reel, 2), 0.5);
    std::cout << "x_reel = " << x_reel << "\n y_reel = " << y_reel << "\n z_reel = " << z_reel << std::endl;
    std::cout << "range = " << data.range << " range_verif = " << range_verif << std::endl;

    donnee_ecrite << x_reel << " " << y_reel << " " << z_reel << "\n";
}

class MyDriver : public SeatracDriver
{
public:
    // defintion du driver par rapport au port USB : cela existe grace a la connexion USB
    MyDriver(const std::string &serialPort = "/dev/ttyUSB0") : SeatracDriver(serialPort), k(0), yaw0(0.0)
    {
    }

    int k;
    float yaw0;

    // fonction de ping
    void ping_beacon(BID_E target, AMSGTYPE_E pingType = MSG_REQU)
    {
        messages::PingSend::Request req;
        req.target = target;
        req.pingType = pingType;
        this->send(sizeof(req), (const uint8_t *)&req);
    }

    message2::msg::Usbl USBL_info_message;
    messages::PingResp response_data;
    std::ofstream log;
    std::ofstream donnee_x_y_z_driver;

    //// ros::Publisher pub;
    rclcpp::Node::SharedPtr node_ = rclcpp::Node::make_shared("USBL_pub_node");
    rclcpp::Publisher<message2::msg::Usbl>::SharedPtr pub_ = node_->create_publisher<message2::msg::Usbl>("/usbl_data", 1000);

    bool data_available = false;

    // this method is called on any message returned by the beacon.
    void on_message(CID_E msgId, const std::vector<uint8_t> &data)
    {
        // replace code in this method by your own
        switch (msgId)
        {
        default:
            // std::cout << "-- Message : " << msgId << std::endl;  // affichage
            //    << std::flush;
            break;
        case CID_PING_ERROR:
            std::cout << "-- Ping Error" << std::endl
                      << std::flush;

            {
                messages::PingError response;
                response = data;

                std::cout << response << std::endl; // affichage

                this->ping_beacon(response.beaconId, MSG_REQU);
            }
            sleep(0.01); // <-- ajout pour laisser un peu de temps entre 2 messages
            break;
        case CID_PING_RESP:
            std::cout << "-- Ping Response " << std::endl
                      << std::flush;
            {

                response_data = data;

                std::cout << response_data << std::endl;
                write_message(USBL_info_message, response_data, *node_, k, yaw0);
                k += 1;
                this->ping_beacon(response_data.acoFix.srcId, MSG_REQU);
                pub_->publish(USBL_info_message);
                rclcpp::spin_some(node_);
            }
            sleep(0.01); // <-- ajout pour laisser un peu de temps entre 2 messages
            break;
        case CID_STATUS:
            // too many STATUS messages so bypassing display.
            // std::cout << "CID_STATUS" << std:: endl;
            break;
        }
    }
};
// Fonction pour obtenir la date et l'heure actuelles au format "aaaammdd_hhminminss"

// Fonction qui écrit dans un fichier texte une ligne de la forme "aaaammdd_hhminminssTinfo1_info2"
// void write_to_file(const std::string& info1, const std::string& info2, const std::string& file_name)
//{

//}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    const char *device = "/dev/ttyVUSB0"; // TODO: change this line if there is a boost error
    MyDriver seatrac(device);
    std::cout << "Avant 1er Ping" << std::endl;

    command::ping_send(seatrac, BEACON_ID_15, MSG_REQU);
    std::cout << "Après 1er ping" << std::endl;
    getchar();

    for (int i = 0; i < 3; i++)
    {
        std::cout << "Dans la boucle à l'indice i = " << i << std::endl;
        // while(1) {
        getchar();
        try
        {
            std::cout << "try" << std::endl;
            std::cout << "ping_send" << command::ping_send(seatrac, BEACON_ID_15) << std::endl;
        }
        catch (const TimeoutReached &e)
        {

            std::cout << "Timeout reached" << std::endl;
        }
    }

    return 0;
}

// POLLUX : BEACON_ID_1
// CASTOR : BEACON_ID_2