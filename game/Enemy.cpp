#include "Enemy.h"

Enemy::Enemy() 
{
    mPos.set(0, 0);
    mCollider = mAttackCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;
}

void Enemy::init(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(0, 0);
    mCollider = mAttackCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
    }
}

Enemy::Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(0, 0);
    mCollider = mAttackCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
    }
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

void Enemy::respawn(Tile* tiles[], const SDL_Rect& camera)
{
    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;
    //Respawn random in wall tiles, but not in camera
    do {
        mCollider.x = rand() % (LEVEL_WIDTH - ENEMY_WIDTH);
        mCollider.y = rand() % (LEVEL_HEIGHT - ENEMY_HEIGHT);
    } while (checkCollision(mCollider, camera) || touchesWall(mCollider, tiles) < TILE_LEFTRIGHT);
    mPos.set(mCollider.x, mCollider.y);
    mAttackCollider = mCollider;
}

void Enemy::react(Player& player)
{
    if (isAlive)
    {
        //Take damage
        if (player.getIsAttack() && checkCollision(mCollider, player.getAttackCollider()))
        {
            gotHit = true;
            mHP--;
        }
        //Die
        if (!player.getIsAttack() && mHP <= 0)
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

void Enemy::move(const SDL_Rect& pCollider, Tile* tiles[])
{
    if (isAlive)
    {
        Point pPos(pCollider.x, pCollider.y);

        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        if (distance(pPos, mPos) <= ATTACK_RANGE)
        {
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

            updateVel(pPos);

            mPos.x += mVelX;
            mCollider.x = mAttackCollider.x = mPos.x;

            mPos.y += mVelY;
            mCollider.y = mAttackCollider.y = mPos.y;
        }
    }
    else isAttack = false;
}

void Enemy::attack(Tile* tiles[])
{
    
}

void Enemy::updateVel(const Point& pPos)
{
    int x = pPos.x - mPos.x;
    int y = pPos.y - mPos.y;

    mVelX = round((double)ENEMY_VEL * x / pytago(x, y));
    mVelY = round((double)ENEMY_VEL * y / pytago(x, y));
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

void Enemy::render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound)
{
    if (isAlive)
    {
        gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, bodyAngle, 0, bodyFlip);
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
    if (gotHit)
    {
        gRedSlashTexture.render(renderer, mPos.x - ENEMY_WIDTH / 2 - camera.x, mPos.y - ENEMY_HEIGHT / 2 - camera.y, NULL, rand(), 0, bodyFlip);
        Mix_PlayChannel(-1, gSwordSlashSound, 0);
        gotHit = false;
    }
}

void Enemy::render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, LTexture& gWeaponTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound)
{
    if (isAlive)
    {
        gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, bodyAngle, 0, bodyFlip);
        gWeaponTexture.render(renderer, mAttackCollider.x - camera.x, mAttackCollider.y - camera.y, NULL, weaponAngle, 0);
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
    if (gotHit)
    {
        gRedSlashTexture.render(renderer, mPos.x - ENEMY_WIDTH / 2 - camera.x, mPos.y - ENEMY_HEIGHT / 2 - camera.y, NULL, rand(), 0, bodyFlip);
        Mix_PlayChannel(-1, gSwordSlashSound, 0);
        gotHit = false;
    }
}

