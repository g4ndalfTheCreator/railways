#include "ui.h"

Ui::Ui(){}
Ui::~Ui(){}


void Ui::ui_begin(){
    // Loops UI through and through..


    std::cout << "Welcome to railway information program!" << std::endl;

    while(true){

        std::cout << "CMD: ";
        std::string input;
        std::getline(std::cin, input);

        std::istringstream ss(input);
        std::string temp;

        std::vector<std::string> commands;


        while(ss >> temp){

            commands.push_back(temp);

        }

        std::string command = *commands.begin();

        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){return std::tolower(c); });

        if(command == "end"){
            // Ends loop

            std::cout << "EXIT!" << std::endl;

            return;
        }

        else if(command == "read"){
            // Presents help message

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "help"){
            // Presents help message

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "station_count"){
            // counts stations and presents result

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "clear_all"){
            // Clears all data from the structures

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "all_stations"){
            // Presents all stations in datastructures

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "add_station"){
            // Adds new station in a dataructure

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "get_station_name"){
            // Showhs a name of chosen station

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }


        else if(command == "get_station_coord"){
            // Shows location of station.

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }



    }


}
