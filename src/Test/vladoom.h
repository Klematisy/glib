#include <unistd.h>
#include <memory>
#include <fstream>
#include <chrono>

#include <nlohmann/json.hpp>
#include "coroutines.h"

#include "DrawUtils/shader.h"
#include "DrawUtils/draw.h"
#include "DrawUtils/frame_buffer.h"

#include "Geometry/entity.h"
#include "Geometry/camera.h"
#include "Geometry/utils.h"

#include "Graphics/RendererCore/window.h"

#define COLLISIONS 1

VLADLIB_NAMESPACE_USING;
using namespace Geom;
using namespace nlohmann;

namespace rc = RendererCore;

static std::unique_ptr<rc::Window> s_window;
static rc::ImageInfo s_wallsAtlas;


struct Door {
    Entity doorEntity;
    std::shared_ptr<Coroutine> doorOpenAction;
    // State
};

static std::vector<Entity*> s_3DEntities;
static std::unordered_map<std::string, int> s_textureTiles;
static std::vector<int> s_collisionsField;
static std::unique_ptr<SceneRenderer> s_sr;
static PerspectiveCamera s_pCamera;
static OrthographicCamera s_oCamera;

static Entity s_blueBackground, s_screen;
static Entity s_floor, s_potolok, s_location;

static std::unique_ptr<FrameBaker> s_FrameBaker;

static int mapW = 0;
static int mapH = 0;

static constexpr uint32_t FPS = 0;
static float delta_fps = 1.f / FPS;

static Entity initFullEntity() {
    return Entity(std::make_shared<Mesh>(),
                  std::make_shared<Transform>(),
                  std::make_shared<Material>());
}

static Mesh merge(const Mesh& m1, const Mesh& m2) {
    Mesh result;
    const auto& p1 = m1.points;
    const auto& p2 = m2.points;
    uint32_t polygon_inds[] = {0, 1, 2, 2, 3, 0};
    uint32_t maxIndex = 0;

    Polygon4 polygon1, polygon2;
    std::vector<Polygon4> polygons_to_remove;

    for (uint32_t i = 0; i < p1.size(); i+=4) {
        for (uint32_t j = 0; j < p2.size(); j+=4) {
            polygon1.Clear();
            polygon2.Clear();

            for (uint32_t k = 0; k < 4; k++) {
                polygon1.AddPoint(p1[i + k]);
                polygon2.AddPoint(p2[j + k]);
            }

            if (polygon1 == polygon2) {
                polygons_to_remove.push_back(polygon1);
                polygons_to_remove.push_back(polygon2);
            }
        }
    }


    auto addMesh = [&](const std::vector<glm::vec3>& points) {
        Polygon4 tempPolygon;
        for (uint32_t i = 0; i < points.size(); i+=4) {
            tempPolygon.Clear();
            for (uint32_t k = 0; k < 4; k++) {
                tempPolygon.AddPoint(points[i + k]);
            }

            bool polygonDeleted = false;
            for (auto& polygon : polygons_to_remove) {
                if (polygon == tempPolygon) {
                    polygonDeleted = true;
                    break;
                }
            }

            if (!polygonDeleted) {
                for (uint32_t k = 0; k < 4; k++)
                    result.points.push_back(points[i + k]);

                for (uint32_t polygon_ind : polygon_inds)
                    result.indices.push_back(maxIndex + polygon_ind);
                maxIndex += 4;
            }
        }
    };

    addMesh(p1);
    addMesh(p2);

    return std::move(result);
}

