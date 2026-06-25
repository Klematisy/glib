#include <cstdint>
#include <memory>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <array>

#include "GraphicsAPI/common.h"
#include "GraphicsAPI/graphics_api.h"
#include "Geometry/mesh.h"
#include "Geometry/transform.h"
#include "GraphicsAPI/window.h"
#include "nlohmann/json.hpp"
#include "coroutines.h"
#include "range.h"

#include "DrawUtils/draw.h"
#include "DrawUtils/frame_buffer.h"

#include "Geometry/entity.h"
#include "Geometry/camera.h"
#include "Geometry/utils.h"

#define COLLISIONS 1

VLADLIB_NAMESPACE_USING;
using namespace Geom;
using namespace nlohmann;

constexpr Rectanglei NUMS {301, 75, 8, 16};
constexpr Rectanglei GUN_TYPES {226, 109, 48, 24};
constexpr Rectanglei GUNS {0, 0, 64, 64};

namespace scripts {
    void wait(coroutine_pointer& coroutine, float seconds);

    void faceEvent(coroutine_pointer co);
    void gunEvent(coroutine_pointer co);
    void doorEvent(coroutine_pointer co);
}

struct Creature {
    glm::vec3 position {0};
    glm::vec3 rotation {0};
    i32 hitPoints = 100;
};

struct Gun {
    Coroutine gunAnim;
    u32 bullets = 0;
    u32 damage = 0;
};

struct PlayerFace {
    Entity playerFace;
    u32 phase = 0;
    Coroutine coroutine;

    static constexpr u32 W = 24;
    static constexpr u32 H = 31;
};

struct Hud {
    Entity hudBackground;
    Entity bulletCount;
    Entity scoreCount;
    Entity hpCount;
    Entity gunType;
    Entity livesCount;
    Entity gun;
    PlayerFace face;
};

struct Player : Creature {
    std::unique_ptr<Gun> inventory[4];
    uint8_t lives = 4;
    uint8_t pickedGun = 0;
    u32 score = 0;
};

struct Soldier : Creature {
    enum class STATE {ON_DUTY, SEEK, ATTACK};

};

struct Level {
    HeapArray<int> collisionsField {};
    int mapW = 0;
    int mapH = 0;

    Entity location;
};

struct Primitive {
    Mesh mesh;
    Transform transform;
};

static Entity initFullEntity() {
    return Entity(std::make_unique<Mesh>(),
                  std::make_unique<Transform>(),
                  std::make_unique<Material>());
}

void compareFrameBakerWithWindow(vladlib::FrameBaker& fm, const GAPI::Window& w) {
    if (fm.image.GetWidth() != w.GetWidth() || fm.image.GetHeight() != w.GetHeight()) {
        fm.image = GAPI::ImageInfo(w.GetWidth(), w.GetHeight(), fm.image.GetBPP(), nullptr);
    }
}

void addWallUvMap(std::vector<glm::vec2>& vector, int tileNum) {
    vector.emplace_back(64.0f * (f32) ((tileNum % 6) + 1), 64.0f * (f32) ((tileNum / 6) + 1));
    vector.emplace_back(64.0f * (f32) ((tileNum % 6) + 1), 64.0f * (f32)  (tileNum / 6));
    vector.emplace_back(64.0f * (f32)  (tileNum % 6),      64.0f * (f32)  (tileNum / 6));
    vector.emplace_back(64.0f * (f32)  (tileNum % 6),      64.0f * (f32) ((tileNum / 6) + 1));
}

