//
//  imGui integration in lolengine
//
//  Copyright © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "imgui.cpp"

#include <cstdio>

using namespace lol;

//Imgui extension ---------------------------------------------------------------------------------
namespace ImGui
{
    IMGUI_API void SetNextWindowDockingAndSize(const ImVec2& size, ImGuiSetDock dock, const ImVec2& padding, ImGuiSetCond cond)
    {
        SetNextWindowDockingAndSize(size, dock, ImVec4(vec2(padding).xyxy), cond);
    }
    IMGUI_API void SetNextWindowDockingAndSize(const ImVec2& size, ImGuiSetDock dock, const ImVec4& padding, ImGuiSetCond cond)
    {
        vec4 pdg = padding;
        vec2 vsz = vec2(Video::GetSize());
        vec2 ctr = pdg.xy + (((vsz - pdg.zw) - pdg.xy) * .5f);
        vec2 pos = vec2();

        switch (dock)
        {
        case ImGuiSetDock_Center:       pos = vec2(ctr.x - (size.x * .5f), ctr.y - (size.y * .5f)); break;
        case ImGuiSetDock_Top:          pos = vec2(ctr.x - (size.x * .5f), pdg.y); break;
        case ImGuiSetDock_TopRight:     pos = vec2(vsz.x - (size.x + pdg.z), pdg.y); break;
        case ImGuiSetDock_Right:        pos = vec2(vsz.x - (size.x + pdg.z), ctr.y - (size.y * .5f)); break;
        case ImGuiSetDock_BottomRight:  pos = vec2(vsz.x - (size.x + pdg.z), vsz.y - (size.y + pdg.w)); break;
        case ImGuiSetDock_Bottom:       pos = vec2(ctr.x - (size.x * .5f), vsz.y - (size.y + pdg.w)); break;
        case ImGuiSetDock_BottomLeft:   pos = vec2(pdg.x, vsz.y - (size.y + pdg.w)); break;
        case ImGuiSetDock_Left:         pos = vec2(pdg.x, ctr.y - (size.y * .5f)); break;
        case ImGuiSetDock_TopLeft:      pos = vec2(pdg.x, pdg.y); break;
        }

        ImGui::SetNextWindowPos(pos, cond);
        ImGui::SetNextWindowSize(size, cond);
    }

    IMGUI_API void SetNextWindowDocking(ImGuiSetDock dock, const ImVec2& padding, ImGuiSetCond cond)
    {
        SetNextWindowDocking(dock, ImVec4(vec2(padding).xyxy), cond);
    }
    IMGUI_API void SetNextWindowDocking(ImGuiSetDock dock, const ImVec4& padding, ImGuiSetCond cond)
    {
        vec2 vsz = vec2(Video::GetSize());
        vec2 size = vec2();
        vec2 pos = vec2();
        vec4 pdg = padding;

        switch (dock)
        {
        case ImGuiSetDock_Center:       size = vsz - vec2(pdg.x + pdg.z, pdg.y + pdg.w);  break;
        case ImGuiSetDock_Top:          size = vec2(vsz.x - (pdg.x + pdg.z), vsz.y *.5f - pdg.y); break;
        case ImGuiSetDock_TopRight:     size = vec2(vsz.x *.5f - pdg.z, vsz.y *.5f - pdg.y); break;
        case ImGuiSetDock_Right:        size = vec2(vsz.x *.5f - pdg.z, vsz.y - (pdg.y + pdg.w)); break;
        case ImGuiSetDock_BottomRight:  size = vec2(vsz.x *.5f - pdg.z, vsz.y *.5f - pdg.w); break;
        case ImGuiSetDock_Bottom:       size = vec2(vsz.x - (pdg.x + pdg.z), vsz.y *.5f - pdg.w); break;
        case ImGuiSetDock_BottomLeft:   size = vec2(vsz.x *.5f - pdg.x, vsz.y *.5f - pdg.w); break;
        case ImGuiSetDock_Left:         size = vec2(vsz.x *.5f - pdg.x, vsz.y - (pdg.y + pdg.w)); break;
        case ImGuiSetDock_TopLeft:      size = vec2(vsz.x *.5f - pdg.x, vsz.y *.5f - pdg.y); break;
        }

        SetNextWindowDockingAndSize(size, dock, padding, cond);
    }

