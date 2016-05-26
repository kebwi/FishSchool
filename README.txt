Make sure to leave the two initially blank directories ScreenCaptures and ScreenSequenceCapture in place with their original names since the screen capture code assumes these directories are available.

The following text is written to cout when the program is launched.

================================================================================
Begin cout transcript
================================================================================

Default prey population is 98.
To use a different population, enter it as the first argument to the program.
The predator population is always 2.

Flocking simulation rendered with a glsl vertex shader and fragment shader that perform an angular fisheye projection and fog.
Keith Wiley, http://www.cs.unm.edu/~kwiley, kwiley@cs.unm.edu

There are two agent species: fast predators and slow prey.  Each agent attempts to swim/fly toward attractor locations, away from detractor locations, and attempts to align its direction of travel with its neighbors.

These parameters, as well as some others, can be controlled using key commands.

As agents become frustrated, due to a dissatisfaction with their present location with respect to their attractor and repulsor drives.  The effect of frustration is a temporary increase in speed (to attempt to satisfy the attractors and repulsors and thus reduce the frustration).

The framerate is output continuously to the terminal while the simulation is running.


================================================================================
End cout transcript
================================================================================

If you aren't getting a decent framerate (say, at least 30 fps), try running it with a smaller population by passing a number for the preferred prey population as the first argument to the program.  Alternatively, agent neighboring relationships can slow down the flocking algorithm.  Thus, reducing the predator agent max neighbors variable might improve performance.  See below for more on this variable and the commands to alter it.

The following table of key commands, variables, and current values is written to cout whenever the user enters a key command (best viewed in a monospace font).  Note that some keys increase and decrease a given value using 'shift' where the key without shift is a decrease in value and the key with shift is an increase in value.  This applies to some nonalphanumeric keys as well, such as (- =), (' "), and (, <).

================================================================================
Begin cout transcript
================================================================================

Keys                  Variables                 Values

 === CAMERA AND PROGRAM CONTROLS ===============

esc Quit
spc Pause/unpause ----------------------------- Off
`   Screen capture to "FlockScreenCapture.tga"
~   Continuous screen capture on/off ---------- Off
j l Camera yaw -------------------------------- 0
i k Camera pitch ------------------------------ 0
u o Camera roll ------------------------------- 0
- = Camera zoom ------------------------------- 1
p   Reset yaw, pitch, roll, zoom
[   Fog on/off -------------------------------- On
]   Angular fisheye/hemispherical projection -- AF
' " Edge fade width (fraction of view radius) - 0.0714
\   Vertex complexity distance scaling on/off - Off
|    Body animation on/off -------------------- On
}    Colors to show frustration type on/off --- Off

 === WORLD =====================================

;   Show/hide world sphere (demo AF vs HS) ---- Off
a A Global speed scalar ----------------------- 1
s S Blue max neighbors (prey always 10)  ------ 1
d D Min agent acceptable distance ------------- 1
f F Desired neighbor distance ----------------- 2

 === BEHAVIOR (ATTRACTIONS AND REPULSIONS) =====

z Z World center attraction ------------------- 10
x X Prey swarm COG attraction (pred always 0) - 20
c C Neighbors COG attraction ------------------ 10
v V Prey nearest neighbor attraction ---------- 10
b B Neighbors travel direction match attraction 50
n N Agent too close avoidance repulsion ------- 100
m M Predator hunger --------------------------- 5
, . Prey evasiveness -------------------------- 100

================================================================================
End cout transcript
================================================================================

Keith Wiley
kwiley@keithwiley.com
http//keithwiley.com
100704

This file previously dated:
060316
