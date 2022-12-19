#include "ui.h"

Ui::Ui(){}
Ui::~Ui(){}


void Ui::ui_begin(Datastructures this_datastructure){
    // Loops UI through and through..

    datastructures_ = this_datastructure;

    std::cout << "Welcome to railway information program!" << std::endl;

    while(true){

        // Loops through commands and on correct commands performs wanted task.

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


        else if(command == "station_count") station_count();
        else if(command == "clear_all") clear_all();


        else if(command == "all_stations"){
            // Presents all stations in datastructures

            for(auto c : commands){
                // All given parameter out
                std::cout << c << std::endl;
            }
        }

        else if(command == "add_station") add_station(commands);

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

        else{

            std::cout << "Erroneus command! Please see command \"help\" for help." << std::endl;

        }
    }
}

/**
 * @brief Ui::commands_amount_checker Checks if given command has enough parameters
 * @param commands
 * @param wanted_size
 * @return if they are or not
 */
bool Ui::commands_amount_checker(std::vector<std::string> commands, int wanted_size){

    int size = commands.size();

    if(size != wanted_size){
        // Size checker checks if correct size of commands are given, if not returns true.

        std::cout << "You have inserted " << size << " parameters instead of " << 4 << std::endl;

        return true;
    }

    return false;

}

/**
 * @brief Ui::clear_all presents user a message of clearing being complete
 */
void Ui::clear_all(){

    datastructures_.clear_all();
    std::cout << "Cleared all data from program!" << std::endl;

}


/**
 * @brief Ui::station_count presents user number of stations
 */
void Ui::station_count(){

    int count_of_stations = datastructures_.station_count();
    std::cout << "Currently you have " << count_of_stations << " stations in your datastrucutres" << std::endl;

}

void Ui::all_stations(){

    int count_of_stations = datastructures_.all_stations();
    std::cout << "Currently you have " << count_of_stations << " stations in your datastrucutres. Here is a list of them all:" << std::endl;

}

/**
 * @brief Ui::add_station adds a station in datastructures
 * @param commands uses these commands to do that.
 */
void Ui::add_station(std::vector<std::string> commands){

    // Checks if command has right amount of parameters
    if(commands_amount_checker(commands, 4)) return;

    StationID id = commands[1];
    Name name = commands[2];

    // Values for intifier
    std::string xstr;
    std::string ystr;
    bool x_done = false;


    for(char c : commands[3]){

        if(c == ','){
            x_done = true;
        }

        if(std::isdigit(c)){

            if(x_done){
                ystr.push_back(c);
            }

            else{
                xstr.push_back(c);
            }
        }
    }

    int x = std::stoi(xstr);
    int y = std::stoi(ystr);

    // Adds values and shows messages accordingly.
    bool status = datastructures_.add_station(id, name, {x, y});

    if(status){

        std::cout << "You have succesfully added a station: " << name << std::endl;
    }

    else{

        std::cout << "Unfortunately something went wrong" << std::endl;
    }
}