    IMGUI_API float GetMainMenuBarHeight()
    {
        ImGuiContext& g = *GImGui;
        return g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    }
}

//LolImGui ----------------------------------------------------------------------------------------
#define Line(s) ((s) + "\n")

//-----------------------------------------------------------------------------
LolImGui::LolImGui()
{
    m_gamegroup = GAMEGROUP_IMGUI;
    m_drawgroup = DRAWGROUP_IMGUI;

    //Build shader code -------------------------------------------------------
    ShaderVar out_vertex = ShaderVar::GetShaderOut(ShaderProgram::Vertex);
    ShaderVar out_pixel = ShaderVar::GetShaderOut(ShaderProgram::Pixel);

    ShaderVar pass_texcoord = ShaderVar(ShaderVariable::Varying, ShaderVariableType::Vec2, "texcoord");
    ShaderVar pass_color = ShaderVar(ShaderVariable::Varying, ShaderVariableType::Vec4, "color");

    ShaderVar in_position = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec2, "position");
    ShaderVar in_texcoord = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec2, "texcoord");
    ShaderVar in_color = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec4, "color");

    m_ortho.m_var = ShaderVar(ShaderVariable::Uniform, ShaderVariableType::Mat4, "ortho");
    m_texture.m_var = ShaderVar(ShaderVariable::Uniform, ShaderVariableType::sampler2D, "texture");

    ShaderBlock imgui_vertex("imgui_vertex");
    imgui_vertex
        << out_vertex << m_ortho << in_position
        << pass_texcoord << in_texcoord
        << pass_color << in_color;
    imgui_vertex.SetMainCode(String() +
        Line(out_vertex + " = .5 *" + m_ortho + " * vec4(" + in_position + ", -1.0, 1.0);")
        + Line(pass_texcoord + " = " + in_texcoord + ";")
        + Line(pass_color + " = " + in_color + ";")
        );

    ShaderBlock imgui_pixel("imgui_pixel");
    imgui_pixel << m_texture << pass_texcoord << pass_color << out_pixel;
    imgui_pixel.SetMainCode(String() +
        Line(String()
        + "vec4 col = " + pass_color + " * texture2D(" + m_texture + ", " + pass_texcoord + ");")
        + Line(String()
        + "if (col.a == 0.0) discard; ")
        + Line(out_pixel + " = col;")
        );

    m_builder
        << ShaderProgram::Vertex << imgui_vertex
        << ShaderProgram::Pixel << imgui_pixel;

    //Input Setup -------------------------------------------------------------
    InputProfile& ip = m_profile;
    ip.AddBindings<LolImGuiKey, LolImGuiKey::KEY_START, LolImGuiKey::KEY_END>(InputProfileType::Keyboard);
    //for (int i = LolImGuiKey::KEY_START; i < LolImGuiKey::KEY_END; ++i)
    //    m_profile << InputProfile::Keyboard(i, LolImGuiKey(i).ToString());
    for (int i = LolImGuiKey::MOUSE_KEY_START; i < LolImGuiKey::MOUSE_KEY_END; ++i)
        m_profile << InputProfile::MouseKey(i, LolImGuiKey(i).ToString());

    for (int i = LolImGuiAxis::MOUSE_AXIS_START; i < LolImGuiAxis::MOUSE_AXIS_END; ++i)
        m_profile << InputProfile::MouseAxis(i, LolImGuiAxis(i).ToString());

    Ticker::Ref(m_controller = new Controller("ImGui_Controller"));
    m_controller->Init(m_profile);
    m_mouse = InputDevice::GetMouse();
    m_keyboard = InputDevice::GetKeyboard();

    //m_controller->Get
    //#   define KB InputProfile::Keyboard
//    m_profile
//        << InputProfile::Keyboard(idx, g_name_key_Left);
//#   undef KB

}
LolImGui::~LolImGui()
{
    ImGui::GetIO().Fonts->TexID = nullptr;
    Ticker::Unref(m_font);
    m_font = nullptr;

    Shader::Destroy(m_shader);
    delete m_vdecl;
}

