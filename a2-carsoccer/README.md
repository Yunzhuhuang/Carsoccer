# My solution to Assignment 2
```
  Clara Huang
  huan2089
```

##Design decisions
1. For drawing boundarys and goals, I use the quickShakes_.DramLines() function's LinesType = LINE_LOOP and LinesType = LINES_STRIP.

2. For handling ball and wall coliision, I set several "if" conditions for ball'position touch the boundary, and I modify the ball's position and velocity based on the model of collision. Those are in the Ball_collision_boundary().
A little desigh here is for the ball touch the ground without the y-dimension speed, then I won't count that as a collision so the ball's speed won't be decreased. Also, I noticed that if the ball fly to the air, since it has a acceleration due to the gravity, it always has y dimension speed after this, so it always hit with the ground and make the x and z's speed ne decreased too. Thus, I do a little modify here, like I set that if the ball's y dimensional speed is super small, in my case, is -0.1<=speed[y]<=0.1, then it will count as stay on the ground and don't hit the ground.
3. For handling car and boundary's collision, my design is the same as the ball and boundary collision. Except that car never fly to the air so I don't need to consider it touch the roof. Also, the car's velocity don't change, only the position changes.
4. For handling car and ball collision, I first detect if a collision exist by calculating the dsitance between two objects' position. Then I update the ball's new postion and new velocity based on the colision model.
5. For handling the ball hitting the goals, I calculate the ball's position relate to the goal's position. If the ball attach the goal, I simply reset the call and the ball to make the game start again.
6. For the whole updating the ball's motion process, I apply the ball-boundary collision, the car-ball collision and ball-hit goals in order and finally update the ball's position and velocity based on all the tests above.
7. For updating the car's position and velocity. I include three private varaibles in class Car, which are a float number speed which stores the speed of the car, a float number angle for indicating the heading of the car and a float number turn_rate_ for the turn rate correspond to speed when the car decided to turn right or left.
I define a function called car_motion to handle the car's motion. UP and DOWN keys are for the car to speed up or speed down and left key for turing left, right key for turning right. I basically call methods set_angle(), set_speed() for changing the heading and speed. Then the whole process for controling the motion of a car in my code is firstly mark the position of the ar and then call car_motion() to let huamn hands control the car, and call car_collision_boundary to prevent the car getting out of the boundary. Finally, set the car's new positio basd on all resitrictions above.
