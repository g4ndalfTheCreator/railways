// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <iostream>

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures(){}
Datastructures::~Datastructures(){}


/**
 * @brief distrance_calc calculates distance between two points
 * @param xy1 First coord
 * @param xy2 second coord
 * @return Returns distance between two points
 */
double Datastructures::distrance_calc(Coord xy1, Coord xy2){
    return sqrt(pow((xy1.x - xy2.x), 2) + pow((xy1.y - xy2.y), 2));
}

/**
 * @brief get_parent_regions gets parents of a region
 * @param parents as vec
 * @param this reg
 */
void Datastructures::get_parent_regions(std::vector<RegionID>* parents, Region* reg){

    if(reg == nullptr){
        return;
    }

    parents->push_back(reg->id);

    get_parent_regions(parents, reg->parent_reg);

}

/**
 * @brief Datastructures::get_sub_regions
 * @param subs upper values
 * @param reg all subs
 */
void Datastructures::get_sub_regions(std::vector<RegionID> *subs, Region *reg){

    if(reg->regions_subregions.size() == 0){
        // checks if no regions present

        return;
    }

    for(Region* current : reg->regions_subregions){
        //goes through all regions subregions and their subregions recursively.

        subs->push_back(current->id);

        get_sub_regions(subs, current);
    }
}
/**
 * @brief Datastructures::redo_all_stations_vec
 */
void Datastructures::redo_all_stations_vec(){
    all_stations_vec.clear();

    for(auto &c : stations){
        // adds all the values to vector again.

        all_stations_vec.push_back(c.first);
    }

    all_stations_vec_missing_values = false;
    all_xy_stations_vec_not_in_order = true;
    all_abc_stations_vec_not_in_order = true;
}


/**
 * @brief Datastructures::pathbuilder Builds a path backwards from goal_node
 * @param begin in this reverse implementation is goal node
 * @param current Current as this is recursive
 * @param path Saves path there needs to be reversed.
 */
void Datastructures::pathbuilder(Station_info *begin, Station_info* current, std::vector<std::pair<StationID, int>> *path){

    if(current->node.d == 999999999){
        // Cheks if using undefined route

        path->clear();
        return;
    }

    if(current->node.prev_node == nullptr){
        // Is startnode found adds and returns

        path->push_back({current->id, current->node.d});

        return;
    }

    path->push_back({current->id, current->node.d});

    pathbuilder(begin, current->node.prev_node , path);

}
/**
 * @brief Datastructures::bfs This makes a search as bfs
 * @param start Staring node
 * @param goal End node
 * @return vector containig necessary information.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::bfs(Station_info *start, Station_info *goal){

    // Implementing BFS function:
    clear_node_data();
    need_clear_node_data = true;

    std::vector<std::pair<StationID, int>> path;
    std::deque<Station_info*> temp;

    // Sets up the first node!!
    start->node.colour = 1;
    start->node.d = 0;
    start->node.de = 0;
    temp.push_front(start);


    while(temp.size() != 0){
        // Loops untill there is no connedted stations

        Station_info* u = temp.back();
        temp.pop_back();


        for(auto v : u->connected_stations){
            // Loops thourgh all wanted stations.

                if(v.first->node.colour == 0){
                    // If node is white

                    v.first->node.colour = 1;
                    v.first->node.de = u->node.de + 1;
                    v.first->node.d = u->node.d + distrance_calc(u->xy, v.first->xy);
                    v.first->node.prev_node = u;
                    temp.push_front(v.first);

                }

                if(v.first->id == goal->id){
                    // If value found finds path and reverses, and returns.

                    pathbuilder(start, v.first, &path);

                    std::reverse(path.begin(), path.end());
                    return path;
                }

            u->node.colour = 2;
        }
    }

     return {};
}


/**
 * @brief Datastructures::recursive_dfs Finds route with cycle
 * @param start Startingpoint of the route
 * @param path Path that route goes thorugh
 * @return route with cycle
 */
