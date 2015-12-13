Comp 465 Warbird Simulation Phase 2 + 3
Therese Horey
Jesus Moran Perez


-----------------------------------

Summary:
	From Project 1:
		***********
		Project 1 creates a small solar system consisting of a sun (Ruber), 
		two planets (Unum and Duo), and two moons which orbit Duo (Primus and Secundus).
		One spaceship (Warbird) and 1 missile also exist in this system.

		The sun, planets, and moons are initially placed along the X axis at:

		Ruber	  0	(the origin)
		Unum	  4000	
		Duo       9000	
		Primus	  900	
		Secundus  1750	

		Since Primus and Secundus orbit Duo, these values represent their distance from Duo.
		Also, Duo should be 180 degrees opposite Unum at -9000, but to keep the planets'
		relative axes the same they are loaded in the above manner.  Duo is given its own
		rotation matrix to move it 180 degrees.  All objects rotate counter-clockwise.
		
		There are 5 camera views: Front, Top, Warbird, Unum, and Duo.
		***********
			
	Project 2 + 3 builds off of Project 1:
	
	- Warbird
		The ship now moves in all x,y,z directions and can yaw, pitch, and roll.
		The ship has a follow camera that stays above and behind the ship,
		looking over the ship.  Can fire missiles which seek out the missile sites.
	- Planets
		No changes made.
	- Missiles
		There can be up to 3 active missiles at once (1 for Warbird, 1 for each missile site).
		Total number of missiles: 10 for Warbird, 5 for each missile site.
		Missile life time is determined by int maxUpdate in the missile class, default is 2000.
		After 200 updates, missiles begin target detection (5000).  Update function uses two vectors 
		to find the axis (cross product) and angle (dot product) to rotate the missile.
		First vector is the direction vector from the missile's position to the target's position, 
		second vector is the missile's looking at vector.  The missile moves on its looking at
		vector at a fixed speed.  This is the only action the missile takes when no targets are detected.
			*****Missile strategy - Missiles are very twitchy and sometimes seem to lose track
			of their target.  Timing the missile firing so that the missile becomes live when it
			reaches its target ensures the best chance of success.
	- Missile sites
		Two missile sites are present, one on Unum and one on Secundus.  When the Warbird enters
		detection range (5000) the site fires a missile which will seek the ship.
	- Collision testing
		Update function in main.cpp processes the collisions.  Collisions tested for are:
		warbird and missiles, warbird and planets, missiles and planets, missiles and missile sites.
		Collision testing method used is bounding spheres calculated from the radius of the model.
			*****The distance^2 is compared to the sumRadius^2 to keep the calculations from using
			square roots because it is computationally intensive and my computer has feelings too.
	- Texture backgrounds
		6 textures added to form a box around the Ruber system.  A square model was created from 
		points (two triangles) and used to hold the texture.
	- Lights
		2 lights added: a point light at Ruber and a directional head light that is attached
		to the current camera.
	- Win or lose
		The game is won when both missile sites are destroyed and the window title displays
		"Cadet passes flight training."  The game is lost if: Warbird collides with any active missile
		or planet and the window title displays "Your ship has been destroyed," or Warbird has
		0 missiles left and one or more missile sites are active, then the window title displays
		"Cadet has resigned from War College".
			*****To continue playing after win or lose states have been reached, comment out this line
			in the intervalTimer function in main.cpp:
			if (!warbirdDestroyed && !((missileCountWarbird == 0 && !activemissileWarbird) 
				&& !(siteUnumDestroyed && siteSecundusDestroyed)) && !(siteUnumDestroyed && siteSecundusDestroyed))


How to run:

	- Place zip file in folder where 'includes465' folder is
	- Unzip Project folder
	- Use Visual Studio to open the Project solution file
	- Build and run solution

Controls:

	w 		warp ship between Unum and Duo
	f 		fire Warbird missile
	g 		toggle Ruber gravity (affects only Warbird)
	v		next camera view
	x		previous camera view
	t 		cycle update rate
	s 		cycle ship speed
	d		toggle debug (colored lights and axes)
	a		toggle ambient light
	p		toggle point light at Ruber
	h		toggle directional head light
	↑		ship forward
	↓ 		ship backward
	→ 		ship yaws "left"
	← 		ship yaws "right"
	control ↑ 	ship pitches "down"
	control ↓ 	ship pitches "up"
	control → 	ship rolls "left"
	control ← 	ship rolls "right"

-----------------------------------
	
See comments in project files for more detailed info regarding specific program structure and code.
