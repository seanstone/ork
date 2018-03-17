/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence.
 * For any assistance, feedback and remarks, you can check out the
 * mailing list on the project page :
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include <fstream>

#include "stbi/stb_image.h"

#include "ork/core/Logger.h"
#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

#include "examples/Main.h"

using namespace std;
using namespace ork;

struct P3_N3_UV_C {

    float x, y, z, nx, ny, nz, u, v;

    unsigned char r, g, b, a;

    P3_N3_UV_C()
    {
    }

    P3_N3_UV_C(float x, float y, float z, float nx, float ny, float nz,
            float u, float v, unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
        x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v), r(r), g(g), b(b), a(a)
    {
    }
};

class RenderExample : public GlutWindow
{
public:
    ptr< Mesh<P3_N3_UV_C, unsigned int> > cube;
    ptr< Mesh<P3_N3_UV_C, unsigned int> > plane;
    ptr<Program> p1;
    ptr<Program> p2;
    ptr<Uniform3f> worldCamera;
    ptr<UniformMatrix4f> localToWorld1;
    ptr<UniformMatrix4f> localToScreen1;
    ptr<UniformMatrix4f> localToWorld2;
    ptr<UniformMatrix4f> localToScreen2;
    ptr<FrameBuffer> fb;
    float fov;
    float alpha;
    float theta;
    float dist;

    static unsigned char* load(const string &file, int &size)
    {
        ifstream fs(file.c_str(), ios::binary);
        fs.seekg(0, ios::end);
        size = fs.tellg();
        char *data = new char[size + 1];
        fs.seekg(0);
        fs.read(data, size);
        fs.close();
        data[size] = 0;
        return (unsigned char*) data;
    }

