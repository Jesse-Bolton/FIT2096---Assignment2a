#include "FirstPersonCharacter.h"
#include "MathsHelper.h"
#include <Windows.h>
#include <MMSystem.h> 

FirstPersonCharacter::FirstPersonCharacter(InputController* input, Vector3 startPos)
{
	m_boundingBox = CBoundingBox(
		startPos + Vector3(-0.5f, -1.5f, -0.5f),
		startPos + Vector3(0.5f, 0.5f, 0.5f)
	);
	m_input = input;
	SetPosition(startPos);
	m_lastFramePosition = startPos;

	m_moveSpeed = 30.0f;
	m_heightChangeSpeed = 10.0f;
	m_rotationSpeed = 0.3f;

	m_heading = 0.0f;
	m_pitch = 0.0f;
	m_health = 10;
	m_bullets = 15;

	m_shooting = false;
	m_isMoving = false;
	m_beenShot = false;
}

void FirstPersonCharacter::Update(float timestep)
{
	// Accumulate change in mouse position 
	m_heading += m_input->GetMouseDeltaX() * m_rotationSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_rotationSpeed * timestep;

	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

	// Limit how far the player can look down and up
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

	// Wrap heading and pitch up in a matrix so we can transform our look at vector
	// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
	// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
	Matrix heading = Matrix::CreateRotationY(m_heading);
	Matrix pitch = Matrix::CreateRotationX(m_pitch);

	// Transform a world right vector from world space into local space
	Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), heading);

	// Essentially our local forward vector but always parallel with the ground
	// Remember a cross product gives us a vector perpendicular to the two input vectors
	Vector3 localForwardXZ = localRight.Cross(Vector3(0, 1, 0));

	// We're going to need this a lot. Store it locally here to save on our function calls 
	Vector3 currentPos = GetPosition();
	if (m_health > 0 && m_kills < 5 && m_rubysFetch < 40) // winning condition
	{
		if (m_input->GetKeyHold('W'))
		{
			currentPos += localForwardXZ * m_moveSpeed * timestep;
			m_isMoving = true;
		}
		if (m_input->GetKeyHold('S'))
		{
			currentPos -= localForwardXZ * m_moveSpeed * timestep;
			m_isMoving = true;
		}
		if (m_input->GetKeyHold('A'))
		{
			currentPos -= localRight * m_moveSpeed * timestep;
			m_isMoving = true;
		}
		if (m_input->GetKeyHold('D'))
		{
			currentPos += localRight * m_moveSpeed * timestep;
			m_isMoving = true;
		}
		if (m_input->GetMouseDown(LEFT_MOUSE))
		{
			//PlaySound(TEXT("Assets/Audio/gunshot.wav"), NULL, SND_SYNC); // ref: http://soundbible.com/grab.php?id=2122&type=wav

			m_shooting = true;
		}
		if (m_input->GetMouseUp(LEFT_MOUSE))
		{
			m_shooting = false;
		}
		if (!m_input->GetKeyHold('W') && !m_input->GetKeyHold('S') && !m_input->GetKeyHold('A') && !m_input->GetKeyHold('D'))
		{
			m_isMoving = false;
		}
		

	}

	// Combine pitch and heading into one matrix for convenience
	Matrix lookAtRotation = pitch * heading;

	// Transform a world forward vector into local space (take pitch and heading into account)
	Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

	// At this point, our look-at vector is still relative to the origin
	// Add our position to it so it originates from the camera and points slightly in front of it
	// Remember the look-at vector needs to describe a point in the world relative to the origin
	lookAt += currentPos;

	// Use parent's mutators so isDirty flags get flipped
	SetLookAt(lookAt);
	SetPosition(currentPos);

	// Give our parent a chance to regenerate the view matrix
	Camera::Update(timestep);

	m_boundingBox.SetMin(currentPos + Vector3(-0.5f, -1.5f, -0.5f));
	m_boundingBox.SetMax(currentPos + Vector3(0.5f, 0.5f, 0.5f));
}