#include <memory>
#include <fstream>
#include <nlohmann/json.hpp>

#include "DrawUtils/shader.h"
#include "DrawUtils/draw.h"
#include "DrawUtils/frame_buffer.h"

#include "Geometry/entity.h"
#include "Geometry/camera.h"

#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_USING;
using namespace Geom;
using namespace nlohmann;

namespace rc = RendererCore;

static std::unique_ptr<rc::Window> s_window;
static rc::ImageInfo s_wallsAtlas;

static std::vector<Entity*> s_3DEntities;
static std::unordered_map<std::string, int> s_textureTiles;
static std::vector<int> s_collisionsField;
static std::unique_ptr<Draw> s_draw;
static PerspectiveCamera s_pCamera;
static OrthographicCamera s_oCamera;

static Entity s_blueScreen;
static Entity s_floor, s_potolok;

static std::unique_ptr<FrameBaker> s_FrameBaker;
static rc::ImageInfo s_BakeImage;

static int mapW = 0;
static int mapH = 0;


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

    std::vector<std::array<glm::vec3, 4>> polygons_to_remove;

    uint32_t maxIndex = 0;

    for (uint32_t i = 0; i < p1.size(); i+=4) {
        for (uint32_t j = 0; j < p2.size(); j+=4) {
            bool polygonsAreEqual = true;
            for (uint32_t k = 0; k < 4; k++) {
                if (p1[i + k] != p2[j + k])
                    polygonsAreEqual = false;
            }

            if (polygonsAreEqual) {
                std::array<glm::vec3, 4> found_polygon{};
                for (uint32_t k = 0; k < 4; k++) {
                    found_polygon[k] = p1[i + k];
                }
                polygons_to_remove.push_back(found_polygon);
            }
        }
    }


    auto addMesh = [&](const std::vector<glm::vec3>& points) {
        for (uint32_t i = 0; i < points.size(); i+=4) {
            bool wasPolygonDelete = false;
            for (const auto& polygon : polygons_to_remove) {
                wasPolygonDelete = true;
                for (uint32_t k = 0; k < 4; k++) {
                    if (points[i + k] != polygon[k]) {
                        wasPolygonDelete = false;
                        break;
                    }
                }
                if (wasPolygonDelete) break;
            }

            if (!wasPolygonDelete) {
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


Entity getLocationFromJson(const json& location) {
    Entity resultEntity = initFullEntity();

    auto& uvCords = resultEntity.material->uvCoordinates;
    resultEntity.transform->deltaPivot.y = 0.5f;
    resultEntity.transform->deltaPivot.x = 1.0f;
    Mesh& mainMesh = *resultEntity.mesh;
    const auto& blocks = location["blocks"];

    for (const auto& block : blocks) {
        Mesh newMesh = MeshFactory::Get().CreateMesh("quad_tube");
        int x = block["x"];
        int y = block["y"];

        s_collisionsField[y * mapW - x] = 1;

        for (auto& point : newMesh.points) {
            point.x += (float) x;
            point.z += (float) y;
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
        for (uint32_t i = 0; i < mainMesh.points.size(); i+=4) {
            for (uint32_t j = 0; j < newMesh.points.size(); j+=4) {
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

        int textureNum = s_textureTiles[block["tileName"]];
        for (uint32_t i = 0; i < polygonsCount; i++) {
            uvCords.emplace_back(64.0f * (float)  (textureNum % 6),      64.0f * (float)  (textureNum / 6));
            uvCords.emplace_back(64.0f * (float)  (textureNum % 6),      64.0f * (float) ((textureNum / 6) + 1));
            uvCords.emplace_back(64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float) ((textureNum / 6) + 1));
            uvCords.emplace_back(64.0f * (float) ((textureNum % 6) + 1), 64.0f * (float)  (textureNum / 6));
        }
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
        }));


    s_textureTiles["floor"]    = 3;
    s_textureTiles["ceiling"]  = 4;
    s_textureTiles["blueRock"] = 98;
    s_textureTiles["Hitler"]   = 12;


    s_window = std::make_unique<rc::Window>(600, 600, "glib");
    s_draw = std::make_unique<Draw>(s_window.get());
    s_wallsAtlas = rc::ImageInfo("resources/images/atlas.png");

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

    s_blueScreen = initFullEntity();

    s_floor = initFullEntity();

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
}

bool collides(int x, int z) {
    x = (x < 0) ? -x : x;

    x = (0 <= x && x < mapW) ? x : -1;
    z = (0 <= z && z < mapH) ? z : -1;

    return s_collisionsField[z * mapW + x] == 0 && x >= 0 && z >= 0;
}

void Update() {
    glm::vec3 cam_rot(s_pCamera.GetRotation());
    glm::vec3 cam_pos(s_pCamera.GetPosition());

    float spd = 0.03f;
    float rot_spd = 1.5f;

    if (s_window->KeyIsPressed(GLFW_KEY_RIGHT)) {
        cam_rot.y += rot_spd;
    } else if (s_window->KeyIsPressed(GLFW_KEY_LEFT)) {
        cam_rot.y -= rot_spd;
    }

    if (s_window->KeyIsPressed(GLFW_KEY_UP)) {
        cam_rot.x -= rot_spd;
    } else if (s_window->KeyIsPressed(GLFW_KEY_DOWN)) {
        cam_rot.x += rot_spd;
    }


    float colSpd = spd * 6;
    float dx = 0.0f;
    float dz = 0.0f;

    if (s_window->KeyIsPressed(GLFW_KEY_W)) {
        dx =  sinf(glm::radians(cam_rot.y));
        dz = -cosf(glm::radians(cam_rot.y));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_S)) {
        dx =  cosf(glm::radians(cam_rot.y));
        dz = -sinf(glm::radians(cam_rot.y));
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
    s_draw->UseCamera(&s_pCamera);

    s_draw->StartBake(*s_FrameBaker);
    for (auto& e : s_3DEntities)
        if (e) s_draw->DrawEntity(*e);
    s_draw->EndBake();

    s_oCamera.SetRenderRange(0, 1, 0, 1, 0, 1);
    s_blueScreen.transform->scale = {0.8f, 0.6f, 1.0f};
    s_blueScreen.transform->position = {0.1f, 0.3f, 0.0f};

    s_draw->UseCamera(&s_oCamera);
    s_draw->DrawEntity(s_blueScreen);
}


int main() {
    json firstLevel = json::parse(std::ifstream("src/Test/LEVEL_ONE.JSON"));
    mapW = firstLevel["info"]["width"];
    mapH = firstLevel["info"]["height"];

    init();

    s_collisionsField.resize(mapW * mapH);

    Entity location = getLocationFromJson(firstLevel);
    location.material->image = &s_wallsAtlas;

    s_3DEntities.push_back(&location);

    s_draw->UseCamera(&s_pCamera);

    s_FrameBaker = std::make_unique<FrameBaker>();
    s_draw->TieImageAndFrameBuffer(s_BakeImage, *s_FrameBaker);

    *s_blueScreen.mesh = MeshFactory::Get().CreateMesh("quad");
    s_blueScreen.material->image = &s_BakeImage;
    s_blueScreen.material->uvCoordinates = {
            {0,       0},
            {0,    3000},
            {3000, 3000},
            {3000,    0},
    };

    while (s_window->IsOpen()) {
        Update();
        s_draw->StartDraw();
        DrawEntities();
        s_draw->EndDraw();
    }

    return 0;
}