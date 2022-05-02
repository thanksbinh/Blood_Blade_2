#include "Enemy.h"

Enemy::Enemy() 
{
    mPos.set(40, 40);
    mCollider = { 40, 40, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;
}

Enemy::Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mPos.set(40, 40);
    mCollider = { 40, 40, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
    }
    hasParticle = true;
}

Enemy::~Enemy()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Enemy::init(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
    }
    hasParticle = true;
}

void Enemy::respawn(Tile* tiles[], const SDL_Rect& camera)
{
    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;
    //Respawn random in level, but not in camera
    do {
        mCollider.x = rand() % (LEVEL_WIDTH - ENEMY_WIDTH);
        mCollider.y = rand() % (LEVEL_HEIGHT - ENEMY_HEIGHT);
    } while (checkCollision(mCollider, camera) || touchesWall(mCollider, tiles) >= TILE_LEFTRIGHT);
    mPos.set(mCollider.x, mCollider.y);
}

void Enemy::react(const SDL_Rect& playerCollider, const bool& playerIsMoving)
{
    SDL_Rect playerAttackCollider = { playerCollider.x - ATTACK_AREA, playerCollider.y - ATTACK_AREA,
                                      playerCollider.w + 2 * ATTACK_AREA, playerCollider.h + 2 * ATTACK_AREA };

    if (isAlive)
    {
        //Take damage
        if (playerIsMoving && checkCollision(mCollider, playerAttackCollider))
        {
            gotHit = true;
            mHP--;
        }
        //Die
        if (!playerIsMoving && mHP <= 0)
        {
            isAlive = false;
            for (int i = 0; i < TOTAL_PARTICLES; ++i)
            {
                particles[i]->reset(mCollider);
            }
        }
    }
    //Gone
    if (!isAlive && mTime.wait(TIME_BEFORE_DISAPPEAR))
    {
        isAppear = false;
        mTime.stop();
    }
}

void Enemy::move(const SDL_Rect& playerCollider, Tile* tiles[])
{
    if (isAlive)
    {
        Point playerPos(playerCollider.x, playerCollider.y);

        //Turn to player
        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        if (distance(playerPos, mPos) <= ATTACK_RANGE)
        {
            isAttack = true;
            if (mTime.wait(TIME_BEFORE_ATTACK))
            {
                attack(tiles);
            }
        }
        else
        {
            isAttack = false;
            mTime.stop();
            bodyAngle = 0;

            updateVel(playerPos);

            mPos.x += mVelX;
            mCollider.x = mPos.x;

            mPos.y += mVelY;
            mCollider.y = mPos.y;
        }
    }
    else 
    {
        isAttack = false;
    }
}

void Enemy::updateVel(const Point& playerPos)
{
    double x = playerPos.x - mPos.x;
    double y = playerPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}

void Enemy::attack(Tile* tiles[])
{
    mPos.x += mVelX * 2;
    mCollider.x = mPos.x;

    mPos.y += mVelY * 2;
    mCollider.y = mPos.y;

    if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > LEVEL_WIDTH) || touchesWall(mCollider, tiles) == TILE_LEFTRIGHT || touchesWall(mCollider, tiles) == TILE_CORNER)
    {
        mVelX = -mVelX;
    }

    if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > LEVEL_HEIGHT) || touchesWall(mCollider, tiles) == TILE_TOPBOTTOM || touchesWall(mCollider, tiles) == TILE_CORNER)
    {
        mVelY = -mVelY;
    } 

    bodyAngle += SPIN_SPEED;
}

void Enemy::renderParticles(LTexture& gRedTexture, const SDL_Rect& camera)
{
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        if (particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(mCollider, gRedTexture);
        }
    }

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i]->render(renderer, camera);
    }
}

void Enemy::render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlash, const SDL_Rect& camera, Mix_Chunk* gSwordSlash)
{
    if (isAlive)
    {
        gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, bodyAngle, 0, bodyFlip);
        if (gotHit)
        {
            gRedSlash.render(renderer, mPos.x - ENEMY_WIDTH / 2 - camera.x, mPos.y - ENEMY_HEIGHT / 2 - camera.y, NULL, rand(), 0, bodyFlip);
            Mix_PlayChannel(-1, gSwordSlash, 0);
            gotHit = false;
        }
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
}

