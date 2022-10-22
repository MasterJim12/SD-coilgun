import matplotlib.pyplot as plt
from math import pi, sqrt

density = 7.874 #g/cm^3 density pure iron
length = 4 #Legnth in cm
Area = pi*(.5**2) #Change this later once projectile is moddeled in solidworks. Cylinder radius is in cm
volume = Area * length
mass = density * volume # mass of projectile in grams

print(str(mass) + " Grams")
mass = mass/1000 #mass in kg

# At the bare minimum, according to prject guidelines we need to be able to accelerate this projectile to around 50m/s
#Using physics and kinematics 
# Distance is more of a constrain than time, this is because barrel has finite length 
#kinematic formula best for this is Vfinal^2 = Vinitial^2 + 2*(acceleration)*distance

#Vinitial, projectile is at rest
Vinitial = 0 #m/s

#Vfinal, projectile is at barrels end
Vfinal  = 200 #m/s 

#Distance of the barrel
Distance = 1 #m,

#Rearrancge kinematic equation to solve for the acceleration needed.
Acceleration = ((Vfinal^2)-(Vinitial^2))/(2*Distance)

#Homeboy Newton found a way for us to calculate force
Force = mass * Acceleration

position = []
velocity = []
currentposition = 0
currentvelocity = 0
time = .0001 #1ms
timelist = []
while currentposition < 1:
    currentposition = currentposition + currentvelocity*time + (1/2)*Acceleration*time**2
    position.append(currentposition)
    currentvelocity = currentvelocity + Acceleration*time
    velocity.append(currentvelocity)
    time = time + .0001
    timelist.append(time)

fig, (ax1, ax2) = plt.subplots(2, sharex=True)
ax1.plot(timelist, position)
ax1.set_title("Position V Time")
ax1.set_ylabel("Position (m)")
ax2.plot(timelist, velocity)
ax2.set_title("Velocity V Time")
ax2.set_ylabel("Velocity (m/s)")

plt.show()
print("Amount of force needed to accelerate a pure iron projectile of " + str(mass) + "kilograms, is " + str(Force) + "Newtons" )

vperm = 4*pi*10**-7 #H/m
ironperm = 6.3*10**-3
Bfieldsquared = (Force * 2*vperm)/((ironperm/vperm - 1)*pi *.5**2)
Bfield = sqrt(Bfieldsquared)

nI = Bfield/vperm

print("Done")