#include "glwidget.h"
#include <QtMath>

GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent), VBO(QOpenGLBuffer::VertexBuffer), camera(this)
{
    vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    timer.setInterval(18);
    connect(&timer, &QTimer::timeout, this, static_cast<void (GlWidget::*)()>(&GlWidget::update));
    timer.start();
}

GlWidget::~GlWidget()
{
}

void GlWidget::initializeGL()
{
    this->initializeOpenGLFunctions(); //初始化opengl函数
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./triangle.vs"))
    {                                                //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果编译出错,打印报错信息
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./triangle.frag"))
    {                                                //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果编译出错,打印报错信息
    }
    if (!shaderProgram.link())
    {                                                //链接着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果链接出错,打印报错信息
    }

    QOpenGLVertexArrayObject::Binder{&VAO};

    VBO.create(); //生成VBO对象
    VBO.bind();   //将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中

    //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    VBO.allocate(vertices.data(), sizeof(float) * vertices.size());

    //设置顶点解析格式，并启用顶点
    shaderProgram.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    shaderProgram.enableAttributeArray("aPos");
    shaderProgram.setAttributeBuffer("aNormal", GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    shaderProgram.enableAttributeArray("aNormal");

    if (!lampShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "./triangle.vs"))
    {                                                //添加并编译顶点着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果编译出错,打印报错信息
    }
    if (!lampShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "./triangle.frag"))
    {                                                //添加并编译片段着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果编译出错,打印报错信息
    }
    if (!lampShader.link())
    {                                                //链接着色器
        qDebug() << "ERROR:" << shaderProgram.log(); //如果链接出错,打印报错信息
    }

    QOpenGLVertexArrayObject::Binder{&lightVAO};
    VBO.bind(); //只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    lampShader.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    lampShader.enableAttributeArray("aPos");

    this->glEnable(GL_DEPTH_TEST);

    camera.init();
}

void GlWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h); //定义视口区域
}

void GlWidget::paintGL()
{
    this->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               //设置清屏颜色
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓存和深度缓存
    float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    shaderProgram.bind();

    QVector3D lightColor(1.0f, 1.0f, 1.0f);
    QVector3D objectColor(1.0f, 0.5f, 0.31f);
    QVector3D lightPos(-4.0f, 0.0f, 0.5f);

    shaderProgram.setUniformValue("objectColor", objectColor);
    shaderProgram.setUniformValue("lightColor", lightColor);

    QMatrix4x4 model;
    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("view", camera.getView());

    shaderProgram.setUniformValue("lightPos", lightPos);
    shaderProgram.setUniformValue("viewPos", camera.getCameraPos());

    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / (float)height(), 0.1f, 100.0f);
    shaderProgram.setUniformValue("projection", projection);
    QOpenGLVertexArrayObject::Binder{&VAO};
    this->glDrawArrays(GL_TRIANGLES, 0, 36);

    lampShader.bind();

    model.translate(lightPos);
    model.scale(0.2);
    lampShader.setUniformValue("model", model);
    lampShader.setUniformValue("view", camera.getView());
    lampShader.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder{&lightVAO};
    this->glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool GlWidget::event(QEvent *e)
{
    camera.handle(e);
    
    return QWidget::event(e); //调用父类的事件分发函数
}