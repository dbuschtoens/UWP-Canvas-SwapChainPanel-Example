#include "pch.h"
#include "SmileyRenderer.h"

using namespace D2D1;
using namespace SwapChainPanelCPPComponent;

SmileyRenderer::SmileyRenderer() {
	CreateDeviceIndependentResources();
	CreateDeviceResources();
	CreateSizeDependentResources();
}

void SmileyRenderer::Render() {

	if (!m_loadingComplete) {
		return;
	}
	auto center = D2D1::Point2F(m_renderTargetWidth/2, m_renderTargetHeight/2);
	auto leftEyePos = D2D1::Point2F(center.x - 30, center.y - 50);
	auto rightEyePos = D2D1::Point2F(center.x + 30, center.y - 50);
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(m_backgroundColor);
	// draw Face
	ID2D1SolidColorBrush *outlineBrush;
	ID2D1SolidColorBrush *faceBrush;
	m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &outlineBrush);
	m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &faceBrush);
	D2D1_ELLIPSE face = D2D1::Ellipse(center, 100, 100);
	m_d2dContext->FillEllipse(face, faceBrush);
	m_d2dContext->DrawEllipse(face, outlineBrush);

	// draw Eyes
	D2D1_ELLIPSE leftEye = D2D1::Ellipse(leftEyePos, 10, 30);
	D2D1_ELLIPSE rightEye = D2D1::Ellipse(rightEyePos, 10, 30);
	m_d2dContext->FillEllipse(leftEye, outlineBrush);
	m_d2dContext->FillEllipse(rightEye, outlineBrush);

	m_d2dContext->EndDraw();

	Present();

}

void SmileyRenderer::CreateDeviceResources()
{
	Direct2DPanel::CreateDeviceResources();

	m_d2dContext->CreateSolidColorBrush(ColorF(ColorF::Black), &m_strokeBrush);

	// Set D2D's unit mode to pixels so that drawing operation units are interpreted in pixels rather than DIPS.
	m_d2dContext->SetUnitMode(D2D1_UNIT_MODE::D2D1_UNIT_MODE_PIXELS);

	m_loadingComplete = true;
}

void SmileyRenderer::OnDeviceLost()
{
	// Handle device lost, then re-render.
	Direct2DPanel::OnDeviceLost();
	Render();
}

void SmileyRenderer::OnSizeChanged(Platform::Object ^ sender, Windows::UI::Xaml::SizeChangedEventArgs ^ e)
{
	// Process SizeChanged event, then re-render at the new size.
	Direct2DPanel::OnSizeChanged(sender, e);
	Render();
}

void SmileyRenderer::OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel ^ sender, Platform::Object ^ args)
{
	// Process CompositionScaleChanged event, then re-render at the new scale.
	Direct2DPanel::OnCompositionScaleChanged(sender, args);
	Render();
}

void SmileyRenderer::OnResuming(Platform::Object ^ sender, Platform::Object ^ args)
{
	// Ensure content is rendered when the app is resumed.
	Render();
}
