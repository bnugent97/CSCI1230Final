#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "settings.h"

// Implementing for initial render
#include "utils/shaderloader.h"
#include "utils/sceneparser.h"

//Including for shape rendering
#include "sphere.h"
#include "cube.h"
#include "cone.h"
#include "cylinder.h"


// ================== Project 5: Lights, Camera



Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    // Delete buffers and vertex arrays
    glDeleteBuffers(1, &sphere_vbo);
    glDeleteVertexArrays(1, &sphere_vao);

    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);

    glDeleteBuffers(1, &cone_vbo);
    glDeleteVertexArrays(1, &cone_vao);

    glDeleteBuffers(1, &cylinder_vbo);
    glDeleteVertexArrays(1, &cylinder_vao);

    // Implementing for FBO cleanup
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);


    glDeleteProgram(m_texture_shader);
    glDeleteProgram(m_shader);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);


    // NEED TO CLEAN OUT FBO HERE

    this->doneCurrent();
}


void Realtime::generateShapes() {

    // Implementing Extra Credit 1

    int shapeParameter1 = settings.shapeParameter1;
    int shapeParameter2 = settings.shapeParameter2;

    // Adaptive LOD implementation
    // Commenting out to open up the extra credit slots

//    if (settings.extraCredit1) {
//        int shapeCount = metaData.shapes.size(); // Assuming metaData.shapes holds all your shapes

//        // Example LOD factor calculation based on shape count
//        float lodFactor = std::max(0.2f, 1.0f - 0.1f * (shapeCount - 1));

//        // Update each shape's parameters with LOD factor

//        shapeParameter1 = int(float(settings.shapeParameter1) * lodFactor);

//        shapeParameter2 = int(float(settings.shapeParameter1) * lodFactor);
//    }

//    if (settings.extraCredit2) {

//        shapeParameter1 = EC2GlobalParam1;
//        shapeParameter2 = EC2GlobalParam2;
//    }

    // Implementing sphere first
    Sphere sphere;
    sphere.updateParams(std::max(shapeParameter1, 2), std::max(shapeParameter2, 3));

    // VBO First
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sphere.generateShape().size() * sizeof(GLfloat),
                 sphere.generateShape().data(),
                 GL_STATIC_DRAW);
    // Noe implementing VAOs

    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3*sizeof(GLfloat)));

    // Cleaning up after the fact
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Implementing cube next
    Cube cube;
    cube.updateParams(std::max(shapeParameter1, 1));

    // VBO First
    glGenBuffers(1, &cube_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 cube.generateShape().size() * sizeof(GLfloat),
                 cube.generateShape().data(),
                 GL_STATIC_DRAW);
    // Noe implementing VAOs

    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3*sizeof(GLfloat)));

    // Cleaning up after the fact
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Implementing cone next
    Cone cone;
    cone.updateParams(std::max(shapeParameter1, 1), std::max(shapeParameter2, 3));

    // VBO First
    glGenBuffers(1, &cone_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, cone_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 cone.generateShape().size() * sizeof(GLfloat),
                 cone.generateShape().data(),
                 GL_STATIC_DRAW);
    // Noe implementing VAOs

    glGenVertexArrays(1, &cone_vao);
    glBindVertexArray(cone_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cone_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3*sizeof(GLfloat)));

    // Cleaning up after the fact
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Implementing cone next
    Cylinder cylinder;
    cylinder.updateParams(std::max(shapeParameter1, 1), std::max(shapeParameter2, 3));

    // VBO First
    glGenBuffers(1, &cylinder_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, cylinder_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 cylinder.generateShape().size() * sizeof(GLfloat),
                 cylinder.generateShape().data(),
                 GL_STATIC_DRAW);
    // Noe implementing VAOs

    glGenVertexArrays(1, &cylinder_vao);
    glBindVertexArray(cylinder_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3*sizeof(GLfloat)));

    // Cleaning up after the fact
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}



