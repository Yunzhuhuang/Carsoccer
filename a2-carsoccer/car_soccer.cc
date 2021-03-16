/** CSci-4611 Assignment 2:  Car Soccer
 */
#define _USE_MATH_DEFINES
#include "car_soccer.h"
#include "config.h"
CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer"), use_mouse_(false) {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}
CarSoccer::~CarSoccer() {
}
void CarSoccer::OnMouseMove(const mingfx::Point2& pos, const Vector2& delta)
{
    mouse_pos_ = PixelsToNormalizedDeviceCoords(pos);
}

void CarSoccer::car_motion(double dt) {
    
    if (IsKeyDown(GLFW_KEY_LEFT))
        car_.set_angle(car_.angle()+car_.turn()*car_.speed()*dt);
    if (IsKeyDown(GLFW_KEY_RIGHT))
        car_.set_angle(car_.angle()-car_.turn()*car_.speed()*dt);
    if (IsKeyDown(GLFW_KEY_UP)){
        car_.set_speed(car_.speed()+0.02f);
    }
    if (IsKeyDown(GLFW_KEY_DOWN)){
        car_.set_speed(car_.speed()-0.02f);
    }   
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}
void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
   
    //update the car
    Point3 old_pos1 = car_.position();
    car_motion(timeStep);
    
    Car_collision_boundary(timeStep, old_pos1, car_.velocity());
   
    car_.set_position(car_.position() + car_.velocity()*timeStep);
    
    
    
    
    
    //update the ball
    Point3 pos = ball_.position();
    Vector3 move = ball_.velocity();
    Ball_collision_boundary(timeStep, pos, move);
    Collision_car(timeStep, ball_.position(), ball_.velocity());
    hitgoals(timeStep, ball_.position(), ball_.velocity());
    ball_.set_position(ball_.position() + ball_.velocity()*timeStep);
    car_.set_position(car_.position() +car_.velocity()*timeStep);
   
     
    
   
    
}

void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}

