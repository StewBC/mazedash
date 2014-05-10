
I.     Introduction

I wrote this game quite quickly.  I asked my son what he would like to see in
a game and this is more or less what he described.  Of course, he saw a 3D
game in his head, but I wanted it done for the Commodore 64 and I wanted to
write it in under a week.  So, this is the version he got.  The game is not
that different from Pac Man.  

There's a video on YouTube: https://www.youtube.com/watch?v=knBh9mtK4Ss


II.    Use and keys

Object of the game:  You play as secret agent Mr. White. Your mission is to
collect all the intelligence dots.  Don't get caught by the guards.  Shoot
guards only when you have no other choice.  Use boost to outrun the guards
when possible.  You get 1 point for every piece of intelligence gathered.  You
get 10 points for ever unused boost.  You get 100 points for every unused
bullet.

Modes:

Survival - you have no gun!  If you do clear the level, you get the full
no-shoot bonus.

Escape - You have a gun but the maze has less escape routes.

Keys:
i - Move up
j - Move left
l - Move right
k - Move down
z - Use boost
x - Fire
p - Pause toggle


III. Building from source

Using a properly installed cc65 distribution, it should build using make in
the folder with the Makefile.  I suggest compiling for speed:
make OPTIONS=optspeed


IV.    Porting

The folder structure is set up such that porting is possible.  I don't think
it's really worth-while.  The game uses the C64's hardware sprites.  But, for
what it's worth, the files in the C64 folder under the src folder should be
all that needs to be "ported" to make a port.


V.     Implementation details

The maze generator goes about things like this:  Pick a starting location and
direction.  Move at least 3 spaces before thinking about turning. 3 is
arbitrary, but it makes for nice looking mazes.  The higher the number, the
more straight corridors the maze will have.  For every space, put the
direction in the space.  Directions are clockwise: 0 - Up, 1 - Right, 2 -Down
and 3 - Left.  In the game, you can see the maze generation at work and you
can see the @abcd representing the 0123.  4 is the start location (d).

When no more forward motion is possible, either punch a hole or backtrack, 
turn and try to go forward again. The Escape mode always backtracks.  The 
Survival mode punches holes.  A hole is punched in the block that's blocking
the path to connect to the path on the other side.  This is only done if 
making the hole connects 2 paths and no more and the forward step of the
algorithm advanced at least one block forward.

The AI guards move like bouncy-balls.  They attempt to move in both X and Y 
but since they are limited to a 1 width corridor, they simply walk down the
corridor.  The advantage of this approach is that it is really simple and
ensures very good coverage of the maze.  A "disadvantage" is that the guards
do jump around corners and through "holes" in un-connected corridors.  All in
all, I still prefer this simple method for AI movement.

I took some care to give the game a good (from a flow perspective) front end
and demo attract loop.  Nothing is flashy (although lots of things flash in
colour) as I used a text-mode and no user-defined characters, but I still
wanted it to show that I paid attention to even the small details.


VI.    Credits

The Makefile has the following notice:
###############################################################################
### Generic Makefile for cc65 projects - full version with abstract options ###
### V1.3.0(w) 2010 - 2013 Oliver Schmidt & Patryk "Silver Dream !" Łogiewa  ###
###############################################################################
cl65 V2.14.0
cl65 --version prints the following in my installation:


VIII.  Contact

Feel free to send me an email if you have any comments.

swessels@email.com

Thank you!