template<int CHARS_NUM>
void setupTextUv(int number, std::vector<glm::vec2>& uvs) {
    std::string str = std::to_string(number);
    std::array<Rectanglei, CHARS_NUM> characters;

    constexpr Rectanglei NULL_RECT {NUMS.x, NUMS.y, 1, 1};
    for (auto& ch : characters) {
        ch = NULL_RECT;
    }

    int delta = (characters.size() - str.size());
    for (int i = (str.size() - 1); 0 <= i; i--) {
        int number = str[i] - '0';
        characters[i + delta] = NUMS;
        characters[i + delta].x = NUMS.x + number + number * NUMS.width;
    }

    uvs.clear();
    for (auto& uv : characters) {
        uvs.emplace_back(uv.x,            uv.y            );
        uvs.emplace_back(uv.x,            uv.y + uv.height);
        uvs.emplace_back(uv.x + uv.width, uv.y + uv.height);
        uvs.emplace_back(uv.x + uv.width, uv.y            );
    }
}

void updateHudData(Hud& hud, Player& player) {
    if (hud.face.coroutine.IsDead()) {
        hud.face.coroutine = scripts::faceEvent;
    }
    hud.face.coroutine.Resume();

    int pickedGun = player.pickedGun;
    Rectanglei gunTypes = GUN_TYPES;
    gunTypes.x = GUN_TYPES.x + (GUN_TYPES.width + 1) * (int)(pickedGun / 2);
    gunTypes.y = GUN_TYPES.y + (GUN_TYPES.height + 1) * (pickedGun % 2);

    hud.gunType.material->uvCoordinates = {
        {gunTypes.x,                  gunTypes.y                  },
        {gunTypes.x,                  gunTypes.y + gunTypes.height},
        {gunTypes.x + gunTypes.width, gunTypes.y + gunTypes.height},
        {gunTypes.x + gunTypes.width, gunTypes.y                  },
    };

    setupTextUv<1>(player.lives, hud.livesCount.material->uvCoordinates);
    setupTextUv<6>(player.score, hud.scoreCount.material->uvCoordinates);
    setupTextUv<2>(player.inventory[player.pickedGun]->bullets, hud.bulletCount.material->uvCoordinates);
    setupTextUv<3>(player.hitPoints, hud.hpCount.material->uvCoordinates);
}

static Player s_Player;
static Hud s_Hud;
static std::shared_ptr<GAPI::Window> s_window;
static GAPI::ImageInfo s_wallsAtlas, s_hudAtlas, s_gunAtlas;

static constexpr u32 FPS = 0;
static float spf = (FPS > 0) ? (1.f / FPS) : 0;
static std::unordered_map<std::string, int> s_textureTiles;
static std::unique_ptr<SceneRenderer> s_sr;
static PerspectiveCamera s_pCamera;
static OrthographicCamera s_oCamera;
static bool s_gameIsRunning = true;

static std::unique_ptr<FrameBaker> s_GamePlayBaker, s_ResultBaker;
static Entity s_blueBackground, s_backedGamePlay, s_result;

static Level s_PickedLevel;

