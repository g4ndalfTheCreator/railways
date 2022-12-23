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

    std::cout << std::endl; // Ends current line


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
    else if(command == "get_station_name") get_station_name(commands);
    else if(command == "get_station_coord") get_station_coord(commands);
    else if(command == "find_station_with_coord") find_station_with_coord(commands);
    else if(command == "change_station_coord") change_station_coord(commands);
    else if(command == "add_departure") add_departure(commands);
    else if(command == "remove_departure") remove_departure(commands);
    else if(command == "station_departures_after") station_departures_after(commands);
    else if(command == "add_region") add_region(commands);
    else if(command == "all_regions") all_regions();
    else if(command == "get_region_name") get_region_name(commands);
    else if(command == "get_region_coords") get_region_coords(commands);
    else if(command == "add_subregion_to_region") add_subregion_to_region(commands);
    else if(command == "add_station_to_region") add_station_to_region(commands);
    else if(command == "station_in_regions") station_in_regions(commands);
    else if(command == "all_subregions_of_region") all_subregions_of_region(commands);
    else if(command == "stations_closest_to") stations_closest_to(commands);
    else if(command == "remove_station") remove_station(commands);
    else if(command == "common_parent_of_regions") common_parent_of_regions(commands);

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
 * @brief Ui::coord_transformer Makes tranform of coord
 * @return Coordinatified value.
 */
Coord Ui::coord_transformer(std::string str_coord){

    // Values for intifier
    std::string xstr;
    std::string ystr;
    bool x_done = false;


    for(char c : str_coord){
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

    if(xstr.size() == 0 or ystr.size() == 0) return NO_COORD;

    int x = std::stoi(xstr);
    int y = std::stoi(ystr);


    return {x, y};

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

    for(StationID& id : ids){

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
    Coord xy = coord_transformer(commands[3]);

    // Adds values and shows messages accordingly.
    bool status = datastructures_.add_station(id, name, xy);

    if(status){

        std::cout << "You have succesfully added a station: " << name << std::endl;
    }

    else{

        std::cout << "Unfortunately something went wrong" << std::endl;
    }
}

/**
 * @brief Ui::get_station_name Prints out stations name
 * @param id From this station we are printing
 */
void Ui::get_station_name(std::vector<std::string> commands){

     if(commands_amount_checker(commands, 2)) return;

    StationID id = commands[1];

    Name name = datastructures_.get_station_name(id);

    std::cout << "Name for \"" << id << "\" is: " << name << std::endl;

}

/**
 * @brief Ui::get_station_coord Gets stations location and prints it out
 * @param commands get parameters from commands.
 */
void Ui::get_station_coord(std::vector<std::string> commands){

     if(commands_amount_checker(commands, 2)) return;

    StationID id = commands[1];

    Coord xy = datastructures_.get_station_coordinates(id);

    std::cout << "Location for \"" << id << "\" is: " << xy.x << " and " << xy.y << std::endl;

}

/**
 * @brief Ui::find_station_with_coord finds a station with coord
 * @param commands get parameters from commands.
 */
void Ui::find_station_with_coord( std::vector<std::string> commands){

    if(commands_amount_checker(commands, 2)) return;

    Coord xy = coord_transformer(commands[1]);

    // Searches for info and after that prints details
    StationID id = datastructures_.find_station_with_coord(xy);

    if(id == NO_STATION){
        std::cout << "No station in this xy location" << std::endl;
        return;
    }

    std::cout << "Found a station with id: " << id << std::endl;
}

/**
 * @brief Ui::change_station_coord Changes a stations coordinate
 * @param commands Changes by these commands
 */
void Ui::change_station_coord(std::vector<std::string> commands){


    if(commands_amount_checker(commands, 3)) return;

    StationID id = commands[1];
    Coord xy = coord_transformer(commands[2]);

    bool status = datastructures_.change_station_coord(id, xy);

    if(status){
        std::cout << id << " Changing coord to " << xy.x << ", " << xy.y << std::endl;
    }

    else{
        std::cout << "Failure! Could not change coord!" << std::endl;
    }
}

/**
 * @brief Ui::add_departure Adds a departure to station and shows info about it
 * @param commands uses these parameters
 */
void Ui::add_departure(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 4)) return;

    StationID id = commands[1];
    TrainID id_t = commands[2];
    Time time = std::stoi(commands[3]);

    bool status = datastructures_.add_departure(id, id_t, time);

    if(status){
        std::cout << id_t << " added to " << id << "with leaving time: " << time << std::endl;
    }

    else{
        std::cout << "Failure! Could not add train!" << std::endl;
    }
}

