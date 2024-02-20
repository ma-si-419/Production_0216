#pragma once

// �Q�[���S�̂Ŏg�p����萔���`����
namespace Game
{
	constexpr int kPlayScreenWidth = 960;	// �v���C��ʂ̉���
	constexpr int kScreenWidth = 1600;		// �S��ʂ̉���
	constexpr int kPlayScreenHeight = 960;	// ��ʂ̏c��

	constexpr int kColorDepth = 32;		// 16 or 32

	//��{�I�ȃL�����N�^�[�̉~�̑傫��
	constexpr int kRadius = 48;
	/*�����Ă��������ێ�����enum*/
	enum class Dir
	{
		kDirDown,	 // ��
		kDirLeftDown,// ����
		kDirLeft,	 // ��
		kDirLeftUp,  // ����
		kDirUp,		 // ��
		kDirRightUp, // �E��
		kDirRight,	 // �E
		kDirRightDown,// �E��
		kDirDeath//����ł�����
	};
	/*�A�C�e���̎�ނ�ێ�����enum*/
	enum class ItemKinds
	{
		kEmpty,//�������p
		kExp,//�o���l
		kGold,//����
		kBlood,//�����𓮂�����
		kPortion//�񕜃A�C�e��
	};
	/*��Ԃ��Ǘ�����enum*/
	enum class State
	{
		kNormal,//��{�I�ȏ��
		kHitPlayer,//�Ԃ��������(�߂荞��łԂ���A�����������s���Ȃ�����)
		kHitMagic,//���@�ɂԂ������Ƃ��̏��
		kDelete,//����ł�����
		kNone,//�����Ȃ����
		kHitEnemy,//�G�ƂԂ��������
		kStop//�ړ��𐧌����Ă�����
	};
	enum  class WitchState
	{
		kMagic,//���@�������Ă�����
		kStone,//�΂ɂȂ��Ă�����
		kBreak//�󂳂ꂽ���
	};
	enum class ItemGraph
	{
		kArmor,
		kDropCoin,
		kDropExp,
		kDropPortion,
		kSword,
		kBoots,
		kDropTreasure,
		kFire,
		kTyphoon,
		kDropBlood
	};
	enum class ParticleKind
	{
		kWhite,//��
		kRed,//��
		kRainbow//��
	};
}