For this assignment I used a fairly complicated formulae to calculate the
heading.

vec3 heading = alpha_n * (h_f)
+ alpha_a  * -1.0f *h_a + alpha_v * (averageVelocity - boid_i->getVelocity())
  + alpha_p * h_p + 0.1f -1.0f * h_ob;


the heading can also be effected by
where
h_f is the heading for following
h_a is the avoidance heading
averageVelocity is the average velocity of nearby boids
h_p is a heading towards the point of interest/light
h_ob is the heading towards an obstacle


I would like to go over some of the key parts to the parameter file

NOTE: The boids positions are randomly generated

numberOfBoids: Controls the number of boids
maxSpawnDistance:Sets the max spawn point of the boids
minSpawnDistance:Sets the min spawn point of the boids
alpha_n:Controls the coefficient for the following heading
alpha_a:Controls the coefficient for the avoidance heading
alpha_v:Controls the coefficient for the velocity matching heading
alpha_p:Controls the coefficient for the point of interest heading
neigbourDistance: The radius of neighbours for the boids
avoidDistance:The avoidance radius for the boids
fieldOfViewMultiplier: Used to control the field of view. Is multiplied by PI
boundRadius: A parameter used to control the bounding sphere
maxVelocity:The max velocity of the boids
maxAcceleration:The max acceleration of the boids

My program also allows the scene to be refreshed by pressing the R
button

My program also allows the user to move the point of interest by using the
standard control scheme of W, A, S, D


My program also allows the user to rotate around the scene by clicking
and dragging on the window


Some of the problems I ran into with this assignment have been:


Boids leaving the scene
To address this I added a bounding radius, I also capped the velocity and
how big the heading could be. However by adding these I also introduced the
problem of "snapping" into my program. I have found that making the the bounding
volume is overall helpful however

Difficulties with controlling boid behavoir. This problem has slightly been
addressed by adding the convergence/point of interest. However by adding that
point I have also sometimes increased the problem of the boids going too fast.
