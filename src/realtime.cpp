#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "settings.h"

// Implementing for initial render
#include "utils/shaderloader.h"



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



    // NEED TO CLEAN OUT FBO HERE

    this->doneCurrent();
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



void Realtime::settingsChanged() {
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    // Set the key state to true since it is being pressed
    m_keyMap[Qt::Key(event->key())] = true;

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
