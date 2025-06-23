#include "enemy_projectile.h"
#include "common.h"
#include "raylib.h"
#include "raymath.h"
#include "ship.h"
#include "enemy.h"
#include "general_utils.h"

List* enemy_projectiles = NULL;

static Texture texture;
static Rectangle source_rects[PROJECTILE_COUNT];

void EnemyProjectile_Init() {

    if (enemy_projectiles != NULL) {
        List_Destroy(enemy_projectiles);
    }

    enemy_projectiles = List_Create(sizeof(EnemyProjectile));
}

void EnemyProjectile_Load(void)
{
    texture = LoadTexture("weapons.png");
    if (texture.id <= 0) {
        TraceLog(LOG_WARNING, "Textura de inimigos (weapons.png) não encontrada.");
        return;
    }

    source_rects[PROJECTILE_PIDGEON_OF_PREY_1] = (Rectangle){ 8 * 1, 8 * 1, 8, 8 };
}

static void InitializeEnemyProjectileSpecifics(EnemyProjectile* enemy_projectile) {
    switch(enemy_projectile->type) {
        case PROJECTILE_PIDGEON_OF_PREY_1:
            float rotation = enemy_projectile->owner->rotation;
            Vector2 speed_vector = Vector2Rotate(enemy_projectile->speed, rotation * DEG2RAD);
            enemy_projectile->speed = speed_vector;
            enemy_projectile->size = (Vector2) { 8 * 10, 8 * 10 };
            enemy_projectile->collision_size = 8 * 2;
            break;
    }
}

void EnemyProjectile_Activate(EnemyProjectile* enemy_projectile, Enemy *enemy, ProjectileType type)
{
    // THE COLLISION_SIZE FOR PROJECTILES IS SEPARATED FROM SIZE FOR BALANCING PURPOSES
    enemy_projectile->position = enemy->position;
    enemy_projectile->size = (Vector2) { 8 * 10, 8 * 10 };
    enemy_projectile->collision_size = 8 * 2;
    enemy_projectile->speed = (Vector2) { 0, 200 };
    enemy_projectile->type = type;
    enemy_projectile->color = WHITE;
    enemy_projectile->rotation = 0.0f;
    enemy_projectile->is_targetable = true;
    enemy_projectile->is_collidable = true;
    enemy_projectile->owner = enemy;
    enemy_projectile->rotation = PI * RAD2DEG;

    InitializeEnemyProjectileSpecifics(enemy_projectile);
}

void EnemyProjectile_Spawn(Enemy* enemy, ProjectileType type)
{
    EnemyProjectile projectile;
    EnemyProjectile_Activate(&projectile, enemy, type);
    List_Add(enemy_projectiles, &projectile);
}

void EnemyProjectile_SpawnPosition(Enemy* enemy, ProjectileType type, Vector2 position)
{
    EnemyProjectile projectile;
    EnemyProjectile_Activate(&projectile, enemy, type);
    projectile.position = position;

    List_Add(enemy_projectiles, &projectile);
}

static void CheckCollision(Ship* ship, EnemyProjectile* enemy_projectile) {
    bool has_collided = CheckCollisionCircles(
        ship->position, ship->draw_size.x / 2.0f,
        enemy_projectile->position, enemy_projectile->collision_size);

    if (!has_collided)
        return;

    Ship_TakeDamage(ship);
}

static void Update_BasicMovement(EnemyProjectile* enemy_projectile) {
    Vector2 prev_pos = enemy_projectile->position;
    
    enemy_projectile->position = Vector2Add(
        enemy_projectile->position,
        Vector2MultiplyScalarF(
            enemy_projectile->speed,
            GetFrameTime()));

    enemy_projectile->rotation = RAD2DEG * CalculateFacingAngle(prev_pos, enemy_projectile->position) - 90;
}

static void Update(Ship* ship, EnemyProjectile* enemy_projectile) {
    switch(enemy_projectile->type) {
        case PROJECTILE_PIDGEON_OF_PREY_1:
            Update_BasicMovement(enemy_projectile);
            break;
    }

    CheckCollision(ship, enemy_projectile);
}

static bool CheckHomingShootOutOfBounds(void* context, EnemyProjectile* item) {
    return IsWithinScreenBounds(item->position, item->size);
}

void EnemyProjectile_Update(Ship* ship)
{
    List_ForEachCtx(enemy_projectiles, ship, Update);
    List_RemoveWithFn(enemy_projectiles, NULL, CheckHomingShootOutOfBounds);
}

#pragma region DRAW
static inline Rectangle GetEnemyRectangle(EnemyProjectile* enemy_projectile) {
    return (Rectangle) {
        enemy_projectile->position.x,
        enemy_projectile->position.y,
        enemy_projectile->size.x,
        enemy_projectile->size.y
    };
}

// THE COLLISION_SIZE FOR PROJECTILES IS SEPARATED FROM SIZE FOR BALANCING PURPOSES
static void Draw(EnemyProjectile* enemy_projectile) {
    if (DEBUG_FLAG) {
        DrawCircleV(enemy_projectile->position, enemy_projectile->collision_size, Fade(GREEN, 0.5f));
    }

    Vector2 origin = Vector2DivideScalarF(enemy_projectile->size, 2);
    Rectangle enemy_rect = GetEnemyRectangle(enemy_projectile);

    DrawTexturePro(texture, source_rects[enemy_projectile->type], enemy_rect, origin, enemy_projectile->rotation, enemy_projectile->color);
}

void EnemyProjectile_Draw(void)
{
    List_ForEach(enemy_projectiles, Draw);
}
#pragma endregion

#pragma region UNLOAD
void EnemyProjectile_Unload(void)
{
    UnloadTexture(texture);
}
#pragma endregion