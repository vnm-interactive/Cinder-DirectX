#include "cinder/app/AppBasic.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Arcball.h"

#include "dx11/RendererDx11.h"
#include "dx11/VertexTypes.h"
#include "dx11/Vbo.h"
#include "dx11/LightHelper.h"
#include "dx11/Texture.h"
#include "dx11/CommonStates.h"

using namespace ci;
using namespace ci::app; 
using namespace std;

class BasicApp : public AppBasic {

public:
	void setup()
	{
		effect = dx11::HlslEffect(loadAsset(L"Basic.fx"));
		effect.useTechnique("Light1Tex");

		duck.read(loadAsset("ducky.msh"));
		vboDuck = dx11::VboMesh(duck);
		vboDuck.createInputLayout(effect);

		texDuck = dx11::Texture(loadImage(loadAsset("ducky.png")));

		// Directional light.
		mDirLights[0].Ambient  = Vec4f(0.3f, 0.3f, 0.3f, 1.0f);
		mDirLights[0].Diffuse  = Vec4f(0.3f, 0.3f, 0.3f, 1.0f);
		mDirLights[0].Specular = Vec4f(0.4f, 0.4f, 0.4f, 1.0f);
		mDirLights[0].Direction = Vec3f(0.57735f, -0.57735f, 0.57735f);

		mtrlDuck.Diffuse = Vec4f(0.48f, 0.77f, 0.46f, 1.0f);
		mtrlDuck.Ambient = Vec4f(0.48f, 0.77f, 0.46f, 1.0f);
		mtrlDuck.Specular = Vec4f(0.2f, 0.2f, 0.2f, 16.0f);

		mTransform.setToIdentity();
	}

	void keyDown( KeyEvent event )
	{
		if (event.getCode() == KeyEvent::KEY_ESCAPE)
			quit();
	}

	void mouseDown( MouseEvent event )
	{
		mArcball.mouseDown( event.getPos() );
	}

	void mouseDrag( MouseEvent event )
	{
		mArcball.mouseDrag( event.getPos() );
	}

	void update()
	{
		// animate our little ducky
		mTransform.setToIdentity();
		mTransform.scale(3);
		mTransform.rotate(Vec3f::yAxis(), 0.1f*getElapsedSeconds());

		Matrix44f rot = mArcball.getQuat().toMatrix44();

		mDirLights[0].Direction = mArcball.getQuat().getAxis();

		effect.uniform("gEyePosW", mCam.getEyePoint());
		effect.uniform("gDiffuseMap", texDuck);
		effect.uniform("gTexTransform", Matrix44f::identity());
		effect.uniform("gDirLights", &mDirLights);
	}

	void draw()
	{
		dx11::clear(ColorA(0.5f, 0.5f, 0.5f));
		// dx11::getImmediateContext()->RSSetState(dx11::CommonStates::CullClockwise());

		effect.bind();

		Matrix44f W = mTransform;
		Matrix44f V = mCam.getModelViewMatrix();
		Matrix44f P = mCam.getProjectionMatrix();
		Matrix44f WVP = P*V*W;
		effect.uniform("gWorld", W);
		effect.uniform("gWorldInvTranspose", W);
		effect.uniform("gWorldViewProj", WVP);
		effect.uniform("gMaterial", &mtrlDuck);

		dx11::draw(vboDuck);
	}

	void resize( ResizeEvent event )
	{
		mCam.lookAt( Vec3f( 0.0f, 0.0f, 10.0f ), Vec3f::zero());
		mCam.setPerspective( 90, event.getAspectRatio(), 0.01f, 1000 );

		mArcball.setWindowSize( getWindowSize() );
		mArcball.setCenter( Vec2f( getWindowWidth() / 2.0f, getWindowHeight() / 2.0f ) );
		mArcball.setRadius( 150 );
	}

private: 
	Arcball	mArcball;
	dx11::CameraPerspDX	mCam;

	dx11::DirectionalLight mDirLights[3];

	Matrix44f mTransform;
	dx11::Material mtrlDuck;
	TriMesh	duck;
	dx11::VboMesh	vboDuck;
	dx11::Texture	texDuck;

	dx11::HlslEffect effect;
};

CINDER_APP_BASIC( BasicApp, RendererDX11)