void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);
    
    // Draw the field with the field texture on it.
    Color col(16.0f/255.0f, 46.0f/255.0f, 9.0f/255.0f);
    Matrix4 M = Matrix4::Translation(Vector3(0.0f, -0.201f, 0.0f)) * Matrix4::Scale(Vector3(50.0f, 1.0f, 60.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f)) * Matrix4::Scale(Vector3(40.0f, 1.0f, 50.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // Draw the car
    Color carcol(0.8f, 0.2f, 0.2f);
    Vector3 axis = Vector3(0,1,0);
    float angle = GfxMath::ToRadians(car_.angle()*180/M_PI);
    
    Matrix4 Mcar =
        Matrix4::Rotation(car_.position(), axis, angle)*
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f));
    
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);
    
    
    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);
    
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2f, 0.4f, 0.15f);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1f, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);
    
    
   //Draw the boundary
    Color colgrid(1,1,1);
    std::vector<Point3> loop1;
    loop1.push_back(Point3(-40.0, 0.0, 50.0));
    loop1.push_back(Point3(-40.0, 0.0, -50.0));
    loop1.push_back(Point3(40.0, 0.0,  -50.0));
    loop1.push_back(Point3(40.0, 0.0,  50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgrid, loop1, QuickShapes::LinesType::LINE_LOOP, 0.1);
    std::vector<Point3> loop2;
    loop2.push_back(Point3(40.0, 0.0, -50.0));
    loop2.push_back(Point3(40.0, 35.0, -50.0));
    loop2.push_back(Point3(40.0, 35.0,  50.0));
    loop2.push_back(Point3(40.0, 0.0,  50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgrid, loop2, QuickShapes::LinesType::LINE_LOOP, 0.1);
    std::vector<Point3> loop3;
    loop3.push_back(Point3(-40.0, 0.0, 50.0));
    loop3.push_back(Point3(-40.0, 35.0, 50.0));
    loop3.push_back(Point3(-40.0, 35.0, -50.0));
    loop3.push_back(Point3(-40.0, 0.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgrid, loop3, QuickShapes::LinesType::LINE_LOOP, 0.1);
    std::vector<Point3> loop4;
    loop4.push_back(Point3(-40.0, 35.0, 50.0));
    loop4.push_back(Point3(40.0, 35.0,  50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgrid, loop4, QuickShapes::LinesType::LINE_LOOP, 0.1);
    std::vector<Point3> loop5;
    loop5.push_back(Point3(-40.0, 35.0, -50.0));
    loop5.push_back(Point3(40.0, 35.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgrid, loop5, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    //Draw the goals
    Color colgoal1(0.8f,0.8f,0);
    std::vector<Point3> goal1;
    goal1.push_back(Point3(-10.0, 0.0, 50.0));
    goal1.push_back(Point3(-10.0, 10.0, 50.0));
    goal1.push_back(Point3(10.0, 10.0, 50.0));
    goal1.push_back(Point3(10.0, 0.0, 50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal1, goal1, QuickShapes::LinesType::LINE_LOOP, 0.1);
    float y = 9.0;
    float x = -9.5;
    while(y>0){
      std::vector<Point3> goaly1;
     
      goaly1.push_back(Point3(-10.0, y, 50.0));
      goaly1.push_back(Point3(10.0, y, 50.0));
      quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal1, goaly1, QuickShapes::LinesType::LINE_STRIP, 0.05);
        y -= 1;
    }
    while(x<10){
      std::vector<Point3> goalx1;
     
      goalx1.push_back(Point3(x, 10, 50.0));
      goalx1.push_back(Point3(x, 0, 50.0));
      quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal1, goalx1, QuickShapes::LinesType::LINE_STRIP, 0.05);
        x += 1;
    }
    
    Color colgoal2(0,0,1);
    std::vector<Point3> goal2;
    goal2.push_back(Point3(-10.0, 0.0, -50.0));
    goal2.push_back(Point3(-10.0, 10.0, -50.0));
    goal2.push_back(Point3(10.0, 10.0, -50.0));
    goal2.push_back(Point3(10.0, 0.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal2, goal2, QuickShapes::LinesType::LINE_LOOP, 0.1);
    y = 9;
    x = -9.5;
    while(y>0){
      std::vector<Point3> goaly2;
     
      goaly2.push_back(Point3(-10.0, y, -50.0));
      goaly2.push_back(Point3(10.0, y, -50.0));
      quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal2, goaly2, QuickShapes::LinesType::LINE_STRIP, 0.05);
        y -= 1;
    }
    while(x<10){
      std::vector<Point3> goalx2;
     
      goalx2.push_back(Point3(x, 10,- 50.0));
      goalx2.push_back(Point3(x, 0, -50.0));
      quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, colgoal2, goalx2, QuickShapes::LinesType::LINE_STRIP, 0.05);
        x += 1;
    }
}

//Control the ball-boundary collision
void CarSoccer::Ball_collision_boundary(double dt, Point3 pos, Vector3 move) {
    if(move[1]*dt>0.0f&&pos[1]>2.6f) {
        ball_.set_accelerate(Vector3(0,-9.8f*dt,0));
        Vector3 a = ball_.accelerate();
        move[1] = move[1] + a[1];
    }
    if(move[1]*dt<0.0f&&pos[1]>2.6f) {
        ball_.set_accelerate(Vector3(0,-9.8f*dt,0));
        Vector3 a = ball_.accelerate();
        move[1] = move[1] + a[1];
    }
    if(pos[0]+move[0]*dt <= -40+2.6 ) {
        pos = Point3(-40+2.6, pos[1],pos[2]);
        Vector3 n =  Vector3(1,0,0);
        move =(move - 2*(move.Dot(n))*n)*0.8;
        
    }
    if(pos[0]+move[0]*dt>= 40-2.6) {
        pos = Point3(40-2.6, pos[1],pos[2]);
        Vector3 n =  Vector3(-1,0,0);
        move = (move - 2*(move.Dot(n))*n)*0.8;
    }
    if(pos[2] + move[2]*dt<= -50+2.6) {
        pos = Point3(pos[0], pos[1],-50+2.6);
        Vector3 n =  Vector3(0,0,1);
        move = (move - 2*(move.Dot(n))*n)*0.8;
    }
    if(pos[2] +move[2]*dt>= 50-2.6) {
        pos = Point3(pos[0], pos[1],50-2.6);
        Vector3 n =  Vector3(0,0,-1);
        move = (move - 2*(move.Dot(n))*n)*0.8;
    }
    if(pos[1]+move[1]*dt <= 2.6) {
        pos = Point3(pos[0], 2.6, pos[2]);
        if(move[1] >= -0.1&&move[1] <= 0.1) {
          Vector3 n =  Vector3(0,1,0);
          move = (move - 2*(move.Dot(n))*n)*0.8;
        }
    }
    if(pos[1]+move[1]*dt >= 35-2.6) {
        pos = Point3(pos[0], 35-2.6, pos[2]);
        Vector3 n =  Vector3(0,-1,0);
        move = (move - 2*(move.Dot(n))*n)*0.8;
    }
    ball_.set_position(pos);
    ball_.set_velocity(move);
}

//Control the car-boundary collision
void CarSoccer::Car_collision_boundary(double dt, Point3 pos, Vector3 move) {
    
    if(pos[0]+move[0]*dt <= -40+1.5 ) {
        pos = Point3(-40+1.5, pos[1],pos[2]);
        Vector3 n =  Vector3(1,0,0);
    }
    if(pos[0]+move[0]*dt>= 40-1.5) {
        pos = Point3(40-1.5, pos[1],pos[2]);
        Vector3 n =  Vector3(-1,0,0);
    }
    if(pos[2] + move[2]*dt<= -50+2) {
        pos = Point3(pos[0], pos[1],-50+2);
        Vector3 n =  Vector3(0,0,1);
    }
    if(pos[2] +move[2]*dt>= 50-2) {
        pos = Point3(pos[0], pos[1],50-2);
        Vector3 n =  Vector3(0,0,-1);
    }
    if(pos[1]+move[1]*dt <= 1 && move[1] != 0) {
        pos = Point3(pos[0], 1, pos[2]);
        Vector3 n =  Vector3(0,1,0);
    }
    if(pos[1]+move[1]*dt >= 35-1) {
        pos = Point3(pos[0], 35-1, pos[2]);
        Vector3 n =  Vector3(0,-1,0);
    }
    car_.set_position(pos);
   
    
}

//control the car-ball collision
void  CarSoccer::Collision_car(double dt, Point3 pos, Vector3 move) {
    Point3 carpos = car_.position();
    Vector3 carv = car_.velocity();
    Point3 ballpos = pos + move*dt;
    Vector3 v = (ballpos - carpos);
    double rcar = car_.collision_radius();
    double d = v.Length();
    if(d <= rcar + ball_.radius()) {
        double differ = rcar + ball_.radius() - d;
        Vector3 rel = (move - carv);
        Vector3 n = v.ToUnit();
        rel = rel - 2*(rel.Dot(n))*n;
        ball_.set_velocity((carv + rel));
        ball_.set_position(ballpos+differ*n);
    }
    
}

//control the ball hits goals
void CarSoccer::hitgoals(double dt, Point3 pos, Vector3 move) {
    pos = pos + move*dt;
    double radius = ball_.radius();
    if((pos[0]-radius>=-10&&pos[0]+radius<=10)&&(pos[1]+radius<=10)&&(pos[2]+radius >= 48||pos[2]-radius <= -48)){
        car_.Reset();
        ball_.Reset();
    }
}


    
    

