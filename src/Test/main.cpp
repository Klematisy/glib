#if 0
#include "vladoom.h"

int main() {
    vladoom();
}

#else

#include "DrawUtils/draw.h"
#include "Geometry/entity.h"
#include "Graphics/RendererCore/window.h"

int main() {
    using namespace glib;
    namespace rc = RendererCore;

    rc::Window window(600, 600, "vlad");
    SceneRenderer sr(&window);

    rc::ImageInfo image("resources/images/beautiful_minimalistic_boy.png");

    Geom::Entity quad, screen;
    quad.transform = std::make_shared<Geom::Transform>();
    quad.material = std::make_shared<Geom::Material>();
    quad.mesh = std::make_shared<Geom::Mesh>();
    *quad.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    quad.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
    quad.transform->position = {1.f, 1.f, 0.f};

    screen.transform = std::make_shared<Geom::Transform>();
    screen.material = std::make_shared<Geom::Material>();
    screen.mesh = std::make_shared<Geom::Mesh>();
    *screen.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    screen.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
    screen.transform->position = {1.f, 1.f, 0.f};
    screen.transform->scale = {2.f, 2.f, 1.f};

    OrthographicCamera camera;
    camera.SetRenderRange(0, 2, 0, 2);
    sr.UseCamera(&camera);

    FrameBaker fb;

    Shader shader;
    shader.AddSrcFiles("src/Test/lol.glsl");
    shader.Compile();

    screen.material->image = &fb.GetImage();
    screen.material->shader = shader.GetShaderProgram();

    sr.RegisterFrameBaker(fb);

    float a = 0.f;
    while (window.IsOpen()) {
        sr.StartDraw();

        sr.StartBake(fb);
        sr.DrawEntity(quad);
        sr.EndBake();

        shader.GetShaderProgram()->SetFloat("u_Time", a);
        sr.DrawEntity(screen);

        sr.EndDraw();
    }

    return 0;
}

#endif