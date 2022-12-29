Railways 1.0

In this project I have implemented trains location and timetable system. There is also
Regions and stations implemeted in the program. Please feel free to explore and test it


This project is based on a school project but has later improved and transformed into completely
my own work. Main idea is to write as efficient as possible functions using STL library functions
Function efficiency evaluations can be found in datastrucutres.h. UI part is mainly to test and 
use Datastructures class. 


GUIDE: 

command p1 p2 p3 p4 :- explanation

NOTE:
StationID is type: std::string
Name is type: std::string
Coord is type: int,int
TrainId is type std::string 
Time is type: int
RegionID is type: int


==================== COMMANDS =========================

read filename :- Reads commands from file. Commands are similar as input would be

station_count :- Returns the number of stations

all_stations :- Returns all the stations in any (arbitrary) order

add_station StationID Name x,y  :- Adds a station to the data structure

station_info StationID :- Returns the name and coord of the station

stations_alphabetically :- Returns station IDs sorted alphabetical.

stations_distance_increasing :- Returns station IDs sorted dist from 0,0

find_station_with_coord x,y :- Returns a station with the given coordinate

change_station_coord StationID x,y :- Changes the location of the station with given ID

add_departure StationID TrainID Time :- Adds information that the given train leaves 
					from the given station at the given time.

remove_departure StationID TrainID Time :- Removes the given train departure from the 
				 	   given station at the given time

station_departures_after StationID Time :- Lists all train departures from the given 
					   station at or after the given time.

add_region ID Name Coord1 Coord2... :- Adds a region to the data structure with given 
				       unique id, name and polygon (coordinates)

all_regions :- Returns all the regions in any (arbitrary) order 

region_info RegionID :- Returns the name and coordinates of the region with the given ID

add_subregion_to_region RegionID1 RegionID2 :- Adds the first given region as a subregion
					       to the second region.

add_station_to_region StationID RegionID :- Adds the given station to the given region

station_in_regions StationID :- Returns a list of regions to which the given station
				belongs either directly or indirectly.

all_subregions_of_region RegionID :- Returns a list of regions which belong either 
				     directly or indirectly to the given region.

stations_closest_to Coord :- Returns the three stations closest to the given
			     coordinate in order of increasing distance

remove_station StationID :- Removes the station with the given id

common_parent_of_regions RegionID1 RegionID2 :- Returns the “nearest” region in the subregion
						hierarchy, to which both given regions belong
						either directly or indirectly.

clear_trains :- Clears out all trains, but doesn’t touch stations or regions

add_train ID Station1:Time1 Station2:Time2... StationN:TimeN :- Adds a train to the data 
structure with given unique id. The train runs through the given stations and departs
from them at given times. 

next_stations_from StationID :- Returns the stations that are immediate next stations on
				trains running through the given station

train_stations_from StationID TrainID :- Returns a list of stations, through which the 
					 given train runs after leaving the given station

route_any StationID1 StationID2 :- Returns any (arbitrary) route between the given stations.

route_least_stations StationID1 StationID2 :- Returns a route between the given stations so 
					      that it contains as few stations as possible

route_with_cycle StationID :- Returns a route starting from the given station such that
			      the route has a single cycle

route_shortest_distance StationID1 StationID2 :- Returns a route between the given stations so 
						 that its length is as small as possible.

route_earliest_arrival StationID1 StationID2 StartTime :- Returns a route between the given 
stations that arrives to the destination as early as possible.
 