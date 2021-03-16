/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>

/// Small data structure for a ball
class Ball {
public:
    
    /// The constructor sets the radius and calls Reset() to start the ball at
    /// the center of the field
    Ball() : radius_(2.6f) {
        Reset();
    }
    
    /// Nothing special needed in the constructor
    virtual ~Ball() {}

    
    void Reset() {
        position_ = Point3(0, radius_, 0);
        velocity_ = Vector3(pow((-1.0),(double)(rand()%2))*(rand()%40),  (rand()%40), pow((-1.0),(double)(rand()%2))*(rand()%20));
        accelerate_ = Vector3(0.0f, 0.0f, 0.0f);
    }

    float radius() { return radius_; }
    
    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }
    Vector3 velocity() {return velocity_; }
    void set_velocity(const Vector3 &v) { velocity_ = v; }
    Vector3 accelerate() {return accelerate_; }
    void set_accelerate(const Vector3 &a) { accelerate_ = a; }
    
private:
    // You will probably need to store some additional data here, e.g., velocity
    Vector3 velocity_;
    Point3 position_;
    float radius_;
    Vector3 accelerate_;
};

#endif
