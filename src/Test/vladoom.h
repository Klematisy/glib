#include <memory>
#include <fstream>
#include <chrono>
#include <thread>

#include "nlohmann/json.hpp"
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
static rc::ImageInfo s_hudAtlas;
static std::vector<Coroutine*> s_coro;

static constexpr uint32_t FPS = 0;
static float spf = (FPS > 0) ? 1.f / FPS : 0;
static std::vector<Entity*> s_3DEntities;
static std::unordered_map<std::string, int> s_textureTiles;
static std::vector<int> s_collisionsField;
static std::unique_ptr<SceneRenderer> s_sr;
static PerspectiveCamera s_pCamera;
static OrthographicCamera s_oCamera;

static std::unique_ptr<FrameBaker> s_GamePlayBaker, s_ResultBaker;
static Entity s_blueBackground, s_backedGamePlay, s_result;
static Entity s_floor, s_celling, s_location;

static int mapW = 0;
static int mapH = 0;


static Entity initFullEntity() {
    return Entity(std::make_unique<Mesh>(),
                  std::make_unique<Transform>(),
                  std::make_unique<Material>());
}

void CompareFrameBakerWithWindow(vladlib::FrameBaker& fm, const RendererCore::Window& w) {
    fm.image = RendererCore::ImageInfo(w.GetWidth(), w.GetHeight(), fm.image.GetBPP(), nullptr);
}

void addWallUvMap(std::vector<glm::vec2>& vector, int tileNum) {
    vector.emplace_back(64.0f * (float) ((tileNum % 6) + 1), 64.0f * (float) ((tileNum / 6) + 1));
    vector.emplace_back(64.0f * (float) ((tileNum % 6) + 1), 64.0f * (float)  (tileNum / 6));
    vector.emplace_back(64.0f * (float)  (tileNum % 6),      64.0f * (float)  (tileNum / 6));
    vector.emplace_back(64.0f * (float)  (tileNum % 6),      64.0f * (float) ((tileNum / 6) + 1));
}

struct GameEntity {
    glm::vec3 position {0};
    glm::vec3 rotation {0};
    float hitPoints = 100.f;
};

struct Gun {
    Entity gunEntity;
    uint32_t bullets = 0;
};

struct PlayerFace {
    Entity playerFace;
    uint32_t phase = 0;
};

struct Hud {
    Entity hudBackground;
    Entity hpCount;
    Entity bulletCount;
    Entity gunType;
    PlayerFace face;
};

struct Player : GameEntity {
    std::vector<Gun> inventory;
    uint8_t pickedGun = 0;
};


static Player s_Player;
static Hud s_Hud;

void delay(coroutine_pointer& coroutine, uint32_t seconds) {
    float fps = 1.f / spf;
    for (int i = 0; i < fps * seconds; i++) {
        Coroutine::Yield(coroutine);
    }
}

class Door {
    using Func = std::function<void(coroutine_pointer)>;
public:
#define self ((Door*)co->user_data)

    static void DoorEvent(coroutine_pointer co) {
        self->SetState(Door::OPENING);
        delay(co, 5);

        while (self->CanBeClosed()) {
            delay(co, 1);
        }

        self->SetState(Door::CLOSING);
    }

#undef self

    enum STATE {OPENING, CLOSING, STAYING};

    Door() = default;

    Door(int x, int z, float orientation) {
        m_DoorEntity = initFullEntity();

        *m_DoorEntity.mesh = MeshFactory::Get().CreateMesh("quad_tube");

        m_DoorEntity.transform->position = {x - 0.5f, 0, z + 0.5f};
        m_DoorEntity.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
        m_DoorEntity.transform->scale = {0.08f, 1, 1};
        m_DoorEntity.transform->rotation.y = orientation;

        auto& uv = m_DoorEntity.material->uvCoordinates;
        addWallUvMap(uv, s_textureTiles["door"]);
        std::reverse(uv.begin(), uv.end());

        m_DoorEntity.material->image = &s_wallsAtlas;
    }

    void SetState(STATE state) {
        m_DoorState = state;
    }

    void Open() {
        m_DoorOpenAction = DoorEvent;
    }

