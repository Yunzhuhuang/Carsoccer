/** CSci-4611 Assignment 2:  Car Soccer
 */ 

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>

/// Small data structure for a car
class Car {
public:
    
    /// The constructor sets the static properties of the car, like its size,
    /// and then calls Reset() to reset the position, velocity, and any other
    /// dynamic variables that change during game play.
    Car() : size_(3,2,4), collision_radius_(2.5) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Car() {}

    /// Resets all the dynamic variables, so if you call this after a goal, the
    /// car will go back to its starting position.
    void Reset() {
        position_ = Point3(0, size_[1]/2, 45);
        angle_ = 0.0;
        speed_= 0;
        turn_rate_ = 0.5;
    }
    
    float collision_radius() { return collision_radius_; }
    
    Vector3 size() { return size_; }
    
    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }
    Vector3 velocity() {
        return Vector3(-sin(angle_), 0, -cos(angle_)) * speed_;
        
       
    }
    
    void set_velocity(float a, float s) {
        angle_ = a;
        speed_ = s;
    }
    
    void set_angle(float a) {
        angle_ = a;
    }
    float angle() {
        return angle_;
    }
    void set_speed(float s) {
        speed_ = s;
    }
    float speed() {
        return speed_;
    }
    float turn() {
        return turn_rate_;
    }
    
    
   
   
    
    
private:
    // You will probably need to store some additional data here, e.g., speed.
    
    Vector3 size_;
    float collision_radius_;
    Point3 position_;
   
   float speed_;
    float angle_;
    float turn_rate_;
};

#endif
