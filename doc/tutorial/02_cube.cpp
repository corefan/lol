//
//  Lol Engine — Cube tutorial
//
//  Copyright © 2011—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(02_cube);

class Cube : public WorldEntity
{
public:
    Cube()
      : m_angle(0),
        m_mesh({ /* Front vertices/colors */
                 { vec3(-1.0, -1.0,  1.0), vec3(1.0, 0.0, 1.0) },
                 { vec3( 1.0, -1.0,  1.0), vec3(0.0, 1.0, 0.0) },
                 { vec3( 1.0,  1.0,  1.0), vec3(1.0, 0.5, 0.0) },
                 { vec3(-1.0,  1.0,  1.0), vec3(1.0, 1.0, 0.0) },
                 /* Back */
                 { vec3(-1.0, -1.0, -1.0), vec3(1.0, 0.0, 0.0) },
                 { vec3( 1.0, -1.0, -1.0), vec3(0.0, 0.5, 0.0) },
                 { vec3( 1.0,  1.0, -1.0), vec3(0.0, 0.5, 1.0) },
                 { vec3(-1.0,  1.0, -1.0), vec3(0.0, 0.0, 1.0) } }),
        m_lines_indices({ 0, 1, 1, 2, 2, 3, 3, 0,
                          4, 5, 5, 6, 6, 7, 7, 4,
                          0, 4, 1, 5, 2, 6, 3, 7, }),
        m_faces_indices({ 0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                          7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                          4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3, }),
        m_ready(false)
    {
        m_camera = new Camera();
        m_camera->SetProjection(mat4::perspective(radians(30.f), 960.f, 600.f, .1f, 1000.f));
        m_camera->SetView(mat4::lookat(vec3(-15.f, 5.f, 0.f),
            vec3(0.f, -1.f, 0.f),
            vec3(0.f, 1.f, 0.f)));
        Scene& scene = Scene::GetScene();
        scene.PushCamera(m_camera);
        Ticker::Ref(m_camera);

    }

    ~Cube()
    {
        Scene& scene = Scene::GetScene();
        scene.PopCamera(m_camera);
        Ticker::Unref(m_camera);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);

        m_angle += seconds * radians(45.0f);

        mat4 anim = mat4::rotate(m_angle, vec3(0, 1, 0));
        mat4 model = mat4::translate(vec3(0, 0, -4.5));
        mat4 view = mat4::lookat(vec3(0, 2, 0), vec3(0, 0, -4), vec3(0, 1, 0));
        mat4 proj = mat4::perspective(radians(45.0f), 640.0f, 480.0f, 0.1f, 10.0f);

        m_matrix = proj * view * model * anim;

        {
            auto context0 = Debug::DrawContext::New(Color::red);
            {
                auto context1 = Debug::DrawContext::New(Color::blue);
                Debug::DrawBox(box3(vec3(0.f), vec3(1.2f)));
                Debug::DrawGrid(vec3(0.f), vec3::axis_x, vec3::axis_y, vec3::axis_z, 10.0f);
                {
                    auto context2 = Debug::DrawContext::New(context0);
                    Debug::DrawBox(box3(vec3(0.f), vec3(1.3f)));
                }
                {
                    auto context2 = Debug::DrawContext::New(context0);
                    context2.SetColor(Color::yellow);
                    Debug::DrawBox(box3(vec3(-1.f), vec3(1.4f)));
                }
            }
            Debug::DrawBox(box3(vec3(0.f), vec3(1.1f)));
        }
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(02_cube));

            m_mvp = m_shader->GetUniformLocation("u_matrix");
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_color = m_shader->GetAttribLocation(VertexUsage::Color, 0);

            m_vdecl =
              new VertexDeclaration(VertexStream<vec3,vec3>(VertexUsage::Position,
                                                            VertexUsage::Color));

            m_vbo = new VertexBuffer(m_mesh.bytes());
            void *mesh = m_vbo->Lock(0, 0);
            memcpy(mesh, &m_mesh[0], m_mesh.bytes());
            m_vbo->Unlock();

            m_lines_ibo = new IndexBuffer(m_lines_indices.bytes());
            void *indices = m_lines_ibo->Lock(0, 0);
            memcpy(indices, &m_lines_indices[0], m_lines_indices.bytes());
            m_lines_ibo->Unlock();

            m_faces_ibo = new IndexBuffer(m_faces_indices.bytes());
            indices = m_faces_ibo->Lock(0, 0);
            memcpy(indices, &m_faces_indices[0], m_faces_indices.bytes());
            m_faces_ibo->Unlock();

            /* FIXME: this object never cleans up */
            m_ready = true;
        }

        Renderer::Get()->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();

        m_shader->SetUniform(m_mvp, m_matrix);
        m_lines_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Lines, m_lines_indices.count());
        m_lines_ibo->Unbind();

        m_shader->SetUniform(m_mvp, m_matrix * mat4::scale(0.5f));
        m_faces_ibo->Bind();
        m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, m_faces_indices.count());
        m_faces_ibo->Unbind();

        m_vdecl->Unbind();
    }

private:
    Camera* m_camera = nullptr;
    float m_angle;
    mat4 m_matrix;
    array<vec3,vec3> m_mesh;
    array<uint16_t> m_lines_indices, m_faces_indices;

    Shader *m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_mvp;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    IndexBuffer *m_lines_ibo, *m_faces_ibo;

    bool m_ready;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 2: Cube", ivec2(640, 480), 60.0f);

    new DebugFps(5, 5);
    new Cube();

    app.Run();

    return EXIT_SUCCESS;
}

