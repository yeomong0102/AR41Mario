
#include "Camera.h"
#include "../Input.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Update(float DeltaTime)
{
	if (m_Target)
	{
		if (!m_Target->GetActive())
			m_Target = nullptr;

		else
			m_Pos = m_Target->GetPos() - m_TargetPivot * m_Resolution + m_TargetOffset;
	}

	// ī�޶� ���带 ����� ���ϵ��� ������ش�.
	if (m_Pos.x < 0.f)
		m_Pos.x = 0.f;

	else if (m_Pos.x + m_Resolution.x > m_WorldResolution.x)
		m_Pos.x = m_WorldResolution.x - m_Resolution.x;

	if (m_Pos.y < 0.f)
		m_Pos.y = 0.f;

	else if (m_Pos.y + m_Resolution.y > m_WorldResolution.y)
		m_Pos.y = m_WorldResolution.y - m_Resolution.y;

	CInput::GetInst()->ComputeWorldMousePos(m_Pos);
}