void Datastructures::recursive_dfs(Station_info *start, std::vector<StationID> *path){

    // First node  is set to gray and added to path
    start->node.colour = 1;
    path->push_back(start->id);

    for(auto v : start->connected_stations){
        // Loops though current stations

        if(v.first->node.colour == 0){
            // New nodes always initiane new level in recursion

            return recursive_dfs(v.first, path);
        }

        else if(v.first->node.colour == 1){
            // Cycle found means that recursion ends and cycle_true is se  to be true.

            path->push_back(v.first->id);
            cycle_true = true;
            return;
        }
    }

    // When this node is completed fully it is set to be black.
    start->node.colour = 2;
}

/**
 * @brief Datastructures::relax_a_star Relaxes a star algoritm
 * @param u Main node
 * @param v Subnode
 * @param g Goalnode
 */
void Datastructures::relax_a_star(Station_info *u, Station_info *v, Station_info *g){

    int distance_u_v = std::floor(distrance_calc(u->xy, v->xy));
    int distance_v_g = std::floor(distrance_calc(v->xy, g->xy));


    if(v->node.d > u->node.d + distance_u_v){
        v->node.d = u->node.d + distance_u_v;
        v->node.de = v->node.d + distance_v_g;
        v->node.prev_node = u;
    }
}

/**
 * @brief Datastructures::a_star Finds shortest path to goal
 * @param start Starting point.
 * @param goal Where we are going to
 * @param path Writes path here
 */
void Datastructures::a_star(Station_info *start, Station_info *goal, std::vector<std::pair<StationID, int>> *path){

    // Starting with the basics
    clear_node_data();
    need_clear_node_data = true;

    std::set<std::pair<int,Station_info*>> Q;


    start->node.colour = 1;
    start->node.d = 0;

    Q.insert({0, start});

    while(Q.size() != 0){

        Station_info* u = Q.begin()->second;
        Q.erase(Q.begin());

        for(auto v : u->connected_stations){

            int v_de_old = v.first->node.de;

            relax_a_star(u, v.first, goal);

            if(v.first->node.colour == 0){
                v.first->node.colour = 1;
                Q.insert({v.first->node.de, v.first});
            }

            else if(v.first->node.colour == 1){
                if(v_de_old < v.first->node.de){
                    Q.erase(Q.find({v_de_old, v.first}));
                    Q.insert({v.first->node.de, v.first});
                }
            }
        }
        u->node.colour = 2;
    }

    pathbuilder(start, goal, path);
    std::reverse(path->begin(), path->end());

    return;

}


void Datastructures::fastest_route(Station_info *start, Station_info *goal,
                                   Time start_time ,
                                   std::vector<std::pair<StationID, Time>> path,
                                   std::vector<std::vector<std::pair<StationID, Time>>>* possible_paths){

    if(start->id == goal->id){
        // If goal found path is saved

        path.push_back({start->id, start_time});
        possible_paths->push_back(path);
        path.clear();

        if(possible_paths->size() > 5){
            // If there are at least 5 possible paths some of the should be good enough

            return;
        }

    }

    std::vector<std::pair<Time, TrainID>>departures_after = station_departures_after(start->id, start_time);

    for(std::pair<Time, TrainID>& current : departures_after){
        // Gets departures from this station.


        std::vector<StationID> stations_this_train_goes = train_stations_from(start->id, current.second);



        for(StationID& current_station : stations_this_train_goes){
            // Gets stations where this departures are headed

            std::vector<std::pair<Time, TrainID>>departuretimes = station_departures_after(current_station, start_time);

            for(std::pair<Time, TrainID>& c_dep : departuretimes){
                // Gets times

                if(c_dep.second == current.second){


                    if(path.size() == 0){
                        path.push_back({start->id, current.first});
                    }


                    if(path.back().first == start->id){
                        path.back().second = current.first;
                    }
                    else{
                        path.push_back({start->id, current.first});
                    }

                    Station_info* this_station = &stations.find(current_station)->second;
                    fastest_route(this_station, goal, c_dep.first, path, possible_paths);

                }
            }
        }
    }



    return;
}


/**
 * @brief ::Datastructures::clear_node_data clears all the node related data.
 */
