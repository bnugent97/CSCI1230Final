#include "glwidget.h"
#include "terraingenerator.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_3_1>
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QDir>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_angleX(0), m_angleY(0), m_zoom(1.0) {
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL()
{
    // GLEW is a library which provides an implementation for the OpenGL API
    // Here, we are setting it up
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glClearColor(0, 0, 0, 1);
    m_program = new QOpenGLShaderProgram;
    std::cout << QDir::currentPath().toStdString() << std::endl;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/resources/shader/vertex.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/resources/shader/fragment.frag");
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

    // dont know if it should be like this
    rebuildMatrices();
}

void GLWidget::paintGL()
{
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

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();

    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_W:
        moveCamera(0, m_moveSpeed); // Move forward
        break;
    case Qt::Key_S:
        moveCamera(0, -m_moveSpeed); // Move backward
        break;
    case Qt::Key_A:
        moveCamera(m_moveSpeed, 0); // Move left
        break;
    case Qt::Key_D:
        moveCamera(-m_moveSpeed, 0); // Move right
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    // Handle key release if needed
    QWidget::keyReleaseEvent(event);
}

void GLWidget::moveCamera(float deltaX, float deltaZ) {
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

void GLWidget::rebuildMatrices() {
    m_camera.setToIdentity();

    // Set camera position
    QVector3D eye(-6.40213e-08, 0.759503, -0.0629183);

    // Set camera direction
    QVector3D direction(8.40059e-08, -0.996586, 0.0825587);

    // Calculate the target position by adding the direction to the eye position
    QVector3D target = eye + direction;

//    std::cout << "Camera Position: "
//              << eye.x() << ", "
//              << eye.y() << ", "
//              << eye.z() << std::endl;
//    std::cout << "Camera Direction: "
//              << direction.x() << ", "
//              << direction.y() << ", "
//              << direction.z() << std::endl;

    // Set the camera to look at the target
    m_camera.lookAt(eye, target, QVector3D(0, 0, 1));

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);

    update();
}

/*void GLWidget::rebuildMatrices() {
    m_camera.setToIdentity();
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(-10 * m_angleX, QVector3D(0, 0, 1));
    QVector3D eye = QVector3D(1, 1, 1);
    eye = rot.map(eye);
    rot.setToIdentity();
    rot.rotate(-10 * m_angleY, QVector3D::crossProduct(QVector3D(0, 0, 1), eye));
    eye = rot.map(eye);

    eye = eye * m_zoom;

    // The target is assumed to be at the origin
    QVector3D target(0, 0, 0);

    // Calculate the direction vector
    QVector3D direction = (target - eye).normalized();

    std::cout << "Camera Position: "
              << eye.x() << ", "
              << eye.y() << ", "
              << eye.z() << std::endl;
    std::cout << "Camera Direction: "
              << direction.x() << ", "
              << direction.y() << ", "
              << direction.z() << std::endl;

    m_camera.lookAt(eye, target, QVector3D(0, 0, 1));

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, 1.0 * width() / height(), 0.01f, 100.0f);

    update();
}*/
