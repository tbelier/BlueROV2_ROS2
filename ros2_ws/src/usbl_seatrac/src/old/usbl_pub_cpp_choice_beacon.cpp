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

#include "message2/msg/usbl.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <seatrac_driver/SeatracDriver.h>
#include <seatrac_driver/messages/Messages.h>
#include <seatrac_driver/commands.h>


using namespace std::chrono_literals;
using namespace narval::seatrac;

//Cette fonctio est sensee ecrire  les data sur le log d'entree
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

   //std::cout << "The local date and time is: " << dt << std::endl;

   // convert now to tm struct for UTC
   // tm *gmtm = gmtime(&now);
   // dt = asctime(gmtm);
   char *test = new char[8];
   for (int i = 0; i < 8; i++)
   {
       test[i] = dt[i + 11];
   }
   std::string s(test, 8);
   return s;
}

/// MODIFIER TOUS LES usbl::Usbl par message2::msg::Usbl

//Cette fonction ecrit le message sous format Usbl qui sera publish
void write_message(message2::msg::Usbl &data, const narval::seatrac::messages::PingResp &data_response, rclcpp::Node &node_)
{
    // std::time_t result = std::time(nullptr);
    // std::cout << data.azimuth<< std::endl;
    // std::cout << data.elevation<< std::endl;
    // std::cout << data.range<< std::endl;
    // std::cout << data.depth<< std::endl;
    // std::cout << data.position.x<< std::endl;
    // std::cout << data.position.y<< std::endl;
    // std::cout << data.position.z<< std::endl;
    // std::cout << data.beacon_id<< std::endl;

    data.header.stamp = node_.get_clock()->now();//ros::Time::now();
    data.azimuth = float(data_response.acoFix.usbl.azimuth) / 10;
    data.elevation = float(data_response.acoFix.usbl.elevation) / 10;
    data.range = float(data_response.acoFix.range.dist) / 10;
    data.depth = float(data_response.acoFix.depthLocal) / 10;
    // data.position.x = float(data_response.acoFix.position.easting) / 10;
    // data.position.y = float(data_response.acoFix.position.northing) / 10;
    // data.position.z = float(data_response.acoFix.position.depth) / 10;

    float az = data.azimuth * M_PI / 180;
    float el = -data.elevation * M_PI / 180;
    data.position.x = data.range * sin(az) * cos(el);
    data.position.y = data.range * sin(az) * sin(el);
    data.position.z = data.range * cos(el);
    
    data.beacon_id =  float(data_response.acoFix.srcId);
    //std::cout << data.header.stamp << std::endl;
    
    /*
    Pour asservissement
    conversion en degree
    x = cos(azimuth) * cos(elevation)
    y = sin(azimuth) * cos (elevation)
    z = sin( elevation) ( a comparer avec profondeur)
    plot(y, -x) (pour la rotation)

    */
}

void ecriture(message2::msg::Usbl data, std::ofstream &donnee_ecrite)
{
    float az = data.azimuth * M_PI / 180;
    float elev = data.elevation * M_PI / 180;
    float x_reel = data.range * cos(az) * sin(elev);
    float y_reel = data.range * sin(az) * sin(elev);
    float z_reel = data.range * cos(elev);
    float range_verif = pow(pow(x_reel,2) + pow(y_reel,2) + pow(z_reel,2),0.5);
    std::cout << "x_reel = "<< x_reel<<"\n y_reel = "<< y_reel << "\n z_reel = " << z_reel << std::endl;
    std::cout << "range = "<< data.range << " range_verif = " << range_verif << std::endl;
    
    donnee_ecrite << x_reel << " " << y_reel << " " << z_reel << "\n";

}
/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

//defintion classe du driver


class MyDriver : public SeatracDriver
{
public:
//defintion du driver par rapport au port USB : cela existe grace a la connexion USB
    MyDriver(const std::string &serialPort = "/dev/ttyUSB0") : SeatracDriver(serialPort)
    {
    }

//fonction de ping
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
            //std::cout << "On a un message Morray : " << msgId << std::endl;  // affichage
                    //   << std::flush;
    
            break;
        case CID_PING_ERROR:
        std::cout << "On a eu un Ping Error" << std::endl << std::flush;

