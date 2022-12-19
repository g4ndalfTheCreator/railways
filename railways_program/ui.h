#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <sstream>
#include <ctype.h>

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


    // Functions that create ui.
    void clear_all();
    void station_count();
    void all_stations();
    void add_station(std::vector<std::string> commands);
    void get_station_name(StationID id);
    void get_station_coord(StationID id);


};

#endif // UI_H