void::Datastructures::clear_node_data(){

    if(need_clear_node_data){
        // If need to clean node;
        cycle_true = false;

        for(auto& station : stations){
            // Sets all nodes to defaults.

            station.second.node.colour = 0;
            station.second.node.d = 999999999;
            station.second.node.de = 999999999;
            station.second.node.prev_node = nullptr;

        }
    }
}

/**
 * @brief Datastructures::station_count counts all stations
 * @return stations.size()
 */
unsigned int Datastructures::station_count(){
    // Just returns size of the station storage
    return stations.size();
}

/**
 * @brief Datastructures::clear_all clears all..
 */
void Datastructures::clear_all(){
    // Just clears all the data from all the storages
    stations.clear();
    all_stations_vec.clear();
    regions_db.clear();
    all_regions_vec.clear();
    need_clear_node_data = false;

}

/**
 * @brief Datastructures::all_stations Gets all stations in a vec and returns it
 * @return all_station_vec
 */
std::vector<StationID> Datastructures::all_stations(){
    // Returns all stations in a vector:

    if(all_stations_vec_missing_values){
        // If values are missing redo vec

        redo_all_stations_vec();
    }

    return all_stations_vec;
}


/**
 * @brief Datastructures::add_station add new station to structures
 * @param id of stat
 * @param name of stat
 * @param xy of stat
 * @return bool if success or not
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy){

    // Creates an iterator that tries to find out whether station alredy exist in the db
    Station_storage::iterator it = stations.find(id);

    if (it != stations.end()){
        // If present DO NOT add anything
        return false;
    }

    // Adds name to the stations name and xy storage and sets their status unordered
    all_stations_vec.push_back(id);
    all_abc_stations_vec_not_in_order = true;
    all_xy_stations_vec_not_in_order = true;


    // Initializes new station and adds it into unordered_map
    Station_info new_station;

    new_station.id = id;
    new_station.name = name;
    new_station.xy = xy;
    new_station.stations_trains = {};
    new_station.parent_reg = nullptr;

    stations.insert({id, new_station});

    return true;
}


/**
 * @brief Datastructures::get_station_name name of station
 * @param by id
 * @return name
 */
Name Datastructures::get_station_name(StationID id){

    // Creates an iterator that tries to find out whether station alredy exist in the db
    Station_storage::iterator it = stations.find(id);

    if (it == stations.end()){
        // If not present return NO_NAME
        return NO_NAME;
    }

    // gets station name from map and struct
    Name station_name = it->second.name;
    return station_name;
}

/**
 * @brief Datastructures::get_station_coordinates same as above but with the coords
 * @param by id
 * @return location
 */

Coord Datastructures::get_station_coordinates(StationID id){

    // Creates an iterator that tries to find out whether station alredy exist in the db
    Station_storage::iterator it = stations.find(id);

    if (it == stations.end()){
        // If not present return NO_COORD
        return NO_COORD;
    }

    // gets station coords from map and struct
    Coord station_coord = it->second.xy;
    return station_coord;
}

/**
 * @brief Datastructures::stations_alphabetically orders stations
 * @return stations alphabetically
 */
std::vector<StationID> Datastructures::stations_alphabetically(){
    // Sorts all stations vector and returns it sorted to alphabetiaclly


    if(all_stations_vec_missing_values){
        // If values are missing redo vec

        redo_all_stations_vec();
    }

    if(all_abc_stations_vec_not_in_order){
        // Checks if statios are in order

        all_abc_stations_vec_not_in_order = false; // Set states
        all_xy_stations_vec_not_in_order = true;

        // Sorts alphabetically all_stations_vec to alphabetical order and sets status.
        sort(all_stations_vec.begin(), all_stations_vec.end(),
             [this](StationID id1, StationID id2){

            // Get names and return comparison:
            Name name1 = stations.find(id1)->second.name;
            Name name2 = stations.find(id2)->second.name;

            return (name1 < name2);

        });
    }

    return all_stations_vec;
}

/**
 * @brief Datastructures::stations_distance_increasing orders stations
 * @return stations distance increasing from 0 0
 */