    bool CanBeClosed() {
        auto camPos = s_pCamera.transform.position;
        auto doorPos = m_DoorEntity.transform->position - m_DeltaPosition;

        int x = camPos.x, z = camPos.z;
        int x1 = doorPos.x, z1 = doorPos.z;

        return (x == x1 && z == z1);
    }

    void Update() {
        if (!m_DoorOpenAction.IsDead()) {
            m_DoorOpenAction.Resume(this);
        }

        const float spd = 1.5f * spf;

        float rotation = m_DoorEntity.transform->rotation.y;
        float dx = sin(glm::radians(rotation)) * spd;
        float dz = cos(glm::radians(rotation)) * spd;

        auto& pos = m_DoorEntity.transform->position;
        if (m_DoorState == OPENING) {
            if (glm::length(m_DeltaPosition) > 1.01f) {
                m_DoorState = STAYING;
                glm::vec3 oldPos = pos - m_DeltaPosition;
                int x = -oldPos.x;
                int z =  oldPos.z;
                if (0 <= x && x < mapW && 0 <= z && z <= mapH)
                    s_collisionsField[z * mapW + x] = 0;
                return;
            }

            pos.x += dx;
            pos.z += dz;

            m_DeltaPosition.x += dx;
            m_DeltaPosition.z += dz;
        } else if (m_DoorState == CLOSING) {
            int x = -pos.x;
            int z =  pos.z;
            if (0 <= x && x < mapW && 0 <= z && z <= mapH)
                s_collisionsField[z * mapW + x] = 1;

            if (glm::length(m_DeltaPosition) <= 0.01f) {
                m_DoorState = STAYING;
                return;
            }

            pos.x -= dx;
            pos.z -= dz;

            m_DeltaPosition.x -= dx;
            m_DeltaPosition.z -= dz;
        }
    }

    const Entity& GetEntity() const {
        return m_DoorEntity;
    }
private:
    glm::vec3 m_DeltaPosition {0.f};
    STATE m_DoorState = STAYING;
    Entity m_DoorEntity;
    Coroutine m_DoorOpenAction;
};