Entity getLevelLocationFromJson(const json& location) {
    Entity resultEntity = initFullEntity();

    auto& uvCords = resultEntity.material->uvCoordinates;
    resultEntity.transform->deltaPivot.x = 1.0f;
    resultEntity.transform->deltaPivot.y = 0.5f;
    Mesh& mainMesh = *resultEntity.mesh;
    const auto& blocks = location["blocks"];
    std::string tileName;

    for (const auto& block : blocks) {
        Mesh newMesh;
        Transform transform;
        int x = block["x"];
        int z = block["y"];
        transform.position = {x, 0, z};

        if (block["type"] == "Wall") {
            newMesh = MeshFactory::Get().CreateMesh("quad_tube");
            s_collisionsField[z * mapW - x] = 1;

            newMesh.points = TransformConfirmer::Confirm(newMesh, transform);
        } else if (block["type"] == "Door") {
            newMesh = MeshFactory::Get().CreateMesh("door_frame");
            float rotation = (block["orientation"] == "AxisX") ? 90.0f : 0.0f;

            transform.position.x += 0.5f;
            transform.position.z += 0.5f;

            transform.deltaPivot = {0.5f, 0.0f, 0.5f};
            transform.rotation.y = rotation;

            newMesh.points = TransformConfirmer::Confirm(newMesh, transform);
        }

        mainMesh = merge(mainMesh, newMesh);
    }

    for (const auto& block : blocks) {
        Mesh newMesh = MeshFactory::Get().CreateMesh("quad_tube");
        int x = block["x"];
        int y = block["y"];

        for (auto& point : newMesh.points) {
            point.x += (float) x;
            point.z += (float) y;
        }

        uint32_t polygonsCount = 0;
        if (block["type"] != "Door") {
            tileName = block["tileName"];
            for (uint32_t i = 0; i < mainMesh.points.size(); i += 4) {
                for (uint32_t j = 0; j < newMesh.points.size(); j += 4) {
                    bool polygonsAreEqual = true;
                    for (uint32_t k = 0; k < 4; k++) {
                        if (mainMesh.points[i + k] != newMesh.points[j + k]) {
                            polygonsAreEqual = false;
                            break;
                        }
                    }

                    if (polygonsAreEqual)
                        polygonsCount++;
                }
            }
        } else {
            tileName = "doorFrame";
            polygonsCount = 2;
        }

        int textureNum = s_textureTiles[tileName];
        for (uint32_t i = 0; i < polygonsCount; i++) {
            uvCords.emplace_back(64.0f * (float)  (textureNum % 6),      64.0f * (float)  (textureNum / 6));
            uvCords.emplace_back(64.0f * (float)  (textureNum % 6),      64.0f * (float) ((textureNum / 6) + 1));
            uvCords.emplace_back(64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float) ((textureNum / 6) + 1));
            uvCords.emplace_back(64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float)  (textureNum / 6));
        }
    }

    for (const auto& door_plane : blocks) {
        if (door_plane["type"] != "Door") continue;

        Mesh newMesh = MeshFactory::Get().CreateMesh("door_frame");
        Transform transform;

        float x = door_plane["x"];
        float z = door_plane["y"];
        float rotation = (door_plane["orientation"] == "AxisX") ? 90.0f : 0.0f;
        transform.position = {x + 0.5f, 0, z + 0.5f};
        transform.deltaPivot = {0.5f, 0, 0.5f};
        transform.rotation.y = rotation;

        newMesh.points = TransformConfirmer::Confirm(newMesh, transform);

        mainMesh = merge(mainMesh, newMesh);
    }

    return resultEntity;
}

void init() {
    MeshFactory::Get().AddMesh("quad_tube", std::function<Mesh()>([]()
        {
            return Mesh({
                { 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },

                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f },

                { 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f },

                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f },
            }, {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,

                8,  9,  10, 10, 11, 8,
                12, 13, 14, 14, 15, 12,
            });
        })
    );

    MeshFactory::Get().AddMesh("door_frame", std::function<Mesh()>([]()
       {
           return Mesh({
               { 0.0f, 0.0f, 0.0f },
               { 0.0f, 1.0f, 0.0f },
               { 1.0f, 1.0f, 0.0f },
               { 1.0f, 0.0f, 0.0f },

               { 0.0f, 0.0f, 1.0f },
               { 0.0f, 1.0f, 1.0f },
               { 1.0f, 1.0f, 1.0f },
               { 1.0f, 0.0f, 1.0f },
           }, {
               0, 1, 2, 2, 3, 0,
               4, 5, 6, 6, 7, 4,
           });
       })
    );

    s_window = std::make_unique<rc::Window>(600, 600, "glib");
    s_sr = std::make_unique<SceneRenderer>(s_window.get());
    s_wallsAtlas = rc::ImageInfo("resources/images/atlas.png");

    s_blueBackground = initFullEntity();
    s_screen = initFullEntity();
    s_floor = initFullEntity();

    s_textureTiles["floor"]     = 3;
    s_textureTiles["ceiling"]   = 4;
    s_textureTiles["blueRock"]  = 98;
    s_textureTiles["Hitler"]    = 12;
    s_textureTiles["doorFrame"] = 16;

    json firstLevel = json::parse(std::ifstream("src/Test/LEVEL_ONE.JSON"));
    mapW = firstLevel["info"]["width"];
    mapH = firstLevel["info"]["height"];

    s_collisionsField.resize(mapW * mapH);

    s_location = getLevelLocationFromJson(firstLevel);
    s_location.material->image = &s_wallsAtlas;

    s_FrameBaker = std::make_unique<FrameBaker>();
    s_sr->RegisterFrameBaker(*s_FrameBaker);

    s_screen = initFullEntity();
    s_screen.transform->scale = {0.8f, 0.6f, 1.0f};
    *s_screen.mesh = MeshFactory::Get().CreateMesh("quad");
    s_screen.material->image = &s_FrameBaker->GetImage();
    s_screen.transform->position = {0.1f, 0.3f, 0.2f};


    s_blueBackground = initFullEntity();
    *s_blueBackground.mesh = MeshFactory::Get().CreateMesh("quad");
    s_blueBackground.transform->position.z = 0.1f;
    s_blueBackground.material->colors = {
            {0.0f, 128.0f / 255, 128.0f / 255, 1.0f},
            {0.0f, 128.0f / 255, 128.0f / 255, 1.0f},
            {0.0f, 128.0f / 255, 128.0f / 255, 1.0f},
            {0.0f, 128.0f / 255, 128.0f / 255, 1.0f},
    };

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);
    gapi.EnableDepthTest();

    s_pCamera.SetRotation({0, 180, 0});
    s_pCamera.SetPosition({-34.5f, 0, 2});

    s_pCamera.zFar = 1000.0f;
    s_pCamera.zNear = 0.001f;
    s_pCamera.aspectRatio = 1;
    s_pCamera.fov = 80.0f;

    s_oCamera.SetRenderRange(0, 1, 0, 1, -1, 1);

    *s_floor.mesh = MeshFactory::Get().CreateMesh("plane");
    s_floor.transform->deltaPivot.x = 0.5f;
    s_floor.transform->deltaPivot.z = 0.5f;

    s_floor.transform->scale = {mapW, 1, mapH};
    s_floor.transform->position = {-mapW / 2.f, -0.5f, mapH / 2.f};
    s_floor.material->image = &s_wallsAtlas;

    s_potolok = s_floor;
    s_potolok.transform->position.y = 0.5f;

    uint32_t textureNum = 2;
    s_floor.material->uvCoordinates = {
            {64.0f * (float)  (textureNum % 6),      64.0f * (float)  (textureNum / 6)     },
            {64.0f * (float)  (textureNum % 6),      64.0f * (float) ((textureNum / 6) + 1)},
            {64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float) ((textureNum / 6) + 1)},
            {64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float)  (textureNum / 6)     },
    };

    textureNum = 3;
    s_potolok.material->uvCoordinates = {
            {64.0f * (float)  (textureNum % 6),      64.0f * (float)  (textureNum / 6)     },
            {64.0f * (float)  (textureNum % 6),      64.0f * (float) ((textureNum / 6) + 1)},
            {64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float) ((textureNum / 6) + 1)},
            {64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float)  (textureNum / 6)     },
    };

    s_3DEntities.push_back(&s_floor);
    s_3DEntities.push_back(&s_potolok);
    s_3DEntities.push_back(&s_location);
}

