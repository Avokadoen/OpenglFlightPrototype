<img src="https://github.com/Avokadoen/AkselhjExamRepo2531/blob/master/frontpage.png" height="450" width="900" />

# Graphics height map

## Author
- [Akselhj, Aksel Hjerpbakk](https://github.com/Avokadoen) (997816)


### To build the project (windows only for now):
- Simply open visual studio
- Select either debug or release
- Choose build at the upper part of the fullscreen

- You can also download the prebuilt version "builtGame.rar" from the repo

### [To-Do List](TODO-LIST.md)

### [Known-Bugs List](KNOWN-BUGS.md)

### keybindings
<img src="https://github.com/Avokadoen/AkselhjExamRepo2531/blob/master/keybindings.png" height="300" width="700" />

##### Description:
- Color coding:
	- Green: 	season
	- Purple: 	day/night cycle
	- Yellow: 	air plane controls
	- Magenta: 	debug features
	- Red:		Camera controls
	- Orange:	Utility
- Seasons:
	- **1** makes it spring
	- **2** makes it summer
	- **3** makes it autumn
	- **4** makes it winter
	- **5** stops progress of months
- Day/night cycle
	- **6** set time to 1200, day
	- **7** set time to 1800, noon
	- **8** set time to 0000, night
	- **9** set time to 0600, morning
	- **0** stops progress of clock
- Air plane:
	- **W** and **S** controls yaw, while **A** and **D** controls pitch
	- **,** reduce speed. There is no minimum speed
	- **.** increase speed. There is no maximum speed
- Debug:
	- **R** resets plane position
	- **F** randoms plane position within the map
	- **O** toggle contour stroke
- Camera controls
	- **-** change camera state and how input effects the camera:
		- LOCK_TO_THIRD_PRS
			- none
		- LOCK_TO_FIRST_PRS
			- none
		- RESTRICTED
			- **I** adds to your x, **K** subtracts from it
			- **L** adds to your z, **J** subtracts from it
			- **Y** adds to your y, **H** subtracts from it
			- **mouse** to look around
			- **scroll wheel** to zoom in/out
		- FREEMOVE
			- **I** moves you forward relative to your orientation
			- **K** moves you backwards relative to your orientation
			- **J** moves you left relative to your orientation
			- **L** moves you right relative to your orientation
			- **mouse** to look around
			- **scroll wheel** to zoom in/out
- Utility
	- **f2** toggle between fullscreen and windowed. Going to fullscreen will save the screen state and apply it when going back to windowed
	- **esc** will exit the program

##### Some sources:
- https://learnopengl.com/
    - For the model loading and model data structure.
- https://www.3dgep.com/multi-textured-terrain-in-opengl/#The_Terrain_Header_File
	- Loading height maps
- https://learnopengl.com/In-Practice/Text-Rendering
	- Using freeType
- https://stackoverflow.com/a/47462358
	- Toggle fullscreen
- https://stackoverflow.com/a/1121731
	- Finding up (and therefore also front) vector.
- https://stackoverflow.com/a/40024726
	- Using quat in a proper manner after decompose
- https://stackoverflow.com/a/13445752
	- Random int values
- https://en.wikipedia.org/wiki/Linear_interpolation#Programming_language_support
	- Lerp
- https://learnopengl.com/Advanced-OpenGL/Blending
	- Blending basics
- ##### All lighting
- https://learnopengl.com/Lighting/Materials
- https://learnopengl.com/Lighting/Basic-Lighting
- https://learnopengl.com/Lighting/Lighting-maps
- https://learnopengl.com/Lighting/Light-casters
- https://learnopengl.com/Lighting/Multiple-lights