void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();
    // Adding in for FBO implementation
    // May need to change this later depending on the default FBO index
    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // Task 4: Set the clear color here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color to light blue

    // Shader setup
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_fxaa_shader = ShaderLoader::createShaderProgram(":/resources/shaders/fxaa.vert", ":/resources/shaders/fxaa.frag");

    generateShapes();

    // Adding this here from the lab

    // Use the texture shader program to set the uniform
    glUseProgram(m_texture_shader);

    // Task 10: Set the texture.frag uniform for our texture
    GLint textureUniform = glGetUniformLocation(m_texture_shader, "u_texture");
    glUniform1i(textureUniform, 0); // Texture unit 0 is for GL_TEXTURE0

    // Return to the default state of program 0
    glUseProgram(0);

    // Task 11: Fix this "fullscreen" quad's vertex data
    std::vector<GLfloat> fullscreen_quad_data =
        {
            // Positions         // Texture Coordinates
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top Left
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom Left
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom Right
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom Right
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top Right
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f   // Top Left
        };

    // Task 12: Play around with different values!

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    // Task 14: modify the code below to add a second attribute to the vertex attribute array
    // Enable the vertex attribute for position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(0));

    // Enable the vertex attribute for UV
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void Realtime::makeFBO(){
    // Task 19: Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    glGenTextures(1, &m_fbo_texture);
    // Bind the texture to texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create the empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Task 20: Generate and bind a renderbuffer of the right size, set its format, then unbind
    // Generate the renderbuffer object
    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    // Bind the renderbuffer object to the GL_RENDERBUFFER target
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Set the storage configuration for the renderbuffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);

    // Unbind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Task 18: Generate and bind an FBO
    // Generate the framebuffer object
    glGenFramebuffers(1, &m_fbo);
    // Bind the framebuffer object to the GL_FRAMEBUFFER target
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Task 21: Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    // Attach the texture as a color attachment to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

    // Attach the renderbuffer as a depth and stencil attachment to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Check if the FBO is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "Framebuffer not complete!";
    }

    // Unbind the FBO until we need to use it again
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Task 22: Unbind the FBO
   // glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here
    // Task 15: Clear the screen here
    // COMMENTTHIS OUT?
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    // Task 23: Uncomment the following code
    //    // Task 24: Bind our FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    //    // Task 28: Call glViewport
    // Maybe need to reposition
    glViewport(0, 0, m_screen_width, m_screen_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the shader
    glUseProgram(m_shader);

    // Location of the uniform in the shader
    //GLuint ctm_location = glGetUniformLocation(m_shader, "ctm");

    // Define the View and Projection matrices
    glm::mat4 View = camera.getViewMatrix();

    glm::mat4 Projection = camera.getProjectionMatrix();

    bool needUpdate = false;
    if (shapeParameter1 != settings.shapeParameter1) {
        shapeParameter1 = settings.shapeParameter1;
        needUpdate = true;
    }
    if (shapeParameter2 != settings.shapeParameter2) {
        shapeParameter2 = settings.shapeParameter2;
        needUpdate = true;
    }

    // Adaptive LOD implementation
    //Commenting out to free up Extra credit slots
//    if (settings.extraCredit1) {
//        int shapeCount = metaData.shapes.size(); // Assuming metaData.shapes holds all your shapes

//        // Example LOD factor calculation based on shape count
//        float lodFactor = std::max(0.2f, 1.0f - 0.1f * (shapeCount - 1));

//        // Update each shape's parameters with LOD factor

//        shapeParameter1 = int(float(settings.shapeParameter1) * lodFactor);

//        shapeParameter2 = int(float(settings.shapeParameter1) * lodFactor);
//    }



    // Task 17: Draw your VAO here
    Sphere sphere;
    sphere.updateParams(std::max(shapeParameter1, 2), std::max(shapeParameter2, 3));
    Cube cube;
    cube.updateParams(std::max(shapeParameter1, 1));
    Cone cone;
    cone.updateParams(std::max(shapeParameter1, 1), std::max(shapeParameter2, 3));
    Cylinder cylinder;
    cylinder.updateParams(std::max(shapeParameter1, 1), std::max(shapeParameter2, 3));

    //generateShapes();

    // passing k_a (scene ambient) into the fragment shader as a uniform
    GLint kaLocation = glGetUniformLocation(m_shader, "k_a");
    glUniform1f(kaLocation, metaData.globalData.ka);

    GLint oaLocation = glGetUniformLocation(m_shader, "o_a");



    // Task 13: pass light position and m_kd into the fragment shader as a uniform
    GLint kdLocation = glGetUniformLocation(m_shader, "k_d");
    glUniform1f(kdLocation, metaData.globalData.kd);

    GLint odLocation = glGetUniformLocation(m_shader, "o_d");

    GLint lightDirection = glGetUniformLocation(m_shader, "lightDirection");

    glm::vec4 tempLight = glm::vec4(10.0f, 0.0f, 0.0f, 1.0f);
    if (!metaData.lights.empty()) {
        glUniform4fv(lightDirection, 1, glm::value_ptr(metaData.lights[0].dir));
    }

        //glm::value_ptr(metaData.lights[0].pos)

    // Task 6: pass in m_model as a uniform into the shader program
    GLint modelMatrixLocation = glGetUniformLocation(m_shader, "modelMatrix");


    // Task 7: pass in m_view and m_proj
    GLint viewMatrixLocation = glGetUniformLocation(m_shader, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(View));

    GLint projectionMatrixLocation = glGetUniformLocation(m_shader, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(Projection));


    // Task 14: pass shininess, m_ks, and world-space camera position
    GLint ksLocation = glGetUniformLocation(m_shader, "k_s");
    glUniform1f(ksLocation, metaData.globalData.ks);

    GLint osLocation = glGetUniformLocation(m_shader, "o_s");

    GLint shininessLocation = glGetUniformLocation(m_shader, "shininess");


    // Compute the world-space position of the camera from the view matrix
    glm::mat4 inverseView = glm::inverse(View);
    glm::vec3 cameraPosition_worldspace = glm::vec3(inverseView[3]);

    GLint cameraPosLocation = glGetUniformLocation(m_shader, "cameraPosition_worldspace");
    glUniform3fv(cameraPosLocation, 1, glm::value_ptr(cameraPosition_worldspace));


    // Set lighting uniforms
    GLint lightCountLocation = glGetUniformLocation(m_shader, "lightCount");
    glUniform1i(lightCountLocation, metaData.lights.size()); // Assuming metaData.lights holds your light data

    for (int i = 0; i < metaData.lights.size(); i++) {
        std::string uniformName = "sceneLights[" + std::to_string(i) + "]";
        // Set lightType

        // Will need to integrate when light types matter
        GLint lightTypeLocation = glGetUniformLocation(m_shader, (uniformName + ".lightType").c_str());

        if (metaData.lights[i].type == LightType::LIGHT_DIRECTIONAL) {
            glUniform1i(lightTypeLocation, 1);
        }
        else if (metaData.lights[i].type == LightType::LIGHT_POINT){
            glUniform1i(lightTypeLocation, 2);
        }
        else if (metaData.lights[i].type == LightType::LIGHT_SPOT){
            glUniform1i(lightTypeLocation, 3);
        }
        else {
            glUniform1i(lightTypeLocation, 1);
        }

        // Normalize light direction
        glm::vec3 normalizedLightDir = glm::normalize(-1.0f * glm::vec3(metaData.lights[i].dir));

        // Set normalized lightDirection
        GLint lightDirectionLocation = glGetUniformLocation(m_shader, (uniformName + ".lightDirection").c_str());
        glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(normalizedLightDir));

        // Set lightColor
        GLint lightColorLocation = glGetUniformLocation(m_shader, (uniformName + ".lightColor").c_str());
        glUniform4fv(lightColorLocation, 1, glm::value_ptr(metaData.lights[i].color));

        // Set lightPosition
        GLint lightPositionLocation = glGetUniformLocation(m_shader, (uniformName + ".lightPosition").c_str());
        glUniform4fv(lightPositionLocation, 1, glm::value_ptr(metaData.lights[i].pos));

        // Set function
        GLint lightFunctionLocation = glGetUniformLocation(m_shader, (uniformName + ".function").c_str());
        glUniform3fv(lightFunctionLocation, 1, glm::value_ptr(metaData.lights[i].function));
        // Set function
        GLint lightAngleLocation = glGetUniformLocation(m_shader, (uniformName + ".angle").c_str());
        glUniform1f(lightAngleLocation, metaData.lights[i].angle);
        // Set function
        GLint lightPenumbraLocation = glGetUniformLocation(m_shader, (uniformName + ".penumbra").c_str());
        glUniform1f(lightPenumbraLocation, metaData.lights[i].penumbra);
    }


    for (const RenderShapeData& shape : metaData.shapes) {
        int numVerticies = 0;

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(shape.ctm));


        glUniform4fv(oaLocation, 1, glm::value_ptr(shape.primitive.material.cAmbient));
        glUniform4fv(odLocation, 1, glm::value_ptr(shape.primitive.material.cDiffuse));

        glUniform1f(shininessLocation, shape.primitive.material.shininess);
        glUniform4fv(osLocation, 1, glm::value_ptr(shape.primitive.material.cSpecular));

        // Adaptive LOD based on distance from camera implementation
        // Commenting out to open up space for the extra credit slots