void Ui::remove_departure(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 4)) return;

    StationID id = commands[1];
    TrainID id_t = commands[2];
    Time time = std::stoi(commands[3]);

    bool status = datastructures_.remove_departure(id, id_t, time);

    if(status){
        std::cout << id_t << " removed from " << id << "with leaving time: " << time << std::endl;
    }

    else{
        std::cout << "Failure! Could not remove train!" << std::endl;
    }
}

/**
 * @brief Ui::station_departures_after Prints out departures after selevted time
 * @param commands
 */
void Ui::station_departures_after(std::vector<std::string> commands){


    if(commands_amount_checker(commands, 4)) return;

    StationID id = commands[1];
    Time time = std::stoi(commands[2]);

    std::vector<std::pair<Time,TrainID>> time_trains = datastructures_.station_departures_after(id, time);

    if(time_trains[0].first == NO_TIME and time_trains[0].second == NO_TRAIN){
        std::cout << "No trains leaving from this station after selected time." << std::endl;
    }

    else{

        for(auto& current : time_trains){
            std::cout << "Time: " << current.first << " Train: " << current.second << std::endl;
        }
    }
}

/**
 * @brief Ui::add_region to db
 * @param commands
 */
void Ui::add_region(std::vector<std::string> commands){

    // Checks if command has right amount of parameters
    if(commands_amount_checker(commands, 4)) return;

    RegionID id = stoi(commands[1]);
    Name name = commands[2];
    std::vector<Coord> coords;

    for(long unsigned int i = 3; i < commands.size(); i++){
        coords.push_back(coord_transformer(commands[i]));
    }

    // Adds values and shows messages accordingly.
    bool status = datastructures_.add_region(id, name, coords);

    if(status){

        std::cout << "You have succesfully added a station: " << name << std::endl;
    }

    else{

        std::cout << "Unfortunately something went wrong" << std::endl;
    }
}

/**
 * @brief Ui::all_regions prints out all regions
 */
void Ui::all_regions(){


    std::vector<RegionID> ids = datastructures_.all_regions();
    int count_of_regions = ids.size();
    std::cout << "Currently you have " << count_of_regions << " regions in your datastrucutres. Here is a list of them all:" << std::endl;

    int counter = 1;

    for(RegionID& id : ids){

        Name name = datastructures_.get_region_name(id);

        std::cout << counter << ". Region id: " << id << " goes by name " << name << std::endl;

        counter++;
    }
}

/**
 * @brief Ui::get_region_name Prints regions name.
 * @param commands
 */
void Ui::get_region_name(std::vector<std::string> commands){

   if(commands_amount_checker(commands, 2)) return;

   RegionID id = stoi(commands[1]);

   Name name = datastructures_.get_region_name(id);

   std::cout << "Name for \"" << id << "\" is: " << name << std::endl;
}

/**
 * @brief Ui::get_region_coords Prints regions borders point locations.
 * @param commands
 */
void Ui::get_region_coords(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 2)) return;

    RegionID id = stoi(commands[1]);

    std::vector<Coord> coords = datastructures_.get_region_coords(id);

    std::cout << "Coords for \"" << id << "\" are: ";

    for(Coord xy : coords){

        std::cout << xy.x << "," << xy.y << " ";
    }

    std::cout << std::endl;
}

/**
 * @brief Ui::add_subregion_to_region adds subregion to upper parent region
 * @param commands
 */
void Ui::add_subregion_to_region(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 3)) return;

    RegionID id = stoi(commands[1]);
    RegionID id_parent = stoi(commands[2]);

    bool status = datastructures_.add_subregion_to_region(id, id_parent);


    if(status){

        std::cout << "You have succesfully region: " << id << " as subregion of " << id_parent << std::endl;
    }

    else{

        std::cout << "Unfortunately something went wrong" << std::endl;
    }
}

/**
 * @brief Ui::add_station_to_region adds station to region
 * @param commands
 */
void Ui::add_station_to_region(std::vector<std::string> commands){

    if(commands_amount_checker(commands, 3)) return;

    StationID stat_id = commands[1];
    RegionID reg_id = stoi(commands[2]);

    bool status = datastructures_.add_station_to_region(stat_id, reg_id);


    if(status){

        std::cout << "You have succesfully station: " << stat_id << " to this region" << reg_id << std::endl;
    }

    else{

        std::cout << "Unfortunately something went wrong" << std::endl;
    }
}

void Ui::station_in_regions(std::vector<std::string> commands){
    std::cout << std::endl;
}

void Ui::all_subregions_of_region(std::vector<std::string> commands){
    std::cout << std::endl;
}

void Ui::stations_closest_to(std::vector<std::string> commands){
    std::cout << std::endl;
}

void Ui::remove_station(std::vector<std::string> commands){
    std::cout << std::endl;
}

void Ui::common_parent_of_regions(std::vector<std::string> commands){
    std::cout << std::endl;
}