class Door {
    using Func = std::function<void(coroutine_pointer)>;
public:
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
        m_DoorOpenAction = scripts::doorEvent;
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
                if (0 <= x && x < s_PickedLevel.mapW && 0 <= z && z <= s_PickedLevel.mapH)
                    s_PickedLevel.collisionsField[z * s_PickedLevel.mapW + x] = 0;
                return;
            }

            pos.x += dx;
            pos.z += dz;

            m_DeltaPosition.x += dx;
            m_DeltaPosition.z += dz;
        } else if (m_DoorState == CLOSING) {
            int x = -pos.x;
            int z =  pos.z;
            if (0 <= x && x < s_PickedLevel.mapW && 0 <= z && z <= s_PickedLevel.mapH)
                s_PickedLevel.collisionsField[z * s_PickedLevel.mapW + x] = 1;

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

namespace scripts {
    void wait(coroutine_pointer& coroutine, float seconds) {
        float fps = (spf > 0.f) ? (1.f / spf) : 60;
        for (int i = 0; i < fps * seconds; i++) {
            Coroutine::Yield(coroutine);
        }
    }

    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 3);
    void faceEvent(coroutine_pointer coroutine) {
        s_Hud.face.phase = dist6(rng);

        int phase = s_Hud.face.phase;

        const float delta = 100.f / 7;
        glm::uvec2 current_face {(u32)((100 - s_Player.hitPoints) / delta) % 4, 0};
        if (s_Player.hitPoints / delta < 3.f) {
            current_face.y = 1;
        }

        if (s_Player.hitPoints <= 0.f) {
            current_face.x = 3;
            current_face.y = 1;
            phase = 2;
        }

        float x0 = phase + current_face.x * 3 * (1 + PlayerFace::W) + PlayerFace::W * (phase - 1);
        float x1 = phase + current_face.x * 3 * (1 + PlayerFace::W) + PlayerFace::W * phase;
        float y0 = 43 + current_face.y * (PlayerFace::H + 2);
        float y1 = 73 + current_face.y * (PlayerFace::H + 2);

        s_Hud.face.playerFace.material->uvCoordinates = {
            {x0, y0},
            {x0, y1},
            {x1, y1},
            {x1, y0},
        };
        wait(coroutine, 1.f);
        return;
    }

    void gunEvent(coroutine_pointer co) {
        int pickedGun = s_Player.pickedGun;
        Rectanglei gun = GUNS;
        float waitTime = 0.5f;
        u32 repeatAnimNum = 2;
        u32 stopAnimNum = 4;

        switch (pickedGun) {
            case 0:
                repeatAnimNum = 2;
                stopAnimNum = 4;
                waitTime = 0.18f;
                break;
            case 1:
                repeatAnimNum = 1;
                stopAnimNum = 4;
                waitTime = 0.2f;
                break;
            case 2:
                repeatAnimNum = 1;
                stopAnimNum = 4;
                waitTime = 0.2f;
                break;
            case 3:
                repeatAnimNum = 2;
                stopAnimNum = 4;
                waitTime = 0.2f;
                break;
        }


        auto updateUvs = [&](u32 animNum) {
            gun.x = GUNS.x + animNum * GUNS.width;
            gun.y = GUNS.y + pickedGun * GUNS.height;

            s_Hud.gun.material->uvCoordinates = {
                {gun.x,             gun.y             },
                {gun.x,             gun.y + gun.height},
                {gun.x + gun.width, gun.y + gun.height},
                {gun.x + gun.width, gun.y             },
            };
        };

        for (u32 i = 0; i < 5; i++) {
            if (s_Player.inventory[s_Player.pickedGun]->bullets == 0 && s_Player.pickedGun != 0)
                break;

            bool isShooting = (co->user_data) ? *(bool*)(co->user_data) : false;
            if (i == repeatAnimNum && s_Player.pickedGun > 0) {
                s_Player.inventory[s_Player.pickedGun]->bullets -= 1;
            }

            if (i == 3 && s_Player.pickedGun == 3) {
                s_Player.inventory[s_Player.pickedGun]->bullets -= 1;
            }

            if (isShooting && i == stopAnimNum) {
                i = repeatAnimNum - 1;
                continue;
            }

            updateUvs(i);
            wait(co, waitTime);
        }


        updateUvs(0);
    }

    void doorEvent(coroutine_pointer co) {
#define self ((Door*)co->user_data)
        self->SetState(Door::OPENING);
        wait(co, 5.f);

        while (self->CanBeClosed()) {
            wait(co, 1.f);
        }

        self->SetState(Door::CLOSING);
#undef self
    }
}



