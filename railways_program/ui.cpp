#include "ui.h"

Ui::Ui(){}
Ui::~Ui(){}

/**
 * @brief Ui::command_parser Selects functionality for this command
 * @param command string based command
 */
void Ui::command_parser(std::string input){

    std::vector<std::string> commands;
    std::string c_command;


    for(char c : input){

        std::cout << c;

        if(c == ' '){

            commands.push_back(c_command);
            c_command = "";
        }

        else if(c != '\"'){

            c_command.push_back(c);
        }


    }
    commands.push_back(c_command);

    std::string command = *commands.begin();

    std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){return std::tolower(c); });

    if(command == "end"){
        // Ends loop

        std::cout << "EXIT!" << std::endl;

        return;
    }

    else if(command == "read") read_file(commands);
    else if(command == "help") help();
    else if(command == "station_count") station_count();
    else if(command == "clear_all") clear_all();
    else if(command == "all_stations") all_stations();
    else if(command == "add_station") add_station(commands);
    else if(command == "get_station_name");
    else if(command == "get_station_coord");

    else{

        std::cout << "Erroneus command! Please see command \"help\" for help." << std::endl;
    }

}


void Ui::ui_begin(Datastructures this_datastructure){
    // Loops UI through and through..

    datastructures_ = this_datastructure;

    std::cout << "Welcome to railway information program!" << std::endl;

    while(true){

        // Loops through commands and on correct commands performs wanted task.

        std::cout << "CMD: ";
        std::string input;
        std::getline(std::cin, input);

        command_parser(input);

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

        std::cout << "You have inserted " << size << " parameters instead of " << wanted_size << std::endl;

        return true;
    }

    return false;

}

/**
 * @brief Ui::read_stations reads a file containig commands.
 */
void Ui::read_file(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 2)) return;

    std::string filename = commands[1];

    std::ifstream file(filename);

    if(!file){
            std::cout << " Error! The file " << filename <<" cannot be opened." << std::endl;
            return;
        }


    std::string input;

    while(getline(file, input)){
        command_parser(input);
    }

}

/**
 * @brief Ui::help Gives out help information.
 */
void Ui::help(){

    std::cout << "Guide goes here!" << std::endl;

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

/**
 * @brief Ui::all_stations Presents all stations to user
 */
void Ui::all_stations(){

    std::vector<StationID> ids = datastructures_.all_stations();
    int count_of_stations = ids.size();
    std::cout << "Currently you have " << count_of_stations << " stations in your datastrucutres. Here is a list of them all:" << std::endl;

    int counter = 1;

    for(StationID id : ids){

        Name name = datastructures_.get_station_name(id);
        Coord xy = datastructures_.get_station_coordinates(id);

        std::cout << counter << ". Station id: " << id << " goes by name " << name << " and its location: {" << xy.x << "," << xy.y << "}"
                  << std::endl;

        counter++;
    }

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
        // Transforms characters to  numbers.

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