//-----------------------------------------------------------------------------
LolImGui* g_lolimgui = nullptr;
void LolImGui::Init()
{
    Ticker::Ref(g_lolimgui = new LolImGui());

    ImGuiIO& io = ImGui::GetIO();
    //ImFont* font0 = io.Fonts->AddFontDefault();

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab]         = LolImGuiKey::Tab;
    io.KeyMap[ImGuiKey_LeftArrow]   = LolImGuiKey::LeftArrow;
    io.KeyMap[ImGuiKey_RightArrow]  = LolImGuiKey::RightArrow;
    io.KeyMap[ImGuiKey_UpArrow]     = LolImGuiKey::UpArrow;
    io.KeyMap[ImGuiKey_DownArrow]   = LolImGuiKey::DownArrow;
    io.KeyMap[ImGuiKey_Home]        = LolImGuiKey::Home;
    io.KeyMap[ImGuiKey_End]         = LolImGuiKey::End;
    io.KeyMap[ImGuiKey_Delete]      = LolImGuiKey::Delete;
    io.KeyMap[ImGuiKey_Backspace]   = LolImGuiKey::Backspace;
    io.KeyMap[ImGuiKey_Enter]       = LolImGuiKey::Enter;
    io.KeyMap[ImGuiKey_Escape]      = LolImGuiKey::Escape;
    io.KeyMap[ImGuiKey_A]           = LolImGuiKey::A;
    io.KeyMap[ImGuiKey_C]           = LolImGuiKey::C;
    io.KeyMap[ImGuiKey_V]           = LolImGuiKey::V;
    io.KeyMap[ImGuiKey_X]           = LolImGuiKey::X;
    io.KeyMap[ImGuiKey_Y]           = LolImGuiKey::Y;
    io.KeyMap[ImGuiKey_Z]           = LolImGuiKey::Z;

    //Func pointer
    io.RenderDrawListsFn = LolImGui::RenderDrawLists;
    io.SetClipboardTextFn = LolImGui::SetClipboardCallback;
    io.GetClipboardTextFn = LolImGui::GetClipboardCallback;
    io.ClipboardUserData = &g_lolimgui->m_clipboard;
}

/* CALLBACKS
void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
ImGuiIO& io = ImGui::GetIO();
if (c > 0 && c < 0x10000)
io.AddInputCharacter((unsigned short)c);
}

*/

void LolImGui::Shutdown()
{
    if (g_lolimgui)
    {
        Ticker::Unref(g_lolimgui);
        g_lolimgui = nullptr;
    }

    ImGui::Shutdown();
}

//-----------------------------------------------------------------------------
String LolImGui::GetClipboard()
{
    return g_lolimgui ? g_lolimgui->m_clipboard : "";
}

void LolImGui::SetClipboardCallback(void *data, const char* text)
{
    String *clipboard = (String *)data;
    *clipboard = text;
}
const char* LolImGui::GetClipboardCallback(void *data)
{
    String *clipboard = (String *)data;
    return clipboard->C();
}

