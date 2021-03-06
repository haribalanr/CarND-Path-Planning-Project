#ifndef Car_H
#define Car_H
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Car
{
  int delay  = 0;
  int lane = 1;

  double speed_limit = 49.5; //.5 less then 50mph limit
  double safety_space = 30; // safe dist
  
  bool left_is_free = true;
  bool right_is_free = true;

  double space_on_left = 10000.0;
  double space_on_right = 10000.0;

  
public:
  Car();
  ~Car();

  bool is_close = false;
  bool car_ahead = false;

  double other_car_vel = 0.0;

  void start(int lane, double speed_limit);
  int next_lane(vector<vector<double>> sensor_fusion, int current_lane, double car_s, double end_path_s, int prev_size);

  vector<vector<double>> predict_others(vector<vector<double>> sensor_fusion, int current_lane);
  vector<double> change_state_costs(int current_lane, vector<vector<double>> predictions);
  
};

#endif /* Car_H */