static std::vector<Door> s_Doors;



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

        s_collisionsField[z * mapW - x] = 1;

        if (block["type"] == "Wall") {
            newMesh = MeshFactory::Get().CreateMesh("quad_tube");
            newMesh.points = TransformConfirmer::Confirm(newMesh, transform);
        } else if (block["type"] == "Door") {
            newMesh = MeshFactory::Get().CreateMesh("door_frame");
            float rotation = (block["orientation"] == "AxisX") ? 90.0f : 0.0f;

            transform.position.x += 0.5f;
            transform.position.z += 0.5f;

            transform.deltaPivot = {0.5f, 0.0f, 0.5f};
            transform.rotation.y = rotation;

            s_Doors.emplace_back(x, z, rotation);

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
            addWallUvMap(uvCords, textureNum);
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

    s_window = std::make_unique<rc::Window>(640, 480, "vladlib");
    s_sr = std::make_unique<SceneRenderer>(s_window.get());
    s_wallsAtlas = rc::ImageInfo("resources/images/atlas.png");

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);
    gapi.EnableDepthTest();

    s_blueBackground = initFullEntity();
    s_backedGamePlay = initFullEntity();
    s_floor = initFullEntity();
    s_blueBackground = initFullEntity();
    s_Hud.hudBackground = initFullEntity();

    s_textureTiles["floor"]     = 111;
    s_textureTiles["celling"]   = 110;
    s_textureTiles["blueRock"]  = 14;
    s_textureTiles["Hitler"]    = 97;
    s_textureTiles["doorFrame"] = 100;
    s_textureTiles["door"]      = 98;

    json firstLevel = json::parse(std::ifstream("src/Test/LEVEL_ONE.JSON"));
    mapW = firstLevel["info"]["width"];
    mapH = firstLevel["info"]["height"];

    s_collisionsField.resize(mapW * mapH);

    s_location = getLevelLocationFromJson(firstLevel);
    s_location.material->image = &s_wallsAtlas;

    s_GamePlayBaker = std::make_unique<FrameBaker>();
    s_ResultBaker = std::make_unique<FrameBaker>();

    s_sr->RegisterFrameBaker(*s_GamePlayBaker);
    s_sr->RegisterFrameBaker(*s_ResultBaker);

    *s_backedGamePlay.mesh = MeshFactory::Get().CreateMesh("quad");
    s_backedGamePlay.material->image = &s_GamePlayBaker->image;
    s_backedGamePlay.transform->scale = {0.95f, 0.78f, 1.0f};
    s_backedGamePlay.transform->position =
        {(1.f - s_backedGamePlay.transform->scale.x) / 2,
        0.03f,
        0.2f};
    s_backedGamePlay.material->uvCoordinates = {
        { 0,  0},
        { 0, -1},
        {-1, -1},
        {-1,  0},
    };

    s_result = initFullEntity();
    *s_result.mesh = MeshFactory::Get().CreateMesh("quad");
    s_result.material->image = &s_ResultBaker->image;
    s_result.material->uvCoordinates = s_backedGamePlay.material->uvCoordinates;


    *s_blueBackground.mesh = MeshFactory::Get().CreateMesh("quad");
    s_blueBackground.transform->position.z = 0.0f;
    s_blueBackground.material->colors = {
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
    };

    auto& hudBack = s_Hud.hudBackground;
    *hudBack.mesh = MeshFactory::Get().CreateMesh("quad");
    s_hudAtlas = rc::ImageInfo("resources/images/hud.png");
    constexpr float HUD_SCALE = 3.f;
    hudBack.transform->scale = {0.335f * HUD_SCALE, 0.055f * HUD_SCALE, 0.0f};
    hudBack.transform->position = {0.0f, 1.f - hudBack.transform->scale.y - 0.01f, 0.1f};
    hudBack.material->image = &s_hudAtlas;
    hudBack.material->uvCoordinates = {
        {1,    1},
        {1,   40},
        {320, 40},
        {320,  1},
    };


    s_Player.position = {-34.5f, 0, 2};
    s_Player.rotation = {0, 180, 0};

    s_pCamera.zFar = 1000.0f;
    s_pCamera.zNear = 0.001f;
    s_pCamera.aspectRatio = 4.f / 3;
    s_pCamera.fov = 80.0f;

    s_oCamera.zFar = 1;
    s_oCamera.zNear = -1;
    s_oCamera.left = 0;
    s_oCamera.right = 1;
    s_oCamera.top = 0;
    s_oCamera.bottom = 1;

    *s_floor.mesh = MeshFactory::Get().CreateMesh("plane");
    s_floor.transform->deltaPivot.x = 0.5f;
    s_floor.transform->deltaPivot.z = 0.5f;

    s_floor.transform->scale = {mapW, 1, mapH};
    s_floor.transform->position = {-mapW / 2.f, -0.5f, mapH / 2.f};
    s_floor.material->image = &s_wallsAtlas;

    s_celling = s_floor;
    s_celling.transform->position.y = 0.5f;

    addWallUvMap(s_celling.material->uvCoordinates, s_textureTiles["floor"]);
    addWallUvMap(s_floor.material->uvCoordinates, s_textureTiles["celling"]);

    s_3DEntities.push_back(&s_floor);
    s_3DEntities.push_back(&s_celling);
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
    CompareFrameBakerWithWindow(*s_GamePlayBaker, *s_window);
    CompareFrameBakerWithWindow(*s_ResultBaker, *s_window);

    glm::vec3& player_r = s_Player.rotation;
    glm::vec3& player_p = s_Player.position;

    float spd = 4.f * spf;
    float colSpd = spd / 7.5f / spf;
    float rot_spd = 110.0f * spf;

    if (s_window->KeyIsPressed(GLFW_KEY_E)) {
        float dx =  sinf(glm::radians(player_r.y)) * (colSpd * 1.8f);
        float dz = -cosf(glm::radians(player_r.y)) * (colSpd * 1.8f);

        float x = player_p.x + dx;
        float z = player_p.z + dz;

        for (auto& door : s_Doors) {
            const auto& pos = door.GetEntity().transform->position;
            if (fabsf(x - pos.x) < 0.4f && fabsf(z - pos.z) < 0.4f) {
                door.Open();
            }
        }
    }

    for (auto& door : s_Doors) {
        door.Update();
    }

    if (s_window->KeyIsPressed(GLFW_KEY_RIGHT))
        player_r.y += rot_spd;
    else if (s_window->KeyIsPressed(GLFW_KEY_LEFT))
        player_r.y -= rot_spd;

    float dx = 0.0f;
    float dz = 0.0f;

    if (s_window->KeyIsPressed(GLFW_KEY_W)) {
        dx =  sinf(glm::radians(player_r.y));
        dz = -cosf(glm::radians(player_r.y));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_S)) {
        dx = -sinf(glm::radians(player_r.y));
        dz =  cosf(glm::radians(player_r.y));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_A)) {
        dx =  sinf(glm::radians(player_r.y - 90.0f));
        dz = -cosf(glm::radians(player_r.y - 90.0f));
    }
    if (s_window->KeyIsPressed(GLFW_KEY_D)) {
        dx =  sinf(glm::radians(player_r.y + 90.0f));
        dz = -cosf(glm::radians(player_r.y + 90.0f));
    }

    if (collides((int)(player_p.x + dx * colSpd), (int) player_p.z))
        player_p.x += dx * spd;

    if (collides((int)player_p.x, (int) (player_p.z + dz * colSpd)))
        player_p.z += dz * spd;

    s_pCamera.transform.position = player_p;
    s_pCamera.transform.rotation = player_r;
}