//-----------------------------------------------------------------------------
void LolImGui::TickGame(float seconds)
{
    super::TickGame(seconds);

    ImGuiIO& io = ImGui::GetIO();

    //Init Texture
    if (!m_font)
    {
        // Build texture
        unsigned char* pixels;
        ivec2 size;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &size.x, &size.y);

        Image* image = new Image();
        image->Copy(pixels, size, PixelFormat::RGBA_8);

        Ticker::Ref(m_font = new TextureImage("", image));
    }
    //Texture has been created
    if (m_font && m_font->GetTexture())
    {
        io.Fonts->TexID = (void *)(intptr_t)m_font;
    }

    // Setup display size (every frame to accommodate for window resizing)
    vec2 video_size = vec2(0);
    video_size = vec2(Video::GetSize());
    io.DisplaySize = ImVec2(video_size.x, video_size.y);

    //Setup time step
    io.DeltaTime = seconds;
    io.MouseDrawCursor = true;

    //Update Keyboard
    io.KeyCtrl = false;
    io.KeyShift = false;
    for (int i = LolImGuiKey::KEY_START; i < LolImGuiKey::KEY_END; ++i)
    {
        switch (i)
        {
        default:
            io.KeysDown[i] = m_controller->IsKeyPressed(i);
            break;
        case LolImGuiKey::LShift:
        case LolImGuiKey::RShift:
            io.KeyShift = (io.KeyShift || m_controller->IsKeyPressed(i));
            break;
        case LolImGuiKey::LCtrl:
        case LolImGuiKey::RCtrl:
            io.KeyCtrl = (io.KeyCtrl || m_controller->IsKeyPressed(i));
            break;
        }
    }

    m_keyboard->SetTextInputActive(io.WantTextInput);

    //Update text input
    String text = m_keyboard->GetText();
    //text.case_change(io.KeyShift);
    for (int i = 0; i < text.count(); ++i)
        io.AddInputCharacter(text[i]);

    //Update mouse
    if (m_mouse)
    {
        vec2 cursor = m_mouse->GetCursor(0);
        cursor.y = 1.f - cursor.y;
        cursor *= video_size;
        io.MousePos = ImVec2(cursor.x, cursor.y);
        //msg::debug("%.2f/%.2f\n", io.MousePos.x, io.MousePos.y);
        io.MouseWheel = m_controller->GetAxisValue(LolImGuiAxis::Scroll);

        for (int i = LolImGuiKey::MOUSE_KEY_START; i < LolImGuiKey::MOUSE_KEY_END; ++i)
        {
            switch (i)
            {
            default:
                io.MouseDown[i - LolImGuiKey::MOUSE_KEY_START] = m_controller->IsKeyPressed(i);
                break;
            case LolImGuiKey::Focus:
                if (m_controller->IsKeyReleased(i))
                {
                    //msg::debug("Not focused .....\n");
                    io.MousePos = ImVec2(-1.f, -1.f);
                }
                else
                {
                    //msg::debug("Focused !!\n");
                }
                break;
            }
        }
    }

    // Start the frame
    ImGui::NewFrame();
}

//-----------------------------------------------------------------------------
void LolImGui::TickDraw(float seconds, Scene &scene)
{
    super::TickDraw(seconds, scene);

    scene.AddPrimitiveRenderer(this, new PrimitiveLolImGui());
}
void PrimitiveLolImGui::Render(Scene& scene, PrimitiveSource* primitive)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.Fonts->TexID)
        ImGui::Render();
}

//// Data
//static GLFWwindow*  g_Window = NULL;
//static double       g_Time = 0.0f;
//static bool         g_MousePressed[3] = { false, false, false };
//static float        g_MouseWheel = 0.0f;
//static GLuint       g_FontTexture = 0;