std::vector<StationID> Datastructures::stations_distance_increasing(){


    if(all_stations_vec_missing_values){
        // If values are missing redo vec

        redo_all_stations_vec();
    }

    if(all_xy_stations_vec_not_in_order){
        // Checks if statios are in xy order

        all_xy_stations_vec_not_in_order = false; // Set states
        all_abc_stations_vec_not_in_order = true;

        // Sorts alphabetically all_stations_vec to alphabetical order and sets status.
        sort(all_stations_vec.begin(), all_stations_vec.end(),
             [this](StationID id1, StationID id2){

            // Get xy and distances and return comparison:
            Coord coord1 = stations.find(id1)->second.xy;
            Coord coord2 = stations.find(id2)->second.xy;

            double distance1 = distrance_calc(coord1);
            double distance2 = distrance_calc(coord2);


            if(distance1 != distance2){
                // Returns station that is closer.

                return (distance1 < distance2);

            }

            else{
                // Returns closer y coordinate station.

                return (coord1.y < coord2.y);
            }

        });
    }

    return all_stations_vec;
}

/**
 * @brief Datastructures::find_station_with_coord finds if there is a station in this coord
 * @param by xy
 * @return station
 */
StationID Datastructures::find_station_with_coord(Coord xy){
    // Tries to find if there is a station in found coordinates

    for(auto &c : stations){
        // Loops through all stations and checks for match

        if(c.second.xy == xy){
            // Returns founded stations id

            return c.first;
        }
    }
    // No station found
    return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord changes stations coord
 * @param finds by id
 * @param new coord as new location
 * @return bool if success or not
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord){
    // Creates an iterator that tries to find out whether station alredy exist in the db
    Station_storage::iterator it = stations.find(id);

    if (it == stations.end()){
        // If not present return false
        return false;
    }

    // Changes stations coordinate:
    it->second.xy = newcoord;
    all_xy_stations_vec_not_in_order = true;

    return true;

}