void DrawEntities() {
    s_window->ChangeViewport({0, 0, s_window->GetWidth(), s_window->GetHeight()});

    s_sr->UseCamera(&s_pCamera);

    s_sr->StartBake(*s_GamePlayBaker);
    for (auto& e : s_3DEntities)
        if (e) s_sr->DrawEntity(*e);
    for (auto& e : s_Doors)
        s_sr->DrawEntity(e.GetEntity());
    s_sr->EndBake();

    s_sr->UseCamera(&s_oCamera);

    s_sr->StartBake(*s_ResultBaker);
    s_sr->DrawEntity(s_backedGamePlay);
    s_sr->DrawEntity(s_blueBackground);
    s_sr->DrawEntity(s_Hud.hudBackground);
    s_sr->EndBake();

    float windowW = s_window->GetWidth();
    float windowH = s_window->GetHeight();

    constexpr float resAspectRatioW = 4.f;
    constexpr float resAspectRatioH = 3.f;

    float windowAR = windowW / windowH;

    int resWindowW = (int)windowW;
    int resWindowH = (int)windowH;

    if ((resAspectRatioW / resAspectRatioH) < windowAR) {
        resWindowW -= windowW - resAspectRatioW * windowH / resAspectRatioH;
    } else {
        resWindowH -= windowH - resAspectRatioH * windowW / resAspectRatioW;
    }

    s_window->ChangeViewport({(int)((windowW - resWindowW) / 2), (int)((windowH - resWindowH) / 2), resWindowW, resWindowH});
    s_sr->DrawEntity(s_result);
}

auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<float>;
float getPassedTime(const Time& tp) {
    Duration dur = now() - tp;
    return dur.count();
}

void vladoom() {
    init();

    Time fps_control_start = now();
    Time fps_encounter_start = now();
    uint32_t fps_count = 0;

    // s_window->VSync(!FPS);

    while (s_window->IsOpen()) {
        if (FPS > 0) {
            float passedTime = getPassedTime(fps_control_start);
            Duration sleepTime = Duration(1.f / FPS - passedTime) - std::chrono::milliseconds(1);
            if (sleepTime.count() > 0.f) {
                std::this_thread::sleep_for(sleepTime);
            }

            passedTime = getPassedTime(fps_control_start);
            while (getPassedTime(fps_control_start) < 1.f / FPS)
            {
                std::this_thread::yield();
            }
            fps_control_start = now();
        }

        fps_count++;
        if (getPassedTime(fps_encounter_start) > 1.f) {
            fps_encounter_start = now();
            spf = 1.f / (float)fps_count;
            LOGINF(std::to_string(fps_count));
            // LOGINF(std::to_string(s_window->GetWidth()));
            fps_count = 0;
        }

        Update();
        s_sr->StartDraw();
        DrawEntities();
        s_sr->EndDraw();
    }
}
