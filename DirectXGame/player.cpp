#include "player.h"

player::player() {}

player::~player() {}

void player::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void player::Update() {}

void player::Draw() {}