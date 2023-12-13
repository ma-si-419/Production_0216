#pragma once

// �Q�[���S�̂Ŏg�p����萔���`����
namespace Game
{
	constexpr int kScreenWidth = 960;	// ��ʂ̉���
	constexpr int kScreenHeight = 800;	// ��ʂ̏c��

	constexpr int kColorDepth = 32;		// 16 or 32

	//��{�I�ȃL�����N�^�[�̉~�̑傫��
	constexpr int kRadius = 24;
	/*�����Ă��������ێ�����enum*/
	enum Dir
	{
		kDirDown,	 // ��
		kDirLeftDown,// ����
		kDirLeft,	 // ��
		kDirLeftUp,  // ����
		kDirUp,		 // ��
		kDirRightUp, // �E��
		kDirRight,	 // �E
		kDirRightDown// �E��
	};
	/*�A�C�e���̎�ނ�ێ�����enum*/
	enum ItemKinds
	{
		kEmpty,//�������p
		kExp,//�o���l
		kGold,//����
		kBlood,//�����𓮂�����
		kPortion,//�񕜃A�C�e��
	};
	/*��Ԃ��Ǘ�����enum*/
	enum State
	{
		kNormal,//��{�I�ȏ��
		kHit,//�Ԃ��������(�߂荞��łԂ���A�����������s���Ȃ�����)
		kHitMagic,//���@�ɂԂ������Ƃ��̏��
		kDelete//����ł�����
	};

}