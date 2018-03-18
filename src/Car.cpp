#include "Car.h"
#include <iostream>
#include <vector>

using namespace std;

Car::Car() {}

Car::~Car() {}

void Car::start(int lane, double speed_limit){
  this->lane = lane;
  this->speed_limit = speed_limit;
}

int Car::next_lane(vector<vector<double>> sensor_fusion, int current_lane, double car_s, double end_path_s, int prev_size){
  
  // update lane
  lane = current_lane;

  is_close = false;

  left_is_free = true;
  right_is_free = true;

  space_on_left = 10000.0;
  space_on_right = 10000.0;
  

  // check which lanes are not free depending on the current lane
  if (lane==0)
  {
    left_is_free = false;
  }
  if (lane==2)
  {
    right_is_free = false;
  }

    // check through the data (cars) from sensor fussion output
    for (int i = 0; i < sensor_fusion.size(); i++)
    {
      float d = sensor_fusion[i][6];

      // observe lane of car
      int lane_obsvd = fabs(d/4);

      // check speed of car in our lane
      double vx = sensor_fusion[i][3];
      double vy = sensor_fusion[i][4];
      double check_speed = sqrt(vx*vx+vy*vy);
      double check_car_s = sensor_fusion[i][5]; // the s coordinate of the car
      double check_car_d = sensor_fusion[i][6];
      // project car position
      check_car_s += ((double)prev_size*0.02*check_speed);
      double space = check_car_s - end_path_s;

      double car_dist = check_car_s - car_s;

      // if car is in front, within 30m
      if ((car_dist > 0) && (car_dist < 30))
      {
        if (lane_obsvd == (lane-1)){ // if an observed car is on the left side
            if (space < space_on_left) {space_on_left = space;}
            left_is_free = false;
          }
          if (lane_obsvd == (lane+1)){ // if an observed car is on the right side
              if (space < space_on_right) {space_on_right = space;}
            right_is_free = false;
          }
      } 
      // if car is at the back, within 20m
      else if ((car_dist < 0) && (car_dist > -20))
      {
        if (lane_obsvd == (lane-1)){ // if an observed car is on the left side
            left_is_free = false;
          }
          if (lane_obsvd == (lane+1)){ // if an observed car is on the right side
            right_is_free = false;
          }
      }

      
      // if observed car is in my lane:
      if ((d<2+4*lane+2) && d > (2+4*lane-2))
      { 
        // if observed car is too close
        if ((check_car_s > car_s) && ((check_car_s-car_s)<safety_space))
        {
          is_close = true;

          if (lane_obsvd == (lane-1)){ // if an observed car is on the left side
            // if (space < space_on_left) {space_on_left = space;}
            left_is_free = false;
          }
          if (lane_obsvd == (lane+1)){ // if an observed car is on the right side
            // if (space < space_on_right) {space_on_right = space;}
            right_is_free = false;
          }
        }
      }
    } // end of search through sensor fussion


    // if car is too close
    if (is_close) 
    {
      if ((lane==0) && right_is_free)
      {
          lane = 1;
      }
      else if (lane==1)
      {
        if (left_is_free && right_is_free)
        {
          if (space_on_right > space_on_left)
          {
            lane += 1;
          } else {
            lane -= 1;
          }
        } else if (left_is_free) { 
          lane -= 1;
        } else if(right_is_free) {
          lane += 1;
        }
      }
      else if ((lane==2) && left_is_free)
      {
          lane = 1;
      }
    }

    if (lane>2)
    {
      lane = 2;
    }

    // avoid instantaneous lane change by applying delay
    if (lane != current_lane){
    		delay ++;
    } else {
    		delay = 0;
    }
    if (delay < 12){
      lane = current_lane;
    } else {
    		delay = 0;
    }
    return lane;
}

// prediction for other cars near by
vector<vector<double>> predict_others(vector<vector<double>> sensor_fusion, int current_lane){
  return sensor_fusion;
}

vector<double> change_state_costs(int current_lane, vector<vector<double>> predictions){
  vector<double> costs;
  return costs;
}
