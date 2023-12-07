#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "camera.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

// Including for terrain generation
#include "GL/glew.h"
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "terraingenerator.h"
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>


class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();
    void saveViewportImage(std::string filePath);

    void generateShapes();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes
    void renderSkybox();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //void timerEvent(QTimerEvent *event) override;
    //void makeFBO();

    void wheelEvent(QWheelEvent *event) override;



    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;


    Camera camera;

    // Putting these here for now to enable the initial render of a sphere

    // Implementing to enable the first render of the sphere
    GLuint m_shader; // Stores id of shader program
    GLuint m_vbo;    // Stores id of VBO
    GLuint m_vao;    // Stores id of VAO

    GLuint sphere_vbo;    // Stores id of VBO
    GLuint sphere_vao;    // Stores id of VAO

    GLuint cube_vbo;    // Stores id of VBO
    GLuint cube_vao;

    GLuint cone_vbo;    // Stores id of VBO
    GLuint cone_vao;

    GLuint cylinder_vbo;    // Stores id of VBO
    GLuint cylinder_vao;

    glm::mat4 m_model = glm::mat4(1);

    int shapeParameter1;
    int shapeParameter2;

    int EC2GlobalParam1;
    int EC2GlobalParam2;

    // Adding for FBO functionality... Not sure if I will need all of these
    GLuint m_defaultFBO;
    int m_fbo_width;
    int m_fbo_height;
    int m_screen_width;
    int m_screen_height;

    GLuint m_texture_shader;
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    QImage m_image;
    GLuint m_kitten_texture;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;

    GLuint m_fxaa_shader;

    // Implementing for Terrain generation
    void rebuildMatrices();

    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLVertexArrayObject m_terrainVao;
    QOpenGLBuffer m_terrainVbo;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    TerrainGenerator m_terrain;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;

    QPoint m_prevMousePos;
    float m_angleX;
    float m_angleY;
    float m_zoom = 1.0f;

    void moveCamera(float deltaX, float deltaZ);
    void timerEvent(QTimerEvent *event) override;
    float m_moveSpeed = 0.1f;

    QOpenGLShaderProgram *m_skyboxProgram = nullptr;
    GLuint m_skyboxVao, m_skyboxVbo;
    GLuint m_skyboxTexture;

};
