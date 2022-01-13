#include "GameScene.h"

GameScene::GameScene()
{
	timer = 0;
	waterCount = 0;
	randCount = 0;
	score = 0;
	side = rand() % 2;
	miss = false;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	timer = 0;
	waterCount = 0;
	randCount = 0;
	score = 0;
	side = rand() % 2;
	miss = false;
}

void GameScene::Update()
{
	if (No == Title) {

		//�^�C�g������

	}

	else if (No == GamePlay) {

		if (Input::isKeyTrigger(DIK_A)) {
			//���s
			if (side == !Right) { miss = true; }
			//����
			else {
				waterCount--;
				score += SCORE;
				side = Left;
			}
		}

		if (Input::isKeyTrigger(DIK_D)) {
			//���s
			if (side == !Left) { miss = true; }
			//����
			else {
				waterCount--;
				score += SCORE;
				side = Right;
			}
		}

		if (Input::isKeyTrigger(DIK_SPACE)) {
			//���s
			if (waterCount != 0) {}
			//����
			else {}
		}

		//�V�[���Ǘ�
		if (miss) { No = Result; }

		//���ԊǗ�
		if (timer <= MAX_TIME) { timer++; }
		else { No = Result; }

	}

	else if (No == Result) {

		//�X�R�A�\���Ƃ�

	}
}

void GameScene::Draw()
{
}
