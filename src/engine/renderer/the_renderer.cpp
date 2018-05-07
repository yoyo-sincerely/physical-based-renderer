/*
 * show rendered images 
 *
 */
#include "the_renderer.h"
//-----------------------------------------------------------------------------
// DEMO CODE
//-----------------------------------------------------------------------------

using namespace std;
#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_RENDERER_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_RENDERER_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_RENDERER_WINDOWS)


RayTracing::Scene                            scene;
RayTracing::RayTracer*                        tracer = nullptr;
ExampleAppLog                    g_Logger;
static bool                        g_ShowLogger = true;
static bool                        g_IsLoadImage = false;
static GLuint                    g_FontTexture = 0;
static ImVector<ImFontAtlas *>    g_Image;
static bool                        g_ShowImage = true;
unsigned int                    g_Image_Index = 0;

void ShowRendererWindow(bool* p_open)
{

    static bool show_app_main_menu_bar = true;

    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;


    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_close) p_open = NULL;
    
    ImGui::SetNextWindowSize(ImVec2(800, 800), ImGuiCond_FirstUseEver);
    ImVec2 size =  ImGui::GetWindowSize();
    ImVec2 pos = ImGui::GetWindowPos();
    //g_Logger.AddLog("size is :: %lf \t %lf \n", size.x, size.y);
    //g_Logger.AddLog("pos is :: %lf \t %lf \n", pos.x, pos.y);

    ImGui::SetWindowPos(pos, ImGuiCond_Always);

    if (!ImGui::Begin("The Renderer", p_open, window_flags))
    {
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

    // Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::Button("last image"))
    {
        ShowLastImage();
    }
    ImGui::SameLine();
    if (ImGui::Button("next image"))
    {
        ShowNextImage();
    }

    if (g_ShowLogger) ShowLogger(&g_ShowLogger);
    if (g_ShowImage) ShowImage();

    ImGui::End();
}

static void ShowMenuFile() 
{
    if (ImGui::MenuItem("show log"))                g_ShowLogger = true;
    if (ImGui::MenuItem("show/close image"))        g_ShowImage ^= 1;
    ImGui::Separator();
    if (ImGui::MenuItem("render test"))                RenderTest();
    if (ImGui::MenuItem("render"))                    Render();
    if (ImGui::MenuItem("ray tracing in weekend"))    raytracing();
}

static void ShowLastImage() 
{
    if (g_Image.empty()) return;

    g_Image_Index--;
    g_Image_Index += g_Image.size(); 
    g_Image_Index %= g_Image.size(); 
}

static void ShowNextImage()
{
    if (g_Image.empty()) return;

    g_Image_Index++;
    g_Image_Index %= g_Image.size();
}

static void raytracing()
{    ImFontAtlas * testBuffer = new ImFontAtlas;
    testBuffer->TexWidth = 400;
    testBuffer->TexHeight = 400;

    testBuffer->TexPixelsRGBA32 = (unsigned int *)malloc(sizeof(*testBuffer->TexPixelsRGBA32) * testBuffer->TexWidth * testBuffer->TexHeight + 1);

    ray_tracing(testBuffer->TexPixelsRGBA32, testBuffer->TexWidth, testBuffer->TexHeight, sizeof(*testBuffer->TexPixelsRGBA32) * testBuffer->TexWidth, 5, 32);
    for (int i = 0; i < testBuffer->TexWidth * testBuffer->TexHeight; i++)
    {
        unsigned int color = testBuffer->TexPixelsRGBA32[i];
        int r = color % 256;
        int g = color / 256 % 256;
        int b = color / (256 * 256) % 256;
        int a = color / (256 * 256 * 256) % 256;
        //g_Logger.AddLog("r : %d, g : %d, b : %d, a : %d, i : %d \n", r, g, b, a, i);
    }
    LoadingImageRGBA(testBuffer);
}

static void RenderTest()
{
    ImFontAtlas * testBuffer = new ImFontAtlas;
    testBuffer->TexWidth = 100;
    testBuffer->TexHeight = 100;

    testBuffer->TexPixelsRGBA32 = (unsigned int *)malloc(sizeof(*testBuffer->TexPixelsRGBA32) * testBuffer->TexWidth * testBuffer->TexHeight + 1);
    for (int i = 0; i < testBuffer->TexHeight; i++)
    {
        auto prt = (unsigned int *)((char *)testBuffer->TexPixelsRGBA32 + i * sizeof(*testBuffer->TexPixelsRGBA32) * testBuffer->TexWidth);
        //g_Logger.AddLog("sizeof prt is : %d", sizeof(*prt));
        g_Logger.AddLog("texPixelrgba size is : %d\n", sizeof(*testBuffer->TexPixelsRGBA32));
        for (int j = 0; j < testBuffer->TexWidth; j++ , prt++)
        {
            double r = 1;
            double g = 0;
            double b = 0;
            *prt = ((255 & 255) << 24) | //alpha
                (((int)(b * 255) & 255) << 16) | //blue
                (((int)(g * 255) & 255) << 8) | //green
                (((int)(r * 255) & 255) << 0); //red
            //g_Logger.AddLog("prt size is : %d\t value is : %d \t%d\t%d\n", sizeof(*prt), *prt, i, j);
            //g_Logger.AddLog("rgb is : %d %d %d \n", (((int)(r * 255) & 0xFF) << 0), (((int)(g * 255) & 0xFF) << 8), (((int)(b * 255) & 0xFF) << 16));
        }
    }
    LoadingImageRGBA(testBuffer);
}