        {
            messages::PingError response;
            response = data;
            
            std::cout << response << std::endl; // affichage
            
            this->ping_beacon(response.beaconId, MSG_REQU);
        }
        sleep(0.01); // <-- ajout pour laisser un peu de temps entre 2 messages
        break;
        case CID_PING_RESP:
            std::cout << "On a Ping Response, Morray " << std::endl << std::flush;
            {
                response_data = data;
                
                std::cout << response_data << std::endl; // affichage
                
                
                //log_write(log, response_data);
                write_message(USBL_info_message, response_data, *node_);
                //std::cout << "avant ecriture"<< std::endl;
                //ecriture(USBL_info_message,donnee_x_y_z_driver);
                //std::cout << "fin ecriture"<< std::endl;

                this->ping_beacon(response_data.acoFix.srcId, MSG_REQU);
                pub_->publish(USBL_info_message);
                rclcpp::spin_some(node_);
            }
            sleep(0.01); // <-- ajout pour laisser un peu de temps entre 2 messages
            break;
        case CID_STATUS:
            // too many STATUS messages so bypassing display.
            break;
        }
    }
    

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

   /// MyDriver seatrac("/dev/ttyUSB0");
    const char* device = "/dev/usbl_usb";
    //const char* device = "/dev/ttyUSB0";


    MyDriver seatrac(device);

    printf( "Enter the Beacon ID (between 1 and 9, else 15) \n");
    int n2 ;
    scanf("%d", &n2);
    printf( "you enter %d \n", n2 );
    
    while ((getchar()) != '\n');
    
    
    std::cout << "Avant 1er Ping" << std:: endl;

    //command::ping_send(seatrac, BEACON_ID_2, MSG_REQU);
    if (n2 == 1) {
        command::ping_send(seatrac, BEACON_ID_1, MSG_REQU);
    }
    else if  (n2 == 2) {
    command::ping_send(seatrac, BEACON_ID_2, MSG_REQU);
    }
    else if  (n2 == 3) {
    command::ping_send(seatrac, BEACON_ID_3, MSG_REQU);

    }
    else if  (n2 == 4) {
    command::ping_send(seatrac, BEACON_ID_4, MSG_REQU);
    }
    else if  (n2 == 5) {
    command::ping_send(seatrac, BEACON_ID_5, MSG_REQU);
    }
    else if  (n2 == 6) {
    command::ping_send(seatrac, BEACON_ID_6, MSG_REQU);
    }
    else if  (n2 == 7) {
    command::ping_send(seatrac, BEACON_ID_7, MSG_REQU);
    }
    else if  (n2 == 8) {
    command::ping_send(seatrac, BEACON_ID_8, MSG_REQU);
    }
    else if  (n2 == 9) {
    command::ping_send(seatrac, BEACON_ID_9, MSG_REQU);
    }
    else {
    command::ping_send(seatrac, BEACON_ID_15, MSG_REQU);
    }

    //std::cout << "ici" << std:: endl;
    //getchar();

    
    while(1) {
            std::cout << "On tourne sur la boucle !!!" << std::endl << std::flush;
            //std::cout << "ping_send" << command::ping_send(seatrac, BEACON_ID_2) << std::endl;
            //seatrac.pub_ = usbl_info_pub;
            if (n2 == 1) {
                    command::ping_send(seatrac, BEACON_ID_1, MSG_REQU);
                }
            else if  (n2 == 2) {
                command::ping_send(seatrac, BEACON_ID_2, MSG_REQU);
                }
            else if  (n2 == 3) {
                command::ping_send(seatrac, BEACON_ID_3, MSG_REQU);

                }
            else if  (n2 == 4) {
                command::ping_send(seatrac, BEACON_ID_4, MSG_REQU);
                }
            else if  (n2 == 5) {
                command::ping_send(seatrac, BEACON_ID_5, MSG_REQU);
                }
            else if  (n2 == 6) {
                command::ping_send(seatrac, BEACON_ID_6, MSG_REQU);
                }
            else if  (n2 == 7) {
                command::ping_send(seatrac, BEACON_ID_7, MSG_REQU);
                }
            else if  (n2 == 8) {
                command::ping_send(seatrac, BEACON_ID_8, MSG_REQU);
                }
            else if  (n2 == 9) {
                command::ping_send(seatrac, BEACON_ID_9, MSG_REQU);
                }
                else {
                command::ping_send(seatrac, BEACON_ID_15, MSG_REQU);
                }
            sleep(1.);
    }
    rclcpp::shutdown();

   return 0;
}

