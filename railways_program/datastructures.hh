// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// From prg1
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <queue>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //
    // Old assignment 1 operations
    //
    // Estimate of performance: O(1)
    // Short rationale for estimate:
    //Has 1 operation: size of unordered map which is always constant
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    // Clear function is linear. Clears all regions(2*m) and all the station(3*n)
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Gets station id:s from a list that already exists.
    // Returns a pre_existing vector that has needed values.
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Adds new value to datastructures. On average
    // Every operation is const. but the worst case is log(n)
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate:. Performs one operation. umap.find() is const on average.
    // but the worst case is log(n)
    Name get_station_name(StationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Works similarly as get_station_name() parameter is only
    // diffrent that is searched for. the worst case is log(n)
    Coord get_station_coordinates(StationID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Vector sort is always n(log(n)) and inside lambda is const
    //but if vector is already sorted (Best case) timing is constant
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Vector sort is always n(log(n)) and inside lambda is const
    //but if vector is already sorted (Best case) timing is constant. Similar mechanism as above
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Loops station values in for loop which is linear operation.
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Uses umap.find that is constant.
    // Also do not have any linear operations. Therefore operation is near to const.
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(9*n)
    // Short rationale for estimate: Uses umap.find that is constant.
    // has a map that has log(n) find operation. is also more complex due the datastructures.
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Uses umap.find that is constant.
    // has a map that has log(n) find operation. Has similar functionality as add departure. But is
    // inverted. On average these operations are bit better
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n^2).
    // In theory. On average log(n) or const.
    // Short rationale for estimate: Has two for loops but in real world there are rarely more than 5 trains
    // Leaving same time. Truthfull efficiency is always better.
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Adds new value to datastructures. On average
    // Every operation is const.
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns an existing vector. No computation needed.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Works similarly as get_region name. Finds value from umap and returns it
    Name get_region_name(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Works similarly as get_region_xy. Finds value from umap and returns it
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Has couple of find from umap that are constant on average.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Has couple of find from umap that are constant on average.
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Has couple of find from umap that are constant on average.
    // Also has recursive function but acording the rules it has pretty constant runtime.
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: has linear operations in total
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: has linear operation
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: has linear operations in total
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: has linear operations in total
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity. On averga this is although constant
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity.
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n)
    // Short rationale for estimate:clear all is linear compleksity
    void clear_trains();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity.´
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity. NOTE! SAME PERFORMANCE AS route_any
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Stations.find has at worst nlogn compleksity
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: VERY BAD PERFORMANCE USES FORLOOPS INSIDE FOR LOOPS
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:
    // Add stuff needed for your class implementation here



    // *** REGION RELATED FUNCTIONALITY ***

    using Region_borders_vec = std::vector<Coord>;

    // Stores information per region
    struct Region{
        RegionID id;
        Name name;
        Region_borders_vec reg_borders;
        Region* parent_reg;
        std::vector<Region*> regions_subregions;
    };

    // Stores information related to all the regions.
    using Region_storage = std::unordered_map<RegionID ,Region>;


    Region_storage regions_db;

    // Stores all the id:s of regions. Improves efficiency.
    using Regions_id_vec = std::vector<RegionID>;

    Regions_id_vec all_regions_vec;



    // *** STATION RELATED FUNCTIONALITY ***

    // Stores information of stations trains
    using Trains_map = std::map<Time, std::vector<TrainID>>;




    // Stores information per station.
    struct Station_info{
        StationID id;
        Name name;
        Coord xy;
        Trains_map stations_trains;
        Region* parent_reg;
        std::vector<std::pair<Station_info*, Time>> connected_stations;

        // Inner stuct if for prg2
        struct Station_node{
            int colour = 0;
            int d = 999999999;
            int de = 999999999;
            Station_info* prev_node = nullptr;

        };

        Station_node node;
    };



    // Diffrent storages for diffrent usages
    using Station_storage = std::unordered_map<StationID ,Station_info>;

    using Station_name_storage = std::vector<StationID>;


    // Using 3 diffrent db:s to store data:

    // Stores data by station ID.
    Station_storage stations;

    //Stores ID:s in general for giving out. Normal state is ordered alphabetically.
    // To set this state requires to RUN: stations_alphabetically().
    Station_name_storage all_stations_vec;
    bool all_abc_stations_vec_not_in_order = true;
    bool all_xy_stations_vec_not_in_order = true;
    bool all_stations_vec_missing_values = false;


    // *** TRAINS RELATED FUNCTIONALITY ***

    using All_existing_trains = std::unordered_map<TrainID, std::vector<std::pair<StationID, Time>>>;


    All_existing_trains Trains_db;


    //*** Defining private functions ***


    // Estimate of performance: 1
    // Short rationale for estimate: Performs one line of mathematical operation. Efficiency is constant.
    double distrance_calc(Coord xy1, Coord xy2 = {0, 0});

    // Estimate of performance: 3
    // Short rationale for estimate:
    // - in theory linear. But bc of the rules maxium n. of loops is always < 3
    void get_parent_regions(std::vector<RegionID>* parents, Region* reg);

    // Estimate of performance: 3
    // Short rationale for estimate:
    // - in theory linear. But bc of the rules maxium n. of loops is always < 3
    void get_sub_regions(std::vector<RegionID>* subs, Region* reg);

    // Estimate of performance: n
    // Short rationale for estimate:
    // Linear loop.
    void redo_all_stations_vec();

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    void pathbuilder(Station_info* begin, Station_info* current, std::vector<std::pair<StationID, int>>* path);

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    std::vector<std::pair<StationID, Distance>> bfs(Station_info* start, Station_info* goal);

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    bool cycle_true = false;
    void recursive_dfs(Station_info* start, std::vector<StationID>* path);

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    void relax_a_star(Station_info* u, Station_info* v, Station_info* g);

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    void a_star(Station_info* start, Station_info* goal ,std::vector<std::pair<StationID, int>>* path);

    void fastest_route(Station_info* start, Station_info* goal,
                       Time start_time,
                       std::vector<std::pair<StationID, Time>> path,
                       std::vector<std::vector<std::pair<StationID, Time>>>* possible_paths);


    // Checks if cleaning is necessary.
    bool need_clear_node_data = false;

    // Estimate of performance: n
    // Short rationale for estimate: Linear implementation
    void clear_node_data();

};

#endif // DATASTRUCTURES_HH
