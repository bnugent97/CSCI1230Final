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
    // GLEW is a library which provides an implementation for the OpenGL API
    // Here, we are setting it up
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glClearColor(0, 0, 0, 1);
    m_program = new QOpenGLShaderProgram;
    std::cout << QDir::currentPath().toStdString() << std::endl;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/resources/shaders/vertex.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/resources/shaders/fragment.frag");
    m_program->link();
    m_program->bind();

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

    m_world.setToIdentity();
    m_world.translate(QVector3D(-0.5,-0.5,0));


    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(1,1,1),QVector3D(0,0,0),QVector3D(0,0,1));

    m_program->release();
}



void Realtime::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    m_program->setUniformValue(m_program->uniformLocation("wireshade"),m_terrain.m_wireshade);

    int res = m_terrain.getResolution();


    glPolygonMode(GL_FRONT_AND_BACK,m_terrain.m_wireshade? GL_LINE : GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, res * res * 6);

    m_program->release();
}



void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    //glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
        m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
        m_prevMousePos = event->pos();
        rebuildMatrices();
    }
}

void Realtime::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void Realtime::rebuildMatrices() {
    m_camera.setToIdentity();
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(-10 * m_angleX,QVector3D(0,0,1));
    QVector3D eye = QVector3D(1,1,1);
    eye = rot.map(eye);
    rot.setToIdentity();
    rot.rotate(-10 * m_angleY,QVector3D::crossProduct(QVector3D(0,0,1),eye));
    eye = rot.map(eye);

    eye = eye * m_zoom;

    m_camera.lookAt(eye,QVector3D(0,0,0),QVector3D(0,0,1));

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);

    update();
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

//void Realtime::mousePressEvent(QMouseEvent *event) {
//    if (event->buttons().testFlag(Qt::LeftButton)) {
//        m_mouseDown = true;
//        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
//    }
//}

//void Realtime::mouseReleaseEvent(QMouseEvent *event) {
//    if (!event->buttons().testFlag(Qt::LeftButton)) {
//        m_mouseDown = false;
//    }
//}

//void Realtime::mouseMoveEvent(QMouseEvent *event) {
//    if (m_mouseDown) {
//        int posX = event->position().x();
//        int posY = event->position().y();
//        int deltaX = posX - m_prev_mouse_pos.x;
//        int deltaY = posY - m_prev_mouse_pos.y;
//        m_prev_mouse_pos = glm::vec2(posX, posY);

//        // Sensitivity factor for rotation (adjust as needed)
//        float sensitivity = 0.1f;

//        // Compute rotation angles in radians
//        float yaw = -glm::radians(sensitivity * deltaX);
//        float pitch = glm::radians(sensitivity * deltaY);

//        // Get rotation axes
//        glm::vec3 rightVector = glm::normalize(glm::cross(camera.getUp(), camera.getTarget() - camera.getPosition()));
//        glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

//        // Compute new look direction
//        glm::vec3 lookDirection = camera.getTarget() - camera.getPosition();

//        // Apply Rodrigues' rotation formula for yaw (around world up vector)
//        lookDirection = lookDirection * cos(yaw) + glm::cross(worldUp, lookDirection) * sin(yaw) + worldUp * glm::dot(worldUp, lookDirection) * (1 - cos(yaw));

//        // Apply Rodrigues' rotation formula for pitch (around right vector)
//        lookDirection = lookDirection * cos(pitch) + glm::cross(rightVector, lookDirection) * sin(pitch) + rightVector * glm::dot(rightVector, lookDirection) * (1 - cos(pitch));

//        // Update camera target
//        camera.target = camera.getPosition() + lookDirection;

//        update(); // Request a PaintGL() call
//    }
//}


