#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QKeyEvent>

#include <minipt/minipt.hpp>


class GlWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    GlWidget(QWidget *parent = 0);
    ~GlWidget();

protected:
    void rebuildVertexArray();

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;

private:
    QVector<float> vertices;
    QVector<QVector3D> cubePositions;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram lampShader;
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    QOpenGLVertexArrayObject lightVAO;
    QTimer timer;

    Minipt minipt;

    Camera camera;
};

#endif // WIDGET_H