static Mesh merge(const Mesh& m1, const Mesh& m2) {
    Mesh result;
    const auto& p1 = m1.points;
    const auto& p2 = m2.points;
    u32 polygon_inds[] = {0, 1, 2, 2, 3, 0};
    u32 maxIndex = 0;

    Polygon4 polygon1, polygon2;
    std::vector<Polygon4> polygons_to_remove;

    for (u32 i = 0; i < p1.size(); i+=4) {
        for (u32 j = 0; j < p2.size(); j+=4) {
            polygon1.Clear();
            polygon2.Clear();

            for (u32 k = 0; k < 4; k++) {
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
        for (u32 i = 0; i < points.size(); i+=4) {
            tempPolygon.Clear();
            for (u32 k = 0; k < 4; k++) {
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
                for (u32 k = 0; k < 4; k++)
                    result.points.push_back(points[i + k]);

                for (u32 polygon_ind : polygon_inds)
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

        s_PickedLevel.collisionsField[z * s_PickedLevel.mapW - x] = 1;

        if (block["type"] == "Wall") {
            newMesh = MeshFactory::Get().CreateMesh("quad_tube");
            newMesh = TransformConfirmer::ConfirmMesh(newMesh, transform);
        } else if (block["type"] == "Door") {
            newMesh = MeshFactory::Get().CreateMesh("door_frame");
            float rotation = (block["orientation"] == "AxisX") ? 90.0f : 0.0f;

            transform.position.x += 0.5f;
            transform.position.z += 0.5f;

            transform.deltaPivot = {0.5f, 0.0f, 0.5f};
            transform.rotation.y = rotation;

            s_Doors.emplace_back(x, z, rotation);

            newMesh = TransformConfirmer::ConfirmMesh(newMesh, transform);
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

        u32 polygonsCount = 0;
        if (block["type"] != "Door") {
            tileName = block["tileName"];
            for (u32 i = 0; i < mainMesh.points.size(); i += 4) {
                for (u32 j = 0; j < newMesh.points.size(); j += 4) {
                    bool polygonsAreEqual = true;
                    for (u32 k = 0; k < 4; k++) {
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
        for (u32 i = 0; i < polygonsCount; i++) {
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

        newMesh = TransformConfirmer::ConfirmMesh(newMesh, transform);

        mainMesh = merge(mainMesh, newMesh);
    }

    Primitive floor, celling;
    floor.mesh = MeshFactory::Get().CreateMesh("plane");
    floor.transform.deltaPivot.x = 0.5f;
    floor.transform.deltaPivot.z = 0.5f;

    floor.transform.scale = {s_PickedLevel.mapW, 1, s_PickedLevel.mapH};
    floor.transform.position = {-s_PickedLevel.mapW / 2.f, 0.f, s_PickedLevel.mapH / 2.f};

    celling = floor;
    celling.transform.position.y = 1.f;

    floor.mesh = vladlib::TransformConfirmer::ConfirmMesh(floor.mesh, floor.transform);
    celling.mesh = vladlib::TransformConfirmer::ConfirmMesh(celling.mesh, celling.transform);

    mainMesh = merge(mainMesh, floor.mesh);
    mainMesh = merge(mainMesh, celling.mesh);

    addWallUvMap(resultEntity.material->uvCoordinates, s_textureTiles["floor"]);
    addWallUvMap(resultEntity.material->uvCoordinates, s_textureTiles["celling"]);

    return resultEntity;
}

void init_meshes() {
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
}

template<int CHARS_NUM>
void setupText(Entity& textEntity) {
    std::array<Primitive, CHARS_NUM> nums;
    for (u32 i = 0; i < nums.size(); i++) {
        nums[i].transform.scale = {1.f / (float)nums.size(), 1.f, 1.f};
        nums[i].transform.position = {i / (float)nums.size(), 0.f, 0.f};
        nums[i].mesh = TransformConfirmer::ConfirmMesh(MeshFactory::Get().CreateMesh("quad"), nums[i].transform);
        *textEntity.mesh = merge(*textEntity.mesh, nums[i].mesh);
    }
}

void init() {
    init_meshes();

    s_window = GAPI::createWindow();
    GAPI::initGraphicsContext(4, 1);
    s_window->CreateWindow(640, 480, "vladlib");
    GAPI::initGraphicsBackend();

    s_sr = std::make_unique<SceneRenderer>(s_window.get());

    s_wallsAtlas = GAPI::ImageInfo("resources/images/atlas.png");
    s_hudAtlas = GAPI::ImageInfo("resources/images/hud.png");
    s_gunAtlas = GAPI::ImageInfo("resources/images/guns.png");

    GAPI::enableBlending();
    GAPI::blendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);
    GAPI::enableDepthTest();

    auto& face = s_Hud.face.playerFace;
    auto& nums = s_Hud.hpCount;
    s_result = initFullEntity();
    nums = initFullEntity();
    face = initFullEntity();
    s_blueBackground = initFullEntity();
    s_backedGamePlay = initFullEntity();
    s_Hud.hudBackground = initFullEntity();
    s_Hud.livesCount = initFullEntity();
    s_Hud.bulletCount = initFullEntity();
    s_Hud.scoreCount = initFullEntity();
    s_Hud.gunType = initFullEntity();
    s_Hud.gun = initFullEntity();

    s_textureTiles["floor"]     = 110;
    s_textureTiles["celling"]   = 111;
    s_textureTiles["blueRock"]  = 14;
    s_textureTiles["Hitler"]    = 97;
    s_textureTiles["doorFrame"] = 100;
    s_textureTiles["door"]      = 98;

    json firstLevel = json::parse(std::ifstream("src/Test/LEVEL_ONE.JSON"));
    s_PickedLevel.mapW = firstLevel["info"]["width"];
    s_PickedLevel.mapH = firstLevel["info"]["height"];
    s_PickedLevel.collisionsField.Resize(s_PickedLevel.mapW * s_PickedLevel.mapH);

    s_PickedLevel.location = getLevelLocationFromJson(firstLevel);
    s_PickedLevel.location.material->image = &s_wallsAtlas;

    s_GamePlayBaker = std::make_unique<FrameBaker>();
    s_ResultBaker = std::make_unique<FrameBaker>();

    s_sr->RegisterFrameBaker(*s_GamePlayBaker);
    s_sr->RegisterFrameBaker(*s_ResultBaker);

    auto meshFactory = MeshFactory::Get();
    *s_backedGamePlay.mesh = meshFactory.CreateMesh("quad");
    s_backedGamePlay.material->image = &s_GamePlayBaker->image;
    s_backedGamePlay.transform->scale = {0.95f, 0.78f, 1.0f};
    s_backedGamePlay.transform->position =
        {(1.f - s_backedGamePlay.transform->scale.x) / 2,
        0.03f,
        0.1f};
    s_backedGamePlay.material->uvCoordinates = {
        { 0,  0},
        { 0, -1},
        {-1, -1},
        {-1,  0},
    };

    *s_result.mesh = meshFactory.CreateMesh("quad");
    s_result.material->image = &s_ResultBaker->image;
    s_result.material->uvCoordinates = s_backedGamePlay.material->uvCoordinates;

    *s_blueBackground.mesh = meshFactory.CreateMesh("quad");
    s_blueBackground.transform->position.z = 0.0f;
    s_blueBackground.material->colors = {
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
        {0.f, 64.f / 255, 64.f / 255, 1.f},
    };

    auto& hudBack = s_Hud.hudBackground;
    *hudBack.mesh = meshFactory.CreateMesh("quad");
    constexpr float HUD_SCALE = 3.f;
    auto& hScale = hudBack.transform->scale;
    auto& hPos = hudBack.transform->position;
    hScale = {0.335f * HUD_SCALE, 0.055f * HUD_SCALE, 0.0f};
    hPos = {0.0f, 1.f - hudBack.transform->scale.y - 0.01f, 0.2f};
    hudBack.material->image = &s_hudAtlas;
    hudBack.material->uvCoordinates = {
        {1,    1},
        {1,   40},
        {320, 40},
        {320,  1},
    };

    *face.mesh = meshFactory.CreateMesh("quad");
    face.transform->scale = {
        hScale.x * 24.f / 320.f,
        hScale.y * 31.5f / 40.f,
        0.0f
    };
    face.transform->position = {
        hScale.x * (137.f / 320.f),
        hPos.y + hScale.y * (5.f / 40.f),
        0.3f
    };
    face.material->image = &s_hudAtlas;
    face.material->uvCoordinates = {
        {1,  43},
        {1,  74},
        {25, 74},
        {25, 43},
    };


    setupText<3>(nums);
    nums.transform->scale = {hScale.x * 24.f/320, hScale.y * 16.f/40, 1.f};
    nums.transform->position = {169.f/320 * hScale.x, hPos.y + 17.f/40.f * hScale.y, 0.3f};
    nums.material->image = &s_hudAtlas;

    *s_Hud.livesCount.mesh = meshFactory.CreateMesh("quad");
    s_Hud.livesCount.material->image = &s_hudAtlas;
    s_Hud.livesCount.transform->scale = {hScale.x * 8.f/320, hScale.y * 16.f/40, 1.f};
    s_Hud.livesCount.transform->position = {113.f/320 * hScale.x, hPos.y + 17.f/40.f * hScale.y, 0.3f};

    setupText<2>(s_Hud.bulletCount);
    s_Hud.bulletCount.material->image = &s_hudAtlas;
    s_Hud.bulletCount.transform->scale = {hScale.x * 16/320, hScale.y * 16.f/40, 1.f};
    s_Hud.bulletCount.transform->position = {216.f/320 * hScale.x, hPos.y + 17.f/40.f * hScale.y, 0.3f};

    setupText<6>(s_Hud.scoreCount);
    s_Hud.scoreCount.material->image = &s_hudAtlas;
    s_Hud.scoreCount.transform->scale = {hScale.x * 48.f/320, hScale.y * 16.f/40, 1.f};
    s_Hud.scoreCount.transform->position = {49.f/320 * hScale.x, hPos.y + 17.f/40.f * hScale.y, 0.3f};

    *s_Hud.gun.mesh = meshFactory.CreateMesh("quad");
    s_Hud.gun.transform->scale = {0.7f, 0.68f, 1.0f};
    s_Hud.gun.transform->position =
        {(1.f - s_Hud.gun.transform->scale.x) / 2,
        0.13f,
        0.2f};

    s_Hud.gun.material->image = &s_gunAtlas;
    s_Hud.gun.material->uvCoordinates = {
        {GUNS.x,              GUNS.y              },
        {GUNS.x,              GUNS.y + GUNS.height},
        {GUNS.x + GUNS.width, GUNS.y + GUNS.height},
        {GUNS.x + GUNS.width, GUNS.y              },
    };

    *s_Hud.gunType.mesh = meshFactory.CreateMesh("quad");
    s_Hud.gunType.transform->scale = {hScale.x * GUN_TYPES.width / 320.f, hScale.y * GUN_TYPES.height / 40.f, 1.0f};
    s_Hud.gunType.transform->position = {hScale.x * 256/320.f, hPos.y + hScale.y * 9 / 40.f, 0.3f};
    s_Hud.gunType.material->image = &s_hudAtlas;

    s_Player.position = {-34.5f, 0, 2};
    s_Player.rotation = {0, 180, 0};
    s_Player.inventory[0] = std::make_unique<Gun>();
    s_Player.inventory[1] = std::make_unique<Gun>((Gun){.bullets = 99});
    s_Player.inventory[3] = std::make_unique<Gun>((Gun){.bullets = 99});

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
}

bool collides(int x, int z) {
#if COLLISIONS == 0
    return true;
#endif
    x = (x < 0) ? -x : x;
    if (0 <= x && x < s_PickedLevel.mapW && 0 <= z && z < s_PickedLevel.mapH)
    {
        return s_PickedLevel.collisionsField[z * s_PickedLevel.mapW + x] == 0 && x >= 0 && z >= 0;
    }

    return true;
}

void Update() {
    compareFrameBakerWithWindow(*s_GamePlayBaker, *s_window);
    compareFrameBakerWithWindow(*s_ResultBaker, *s_window);

    updateHudData(s_Hud, s_Player);

    glm::vec3& player_r = s_Player.rotation;
    glm::vec3& player_p = s_Player.position;

    float spd = 4.f * spf;
    float colSpd = spd / 7.5f / spf;
    float rot_spd = 110.0f * spf;

    if (s_window->KeyIsPressed(GAPI::KEY::X)) {
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

    bool shoot = false;
    if (s_window->KeyIsPressed(GAPI::KEY::Z)) {
        shoot = true;
        if (s_Player.inventory[s_Player.pickedGun]->gunAnim.IsDead()) {
            s_Player.inventory[s_Player.pickedGun]->gunAnim = scripts::gunEvent;
        }
    }
    s_Player.inventory[s_Player.pickedGun]->gunAnim.Resume(&shoot);

    float dx = 0.0f;
    float dz = 0.0f;

    if (s_window->KeyIsTapped(GAPI::KEY::SPACE) && s_Player.inventory[s_Player.pickedGun]->gunAnim.IsDead()) {
        do {
            s_Player.pickedGun++;
            s_Player.pickedGun %= 4;
        } while (!s_Player.inventory[s_Player.pickedGun]);
        Rectanglei uv = GUNS;
        uv.y = GUNS.y + s_Player.pickedGun * GUNS.height;
        s_Hud.gun.material->uvCoordinates = {
            {uv.x,            uv.y            },
            {uv.x,            uv.y + uv.height},
            {uv.x + uv.width, uv.y + uv.height},
            {uv.x + uv.width, uv.y            },
        };
    }

    if (s_window->KeyIsPressed(GAPI::KEY::UP)) {
        dx =  sinf(glm::radians(player_r.y));
        dz = -cosf(glm::radians(player_r.y));
    }
    if (s_window->KeyIsPressed(GAPI::KEY::DOWN)) {
        dx = -sinf(glm::radians(player_r.y));
        dz =  cosf(glm::radians(player_r.y));
    }

    if (s_window->KeyIsPressed(GAPI::KEY::RIGHT))
        player_r.y += rot_spd;
    else if (s_window->KeyIsPressed(GAPI::KEY::LEFT))
        player_r.y -= rot_spd;

    if (s_window->KeyIsPressed(GAPI::KEY::ESCAPE)) {
        s_gameIsRunning = false;
    }

    if (collides((int)(player_p.x + dx * colSpd), (int) player_p.z))
        player_p.x += dx * spd;

    if (collides((int)player_p.x, (int) (player_p.z + dz * colSpd)))
        player_p.z += dz * spd;

    s_pCamera.transform.position = player_p;
    s_pCamera.transform.rotation = player_r;
}

void DrawEntities() {
    s_window->SetViewport({0, 0, s_window->GetWidth(), s_window->GetHeight()});

    s_sr->UseCamera(&s_pCamera);

    s_sr->StartBake(*s_GamePlayBaker);
    s_sr->DrawEntity(s_PickedLevel.location);
    for (auto& e : s_Doors)
        s_sr->DrawEntity(e.GetEntity());
    s_sr->EndBake();

    s_sr->UseCamera(&s_oCamera);

    s_sr->StartBake(*s_ResultBaker);
    s_sr->DrawEntity(s_backedGamePlay);
    s_sr->DrawEntity(s_blueBackground);
    s_sr->DrawEntity(s_Hud.face.playerFace);
    s_sr->DrawEntity(s_Hud.hudBackground);
    s_sr->DrawEntity(s_Hud.hpCount);
    s_sr->DrawEntity(s_Hud.livesCount);
    s_sr->DrawEntity(s_Hud.scoreCount);
    s_sr->DrawEntity(s_Hud.gun);
    s_sr->DrawEntity(s_Hud.gunType);
    s_sr->DrawEntity(s_Hud.bulletCount);
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

    s_window->SetViewport({(int)((windowW - resWindowW) / 2), (int)((windowH - resWindowH) / 2), resWindowW, resWindowH});
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
    u32 fps_count = 0;

    s_window->VSync(!FPS);

    while (s_window->IsOpen() && s_gameIsRunning) {
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
            fps_count = 0;
        }

        Update();
        s_sr->StartDraw();
        DrawEntities();
        s_sr->EndDraw();
    }
}
