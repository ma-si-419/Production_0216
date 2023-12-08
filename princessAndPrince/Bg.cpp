#include <DxLib.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Bg.h"

#include "Game.h"

namespace
{
	// �}�b�v�`�b�v�̏��
	constexpr int kChipWidth = 32;
	constexpr int kChipHeight = 32;
	// �`�b�v��u����
	constexpr int kChipNumX = Game::kScreenWidth / kChipWidth;
	constexpr int kChipNumY = Game::kScreenHeight / kChipHeight;

	// �}�b�v�`�b�v�̔z�u���
	constexpr int kChipData[kChipNumY][kChipNumX] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,6,0,0,0,0,6,0,0,0,186,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,7,0,0,0,0,190,191,189,1,0,0,185,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,191,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,191,191,191,0,189,0,0,0,178,0,0,0,7,0,0},
		{0,0,0,0,0,190,0,0,1,1,1,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,187,179,0,0,0,0,0,0,191,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,191,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,191,0,191,0,0,0,0,0,0,0},
		{0,0,0,0,0,8,0,0,0,0,0,191,191,0,0,0,0,186,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
}

Bg::Bg() :
	m_handle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0)
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			m_chipData[y][x] = kChipData[y][x];
		}
	}
}

Bg::~Bg()
{
}

void Bg::Init()
{
	// �}�b�v�`�b�v�̐��𐔂���
	int graphW;
	int graphH;
	GetGraphSize(m_handle, &graphW, &graphH);

	m_graphChipNumX = graphW / kChipWidth;
	m_graphChipNumY = graphH / kChipHeight;

#if false
	// �f�o�b�N�\��
	printfDx("ChipNumX : %d\n", m_graphChipNumX);
	printfDx("ChipNumY : %d\n", m_graphChipNumY);
#endif

	// �}�b�v�`�b�v�z�u�f�[�^�̓ǂݍ���
	std::fstream file;

	file.open("data/map/map.bin", std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		printfDx("�t�@�C�����J���̂Ɏ��s���܂���\n");
		assert(false);
	}
	else
	{
		// �t�@�C���̒��g����������ɃR�s�[����
		// static_cast<>()		����̓A�h���X�ȊO�̌^
		// reinterpret_cast<>()	����̓A�h���X�p�̌^
		// const_cast<>()		�����const�����Ȃ��ꍇ�ɂ��������(�g�������ƂȂ�����悭�킩���)
		// �������͈Ⴄ���낤�����͂��̉��߂Ŋ撣�ꎩ��
		file.read(reinterpret_cast<char*>(&m_chipData), sizeof(m_chipData));

		file.close();
	}
}

void Bg::Update()
{
}

void Bg::Draw() const
{
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			// �}�b�v��񂩂�u���`�b�v������Ă���
			int chipNo = m_chipData[y][x];

			// �}�b�v�`�b�v�̃O���t�B�b�N�؂�o�����W
			int srcX = kChipWidth * (chipNo % m_graphChipNumX);
			int srcY = kChipHeight * (chipNo / m_graphChipNumX);

			DrawRectGraph(kChipWidth * x, kChipHeight * y,
				srcX, srcY,
				kChipWidth, kChipHeight,
				m_handle, true);
		}
	}
}
