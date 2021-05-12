#include "glwidget.h"
#include <QtMath>

GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent), VBO(QOpenGLBuffer::VertexBuffer), camera(this)
{
    vertices = {};
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

    minipt.scene.triangles.push_back({{0, 0, 0}, {0, 0, 1}, {0, 1, 0}});
    minipt.AutoNormal();
    rebuildVertexArray();

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
    // QVector3D lightPos(4.0f, 0.2f, 0.5f);
    QVector3D cameraPos = camera.getCameraPos();
    QVector3D cameraDir = camera.getCameraDir();
    QVector3D lightPos = cameraPos - cameraDir;

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
    this->glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
}

bool GlWidget::event(QEvent *e)
{
    camera.handle(e);

    return QWidget::event(e); //调用父类的事件分发函数
}

void GlWidget::rebuildVertexArray()
{
    this->vertices.clear();
    minipt.AutoNormal();
    for (auto t : minipt.scene.triangles)
    {
        this->vertices.push_back(t.p0.x);
        this->vertices.push_back(t.p0.y);
        this->vertices.push_back(t.p0.z);
        this->vertices.push_back(t.n.x);
        this->vertices.push_back(t.n.y);
        this->vertices.push_back(t.n.z);
        this->vertices.push_back(t.p1.x);
        this->vertices.push_back(t.p1.y);
        this->vertices.push_back(t.p1.z);
        this->vertices.push_back(t.n.x);
        this->vertices.push_back(t.n.y);
        this->vertices.push_back(t.n.z);
        this->vertices.push_back(t.p2.x);
        this->vertices.push_back(t.p2.y);
        this->vertices.push_back(t.p2.z);
        this->vertices.push_back(t.n.x);
        this->vertices.push_back(t.n.y);
        this->vertices.push_back(t.n.z);
    }

    qDebug() << "rebuildVertexArray size=" << vertices.size();

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
}

void GlWidget::ExecuteCmd(const QString& cmd)
{
    QString str=cmd;
    QRegExp rx("-?[1-9]\\d*\\.\\d*|0+.[0-9]+|-?0\\.\\d*[1-9]\\d*|-?\\d+");
	int pos = 0;
	QVector<double> v;
	while ((pos = rx.indexIn(str, pos)) != -1)
	{
		pos += rx.matchedLength();
		v.push_back(rx.cap(0).toDouble());
	}
    if(v.size()!=9)
    {
        qDebug()<<"Error: wrong cmd format! "<<v.size();
        return;
    }
    minipt.scene.triangles.push_back({{v[0],v[1],v[2]},{v[3],v[4],v[5]},{v[6],v[7],v[8]}});
    rebuildVertexArray();
}
