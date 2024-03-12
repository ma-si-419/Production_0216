#include <DxLib.h>
#include "Pad.h"

namespace
{
	// �O�̃t���[���̃p�b�h�����������
	int lastPad = 0;
	// �̃t���[���̃p�b�h�����������
	int nowPad = 0;
}
	void Pad::Update()
	{
		// �O�̃t���[���Ɏ擾�����p�b�h������Â����ɂ���
		lastPad = nowPad;

		// ���݂̃p�b�h�̏����擾����
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool Pad::IsPress(int key)
	{
		return (nowPad & key);
	}

	bool Pad::IsTrigger(int key)
	{
		bool isNow = (nowPad & key);	// ���̃t���[��
		bool isLast = (lastPad & key);	// �O�̃t���[��

		return (isNow && !isLast);
	}

	bool Pad::IsRelease(int key)
	{
		bool isNow = (nowPad & key);	// ���̃t���[��
		bool isLast = (lastPad & key);	// �O�̃t���[��

		return (!isNow && isLast);
	}

	bool Pad::IsRepeat(int key, int& frame, int RepeatFrame)
	{
		if (IsTrigger(key)) return true;

		if (IsPress(key))
		{
			frame++;

			if (RepeatFrame <= frame)
			{
				frame = 0;

				return true;
			}
		}
		else
		{
			frame = 0;
		}

		return false;
	}