/**
 * @brief Datastructures::add_departure adds a depaerture to station
 * @param stationid to what station
 * @param trainid unique
 * @param time what is dep time
 * @return bool if success or not
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time){
    // Adds a departure to station.

    // Creates an iterator that tries to find out whether station exist in the db
    Station_storage::iterator station_place = stations.find(stationid);

    if (station_place == stations.end()){
        // If not present return false
        return false;
    }

    // Finding if time is already added to station:


    Trains_map::iterator time_place = station_place->second.stations_trains.find(time);

    if(time_place != station_place->second.stations_trains.end()){
        // If time is already added tries to find train by id

        auto found_train = std::find(time_place->second.begin(), time_place->second.end(), trainid);

        if(found_train != time_place->second.end()){
            // If finds train returns false.

            return false;
        }

        else{
            // If train was now found adds new train under the same clock
            time_place->second.push_back(trainid);
        }
    }

    else{
        // If no time added adds new time and new vec including train id.
        std::vector<TrainID> temp;
        temp.push_back(trainid);
        station_place->second.stations_trains.insert({time, temp});

    }

    return true;
}
/**
 * @brief Datastructures::remove_departure. removes added departure
 * @param stationid as where
 * @param trainid what is to be removed
 * @param time in what time
 * @return bool if success or not
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time){
    // removes departure from station.

    // Creates an iterator that tries to find out whether station exist in the db
    Station_storage::iterator station_place = stations.find(stationid);

    if (station_place == stations.end()){

        // If not present return false
        return false;
    }

    // Finding if time is present to station:


    Trains_map::iterator time_place = station_place->second.stations_trains.find(time);
    std::vector<TrainID> &trains_time = time_place->second;


    if(time_place != station_place->second.stations_trains.end()){
        // If time is present tries to find train by id

        auto found_train = std::find(trains_time.begin(), trains_time.end(), trainid);

        if(found_train != trains_time.end()){
            // If find train. makes removals and returns false.

            // Finds train and deletes it.
            trains_time.erase(found_train);

            return true;

        }
    }

    // If removal did not happen returns false.
    return false;
}

/**
 * @brief Datastructures::station_departures_after gets departures after chosen time
 * @param stationid from what station
 * @param time after this time ->
 * @return departures after asked time
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time){
    //gets all the departures after given time:

    // Creates an iterator that tries to find out whether station exist in the db
    Station_storage::iterator station_place = stations.find(stationid);
    std::vector<std::pair<Time, TrainID>> a;

    if (station_place == stations.end()){
        // If not present return false

        a.push_back({NO_TIME,NO_TRAIN});
    }

    // Finding if time is present to station:

    else{

        Time time_new;

        if(time == 0){
            time_new = time;
        }

        else{
            time_new = time - 1; // Taking time one minute down bc upper_bound takes only val upper
        }

        // Takes iterator to times after till end of the stations train info
        Trains_map::iterator time_place = station_place->second.stations_trains.upper_bound(time_new);
        Trains_map::iterator end_point = station_place->second.stations_trains.end();


        for(Trains_map::iterator it = time_place; it != end_point; it++){
            // Loops through diffrent times with iterators.

            for(TrainID current_train : it->second){
                // Loops thorough trains in current time. Complexity is constant by average

                a.push_back({it->first, current_train});
            }
        }
    }
    // Returns leving trains or error status if was not found.
    return a;
}
/**
 * @brief Datastructures::add_region adds new region
 * @param id by what id
 * @param name by what name
 * @param coords borders of reg
 * @return bool if success or not
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords){
    // Adds region to all the db:s

    // Creates an iterator that tries to find out whether a region alredy exist in the db
    Region_storage::iterator it = regions_db.find(id);

    if (it != regions_db.end()){
        // If present DO NOT add anything
        return false;
    }

    all_regions_vec.push_back(id);

    // Initializes new region and adds it into unordered_map.
    Region new_region;

    new_region.id = id;
    new_region.name = name;
    new_region.reg_borders = coords;
    new_region.parent_reg = nullptr;

    regions_db.insert({id, new_region});

    return true;
}

/**
 * @brief Datastructures::all_regions gets all regions in db
 * @return all_regions_vec
 */
std::vector<RegionID> Datastructures::all_regions(){
    // Returns all region id:s

    return all_regions_vec;
}

/**
 * @brief Datastructures::get_region_name gets region name
 * @param id what region in case
 * @return region_name
 */
Name Datastructures::get_region_name(RegionID id){
    // Gets regions coordinates and returns them.


    // Creates an iterator that tries to find out whether region alredy exist in the db
    Region_storage::iterator it = regions_db.find(id);

    if (it == regions_db.end()){
        // If not present return NO_NAME
        return NO_NAME;
    }

    // gets station name from map and struct
    Name region_name = it->second.name;
    return region_name;
}

