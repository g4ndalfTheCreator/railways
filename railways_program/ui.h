#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <fstream>

#include <datastructures.hh>


class Ui
{
public:

    Ui();
    ~Ui();

    void ui_begin(Datastructures main_datastructure);

private:
    // Private stuff here

    Datastructures datastructures_;

    // NON-UI functionality

    bool commands_amount_checker(std::vector<std::string> commands, int wanted_size);
    void command_parser(std::string command);


    // Functions that create ui.
    void read_file(std::vector<std::string> commands);
    void help();
    void clear_all();
    void station_count();
    void all_stations();
    void add_station(std::vector<std::string> commands);
    void get_station_name(std::vector<std::string> commands);
    void get_station_coord(std::vector<std::string> commands);
    void stations_alphabetically();
    void stations_distance_increasing();
    void find_station_with_coord(std::vector<std::string> commands);
    void change_station_coord(std::vector<std::string> commands);
    void add_departure(std::vector<std::string> commands);
    void remove_departure(std::vector<std::string> commands);
    void station_departures_after(std::vector<std::string> commands);
    void add_region(std::vector<std::string> commands);
    void all_regions();
    void get_region_name(std::vector<std::string> commands);
    void get_region_coords(std::vector<std::string> commands);
    void add_subregion_to_region(std::vector<std::string> commands);
    void add_station_to_region(std::vector<std::string> commands);
    void station_in_regions(std::vector<std::string> commands);
    void all_subregions_of_region(std::vector<std::string> commands);
    void stations_closest_to(std::vector<std::string> commands);
    void remove_station(std::vector<std::string> commands);
    void common_parent_of_regions(std::vector<std::string> commands);
};

#endif // UI_H
