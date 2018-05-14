
# Graphics height map

## Author
- [Akselhj, Aksel Hjerpbakk](https://github.com/Avokadoen) (997816)


### To build the project (windows only for now):
- Simply open visual studio
- Select either debug or release
- Choose build at the upper part of the fullscreen

- You can also download the prebuilt version "buildedGame.rar" from the repo

### [Project Report (not made yet)]()

### [To-Do List](TODO-LIST.md)

### [Known-Bugs List](KNOWN-BUGS.md)

### keybindings
<img src="https://github.com/Avokadoen/AkselhjExamRepo2531/blob/master/keybindings.png" height="300" width="540" >
- Color coding:
	- Green: 	season
	- Purple: 	day/night cycle
	- Yellow: 	air plane controls
	- Magenta: 	debug features
	- Red:		Camera controls
- Seasons:
	- 1 makes it spring
	- 2 makes it summe
	- 3 makes it autumn
	- 4 makes it winter
	- 5 stops progress of months
- Day/night cycle
	- 6 set time to 1200, day
	- 7 set time to 1800, noon
	- 8 set time to 0000, night
	- 9 set time to 0600, morning
	- 0 stops progress of clock
- Air plane:
	- w and s controls yaw, while a and d controls pitch
	- ',' reduce speed. There is no minimum speed
	- '.' increase speed. There is no maximum speed
- Debug:
	- R resets plane position
	- F randoms plane position within the map
	- O toggle contour stroke
- Camera controls
	- '-' change camera state and how input effects the camera:
		- LOCK_TO_THIRD_PRS
			- none
		- LOCK_TO_FIRST_PRS
			- none
		- RESTRICTED
			- i adds to your x, k subtracts from it
			- l adds to your z, j subtracts from it
			- y adds to your y, h subtracts from it
			- mouse to look around
		- FREEMOVE
			- i moves you forward relative to your orientation
			- k moves you backwards relative to your orientation
			- j moves you left relative to your orientation
			- l moves you right relative to your orientation
			- mouse to look around

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
- ##### All lighting
- https://learnopengl.com/Lighting/Materials
- https://learnopengl.com/Lighting/Basic-Lighting
- https://learnopengl.com/Lighting/Lighting-maps
- https://learnopengl.com/Lighting/Light-casters
- https://learnopengl.com/Lighting/Multiple-lights