/**
 * @brief Datastructures::get_region_coords gets regions borders
 * @param id from what region
 * @return region_borders
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id){

    // Creates an iterator that tries to find out whether region alredy exist in the db
    Region_storage::iterator it = regions_db.find(id);

    if (it == regions_db.end()){
        // If not present return NO_COORD in a vec
        return {NO_COORD};
    }

    // gets station name from map and struct and return border info
    Region_borders_vec region_borders = it->second.reg_borders;
    return region_borders;
}
/**
 * @brief Datastructures::add_subregion_to_region adds new subregion to region
 * @param id to be sub
 * @param parentid to be sub's parent
 * @return bool if success or not
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid){
    // Adds region to parents list and gives region a paren


    // Creates an iterator that tries to find out whether target region alredy exist in the db
    Region_storage::iterator it_sub_reg = regions_db.find(id);

    // Creates an iterator that tries to find out whether target region alredy exist in the db
    Region_storage::iterator it_par_reg = regions_db.find(parentid);


    if (it_sub_reg == regions_db.end() or it_par_reg == regions_db.end()){
        // If not present return NO_COORD in a vec
        return false;
    }

    if(it_sub_reg->second.parent_reg != nullptr){
        // Makes final check that becoming sub regdo no have parent region.
        return false;
    }

    // Assignin pointer to a parent region.
    it_sub_reg->second.parent_reg = &it_par_reg->second;

    // Adding subregion to parent regions subregions vector.
    it_par_reg->second.regions_subregions.push_back(&it_sub_reg->second);

    return true;

}
/**
 * @brief Datastructures::add_station_to_region adds new station to reg
 * @param id station id
 * @param parentid parent reg
 * @return bool if success or not
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid){
    // Adds station to region's station list.


    // Creates an iterator that tries to find out whether target station alredy exist in the db
    Station_storage::iterator it_sub_stat = stations.find(id);

    // Creates an iterator that tries to find out whether target region alredy exist in the db
    Region_storage::iterator it_par_reg = regions_db.find(parentid);


    if (it_sub_stat == stations.end() or it_par_reg == regions_db.end()){
        // If not present return NO_COORD in a vec
        return false;
    }

    if(it_sub_stat->second.parent_reg != nullptr){
        // Makes final check that becoming station do no thave parent region.
        return false;
    }

    // Assignin stations pointer to a parent region.
    it_sub_stat->second.parent_reg = &it_par_reg->second;

    return true;

}
/**
 * @brief Datastructures::station_in_regions gets staions parent regions
 * @param id of station
 * @return upper_regions
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id){
    // Gets all the stations parent regions.

    std::vector<RegionID> upper_regions;

    // Creates an iterator that tries to find out whether target station alredy exist in the db
    Station_storage::iterator it_stat = stations.find(id);

    if (it_stat == stations.end()){
        // If not present return NO_COORD
        return {NO_REGION};
    }

    get_parent_regions(&upper_regions, it_stat->second.parent_reg);

    return upper_regions;
}

/**
 * @brief Datastructures::all_subregions_of_region gets all subregions
 * @param id from what region
 * @return all_subregions
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id){
    // Gets all the subregions of the asked region.
    std::vector<RegionID> all_subregions;


    // Creates an iterator that tries to find out whether region alredy exist in the db
    Region_storage::iterator it_to_reg = regions_db.find(id);

    if (it_to_reg == regions_db.end()){
        // If not present return NO_COORD in a vec
        all_subregions.push_back(NO_REGION);
    }

    else{
        // Elsewise gets all subs
        get_sub_regions(&all_subregions, &it_to_reg->second);
    }

    return all_subregions;
}
/**
 * @brief Datastructures::stations_closest_to returns 3 stations closest to target
 * @param xy compare coord
 * @return to_be_returned
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy){
    // gets top three closest stations to the point

    std::vector<StationID> to_be_returned;

    std::pair<double,StationID> first = {999999, NO_STATION};
    std::pair<double,StationID> second = {999999, NO_STATION};
    std::pair<double,StationID> third = {999999, NO_STATION};

    int size = stations.size();

    for(auto &c : stations){
        // Goes through vector to find distances

        double dist = distrance_calc(c.second.xy, xy);


        if(size <= 3){
            // If te size is below 3 add any value

            to_be_returned.push_back(c.first);
        }

        if(dist < third.first){
            // if smaller than third smalles check if smaller than second

            if(dist < second.first){
                // if smaller than second smalles check if smaller than first
                if(dist < first.first){
                    third = second;
                    second = first;
                    first = {dist, c.first};
                    continue;
                }

                third = second;
                second = {dist, c.first};

                continue;
            }

            third = {dist, c.first};
        }
    }


    if(first.second != NO_STATION){
        // Adds only one no station. If the first is no station so are the rest.
        to_be_returned.push_back(first.second);
        to_be_returned.push_back(second.second);
        to_be_returned.push_back(third.second);
    }
    return to_be_returned;
}

/**
 * @brief Datastructures::remove_station removes stations from db
 * @param id what is to be removed
 * @return bool if success or not
 */
bool Datastructures::remove_station(StationID id){
    // removes chosen station.

    Station_storage::iterator it = stations.find(id);

    if (it == stations.end()){
        // If not present return false
        return false;
    }

    stations.erase(it);

    all_stations_vec_missing_values = true;

    return true;
}

