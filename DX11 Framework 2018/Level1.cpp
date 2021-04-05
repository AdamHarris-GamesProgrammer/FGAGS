#include "Level1.h"

void Level1::LoadLevel()
{
	Level::LoadLevel();

	if (_pParticleComponent == nullptr) {
		_pParticleComponent = std::make_unique<ParticleComponent>(_pGameObjects[0]);
		_pParticleComponent->SetInverseMass(3.0f);
		_pParticleComponent->SetDamping(0.9f);
		_pParticleComponent->SetAcceleration(Vector3(5.0f, 0.0f, 0.0f));
	}
	else
	{
		_pParticleComponent->SetOwner(_pGameObjects[0]);
	}

	//Sets velocity to 0. This is needed as the same particle component is used for each reset and the velocity will be carried over
	_pParticleComponent->SetVelocity(Vector3());


	if (_pParticalDrag == nullptr) {
		_pParticalDrag = std::make_unique<ParticleDrag>(1.05f, 1.05f);
	}
}

void Level1::Update(float dt)
{
	_pParticalDrag->Update(_pParticleComponent.get(), dt);
}

void Level1::DrawUI()
{
	ImGui::Begin("Test 1");

	//Print Movement Information about our cube.
	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Object Velocity: ", _pParticleComponent->GetVelocity());
	OutputVector3("Object Acceleration", _pParticleComponent->GetAcceleration());

	float drag = _pParticalDrag->GetDrag();
	float squaredDrag = _pParticalDrag->GetSquaredDrag();

	ImGui::SliderFloat("Drag Coeff: ", &drag, 0.0f, 2.0f, "%.2f", 1.0f);
	ImGui::SliderFloat("Squared Drag Coeff", &squaredDrag, 0.0f, 2.0f, "%.2f", 1.0f);

	_pParticalDrag->SetDrags(drag, squaredDrag);

	ResetButton();

	ImGui::End();
}
