#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// ===================
// === Global defs ===
// ===================

// fixed parameters
#define _MPR_CHECKPOINT_TRIGGER_RADIUS 600
#define _MPR_POD_COLLISION_RADIUS 400
#define _MPR_BOOST_EXECUTE_STRING "BOOST"
#define _MPR_STARTING_BOOST_COUNT 1

// modifiable variables
#define _MPR_MAX_HEADING_ERROR 90

// =================
// === 2D Vector ===
// =================

struct Vec2
{
    float x;
    float y;

    Vec2(float f = 0.0f)
        : x(f), y(f)
    { }

    Vec2(float x, float y)
        : x(x), y(y)
    { }

    float dot(const Vec2 & rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    float magnitude() const
    {
        return std::sqrt(dot(*this));
    }

    void normalise()
    {
        *this /= magnitude();
    }

    Vec2 normal() const
    {
        return Vec2(x, y) /= magnitude();
    }

    float distance(Vec2 rhs) const
    {
        return (rhs -= *this).magnitude();
    }

    Vec2 & operator+=(const Vec2 & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2 & operator-=(const Vec2 & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2 & operator*=(const Vec2 & rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vec2 & operator/=(const Vec2 & rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }
};

inline Vec2 operator+(Vec2 lhs, const Vec2 & rhs)
{
    return lhs += rhs;
}

inline Vec2 operator-(Vec2 lhs, const Vec2 & rhs)
{
    return lhs -= rhs;
}

inline Vec2 operator*(Vec2 lhs, const Vec2 & rhs)
{
    return lhs *= rhs;
}

inline Vec2 operator/(Vec2 lhs, const Vec2 & rhs)
{
    return lhs /= rhs;
}

inline bool operator==(const Vec2 & lhs, const Vec2 & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Vec2 & lhs, const Vec2 & rhs)
{
    return !(lhs == rhs);
}

// =====================
// === State Tracker ===
// =====================

class StateTracker
{
public:

    StateTracker()
        : m_current_lap(1), m_all_checkpoints_found(false)
    { }

    void processTargetCheckpoint(int target_checkpoint_x, int target_checkpoint_y)
    {
        if (m_all_checkpoints_found)
        {
            return;
        }

        Vec2 new_checkpoint_position(target_checkpoint_x, target_checkpoint_y);

        if (m_checkpoint_positions.empty())
        {
            m_checkpoint_positions.push_back(new_checkpoint_position);
        }
        else if (m_checkpoint_positions.back() != new_checkpoint_position)
        {
            if (m_checkpoint_positions.front() == new_checkpoint_position)
            {
                m_all_checkpoints_found = true;
                ++m_current_lap;
            }
            else
            {
                m_checkpoint_positions.push_back(new_checkpoint_position);
            }
        }
    }

private:

    int m_current_lap;
    std::vector<Vec2> m_checkpoint_positions;
    bool m_all_checkpoints_found;
};

// ====================
// === Boost Helper ===
// ====================

class BoostHelper
{
public:

    BoostHelper()
        : m_boosts_remaining(_MPR_STARTING_BOOST_COUNT)
    { }

    bool tryUseBoost()
    {
        // ideally if we only have one boost available then we want to use it on the longest stretch.
        // this means waiting until the second lap (after we know we've found all checkpoints) and
        // using the boost between the two checkpoints with the greatest distance between them.

        if (m_boosts_remaining <= 0)
        {
            return false;
        }

        --m_boosts_remaining;

        return true;
    }

private:

    uint m_boosts_remaining;
};

// ====================
// === Entry point! ===
// ====================

int main()
{
    StateTracker state;
    BoostHelper boost_helper;

    // game loop
    while (true)
    {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
        int opponentX;
        int opponentY;
        cin >> opponentX >> opponentY; cin.ignore();

        state.processTargetCheckpoint(next_checkpoint_x, next_checkpoint_y);

        // TARGET CONTROL

        Vec2 target_pos(next_checkpoint_x, next_checkpoint_y);

        cout << static_cast<int>(target_pos.x) << " " << static_cast<int>(target_pos.y) << " ";
        
        // THRUST CONTROL

        if (boost_helper.tryUseBoost())
        {
            cout << _MPR_BOOST_EXECUTE_STRING << endl;
        }
        else 
        {
            int thrust;

            int heading_error = abs(next_checkpoint_angle);

            if (heading_error > _MPR_MAX_HEADING_ERROR)
            {
                thrust = 0;
            }
            else 
            {
                thrust = 100;
            }

            cout << thrust << endl;
        }
    }
}
