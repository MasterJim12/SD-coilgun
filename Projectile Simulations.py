
from math import pi

#projectile properties based from 5.56*45 https://en.wikipedia.org/wiki/5.56%C3%9745mm_NATO
density = 7.874 #g/cm^3 density pure iron
length = 1.785
Area = pi*(.3215**2) #Change this later once projectile is moddeled in solidworks. Right now pretend cylinder
volume = Area * length
mass = density * volume # mass of projectile in grams

print(mass)

# At the bare minimum, according to prject guidelines we need to be able to accelerate this projectile to around 50m/s
#Using physics and kinematics 
# Distance is more of a constrain than time, this is because barrel has finite length 
#kinematic formula best for this is Vfinal^2 = Vinitial^2 + 2*(acceleration)*distance

#Vinitial, projectile is at rest
Vinitial = 0 #m/s

#Vfinal, projectile is at barrels end
Vfinal  = 50 #m/s 

#Distance of the barrel
Distance = .368 #m, Based on M4 Carbine

#Rearrancge kinematic equation to solve for the acceleration needed.
Acceleration = ((Vfinal^2)-(Vinitial^2))/(2*Distance)

#Homeboy Newton found a way for us to calculate force
Force = mass * Acceleration

print("Amount of force needed to accelerate a pure iron projectile of " + str(mass) + "grams, is " + str(Force) + "Newtons" )

