#include "Level1.h"

void Level1::Reset()
{
	LoadLevel();
}

void Level1::PollInput(float dt)
{
	if (GetAsyncKeyState('R')) {
		Reset();
	}
}

void Level1::Update(float dt)
{
	BeginUpdate(dt);


	pTopCube->CalculateInternals();
	pBottomCube->CalculateInternals();



	cData.Reset(MAX_CONTACTS);
	cData._friction = (float)0.9;
	cData._restitution = (float)0.1;
	cData._tolerance = (float)0.1;

	if (cData.HasMoreContacts()) {
		CollisionDetector::BoxAndHalfSpace(*pTopCube, *pGround, &cData);
		CollisionDetector::BoxAndHalfSpace(*pBottomCube, *pGround, &cData);
		CollisionDetector::BoxAndBox(*pTopCube, *pBottomCube, &cData);

		cResolver->ResolveContacts(cData._contactArray, cData._contactCount, dt);
	}

	EndUpdate(dt);
}

void Level1::LoadLevel()
{
	Level::LoadLevel();

	pBottomCube = new Box();
	pBottomCube->_halfSize = Vector3(1.0, 1.0, 1.0);
	pBottomCube->_body = _pGameObjects[0]->GetBody();
	pBottomCube->CalculateInternals();

	pTopCube = new Box();
	pTopCube->_halfSize = Vector3(1.0, 1.0, 1.0);
	pTopCube->_body = _pGameObjects[1]->GetBody();
	pTopCube->CalculateInternals();

	_pGameObjects[0]->GetBody()->SetAwake();
	_pGameObjects[1]->GetBody()->SetAwake();

	pGround = new CollisionPlane();
	pGround->_direction = Vector3(0, 1, 0);
	pGround->_offset = 0;

	cResolver = new ContactResolver(MAX_CONTACTS);
	cData._contactArray = contacts;
	cData._friction = 0.9;
	cData._restitution = 0.1;
	cData._tolerance = 0.1;

	pGroundPlane = new Plane(_pGfx);
	pGroundPlane->Make(20.0f, 20.0f, 8, 8);

	//Sets default positions
	pGroundPlane->GetTransform().SetPosition(30.0f, 100.0f, -300.0f);

	//Loads the texture for the ground plane
	pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");

	_pGameObjects.push_back(pGroundPlane);
}
