#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "settings.h"

// Implementing for initial render
#include "utils/shaderloader.h"

// Implementing for terrain generation
#include "terraingenerator.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_3_1>
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QDir>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"



// ================== Project 5: Lights, Camera

GLuint loadSkyBoxTextures(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;
    for(GLuint i = 0; i < faces.size(); i++)
    {
        QImage myTexture;
        if (!myTexture.load(QString::fromStdString(faces[i]))) {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            continue;
        }
        myTexture = myTexture.convertToFormat(QImage::Format_RGBX8888);
        width = myTexture.width();
        height = myTexture.height();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, myTexture.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


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
    m_elapsedTimer.start();
    startTimer(16); // Approximately 60 frames per second
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    // Delete buffers and vertex arrays



    // NEED TO CLEAN OUT FBO HERE

    this->doneCurrent();
}

void Realtime::initializeGL()
{
    // GLEW is a library which provides an implementation for the OpenGL API
    // Here, we are setting it up
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glClearColor(0, 0, 0, 1);

    // Initialize terrain shader
    m_program = new QOpenGLShaderProgram;
    std::cout << QDir::currentPath().toStdString() << std::endl;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/resources/shaders/vertex.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/resources/shaders/fragment.frag");
    m_program->link();
    m_program->bind();

    // Initialize skybox shader
    m_skyboxProgram = new QOpenGLShaderProgram;
    m_skyboxProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/skybox.vert");
    m_skyboxProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/skybox.frag");
    m_skyboxProgram->link();
    m_skyboxProgram->bind();

    // Initialize terrain
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");

    m_terrainVao.create();
    m_terrainVao.bind();

    std::vector<GLfloat> verts = m_terrain.generateTerrain();

    m_terrainVbo.create();
    m_terrainVbo.bind();
    m_terrainVbo.allocate(verts.data(),verts.size()*sizeof(GLfloat));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                          nullptr);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                          reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                          reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    m_terrainVbo.release();

    // Initialize skybox
    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_skyboxVao);
    glGenBuffers(1, &m_skyboxVbo);
    glBindVertexArray(m_skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    // Load skybox textures
    std::vector<std::string> faces{
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/front.png",
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/back.png",
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/left.png",
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/right.png",
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/top.png",
        "/Users/brennannugent/CS2230/CSCI1230Final/resources/skyboxTextures/bottom.png"
    };
    m_skyboxTexture = loadSkyBoxTextures(faces);

    m_world.setToIdentity();
    m_world.translate(QVector3D(-0.5,-0.5,0));

    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(1,1,1),QVector3D(0,0,0),QVector3D(0,0,1));

    m_program->release();
    m_skyboxProgram->release();

    // Timer for frame updates
    m_elapsedTimer.start();
    rebuildMatrices();
    // Disable face culling
    //    glDisable(GL_CULL_FACE);
}

void Realtime::renderSkybox() {
    glDepthFunc(GL_LEQUAL);
    m_skyboxProgram->bind();
    QMatrix4x4 view = m_camera.inverted();
    view.setColumn(3, QVector4D(0, 0, 0, 1)); // Remove translation from the view matrix
    m_skyboxProgram->setUniformValue("view", view);
    m_skyboxProgram->setUniformValue("projection", m_proj);
    glBindVertexArray(m_skyboxVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    m_skyboxProgram->release();
}

void Realtime::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Render Skybox
    renderSkybox();

    // Restore default depth function for terrain rendering
    // glDepthFunc(GL_ALWAYS);

    // Bind the terrain shader program
    m_program->bind();

    // Set uniforms for the terrain shader (like view and projection matrices)
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);

    // Bind terrain VAO and render terrain
    m_terrainVao.bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    m_program->setUniformValue(m_program->uniformLocation("wireshade"),m_terrain.m_wireshade);


    int resX = m_terrain.getResolutionX();
    int resY = m_terrain.getResolutionY();


    glPolygonMode(GL_FRONT_AND_BACK,m_terrain.m_wireshade? GL_LINE : GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, resX * resY * 6);

    m_program->release();
}

void Realtime::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void Realtime::mousePressEvent(QMouseEvent *event) {
//    m_prevMousePos = event->pos();
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
//    m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
//    m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
//    m_prevMousePos = event->pos();

//    rebuildMatrices();
}

void Realtime::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::moveCamera(float deltaX, float deltaZ) {
    // Calculate the inverse of the camera matrix to get the transformation matrix
    QMatrix4x4 inverseCamera = m_camera.inverted();

    // Extract the position from the transformation matrix
    QVector3D position = inverseCamera.column(3).toVector3D();

    // Update the position
    position.setX(position.x() + deltaX);
    position.setZ(position.z() + deltaZ);

    // Reconstruct the view matrix with the new position
    m_camera.setToIdentity();
    m_camera.lookAt(position, QVector3D(0, 0, 0), QVector3D(0, 0, 1));

    update(); // Request a redraw
}

void Realtime::rebuildMatrices() {
    m_camera.setToIdentity();

    // Set camera position
    //QVector3D eye(-6.40213e-08, 0.759503, -0.0629183);
    QVector3D eye(-6.40213e-08, 26, -0.0629183);

    // Set camera direction
    QVector3D direction(8.40059e-08, -0.996586, 0.0825587);

    // Calculate the target position by adding the direction to the eye position
    QVector3D target = eye + direction;

    // Set the camera to look at the target
    m_camera.lookAt(eye, target, QVector3D(0, 0, 1));

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);

    update();
}

void Realtime::timerEvent(QTimerEvent *event) {
    float deltaTime = m_elapsedTimer.elapsed() * 0.001f; // Convert milliseconds to seconds
    m_elapsedTimer.restart();

    float moveSpeed = 0.1f; // Adjust as needed
    float moveDistance = moveSpeed * deltaTime;

    QVector3D forward = -m_camera.column(2).toVector3D().normalized();
    //QVector3D right = QVector3D::crossProduct(QVector3D(0, 0, 1), forward).normalized();

    // Remove vertical component from the forward vector
    forward.setZ(0);
    forward.normalize();

    // Calculate the inverse of the camera matrix to get the transformation matrix
    QMatrix4x4 inverseCamera = m_camera.inverted();

    // Extract the position from the transformation matrix
    QVector3D position = inverseCamera.column(3).toVector3D();

    QVector3D movement(0.0f, 0.0f, 0.0f);

    //automatic with slider
    movement += forward * settings.shapeParameter1 * moveDistance ;

    QVector3D newPosition = position + movement;
    m_camera.setToIdentity();
    m_camera.lookAt(newPosition, newPosition + forward, QVector3D(0, 0, 1));

    update();
}

void Realtime::settingsChanged() {
    update(); // asks for a PaintGL() call to occur
}