    RenderExample(const string &dir) :
        GlutWindow(Window::Parameters().size(1024, 768).depth(true)), fov(80.0), alpha(135), theta(45), dist(15)
    {
        //Logger::DEBUG_LOGGER = new Logger("DEBUG");

        cube = new Mesh<P3_N3_UV_C, unsigned int>(TRIANGLES, GPU_STATIC);
        cube->addAttributeType(0, 3, A32F, false);
        cube->addAttributeType(1, 3, A32F, false);
        cube->addAttributeType(2, 2, A32F, false);
        cube->addAttributeType(3, 4, A8UI, true);
        cube->addVertex(P3_N3_UV_C(-1, -1, +1, 0, 0, +1, 0, 0, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, +1, 0, 0, +1, 1, 0, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, +1, 0, 0, +1, 1, 1, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, +1, 0, 0, +1, 1, 1, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, +1, 0, 0, +1, 0, 1, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, +1, 0, 0, +1, 0, 0, 255, 0, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, +1, +1, 0, 0, 0, 0, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, -1, +1, 0, 0, 1, 0, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, -1, +1, 0, 0, 1, 1, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, -1, +1, 0, 0, 1, 1, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, +1, +1, 0, 0, 0, 1, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, +1, +1, 0, 0, 0, 0, 0, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, +1, 0, +1, 0, 0, 0, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, +1, 0, +1, 0, 1, 0, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, -1, 0, +1, 0, 1, 1, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, -1, 0, +1, 0, 1, 1, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, -1, 0, +1, 0, 0, 1, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, +1, 0, +1, 0, 0, 0, 0, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, -1, 0, 0, -1, 0, 0, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, -1, 0, 0, -1, 1, 0, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, -1, 0, 0, -1, 1, 1, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, -1, 0, 0, -1, 1, 1, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, +1, -1, 0, 0, -1, 0, 1, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, -1, 0, 0, -1, 0, 0, 0, 255, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, -1, -1, 0, 0, 0, 0, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, +1, -1, 0, 0, 1, 0, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, +1, -1, 0, 0, 1, 1, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, +1, -1, 0, 0, 1, 1, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, +1, -1, -1, 0, 0, 0, 1, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, -1, -1, 0, 0, 0, 0, 255, 0, 255, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, -1, 0, -1, 0, 0, 0, 255, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, -1, 0, -1, 0, 1, 0, 255, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, +1, 0, -1, 0, 1, 1, 255, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(+1, -1, +1, 0, -1, 0, 1, 1, 255, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, +1, 0, -1, 0, 0, 1, 255, 255, 0, 0));
        cube->addVertex(P3_N3_UV_C(-1, -1, -1, 0, -1, 0, 0, 0, 255, 255, 0, 0));

        plane = new Mesh<P3_N3_UV_C, unsigned int>(TRIANGLES, GPU_STATIC);
        plane->addAttributeType(0, 3, A32F, false);
        plane->addAttributeType(1, 3, A32F, false);
        plane->addAttributeType(2, 2, A32F, false);
        plane->addAttributeType(3, 4, A8UI, true);
        plane->addVertex(P3_N3_UV_C(-10, +10, 0, 0, 0, +1, 0, 0, 248, 166, 10, 0));
        plane->addVertex(P3_N3_UV_C(+10, +10, 0, 0, 0, +1, 1, 0, 248, 166, 10, 0));
        plane->addVertex(P3_N3_UV_C(+10, -10, 0, 0, 0, +1, 1, 1, 248, 166, 10, 0));
        plane->addVertex(P3_N3_UV_C(+10, -10, 0, 0, 0, +1, 1, 1, 248, 166, 10, 0));
        plane->addVertex(P3_N3_UV_C(-10, -10, 0, 0, 0, +1, 0, 1, 248, 166, 10, 0));
        plane->addVertex(P3_N3_UV_C(-10, +10, 0, 0, 0, +1, 0, 0, 248, 166, 10, 0));

        ptr<Texture> logoTexture;
        int w;
        int h;
        int channels;
        int size;
        unsigned char* data = load(dir + "/textures/checker.png", size);
        unsigned char *logo = stbi_load_from_memory(data, size, &w, &h, &channels, 0);
        if (channels == 3) {
            logoTexture = new Texture2D(w, h, RGB8, RGB, UNSIGNED_BYTE, Texture::Parameters(), Buffer::Parameters(), CPUBuffer(logo));
        } else {
            logoTexture = new Texture2D(w, h, RGBA8, RGBA, UNSIGNED_BYTE, Texture::Parameters(), Buffer::Parameters(), CPUBuffer(logo));
        }

        string
        source1 = string((char*)load(dir + "/shaders/precision.glsl", size))
                + string((char*)load(dir + "/shaders/camera.glsl", size))
                + string((char*)load(dir + "/shaders/spotlight.glsl", size))
                + string((char*)load(dir + "/shaders/plasticVS.glsl", size))
                + string((char*)load(dir + "/shaders/plasticFS.glsl", size)),
        source2 = string((char*)load(dir + "/shaders/precision.glsl", size))
                + string((char*)load(dir + "/shaders/camera.glsl", size))
                + string((char*)load(dir + "/shaders/spotlight.glsl", size))
                + string((char*)load(dir + "/shaders/texturedPlastic.glsl", size));

        ptr<Module> module1 = new Module(330, (char*) source1.c_str());
        vector< ptr<Module> > modules1;
        modules1.push_back(module1);
        p1 = new Program(modules1);

        ptr<Module> module2 = new Module(330, (char*) source2.c_str());
        vector< ptr<Module> > modules2;
        modules2.push_back(module2);
        p2 = new Program(modules2);

        p1->getUniform3f("worldLightPos")->set(vec3f(3.0f, 3.0f, 3.0f));
        p1->getUniform3f("worldLightDir")->set(vec3f(-0.5f, -0.5f, -0.707107f));
        p1->getUniform2f("spotlightAngle")->set(vec2f(0.4f, 0.6f));

        p2->getUniformSampler("tex")->set(logoTexture);

        worldCamera = p1->getUniform3f("worldCameraPos");
        localToWorld1 = p1->getUniformMatrix4f("localToWorld");
        localToScreen1 = p1->getUniformMatrix4f("localToScreen");
        localToWorld2 = p2->getUniformMatrix4f("localToWorld");
        localToScreen2 = p2->getUniformMatrix4f("localToScreen");

        fb = FrameBuffer::getDefault();
        fb->setDepthTest(true, LESS);
    }

    void redisplay(double t, double dt)
    {
        fb->clear(true, false, true);

        mat4f cameraToWorld = mat4f::rotatex(90);
        cameraToWorld = cameraToWorld * mat4f::rotatey(-alpha);
        cameraToWorld = cameraToWorld * mat4f::rotatex(-theta);
        cameraToWorld = cameraToWorld * mat4f::translate(vec3f(0.0, 0.0, dist));

        mat4f worldToCamera = cameraToWorld.inverse();

        vec4<GLint> vp = fb->getViewport();
        float width = (float) vp.z;
        float height = (float) vp.w;
        float vfov = degrees(2 * atan(height / width * tan(radians(fov / 2))));
        mat4f cameraToScreen = mat4f::perspectiveProjection(vfov, width / height, 0.1f, 1e5f);

        worldCamera->set(cameraToWorld * vec3f::ZERO);

        localToWorld1->setMatrix(mat4f::rotatez(15));
        localToScreen1->setMatrix(cameraToScreen * worldToCamera * mat4f::rotatez(15));
        fb->draw(p1, *cube);

        localToWorld2->setMatrix(mat4f::translate(vec3f(0.0, 0.0, -2.0)) * mat4f::rotatez(180));
        localToScreen2->setMatrix(cameraToScreen * worldToCamera * mat4f::translate(vec3f(0.0, 0.0, -2.0)) * mat4f::rotatez(180));
        fb->draw(p2, *plane);

        GlutWindow::redisplay(t, dt);

        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->flush();
        }
    }

    void reshape(int x, int y)
    {
        fb->setViewport(vec4<GLint>(0, 0, x, y));
        fb->setDepthTest(true, LESS);
        GlutWindow::reshape(x, y);
        idle(false);
    }

    bool keyTyped(unsigned char c, modifier m, int x, int y)
    {
        if (c == 27) {
            ::exit(0);
        }
        return true;
    }

    bool mouseWheel(wheel b, modifier m, int x, int y)
    {
        dist = b == WHEEL_UP ? dist * 1.05f : dist / 1.05f;
        return true;
    }

    bool mousePassiveMotion(int x, int y)
    {
        alpha = float(x) / getWidth() * 360.0;
        theta = (float(y) / getHeight() - 0.5) * 180.0;
        return true;
    }

    static static_ptr<Window> app;
};

static_ptr<Window> RenderExample::app;

int renderExample(int argc, char* argv[])
{
    assert(argc > 2);
    atexit(Object::exit);
    RenderExample::app = new RenderExample(string(argv[2]));
    RenderExample::app->start();
    return 0;
}

MainFunction renderMain("render", renderExample);