//-------------------------------------------------------------------------
// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
//-------------------------------------------------------------------------
void LolImGui::RenderDrawLists(ImDrawData* draw_data)
{
    g_lolimgui->RenderDrawListsMethod(draw_data);
}
void LolImGui::RenderDrawListsMethod(ImDrawData* draw_data)
{
    if (draw_data == nullptr)
        return;

    vec2 size = vec2(Video::GetSize());
    float alpha = 1.f;
    mat4 ortho = mat4::ortho(size.x * alpha, size.y * alpha, -1000.f, 1000.f)
        * mat4::lookat(vec3::axis_z, vec3::zero, vec3::axis_y)
        * mat4::scale(vec3::axis_x - vec3::axis_y - vec3::axis_z)
        * mat4::translate(-size.x * .5f * alpha, -size.y * .5f * alpha, 0.f);

    //Create shader
    if (!m_shader)
    {
        String code;
        m_builder.Build(code);

        m_shader = Shader::Create(m_builder.GetName(), code);
        ASSERT(m_shader);

        m_ortho.m_uniform = m_shader->GetUniformLocation(m_ortho.m_var);
        m_texture.m_uniform = m_shader->GetUniformLocation(m_texture.m_var);

        m_attribs
            << m_shader->GetAttribLocation(VertexUsage::Position, 0)
            << m_shader->GetAttribLocation(VertexUsage::TexCoord, 0)
            << m_shader->GetAttribLocation(VertexUsage::Color, 0);

        m_vdecl = new VertexDeclaration(
            VertexStream<vec2, vec2, u8vec4>(
                VertexUsage::Position,
                VertexUsage::TexCoord,
                VertexUsage::Color));
    }

    //Do not render without shader
    if (!m_shader)
        return;

    RenderContext rc;
    rc.SetCullMode(CullMode::Disabled);
    rc.SetDepthFunc(DepthFunc::Disabled);
    rc.SetScissorMode(ScissorMode::Enabled);

    m_shader->Bind();
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();

        //Register uniforms
        m_shader->SetUniform(m_ortho, ortho);
        m_shader->SetUniform(m_texture, m_font->GetTexture()->GetTextureUniform(), 0);

        struct Vertex
        {
            vec2 pos, tex;
            u8vec4 color;
        };

        VertexBuffer* vbo = new VertexBuffer(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
        ImDrawVert *vert = (ImDrawVert *)vbo->Lock(0, 0);
        memcpy(vert, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
        vbo->Unlock();

        IndexBuffer *ibo = new IndexBuffer(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        ImDrawIdx *indices = (ImDrawIdx *)ibo->Lock(0, 0);
        memcpy(indices, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        ibo->Unlock();

        m_font->Bind();
        ibo->Bind();
        m_vdecl->Bind();
        m_vdecl->SetStream(vbo, m_attribs[0], m_attribs[1], m_attribs[2]);

        const ImDrawIdx* idx_buffer_offset = 0;
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[(int)cmd_i];
            TextureImage* image = (TextureImage*)pcmd->TextureId;
            if (image) image->Bind();

            rc.SetScissorRect(vec4(pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w));

#ifdef SHOW_IMGUI_DEBUG
            //-----------------------------------------------------------------
            //<Debug render> --------------------------------------------------
            //-----------------------------------------------------------------
            //Doesn't work anymore ......
            static uint32_t idx_buffer_offset_i = 0;
            if (cmd_i == 0)
                idx_buffer_offset_i = 0;

            float mod = -200.f;
            vec3 off = vec3(vec2(-size.x, -size.y), 0.f);
            vec3 pos[4] = {
                (1.f / mod) * (off + vec3(0.f)),
                (1.f / mod) * (off + size.x * vec3::axis_x),
                (1.f / mod) * (off + size.x * vec3::axis_x + size.y * vec3::axis_y),
                (1.f / mod) * (off + size.y * vec3::axis_y)
            };
            for (int i = 0; i < 4; ++i)
                Debug::DrawLine(pos[i], pos[(i + 1) % 4], Color::white);
            ImDrawVert* buf = vert;
            for (uint16_t i = 0; i < pcmd->ElemCount; i += 3)
            {
                uint16_t ib = indices[idx_buffer_offset_i + i];
                vec2 pos[3];
                pos[0] = vec2(buf[ib + 0].pos.x, buf[ib + 0].pos.y);
                pos[1] = vec2(buf[ib + 1].pos.x, buf[ib + 1].pos.y);
                pos[2] = vec2(buf[ib + 2].pos.x, buf[ib + 2].pos.y);
                vec4 col[3];
                col[0] = vec4(Color::FromRGBA32(buf[ib + 0].col).arg, 1.f);
                col[1] = vec4(Color::FromRGBA32(buf[ib + 1].col).arg, 1.f);
                col[2] = vec4(Color::FromRGBA32(buf[ib + 2].col).arg, 1.f);
                Debug::DrawLine((off + vec3(pos[0], 0.f)) / mod, (off + vec3(pos[1], 0.f)) / mod, col[0]);
                Debug::DrawLine((off + vec3(pos[1], 0.f)) / mod, (off + vec3(pos[2], 0.f)) / mod, col[1]);
                Debug::DrawLine((off + vec3(pos[2], 0.f)) / mod, (off + vec3(pos[0], 0.f)) / mod, col[2]);
            }
            idx_buffer_offset_i += pcmd->ElemCount;

            //-----------------------------------------------------------------
            //<\Debug render> -------------------------------------------------
            //-----------------------------------------------------------------
#endif //SHOW_IMGUI_DEBUG
            //Debug::DrawLine(vec2::zero, vec2::axis_x /*, Color::green*/);

            m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, pcmd->ElemCount, (const short*)idx_buffer_offset);

            idx_buffer_offset += pcmd->ElemCount;

            if (image) image->Unbind();
        }

        m_vdecl->Unbind();
        ibo->Unbind();
        m_font->Unbind();

        delete vbo;
        delete ibo;
    }

    m_shader->Unbind();
}