//        if (settings.extraCredit2) {
//            //May need to modify
//            float distance = glm::length(glm::vec3(shape.ctm[3]) - glm::vec3(cameraPosition_worldspace));

//            // For simplicity, let's use a linear drop-off from maxTessellation at distance 0 to minTessellation at a certain max distance.
//            float maxDistance = 20.0f; // Maximum distance for tessellation adjustment
//            float t = glm::clamp(distance / maxDistance, 0.0f, 1.0f);
//            float tIn = glm::min(t, .8f);
//            float lodFactor = (1.0f - tIn);
//            EC2GlobalParam1 = int(float(settings.shapeParameter1) * lodFactor);

//            EC2GlobalParam2 = int(float(settings.shapeParameter2) * lodFactor);

//            //Task 17: Draw your VAO here

//            sphere.updateParams(std::max(EC2GlobalParam1, 2), std::max(EC2GlobalParam2, 3));
//            cube.updateParams(std::max(EC2GlobalParam1, 1));
//            cone.updateParams(std::max(EC2GlobalParam1, 1), std::max(EC2GlobalParam2, 3));
//            cylinder.updateParams(std::max(EC2GlobalParam1, 1), std::max(EC2GlobalParam2, 3));
//        }

        // Implemented fix to only update when the parameters change
        if (needUpdate) {
            generateShapes();
        }

        switch (shape.primitive.type) {
        case PrimitiveType::PRIMITIVE_SPHERE:
            glBindVertexArray(sphere_vao);
            numVerticies = sphere.generateShape().size() / 6;

            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            glBindVertexArray(cube_vao);
            numVerticies = cube.generateShape().size() / 6;


            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            glBindVertexArray(cylinder_vao);
            numVerticies = cylinder.generateShape().size() / 6;


            break;
        case PrimitiveType::PRIMITIVE_CONE:
            glBindVertexArray(cone_vao);
            numVerticies = cone.generateShape().size() / 6;

            break;
        default:
            break;
        }

        // Send the CTM to the shader
        //glUniformMatrix4fv(ctm_location, 1, GL_FALSE, glm::value_ptr(shape.ctm));

        glDrawArrays(GL_TRIANGLES, 0, numVerticies);
        // Task 18: Unbind your VAO here
        glBindVertexArray(0);


    }


    //generateShapes();

    //    // Task 25: Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

    // Check if FXAA should be applied
    if (settings.extraCredit1) {
        glUseProgram(m_fxaa_shader);

        // Bind the scene texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

        // Render a fullscreen quad
        glBindVertexArray(m_fullscreen_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Unbind everything
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    else {

    //    // Task 26: Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    // Task 27: Call paintTexture to draw our FBO color attachment texture | Task 31: Set bool parameter to true
    paintTexture(m_fbo_texture, settings.perPixelFilter, settings.kernelBasedFilter, settings.extraCredit2, settings.extraCredit3);

    }
    // Unbind the shader
    //glUseProgram(0);
}

// Task 31: Update the paintTexture function signature
void Realtime::paintTexture(GLuint texture, bool postProcess1, bool postProcess2, bool postProcess3, bool postProcess4){
    glUseProgram(m_texture_shader);
    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    GLint postProcessUniform = glGetUniformLocation(m_texture_shader, "u_filterTexture");
    glUniform1i(postProcessUniform, postProcess1 ? 1 : 0); // Convert bool to int

    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    GLint boxFilterUniform = glGetUniformLocation(m_texture_shader, "u_filterBox");
    glUniform1i(boxFilterUniform, postProcess2 ? 1 : 0); // Convert bool to int

    // Extra Credit implementation
    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    GLint postProcessUniform2 = glGetUniformLocation(m_texture_shader, "u_filterTexture2");
    glUniform1i(postProcessUniform2, postProcess3 ? 1 : 0); // Convert bool to int

    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    GLint boxFilterUniform2 = glGetUniformLocation(m_texture_shader, "u_filterBox2");
    glUniform1i(boxFilterUniform2, postProcess4 ? 1 : 0); // Convert bool to int

    glBindVertexArray(m_fullscreen_vao);
    // Task 10: Bind "texture" to slot 0

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Now you can call glDrawArrays or any other drawing function
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;
    // Task 34: Regenerate your FBOs
    makeFBO();

    // may need to implement later

    // May need to implement a change to perspective
}

void Realtime::sceneChanged() {

    std::string iScenePath = settings.sceneFilePath;

    bool success = SceneParser::parse(iScenePath, metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << iScenePath << "\"" << std::endl;
    }

    // May need to change height and width

    //camera = Camera(metaData.cameraData.pos, metaData.cameraData.look, metaData.cameraData.up, metaData.cameraData.heightAngle, int(size().width()) / int(size().height()), settings.nearPlane, settings.farPlane);
    camera = Camera(metaData.cameraData.pos, metaData.cameraData.look, metaData.cameraData.up, metaData.cameraData.heightAngle, 1.0f * size().width() / size().height(), settings.nearPlane, settings.farPlane);
    auto tempRenderData = metaData;

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    camera = Camera(metaData.cameraData.pos, metaData.cameraData.look, metaData.cameraData.up, metaData.cameraData.heightAngle, 1.0f * size().width() / size().height(), settings.nearPlane, settings.farPlane);
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    // Set the key state to true since it is being pressed
    m_keyMap[Qt::Key(event->key())] = true;

    // Check if the key pressed is W, A, S, or D and print it
//    switch (event->key()) {
//    case Qt::Key_W:
//        std::cout << "W key pressed" << std::endl;
//        break;
//    case Qt::Key_A:
//        std::cout << "A key pressed" << std::endl;
//        break;
//    case Qt::Key_S:
//        std::cout << "S key pressed" << std::endl;
//        break;
//    case Qt::Key_D:
//        std::cout << "D key pressed" << std::endl;
//        break;
//    default:
//        // Do nothing for other keys
//        break;
//    }
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Sensitivity factor for rotation (adjust as needed)
        float sensitivity = 0.1f;

        // Compute rotation angles in radians
        float yaw = -glm::radians(sensitivity * deltaX);
        float pitch = glm::radians(sensitivity * deltaY);

        // Get rotation axes
        glm::vec3 rightVector = glm::normalize(glm::cross(camera.getUp(), camera.getTarget() - camera.getPosition()));
        glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

        // Compute new look direction
        glm::vec3 lookDirection = camera.getTarget() - camera.getPosition();

        // Apply Rodrigues' rotation formula for yaw (around world up vector)
        lookDirection = lookDirection * cos(yaw) + glm::cross(worldUp, lookDirection) * sin(yaw) + worldUp * glm::dot(worldUp, lookDirection) * (1 - cos(yaw));

        // Apply Rodrigues' rotation formula for pitch (around right vector)
        lookDirection = lookDirection * cos(pitch) + glm::cross(rightVector, lookDirection) * sin(pitch) + rightVector * glm::dot(rightVector, lookDirection) * (1 - cos(pitch));

        // Update camera target
        camera.target = camera.getPosition() + lookDirection;

        update(); // Request a PaintGL() call
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    // Compute the look vector (normalized direction from position to target)
    glm::vec3 lookVector = glm::normalize(camera.getTarget() - camera.getPosition());
    // Compute the right vector (perpendicular to both the look and up vectors)
    glm::vec3 rightVector = glm::normalize(glm::cross(camera.getUp(), lookVector));
    // World space up vector
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    float speed = 5.0f; // Speed in world-space units per second

    if (m_keyMap[Qt::Key_W]) {
        // Move the camera forward along the look vector
        camera.position += lookVector * speed * deltaTime;
    }

    if (m_keyMap[Qt::Key_S]) {
        // Move the camera backward along the look vector
        camera.position -= lookVector * speed * deltaTime;
    }

    if (m_keyMap[Qt::Key_A]) {
        // Move the camera to the left
        camera.position += rightVector * speed * deltaTime;
    }

    if (m_keyMap[Qt::Key_D]) {
        // Move the camera to the right
        camera.position -= rightVector * speed * deltaTime;
    }

    if (m_keyMap[Qt::Key_Space]) {
        // Move the camera upwards along the world space up vector
        camera.position += worldUp * speed * deltaTime;
    }

    if (m_keyMap[Qt::Key_Control]) {
        // Move the camera downwards along the world space up vector
        camera.position -= worldUp * speed * deltaTime;
    }

    // Ensuring that the camera's target remains unchanged to avoid rotation
    camera.target = camera.position + lookVector;



    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
