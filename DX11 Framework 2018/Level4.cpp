#include "Level4.h"

void Level4::LoadLevel()
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

	LoadGround();
}

void Level4::ExitLevel()
{

}

void Level4::PollInput(float dt)
{
	Level::PollInput(dt);
}

void Level4::Update(float dt)
{
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
}

void Level4::Reset()
{
	LoadLevel();
}

void Level4::DrawUI()
{
	ImGui::Begin("Test 4");
	
	ImGui::Text("Contacts this frame: %d", cData._contactCount);
	

	ResetButton();

	ImGui::End();
}
