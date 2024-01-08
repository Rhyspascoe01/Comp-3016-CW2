_**Comp 3016 Report for Coursework 2**_
_**Introduction**_

Within this document, I shall detail my  OpenGL prototype including the signatures of the scene as well as how the user controls and interacts with the executable, how the code fits together with the header files classes and functions, what makes this program unique and a link to the video of my program at the end.

**Summary of Prototype**

My prototype is known as Parliament Hill which allows the player to move around and explore  Parliament Hill which is a procedurally generated environment based on a hills of Cornwall which were often used as mines or atomic experiments in history. The main Signatures are the two larger then average hills in the environment where the player  starts which is supposed to represent the mine “chimney’s”  which were built to be higher then most areas of a mine hill so people knew where the mine was as well as the areas where they had mined for deposit which gives the area a canyon-like feeling.

**How Do you open and control the prototype**

In order to get the prototype to run you must click on the EXE file where you should then be able to access the prototype and all of it’s features. To control the game you must use the WASD keys and mouse to move and the enter button to exit the game.

**Code Function Explanation and analysis**

The First thing that I would like to go over with my code Explanation is the Camera movement using both mouse movement and key movement. This is achieved using a mixture of code within the main cpp file for the project and the Camera class within the header file with the camera class containing the variables for moment speed which is a float, the camera position, front and up variables which are all three point vectors due to this taking place on a 3D plain and as such needs the X,Y,Z coordinates as well as the View, Movement and MouseMovement voids. The first time this class is called is halfway through the Mouse void in the main where after the mouse is movement is registered are established and the values for the X and Y offset, which is how much the mouse has moved, is established it is then taken into the  MouseMovement void in the camera class. Within the mouse movement area the Yaw and Pitch are then changed by adding the X and Y offset too them which is when the direction vector is changed which allows for the mouse movement to be  fully registered. Then, within the render loop, the view void is called which allows for the view of the player to changed based on the newly changed camera position, front and up values.Movement also happens within the render loop where the UserInput void is called to process a user input. When one of the WASD keys is pressed it shall call the Movement() void within the Camera Class and shall also send a number from 1 – 4 depending on what key is pressed. For example, if the user was to press the W button then they would be sending the number one into the movement void which then goes into the if(number = 1) function which then changes the camera position by multiplying the speed, which is always at the number of 0.0001f, by the camera front which is then added to the camera position which moves the camera forward in this case. This is repeated for the rest of the direction movements with changes based on which direction the player is moving in like the speed and camera front calculation,see  lines 216 – 264 of the Main.cpp file and 29 – 64 within Header.H file for more details.


























The third thing I would like to explain is the Procedural generated terrain that acts as the main setting for my prototype. The first action is that of seeding is to set the frequency of terrain changes and colouring for hill and then placing this generated landmass into a seed which is then fed into the TerrainNoise and Colornoise Variables. These variables are then called within the Array section of the main, allowing the terrain vertices which firstly generates all the noise for the terrain variation before establishing the colour of the terrain of which is generated in such a way to ensure the dirt pathing style that is seen within the prototype. Then the Vertices are generated, going row by row until the map size and render distance have been entirely filled where then the same occurs for the indices for the terrain which is when it reaches the main sequence. This main sequence starts with the array objects being set and binded as well as objects within the buffer being indexed which is when our vertices and  indices are binded and allocated to the buffer. After that the position and textures (Which here are the aforementioned ColorNoise) which are then unbinded when everything  has been generated. After that these are pushed into the MVP which, in combination with the shaders allow for the terrain model to be generated within the prototype, see lines 116 – 210 for more information

The fourth thing that I would like to explain is MVP itself, also known as a model-view-projection matrix. MVP has already been mentioned above within the camera segment due to the View being an integral part of that function so I shall explain more about the Model and projection parts within this section. The model section of the model-view-projection matrix allows for the scene to be manipulated as to give the player the illusion that the scene may have moved downward so the player is above it or have the scene moved forward so it seems the player starts right at the edge of the scene using the Rotate and translate functions. The projection section of the  model-view-projection matrix allows for the scene of the project which uses the perspective() function as to make sure that the environment looks smaller the further away you are from it, see lines  78 through 92
and within the View() section of the Header.H  for more details.

**What makes this Prototype unique?**
One of the things that I believe make my prototype unique is the setting for my prototype which is the environment of Cornish mining hills. This idea came from the fact that I have lived within Cornwall all of my life and as such I have walked across these type of areas several times and feel like they would be more interesting to explore then the standard city/village environments that appear within games. Not only that but this type of environment under-represented in media but it is also  under-represented within media that takes place in Cornwall which instead usual focuses on the more coastal areas of the county. I started with the labs within the comp 3016 model in order to help me structure my code as well as gaining the knowledge to create the hills within the first place.
**Link to Video** : https://youtu.be/KyXt7ndPoPY
