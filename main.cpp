#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct ivec2
{
    int x;
    int y;
};

int main()
{
    const int k_checkpoint_radius = 600;
    const char * k_thrust_boost = "BOOST";
    bool is_boost_available = true;

    // game loop
    while (true)
    {
        ivec2 pod_pos;
        ivec2 oponent_pod_pos;
        ivec2 next_checkpoint_pos;
        int next_checkpoint_dist;
        int next_checkpoint_angle;
        
        cin >> 
        pod_pos.x >> pod_pos.y >> 
        next_checkpoint_pos.x >> next_checkpoint_pos.y >> 
        next_checkpoint_dist >> 
        next_checkpoint_angle >> 
        oponent_pod_pos.x >> oponent_pod_pos.y;
        cin.ignore();

        ivec2 target_pos = next_checkpoint_pos;

        cout << target_pos.x << " " << target_pos.y << " ";

        int thrust;

        if (next_checkpoint_angle > 90 || next_checkpoint_angle < -90)
        {
            thrust = 10;
        }
        else 
        {
            thrust = 100;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"

        if (is_boost_available && next_checkpoint_dist > k_checkpoint_radius * 3 && thrust == 100)
        {
            cout << k_thrust_boost << endl;
            is_boost_available = false;
        }
        else 
        {
            cout << thrust << endl;
        }
    }
}
