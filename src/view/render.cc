#include "render.h"

namespace s21 {
Render::Render(QWidget *parent)
    : QOpenGLWidget(parent),
      data_(nullptr),
      shaderProgramP(),
      shaderProgramL(){};

void Render::initializeGL() {
  // qDebug() <<
  // context()->functions()->hasOpenGLFeature(QOpenGLFunctions::FixedFunctionPipeline);
  // qDebug() << Get
  context()->functions()->initializeOpenGLFunctions();
  const char *vertexShaderSource = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;
  void main() {
      gl_Position = projection*view*model*vec4(aPos, 1.0f);
  }
  )";
  const char *pointFragmentShaderSource = R"(
  #version 330 core
  out vec4 FragColor;
  uniform vec4 ourColor;
  uniform int  shape;
  int triangle (vec2 pos) {
     int res = 1;
     vec2 a1 = vec2(0.0, 1.0);
     vec2 a2 = vec2(0.5, 0.0);
     vec2 a3 = vec2(1.0, 1.0);
     vec2 p = gl_PointCoord;
     float l1 = (a1.x - p.x) * (a2.y - a1.y) - (a2.x - a1.x) * (a1.y - p.y);
     float l2 = (a2.x - p.x) * (a3.y - a2.y) - (a3.x - a2.x) * (a2.y - p.y);
     float l3 = (a3.x - p.x) * (a1.y - a3.y) - (a1.x - a3.x) * (a3.y - p.y);
     if (!((sign(l1) <= 0.0 && sign(l2) <= 0.0 && sign(l3) <= 0.0) || (sign(l1) >= 0.0 && sign(l2) >= 0.0 && sign(l3) >= 0.0)))
        res = 0;
     return res;
  }
  int circle(vec2 pos) {
    int res = 1;
    if (dot(pos-0.5, pos-0.5)>0.25) res = 0;

    return res;
  }
  int cross(vec2 pos) {
    int res = 0;
    float r = step(0.1, abs(pos.x - pos.y));
    float l = step(0.1, abs(1.0 - (pos.x + pos.y)));
    if(r == 0.0 || l == 0.0) res = 1;
    return res;
  }
  int plus(vec2 pos) {
    int res = 0;
    float s = step(0.1, abs(0.5 - pos.x));
    s *= step(0.1, abs(0.5 - pos.y));
    if(s == 0) res = 1;
    return res;
  }
  void main() {
    if (shape == 1) {
      if (circle(gl_PointCoord) == 0) discard;
    } else if (shape == 3) {
       if(triangle(gl_PointCoord) == 0) discard;
    } else if (shape == 4) {
       if(cross(gl_PointCoord) == 0) discard;
    } else if(shape == 5) {
        if(plus(gl_PointCoord) == 0) discard;
    }
    FragColor = ourColor;
  }
  )";
  const char *lineFragmentShaderSource = R"(
  #version 330 core

  uniform vec2 viewport;
  uniform vec4 ourColor;
  uniform uint pattern;

  in vec2 cur;
  in vec2 v1;
  in vec2 v2;

  out vec4 FragColor;

  void main() {
    //vec2 pos = gl_FragCoord.xy * viewport;

    if (pattern != 65535U) {
        vec2 dir = (v2.xy - v1.xy) * viewport ;
        vec2 dirC = (cur - v1) * viewport;

        float dist = dot(dirC, dir) / length(dir); // A (.) B = |A| |B| cos(a); cos(a) = Ab / |A|;

        uint bit  = uint(dist / 2.0) & 15U;
        if ((pattern & (1U << bit)) == 0U) discard;
    }

    FragColor = ourColor;
  }
  )";

  const char *pointGeometryShaderSource = R"(
  #version 330
  layout(points) in;
  layout(points, max_vertices = 1) out;
  void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
  }
)";
  const char *lineGeometryShaderSource = R"(
  #version 330
  layout(lines) in;
  layout(triangle_strip, max_vertices = 4) out;

  uniform float thick;
  uniform vec2 viewport;

  //out vec2 ver;
  out vec2 cur;
  out vec2 v1;
  out vec2 v2;

  void main() {
    vec4 p0 = gl_in[0].gl_Position/gl_in[0].gl_Position.w;
    vec4 p1 = gl_in[1].gl_Position/gl_in[1].gl_Position.w;

    /*
    vec2 dir = normalize(p1.xy - p0.xy);
    vec2 normal = vec2(-dir.y, dir.x);
    vec4 offset = vec4((normal * thick + 0.01) / viewport, 0.0, 0.0);
   */
    vec2 line = (p1.xy - p0.xy) * viewport;
    vec2 dir;
    if (viewport.x > viewport.y) {
       dir = normalize(vec2(line.x * (viewport.x / viewport.y), line.y));
    } else {
      dir = normalize(vec2(line.x, line.y * (viewport.y / viewport.x)));
    }
    vec2 normal = vec2(-dir.y, dir.x);
    vec4 offset = vec4((normal * thick) / viewport, 0.0, 0.0);

    p0 += offset * 0.5;
    p1 += offset * 0.5;
    vec4 pm0 = p0 - offset;
    vec4 pm1 = p1 - offset;

    gl_Position = p0;
    cur = gl_Position.xy;
    gl_Position *= gl_in[0].gl_Position.w;
    v1 = p0.xy;
    v2 = p1.xy;
    EmitVertex();

    gl_Position = pm0;
    cur = gl_Position.xy;
    gl_Position *= gl_in[0].gl_Position.w;
    v1 = p0.xy;
    v2 = p1.xy;
    EmitVertex();

    gl_Position = p1;
    cur = gl_Position.xy;
    gl_Position *= gl_in[1].gl_Position.w;
    v1 = p0.xy;
    v2 = p1.xy;
    EmitVertex();

    gl_Position = pm1;
    cur = gl_Position.xy;
    gl_Position *= gl_in[1].gl_Position.w;
    v1 = p0.xy;
    v2 = p1.xy;
    EmitVertex();

    EndPrimitive();
}
)";
  // создание объекта шейдера
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check correct compile
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // fragment shader point
  GLuint fragmentShaderP = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderP, 1, &pointFragmentShaderSource, NULL);
  glCompileShader(fragmentShaderP);
  // Check correct compile
  glGetShaderiv(fragmentShaderP, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderP, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // fragment shader line
  GLuint fragmentShaderL = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderL, 1, &lineFragmentShaderSource, NULL);
  glCompileShader(fragmentShaderL);
  // Check correct compile
  glGetShaderiv(fragmentShaderL, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderL, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // geometry shader point
  GLuint geometryShaderP = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShaderP, 1, &pointGeometryShaderSource, NULL);
  glCompileShader(geometryShaderP);
  // Check correct compile geometry shader
  glGetShaderiv(geometryShaderP, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(geometryShaderP, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // geometry shader line
  GLuint geometryShaderL = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShaderL, 1, &lineGeometryShaderSource, NULL);
  glCompileShader(geometryShaderL);
  // Check correct compile geometry shader
  glGetShaderiv(geometryShaderL, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(geometryShaderL, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // create program
  shaderProgramP = glCreateProgram();
  glAttachShader(shaderProgramP, vertexShader);
  glAttachShader(shaderProgramP, geometryShaderP);
  glAttachShader(shaderProgramP, fragmentShaderP);
  glLinkProgram(shaderProgramP);
  glGetProgramiv(shaderProgramP, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramP, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  shaderProgramL = glCreateProgram();
  glAttachShader(shaderProgramL, vertexShader);
  glAttachShader(shaderProgramL, geometryShaderL);
  glAttachShader(shaderProgramL, fragmentShaderL);
  glLinkProgram(shaderProgramL);
  glGetProgramiv(shaderProgramL, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramL, 512, NULL, infoLog);
    qDebug() << infoLog;
  }
  // delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderP);
  glDeleteShader(geometryShaderP);
  glDeleteShader(fragmentShaderL);
  glDeleteShader(geometryShaderL);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
}

void Render::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void Render::paintGL() {
  glClearColor(back_color.redF(), back_color.greenF(), back_color.blueF(),
               back_color.alphaF());
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (data_ && !data_->vertex.empty()) {
    glUseProgram(shaderProgramP);
    glBindVertexArray(VAO);
    if (vertex_shape) {
      glPointSize(vertex_size);
      // set color and shape for vertex
      PassColorInShader();
      Projection();
      glDrawArrays(GL_POINTS, 0, data_->vertex.size() / 3);
    }
    // set color and thick for edge
    glUseProgram(shaderProgramL);
    PassColorInShader(1);
    Projection(1);
    glDrawElements(GL_LINES, data_->polygon.size(), GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);
    glFlush();
  }
}

void Render::Clear() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteVertexArrays(1, &EBO);
}

void Render::PassColorInShader(int i) {
  if (!i) {
    GLint color_location = glGetUniformLocation(shaderProgramP, "ourColor");
    GLint shape_location = glGetUniformLocation(shaderProgramP, "shape");
    glUniform4f(color_location, vertex_color.redF(), vertex_color.greenF(),
                vertex_color.blueF(), 1.0f);
    glUniform1i(shape_location, vertex_shape);
  } else {
    GLint color_location = glGetUniformLocation(shaderProgramL, "ourColor");
    glUniform4f(color_location, edge_color.redF(), edge_color.greenF(),
                edge_color.blueF(), 1.0f);
    GLint thick_location = glGetUniformLocation(shaderProgramL, "thick");
    glUniform1f(thick_location, edge_size);
    GLint viewport_location = glGetUniformLocation(shaderProgramL, "viewport");
    GLint w = width(), h = height();
    /*
    if (w > h) {
        h /= width()/height();
    } else {
        w /= height()/width();
    }*/
    glUniform2f(viewport_location, w, h);
    GLuint pattern = edge_shape == 0   ? 0xFFFF
                     : edge_shape == 1 ? 0x0F0F
                                       : 0xC3C3;
    GLint pattern_location = glGetUniformLocation(shaderProgramL, "pattern");
    glUniform1ui(pattern_location, pattern);
  }
}

void Render::Projection(int i) {
  GLint model_location = glGetUniformLocation(shaderProgramP, "model");
  GLint view_location = glGetUniformLocation(shaderProgramP, "view");
  GLint pojec_location = glGetUniformLocation(shaderProgramP, "projection");
  if (i) {
    model_location = glGetUniformLocation(shaderProgramL, "model");
    pojec_location = glGetUniformLocation(shaderProgramL, "projection");
    view_location = glGetUniformLocation(shaderProgramL, "view");
  }
  float m[] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
               0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
  float p[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
               0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  float v[] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
               0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
  double max = std::max(data_->minmax[1] - data_->minmax[0],
                        data_->minmax[3] - data_->minmax[2]);
  max = std::max(data_->minmax[5] - data_->minmax[4], max) + 1;
  double w = width(), h = height();
  double ratio = (w > h) ? w / h : h / w;
  if (!project) {
    double left = -max, right = max, top = max, bot = -max, near = -2 * max,
           far = 2 * max;
    if (w > h) {
      left *= ratio;
      right *= ratio;
    } else {
      top *= ratio;
      bot *= ratio;
    }
    p[0] = 2 / (right - left);
    p[5] = 2 / (top - bot);
    p[10] = -2 / (far - near);
    p[12] = -(right + left) / (right - left);
    p[13] = -(top + bot) / (top - bot);
    p[14] = -(far + near) / (far - near);
    p[15] = 1.0;
  } else {
    double left = -1, right = 1, top = 1, bot = -1, near = 1.0,
           far = max * 3 + 1;
    if (w > h) {
      left *= ratio;
      right *= ratio;
    } else {
      top *= ratio;
      bot *= ratio;
    }
    p[0] = 2 * near / (right - left);        // x
    p[8] = (right + left) / (right - left);  // A
    p[5] = 2 * near / (top - bot);
    p[9] = (top + bot) / (top - bot);        // B
    p[10] = -(far + near) / (far - near);    // C
    p[14] = -2 * far * near / (far - near);  // D
    p[11] = -1;
    p[15] = 0.0;
    v[14] = -max;
  }
  glUniformMatrix4fv(pojec_location, 1, GL_FALSE, p);
  glUniformMatrix4fv(view_location, 1, GL_FALSE, v);
  glUniformMatrix4fv(model_location, 1, GL_FALSE, m);
}

void Render::Load() {
  makeCurrent();
  if (data_) {
    if (!VBO) {
      glGenBuffers(1, &VBO);
    }
    size_t end = data_->vertex.size();
    std::vector<float> temp(end);
    for (size_t i = 0; i < end; ++i) {
      temp[i] = data_->vertex[i];
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, end * sizeof(float), temp.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  doneCurrent();
}

void Render::Set(Data &data) {
  makeCurrent();
  data_ = &data;
  Clear();
  if (data_) {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    size_t end = data_->vertex.size();
    std::vector<float> temp(end);
    for (size_t i = 0; i < end; ++i) {
      temp[i] = data_->vertex[i];
    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, end * sizeof(float), temp.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_->polygon.size() * sizeof(int),
                 data_->polygon.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }
}
}  // end namespace s21