bool collides(int x, int z) {
    x = (x < 0) ? -x : x;

    x = (0 <= x && x < mapW) ? x : -1;
    z = (0 <= z && z < mapH) ? z : -1;

#if COLLISIONS == 0
    return true;
#endif    
    return s_collisionsField[z * mapW + x] == 0 && x >= 0 && z >= 0;
}

void Update() {
    glm::vec3 cam_rot(s_pCamera.GetRotation());
    glm::vec3 cam_pos(s_pCamera.GetPosition());

    float spd = 4.f * delta_fps;
    float rot_spd = 110.0f * delta_fps;

    if (s_window->KeyIsPressed(GLFW_KEY_RIGHT))
        cam_rot.y += rot_spd;
    else if (s_window->KeyIsPressed(GLFW_KEY_LEFT))
        cam_rot.y -= rot_spd;

    float colSpd = spd / 7.5f / delta_fps;
    float dx = 0.0f;
    float dz = 0.0f;

    if (s_window->KeyIsPressed(GLFW_KEY_W)) {
        dx =  sinf(glm::radians(cam_rot.y));
        dz = -cosf(glm::radians(cam_rot.y));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_S)) {
        dx = -sinf(glm::radians(cam_rot.y));
        dz =  cosf(glm::radians(cam_rot.y));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_A)) {
        dx =  sinf(glm::radians(cam_rot.y - 90.0f));
        dz = -cosf(glm::radians(cam_rot.y - 90.0f));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_D)) {
        dx =  sinf(glm::radians(cam_rot.y + 90.0f));
        dz = -cosf(glm::radians(cam_rot.y + 90.0f));
    }

    if (collides((int)(cam_pos.x + dx * colSpd), (int) cam_pos.z))
        cam_pos.x += dx * spd;

    if (collides((int)cam_pos.x, (int) (cam_pos.z + dz * colSpd)))
        cam_pos.z += dz * spd;

    s_pCamera.SetPosition(cam_pos);
    s_pCamera.SetRotation(cam_rot);
}

void DrawEntities() {
    s_sr->UseCamera(&s_pCamera);

    s_sr->StartBake(*s_FrameBaker);
    for (auto& e : s_3DEntities)
        if (e) s_sr->DrawEntity(*e);
    s_sr->EndBake();

    s_sr->UseCamera(&s_oCamera);
    s_sr->DrawEntity(s_screen);
    s_sr->DrawEntity(s_blueBackground);
}

auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
float getPassedTime(const Time& tp) {
    std::chrono::duration<float> dur = now() - tp;
    return dur.count();
}

void vladoom() {
    init();

    Time fps_control_start = now();
    Time fps_encounter_start = now();
    uint32_t fps_count = 120;
    delta_fps = 1.f / fps_count;

    while (s_window->IsOpen()) {
        if (getPassedTime(fps_control_start) < (1.f / FPS) && FPS > 0) continue;
        else fps_control_start = now();

        if (getPassedTime(fps_encounter_start) < 1.f)
            fps_count++;
        else {
            fps_encounter_start = now();
            delta_fps = 1.f / (float)fps_count;
            LOGINF(std::to_string(fps_count));
            fps_count = 0;
        }

        Update();
        s_sr->StartDraw();
        DrawEntities();
        s_sr->EndDraw();
    }
}