/**
 * @brief Datastructures::common_parent_of_regions gets common parents of a region
 * @param id1 first region
 * @param id2 second region
 * @return RegionID
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2){
    // Checks if regions


    // Creates an iterator that tries to find out whether target region alredy exist in the db
    Region_storage::iterator it_reg1 = regions_db.find(id1);

    // Creates an iterator that tries to find out whether target region alredy exist in the db
    Region_storage::iterator it_reg2 = regions_db.find(id2);


    if (it_reg1 == regions_db.end() or it_reg2 == regions_db.end()){
        // If not present return NO_REGION
        return NO_REGION;
    }

    if(it_reg1->second.parent_reg == nullptr or it_reg2->second.parent_reg == nullptr){
        // If present but parameters are bad return NO_REGION
        return NO_REGION;
    }

    // Save parents here.
    std::vector<RegionID>reg1_parents;
    std::vector<RegionID>reg2_parents;

    // For 1.
    get_parent_regions(&reg1_parents, it_reg1->second.parent_reg);

    // For 2.
    get_parent_regions(&reg2_parents, it_reg2->second.parent_reg);

    // Double loop checks what is the first same value and returns it
    for(auto &c : reg1_parents){
        for(auto &d : reg2_parents){

            if(c == d){
                return c;
            }
        }
    }

    return NO_REGION;
}
/**
 * @brief Datastructures::add_train Adds train into main datastructure and stations
 * @param trainid Adds by this id
 * @param stationtimes Train goes through these stations
 * @return If addition was succesfull or not
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes){
    // Adds info to the stations and Trains_db'


    if(Trains_db.find(trainid) != Trains_db.end()){return false;} // Checks if train is already in the databse


    Trains_db.insert({trainid, stationtimes}); // Inserts train into main db

    int size = stationtimes.size();

    for(int i = 0; i < size; ++i){
        // Gets all stations that this train is passing thorugh

        auto c = stationtimes[i]; //

        auto ptr_to_station = stations.find(c.first); // Station ptr

        if(ptr_to_station == stations.end()) return false; // Checks if ptr is to valid station

        if(i != size - 1){
            // At first index adds only next station.

            auto next_station = stationtimes[i + 1];

             //

            int time_between_stations = stationtimes[i + 1].second - stationtimes[i].second;

            auto ptr_to_next_station = stations.find(next_station.first);

            ptr_to_station->second.connected_stations.push_back({&ptr_to_next_station->second,  time_between_stations});
        }



        add_departure(c.first, trainid, c.second);

    }

    return true;
}


/**
 * @brief Datastructures::next_stations_from Gets next stations from selected station
 * @param id Finds station by this id
 * @return all next stations of this station in vec.
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id){

    // checks existance of the station
    auto ptr_to_station = stations.find(id);

    if(ptr_to_station == stations.end()) return {NO_STATION}; // If not a real station

    std::vector<StationID> connections;

    // Gets connections from a station and returns them.
    for(auto current_station : ptr_to_station->second.connected_stations){
      connections.push_back(current_station.first->id);

    }

    return connections;
}


/**
 * @brief Datastructures::train_stations_from  Finds trains stations after this station
 * @param stationid from this station gets data
 * @param trainid with this trains route.
 * @return vector containing all remaining stations.
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid){


    // Checks if station exists. If not return no station
    auto ptr_to_station = stations.find(stationid);
    if(ptr_to_station == stations.end()) return {NO_STATION};

    // Create switches for return
    std::vector<StationID> return_stations;
    bool push_switch = false;


    for(auto& c : ptr_to_station->second.stations_trains){
        // Loops though all pair<time, vector<train>> values

        for(TrainID& train : c.second){
            // Gets train from the vector

            if(train == trainid){
                // If train is found...

                auto ptr_to_train = Trains_db.find(trainid);

                for(auto& station_time : ptr_to_train->second){
                    // Gets remaining stations from station time vector

                    StationID station = station_time.first;


                    if(push_switch){
                        // Remaining values are going to the bucket

                        return_stations.push_back(station);
                    }

                    if(station == stationid){
                        // If station found set switch true

                        push_switch = true;
                    }

                }
                // Returns found values

                if(return_stations.size() == 0){

                    return_stations.push_back({NO_STATION});
                }

                return return_stations;
            }
        }
    }

    // If value not found goes thorugh all the loops.
    return {NO_STATION};

}


/**
 * @brief Datastructures::clear_trains Clears all Trains from datastructure and stations.
 */
