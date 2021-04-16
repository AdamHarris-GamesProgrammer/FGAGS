#include "Level1.h"

void Level1::LoadLevel()
{
	Scene::LoadLevel();

	//Initializes the particle component
	if (_pParticleComponent == nullptr) _pParticleComponent = std::make_unique<ParticleComponent>(_pGameObjects[0]);
	else _pParticleComponent->SetOwner(_pGameObjects[0]);

	//Sets velocity to 0. This is needed as the same particle component is used for each reset and the velocity will be carried over
	_pParticleComponent->SetVelocity(Vector3());
	_pParticleComponent->SetAcceleration(Vector3());

	if (_pParticalDrag == nullptr) _pParticalDrag = std::make_unique<DragForceGenerator>(1.05f, 1.05f);
}

void Level1::PollInput(float dt)
{
	if (GetAsyncKeyState('F')) {
		//Adds acceleration to the object when you press F
		_pParticleComponent->SetAcceleration(Vector3(5.0, 0.0, 0.0));
		_pParticleComponent->SetAwake(true);
	}
	//Overriding the base poll input method so just adding this to re enable R to reset level
	if (GetAsyncKeyState('R')) {
		Reset();
	}
}

void Level1::Update(float dt)
{
	//Applies drag to the particle based on its velocity
	_pParticalDrag->Update(_pParticleComponent.get(), dt);
}

void Level1::DrawUI()
{
	ImGui::Begin("Test 1");
	ImGui::Text("This test showcases the inclusion of a particle model with a drag force generator being applied to it to add drag");
	ImGui::Text("Press F to apply acceleration");
	//Print Movement Information about our cube.
	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Object Velocity: ", _pParticleComponent->GetVelocity());
	OutputVector3("Object Acceleration", _pParticleComponent->GetAcceleration());


	//Allows the user to change the drag values
	float drag = _pParticalDrag->GetDrag();
	float squaredDrag = _pParticalDrag->GetSquaredDrag();

	ImGui::SliderFloat("Drag Coeff: ", &drag, 0.0f, 2.0f, "%.2f", 1.0f);
	ImGui::SliderFloat("Squared Drag Coeff", &squaredDrag, 0.0f, 2.0f, "%.2f", 1.0f);

	_pParticalDrag->SetDrags(drag, squaredDrag);

	ResetButton();

	ImGui::End();
}