//test render
static void Render()
{
    //init
    SetDefaultScene(&scene);
    tracer = new RayTracing::RayTracer(&scene);
    ImFontAtlas * buffer = new ImFontAtlas;
    buffer->TexWidth = 800;
    buffer->TexHeight = 600;
    buffer->TexPixelsRGBA32 = (unsigned int *)malloc(sizeof(*buffer->TexPixelsRGBA32) * buffer->TexWidth * buffer->TexHeight);
    //memset(buffer->TexPixelsRGBA32, 0, sizeof(buffer->TexPixelsRGBA32) * buffer->TexWidth * buffer->TexHeight);
    //g_Logger.AddLog("TexPixelsRGBA32 is %d \n", sizeof(buffer->TexPixelsRGBA32));

    tracer->Render(buffer->TexPixelsRGBA32, buffer->TexWidth, buffer->TexHeight, sizeof(*buffer->TexPixelsRGBA32) * buffer->TexWidth, 5);

    LoadingImageRGBA(buffer);
}

// Demonstrate creating a simple log window with basic filtering.
static void ShowLogger(bool* p_open)
{
    g_Logger.Draw("Render Logger", p_open);
}

static void LoadingImage(const char * imagePath)
{
    //ImDrawList drawList;
    int width, height, nrChannels;
    ImFontAtlas * buffer = new ImFontAtlas;
    buffer->TexPixelsAlpha8 = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    buffer->TexWidth = width;
    buffer->TexHeight = height;
    g_Logger.AddLog("[Loading Image] image width is : %d, image height is : %d, length is : %d\n", width, height, strlen((char *)buffer->TexPixelsAlpha8));
    LoadingImageRGB(buffer);
}

static void LoadingImageRGBA(ImFontAtlas * texImAtlas)
{
    if (texImAtlas->TexPixelsRGBA32 == NULL && texImAtlas->TexPixelsAlpha8 == NULL) {
        g_Logger.AddLog("data is NULL");
        return;
    }

    g_IsLoadImage = true;

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (texImAtlas->TexPixelsAlpha8 != NULL)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImAtlas->TexWidth, texImAtlas->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImAtlas->TexPixelsAlpha8);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImAtlas->TexWidth, texImAtlas->TexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImAtlas->TexPixelsRGBA32); 

    // Store our identifier
    //g_Logger.AddLog("texture id %d\n", g_FontTexture);
    texImAtlas->TexID = (void *)(intptr_t)g_FontTexture;
    g_Image.push_back(texImAtlas);
    g_Image_Index = g_Image.size() - 1;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

static void LoadingImageRGB(ImFontAtlas * texImAtlas)
{
    // Build texture atlas
    //ImDrawList drawList;
    //unsigned char *pixels = stbi_load("../data/images/dog.jpg", &width, &height, &nrChannels, 0);
    if (texImAtlas->TexPixelsAlpha8 == NULL)
    {
        return;
    }

    g_IsLoadImage = true;

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImAtlas->TexWidth, texImAtlas->TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texImAtlas->TexPixelsAlpha8);

    // Store our identifier
    //g_Logger.AddLog("texture id %d\n", g_FontTexture);
    texImAtlas->TexID = (void *)(intptr_t)g_FontTexture;
    g_Image.push_back(texImAtlas);
    g_Image_Index = g_Image.size() - 1;

    //g_Logger.AddLog("g_Image's length is : %d ", g_Image.size());
    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

static void ShowImage() 
{
    if (g_Image.empty()) return;
    if (g_FontTexture == NULL) return;
    //g_Logger.AddLog("g_Image_Index is : %d\n", g_Image_Index);
    if (g_Image_Index < 0 || g_Image_Index >= g_Image.size())
    {
        g_Logger.AddLog("g_Image_Index is NULL !!!");
        g_Image_Index = g_Image.size() - 1;
    }

    ImFontAtlas * tex = g_Image[g_Image_Index];
    if (tex != NULL) {
        ImGui::Image(tex->TexID, ImVec2((float)tex->TexWidth, (float)tex->TexHeight));
        //g_Logger.AddLog("the texID is  %d  ", tex->TexID);
    }else {
        g_Logger.AddLog("tex is NULL!!! \n");
    }
}
#else

#endif