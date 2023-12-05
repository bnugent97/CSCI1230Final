## Project 5: Lights, Camera

Please fill this out for Lights, Camera only. The project handout can be found [here](https://cs1230.graphics/projects/realtime/1).

### Output Comparison

Run the program, open the specified `.json` file, follow the instructions to set the parameters, and save the image with the specified file name using the "Save image" button in the UI. It should automatically suggest the correct directory - again, be sure to follow the instructions in the left column to set the file name. Once you save the images, they will appear in the table below.

> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
>
> 1. Set your working directory to the project directory
> 2. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

|                                           File/Method To Produce Output                                            |                                                     Expected Output                                                     |                                                                     Your Output                                                                     |
| :----------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------: |
|                Input: `unit_cone.json`<br/>Output: `unit_cone.png`<br/>Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone.png)      |            ![Place unit_cone.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone.png)            |
|            Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap.png)    |        ![Place unit_cone_cap.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap.png)        |
|               Input: `unit_cube.json`<br/>Output: `unit_cube.png`<br/> Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube.png)      |            ![Place unit_cube.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube.png)            |
|            Input: `unit_cylinder.json`<br/>Output: `unit_cylinder.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder.png)    |        ![Place unit_cylinder.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder.png)        |
|              Input: `unit_sphere.json`<br/>Output: `unit_sphere.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere.png)     |          ![Place unit_sphere.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere.png)          |
|          Input: `unit_cone.json`<br/>Output: `unit_cone_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_min.png)      |        ![Place unit_cone_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_min.png)        |
|      Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap_min.png)    |    ![Place unit_cone_cap_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap_min.png)    |
|          Input: `unit_cube.json`<br/>Output: `unit_cube_min.png`<br/>Parameters: (**1**, **1**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube_min.png)      |        ![Place unit_cube_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube_min.png)        |
|      Input: `unit_cylinder.json`<br/>Output: `unit_cylinder_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder_min.png)    |    ![Place unit_cylinder_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder_min.png)    |
|        Input: `unit_sphere.json`<br/>Output: `unit_sphere_min.png`<br/>Parameters: (**2**, **3**, 0.1, 100)        |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere_min.png)     |      ![Place unit_sphere_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere_min.png)      |
|           Input: `parse_matrix.json`<br/>Output: `parse_matrix.png`<br/>Parameters: (**3**, 5, 0.1, 100)           |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/parse_matrix.png)     |         ![Place parse_matrix.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/parse_matrix.png)         |
|            Input: `ambient_total.json`<br/>Output: `ambient_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/ambient_total.png)    |        ![Place ambient_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/ambient_total.png)        |
|            Input: `diffuse_total.json`<br/>Output: `diffuse_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/diffuse_total.png)    |        ![Place diffuse_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/diffuse_total.png)        |
|           Input: `specular_total.json`<br/>Output: `specular_total.png`<br/>Parameters: (5, 5, 0.1, 100)           |   ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/specular_total.png)    |       ![Place specular_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/specular_total.png)       |
|              Input: `phong_total.json`<br/>Output: `phong_total.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total.png)     |          ![Place phong_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total.png)          |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1.png`<br/>Parameters: (5, 5, 0.1, 100)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1.png) |  ![Place directional_light_1.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1.png)  |
| Input: `directional_light_2.json`<br/>Output: `directional_light_2.png`<br/>Parameters: (**10**, **10**, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_2.png) |  ![Place directional_light_2.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_2.png)  |
|      Input: `phong_total.json`<br/>Output: `phong_total_near_far.png`<br/>Parameters: (5, 5, **9.5**, **12**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total_near_far.png) | ![Place phong_total_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total_near_far.png) |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1_near_far.png`<br/>Parameters: (**25**, **25**, **8**, **10**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1_near_far.png) | ![Place directional_light_1_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1_near_far.png) |

### Design Choices
#### Scene Parsing
The implementation of my scene parsing was fairly straightforward and followed the implementation I had in the Raytracing project. 
#### Shape Tessellation
Broadly inspired by the structure used in Lab 8, I separated out the functionalities for shape generation into their resepective Sphere.cpp, Cube.cpp, Cylinder.cpp, and Cone.cpp files and helpers. Within each shape they become radiallys more tessellated as parameter 2 increases, and vertically more tesselated as parameter 1 increases. The implementation for each of these shapes was fairly straightforward, really the key insight here was figuring out how to properly orient the cylinder and cone as they had to be editied from the standard sphere to cartesian coordinates.  

#### Camera
My camera implementation was fairly straightforward and standard as well. Broadly follows the same structure as that which was implemented for the raytracing project, however the key difference with this camera implementation is the inclusion of both the view matrix and the projection matrix. 

#### Data Handling
This was really the bulk of the implementation of my code. My code is structured in such a way that the VBOs are largely generated within the generateShapes() function, this is called both within InitializeGL and PaintGL depending on the new shapes which needs to be generated at each point based upon the updated parameters. I implemented parameter minimums for each shape to ensure the 3-dimensionality of each shape regardless of how low the parameters may get within the interface. I would like to apologize in advance for the bulkiness of my gl___ function calls within each respective function. I tried to compartmentalize them into other files, however it becaome cumbersome and buggy to do so I made the ultimate decision to leave them within the realtime.cpp file. 

#### Shaders
The implementation of my shaders was fairly straightforward and broadly based on the implementation in lab 10. However, where in lab 10 only a single point light was being handled, my code is stuctured in such a way that it can handle 8 lights as per the instructions, and primarily directional lights for the time being as is within the scope of this project. Really the key here was ensuring all the proper values are being passed from realtime.cpp to the right uniforms in the shaders, and ultimately making sure everything was in world space to perform calculations. 

### Collaboration/References

I referenced and used code generated from ChatGPT across all segments of this project. In particular, I used its guidance in working through how to structure/implement shape generation and VBO/VAO handling, debugging my shape tessellation calculations, etc.

### Known Bugs
Cone normals are slightly off. Other than that looking good.

### Extra Credit
#### Adaptive Tessellation
##### By Number of Shapes
I successfully implemented adaptive tesselation based upon the number of shapes in a scene. My LOD calculation can be seen below along with examples of how the adaptive tesselation is implemented in both directional light, and primitive salad. 

<img width="746" alt="Screenshot 2023-11-19 at 1 39 52 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/13af2e5e-e539-4f93-b57d-7800d440419b">

###### Before

<img width="784" alt="Screenshot 2023-11-19 at 2 12 53 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/8ea62870-f9f8-48f7-9961-fc745143f85d">


###### After 
<img width="789" alt="Screenshot 2023-11-19 at 2 13 10 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/8fd97df8-3670-45bf-b98b-9bbaf7e96252">

###### Before
<img width="785" alt="Screenshot 2023-11-19 at 2 13 38 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/0150a02d-6cbc-40e8-bd5c-673bee9ab275">

###### After
<img width="786" alt="Screenshot 2023-11-19 at 2 13 52 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/998fdb36-85d0-4f75-8eaa-3b824c88996a">

##### By Distance
I successfully implemented adaptive tesselation based upon the number of shapes in a scene. My LOD calculation can be seen below along with examples of how the adaptive tesselation is implemented in primitive salad. As you can see, the closer object are rendered in higher tesselation, and the further away they are, the less tessellated they are.

<img width="721" alt="Screenshot 2023-11-19 at 2 20 40 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/605d48ab-982f-424e-b5c4-4ed0c917a567">


###### Before

<img width="783" alt="Screenshot 2023-11-19 at 2 16 48 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/817b5ee8-73a8-4ff6-b146-f8e66282190e">


###### After
<img width="777" alt="Screenshot 2023-11-19 at 2 17 14 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/24c052d7-f0c2-4001-a283-ae0fcc8a04b1">



##### Custom Scene
I created my own scene depicting my favorite instrument, the Otamatone. My scene file for this can be found in scene_files/lights_camera/extra_credit/otamatone.json. Here is what it looks like!

<img width="546" alt="Screenshot 2023-11-19 at 2 18 26 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/4c299084-5017-4c24-a621-f9a395ec96ca">


A real otamatone for reference
<img width="299" alt="Screenshot 2023-11-19 at 2 19 16 AM" src="https://github.com/BrownCSCI1230/projects-realtime-bnugent97/assets/123997718/c6c84919-7b5a-4d07-84c4-54e57e675c7d">
