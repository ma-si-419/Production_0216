#include <DxLib.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Bg.h"

#include "Game.h"

namespace
{
	// マップチップの情報
	constexpr int kChipWidth = 32;
	constexpr int kChipHeight = 32;
	// チップを置く数
	constexpr int kChipNumX = Game::kScreenWidth / kChipWidth;
	constexpr int kChipNumY = Game::kScreenHeight / kChipHeight;

	// マップチップの配置情報
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
	// マップチップの数を数える
	int graphW;
	int graphH;
	GetGraphSize(m_handle, &graphW, &graphH);

	m_graphChipNumX = graphW / kChipWidth;
	m_graphChipNumY = graphH / kChipHeight;

#if false
	// デバック表示
	printfDx("ChipNumX : %d\n", m_graphChipNumX);
	printfDx("ChipNumY : %d\n", m_graphChipNumY);
#endif

	// マップチップ配置データの読み込み
	std::fstream file;

	file.open("data/map/map.bin", std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		printfDx("ファイルを開くのに失敗しました\n");
		assert(false);
	}
	else
	{
		// ファイルの中身をメモリ上にコピーする
		// static_cast<>()		これはアドレス以外の型
		// reinterpret_cast<>()	これはアドレス用の型
		// const_cast<>()		これはconstをしない場合にしたいやつ(使ったことないからよくわからん)
		// 正しくは違うだろうが今はこの解釈で頑張れ自分
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
			// マップ情報から置くチップを取ってくる
			int chipNo = m_chipData[y][x];

			// マップチップのグラフィック切り出し座標
			int srcX = kChipWidth * (chipNo % m_graphChipNumX);
			int srcY = kChipHeight * (chipNo / m_graphChipNumX);

			DrawRectGraph(kChipWidth * x, kChipHeight * y,
				srcX, srcY,
				kChipWidth, kChipHeight,
				m_handle, true);
		}
	}
}
