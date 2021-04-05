#include "Level0.h"

void Level0::DrawUI()
{
	ImGui::Begin("Physics Assignment - Adam Harris");

	ImGui::Text("Controls");
	ImGui::Text("Use Keys 1 - 6 to load each level");
	ImGui::Text("Use Key 0 to return to this screen at any time");
	ImGui::Text("Use Key R to restart a level \nor press the \"Reset Simulation\" button in the ImGUI windows");
	ImGui::End();

	ImGui::Begin("Test 1 Information");
	ImGui::Text("The first test is using the particle model to control to move a cube\nfrom the left to the right");
	ImGui::Text("This is done through setting a constant acceleration of (10,0,0)");
	ImGui::Text("This test also contains settings for tweaking drag coefficients.");
	ImGui::Text("Drag is calculated based on laminar and tubular flow which are related to how fast an object is traveling");
	ImGui::End();

	ImGui::Begin("Test 2 Information");
	ImGui::Text("Test 2 is utilizing a Rigidbody model with a \"gravity force generator\" to apply a gravity force to a cube");
	ImGui::Text("Then through a \"resting force generator\" the velocity is evened out so the object doesn't go below a certain point");
	ImGui::End();

	ImGui::Begin("Test 3 Information");
	ImGui::Text("Test 3 is utilizing a Rigidbody to make a cube rotate based on where the user presses on the screen");
	ImGui::Text("This screen position is then converted to NDC (Normalized device coordinates), these are then scaled to make each click meaningful");
	ImGui::Text("As this approach uses NDC the closer to the center of the screen you are the less rotation will be applied");
	ImGui::Text("The rotation power of each click is controllable via a ImGUI window");
	ImGui::End();

	ImGui::Begin("Test 4 Information");
	ImGui::Text("Test 4 is utilizing two Rigidbodies with box colliders on them, there is also gravity in the scene forcing the cubes down to the floor");
	ImGui::Text("The boxes collide with the floor using a \"collision plane\".");
	ImGui::Text("The boxes collide with each other and respond to each other");
	ImGui::Text("You can also see the amount of contacts in each frame through the ImGUI window");
	ImGui::End();

	ImGui::Begin("Test 5 Information");
	ImGui::Text("Test 5 is the same as Test 4 but it's main showcase is collision response.");
	ImGui::End();

	ImGui::Begin("Test 6 Information");
	ImGui::Text("Test 6 showcases a sphere which falls onto a cube and generates a proper response. ");
	ImGui::Text("This is to showcase the inclusion of additional collider types in the physics engine.");
	ImGui::End();
}

void Level0::Update(float dt)
{
}
