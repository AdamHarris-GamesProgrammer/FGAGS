#include "Level5.h"

void Level5::Reset()
{
	LoadLevel();
}

void Level5::ExitLevel()
{
	delete _pBottomCube;
	delete _pTopCube;
	delete _pGround;
	delete _pContactResolver;

	_pBottomCube = nullptr;
	_pTopCube = nullptr;
	_pGround = nullptr;
	_pContactResolver = nullptr;

	_pCameras.clear();
	
}

void Level5::PollInput(float dt)
{
	Level::PollInput(dt);
}

void Level5::Update(float dt)
{
	_pTopCube->CalculateInternals();
	_pBottomCube->CalculateInternals();

	_contactData.Reset(MAX_CONTACTS);
	_contactData._friction = (float)0.9;
	_contactData._restitution = (float)0.1;
	_contactData._tolerance = (float)0.1;

	if (_contactData.HasMoreContacts()) {
		CollisionDetector::BoxAndHalfSpace(*_pTopCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndHalfSpace(*_pBottomCube, *_pGround, &_contactData);
		CollisionDetector::BoxAndBox(*_pTopCube, *_pBottomCube, &_contactData);

		_pContactResolver->ResolveContacts(_contactData._contactArray, _contactData._contactCount, dt);
	}

}

void Level5::LoadLevel()
{
	Level::LoadLevel();

	_pBottomCube = new Box();
	_pBottomCube->_halfSize = Vector3(1.0, 1.0, 1.0);
	_pBottomCube->_body = _pGameObjects[0]->GetBody();
	_pBottomCube->CalculateInternals();

	_pTopCube = new Box();
	_pTopCube->_halfSize = Vector3(1.0, 1.0, 1.0);
	_pTopCube->_body = _pGameObjects[1]->GetBody();
	_pTopCube->CalculateInternals();

	_pGameObjects[0]->GetBody()->SetAwake();
	_pGameObjects[1]->GetBody()->SetAwake();

	_pGround = new CollisionPlane();
	_pGround->_direction = Vector3(0, 1, 0);
	_pGround->_offset = 0;

	_pContactResolver = new ContactResolver(MAX_CONTACTS);
	_contactData._contactArray = _contactsArray;
	_contactData._friction = 0.9;
	_contactData._restitution = 0.1;
	_contactData._tolerance = 0.1;

	LoadGround();
}

void Level5::DrawUI()
{
	ImGui::Begin("Test 5");

	ResetButton();

	ImGui::End();
}