void Datastructures::clear_trains(){

    Trains_db.clear();

    for(auto& station : stations){
        // Clears all route data and trains added to station.

        station.second.connected_stations.clear();
        station.second.stations_trains.clear();

    }

}
/**
 * @brief Datastructures::route_any Finds a route from starting station to ending stations
 * @param fromid Start point
 * @param toid End point
 * @return Vector containing route stations and distance from start.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid){

    // Checks if stations were found
    auto ptr_to_fromid = stations.find(fromid);
    auto ptr_to_toid = stations.find(toid);

    // If one not found returns vector...
    if(ptr_to_fromid == stations.end() or ptr_to_toid == stations.end()) return {{NO_STATION, NO_DISTANCE}};


    return bfs(&ptr_to_fromid->second, &ptr_to_toid->second);


}


/**
 * @brief Datastructures::route_least_stations Returns route with least stations.
 * @param fromid Where we are coming from
 * @param toid Where we are headed
 * @return Vector containing route stations and distance from start.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid){

    // Uses route any becauset it uses bfs. whitch is the best way to find route with least stations
    return route_any(fromid, toid);
}

std::vector<StationID> Datastructures::route_with_cycle(StationID fromid){

    auto ptr_to_fromid = stations.find(fromid);

    // If one not found returns vector...
    if(ptr_to_fromid == stations.end()) return {NO_STATION};

    // Initiate recursive dfs

    clear_node_data();
    need_clear_node_data = true;

    // Route is saved here
    std::vector<StationID> return_vec;

    recursive_dfs(&ptr_to_fromid->second, &return_vec); // Performs the great search

    if(cycle_true){
        return return_vec;
    }

    // If cycle is not present
    return {};


}

/**
 * @brief Datastructures::route_shortest_distance Finds a route with with shortest distance
 * @param fromid from here
 * @param toid to here
 * @return the shortest distance route.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID fromid, StationID toid){

    // Checks if stations were found
    auto ptr_to_fromid = stations.find(fromid);
    auto ptr_to_toid = stations.find(toid);

    // If one not found returns vector...
    if(ptr_to_fromid == stations.end() or ptr_to_toid == stations.end()) return {{NO_STATION, NO_DISTANCE}};

    std::vector<std::pair<StationID, int>> path;

    a_star(&ptr_to_fromid->second, &ptr_to_toid->second, &path);

    return path;

}
/**
 * @brief Datastructures::route_earliest_arrival Finds a route that has earliest arrival time
 * @param fromid from here
 * @param toid To end point
 * @param starttime when is started
 * @return The asked route
 */
std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID fromid, StationID toid, Time starttime){
    // Checks if stations were found
    auto ptr_to_fromid = stations.find(fromid);
    auto ptr_to_toid = stations.find(toid);

    // If one not found returns vector...
    if(ptr_to_fromid == stations.end() or ptr_to_toid == stations.end()) return {{NO_STATION, NO_TIME}};

    // Starting with the basics
    clear_node_data();
    need_clear_node_data = true;


    // Making places to data save
    std::vector<std::pair<StationID, Time>> path;

    std::vector<std::vector<std::pair<StationID, Time>>> possible_paths;

    fastest_route(&ptr_to_fromid->second, &ptr_to_toid->second, starttime, path, &possible_paths);

    std::vector<std::pair<StationID, Time>> selected;

    for(auto path : possible_paths){
        // Finds the best route  from available options

        if(selected.size() == 0){
            selected = path;
        }

        if((path.end() - 1)->second < (selected.end() - 1)->second ){
            selected = path;
        }


    }

    return selected;
}

