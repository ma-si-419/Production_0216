#include "Character.h"

Character::Character() :
	m_handle(-1),
	m_knockBackTime(0),
	m_hp(1.0f),
	m_atk(0.0f),
	m_spd(0.0f),
	m_def(0.0f),
	m_animFrame(0),
	m_dir(Game::Dir::kDirDown),
	m_dirX(0),
	m_dirY(0),
	m_nowState(Game::State::kNormal),
	m_radius(0),
	m_knockBack()
{
}

Character::~Character()
{
}

void Character::Init()
{
}

void Character::Update()
{
}

void Character::Draw()
{
}
